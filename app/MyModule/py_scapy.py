from scapy.all import DNSRR
from scapy.all import *
from collections import defaultdict
from concurrent.futures import ProcessPoolExecutor
from multiprocessing import cpu_count
import numpy as np


def microseconds_minus(t1, t2):
    """

    :param t1:  [second, ms]
    :param t2:  [second, ms]
    :return: timedelta
    """
    return ((t1[0] - t2[0]) * 1000000 - t2[1] + t1[1]) / 1000000


def nesteddict():
    return defaultdict(nesteddict)


def read_pcap(pcap_file=None):
    return [(p[0], p[1][0], p[1][1]) for p in RawPcapReader(pcap_file).read_all()]


def _process_pkts(raw_pkt_cut_):
    raw_process_result = []

    for raw_pkt_, timestamp_s, timestamp_ms in raw_pkt_cut_:
        pkt = Ether(raw_pkt_)
        if pkt.haslayer('IP'):
            if pkt.haslayer('TCP'):
                ip_set = tuple(sorted([pkt['IP'].src, pkt['IP'].dst]))
                port_set = tuple(sorted([pkt['TCP'].sport, pkt['TCP'].dport]))

                # 计算净载大小 0x002: syn  0x012: syn, ack   0x010:ack
                if pkt['TCP'].flags == 0x002 or pkt['TCP'].flags == 0x0c2 or pkt['TCP'].flags == 0x052 or pkt[
                    'TCP'].flags == 0x012 or pkt['TCP'].flags == 0x011:
                    load_len = 1
                else:
                    load_len = pkt['IP'].len - 20 - pkt['TCP'].dataofs * 4

                # 获取净载内容
                if pkt.haslayer('Raw'):
                    try:
                        pkt_load = '\n'.join(pkt['Raw'].load.decode('GBK').split('\r\n'))
                    except:
                        pkt_load = ''
                else:
                    pkt_load = ''

                raw_process_result.append(('TCP', ip_set, port_set,
                                           (pkt['IP'].src, pkt['IP'].dst, pkt['TCP'].sport, pkt['TCP'].dport), (
                                               pkt['TCP'].seq, pkt['TCP'].seq + load_len, [timestamp_s, timestamp_ms],
                                               pkt['IP'].len,
                                               pkt_load, pkt['TCP'].flags)))

            elif pkt.haslayer('UDP'):
                ip_set = tuple(sorted([pkt['IP'].src, pkt['IP'].dst]))
                port_set = tuple(sorted([pkt['UDP'].sport, pkt['UDP'].dport]))

                raw_process_result.append(('UDP', ip_set, port_set, raw_pkt_))
            else:
                # 例如 ICMP等，目前不处理
                pass
        else:
            raw_process_result.append(('other', raw_pkt_))

    return raw_process_result


def build_session(raw_packet):
    net_session = nesteddict()

    step = int(len(raw_packet) / cpu_count()) + 1

    raw_packet_cut = []

    for i in range(0, len(raw_packet), step):
        raw_packet_cut.append(raw_packet[i: i + step])

    with ProcessPoolExecutor() as pool:
        for raw_result in pool.map(_process_pkts, raw_packet_cut):
            for pkt in raw_result:
                if pkt[0] == 'TCP':
                    if len(net_session['TCP'][pkt[1]][pkt[2]][pkt[3]]) == 0:
                        net_session['TCP'][pkt[1]][pkt[2]][pkt[3]] = []
                    net_session['TCP'][pkt[1]][pkt[2]][pkt[3]].append(pkt[4])
                elif pkt[0] == 'UDP':
                    if len(net_session['UDP'][pkt[1]][pkt[2]]) == 0:
                        net_session['UDP'][pkt[1]][pkt[2]] = []
                    net_session['UDP'][pkt[1]][pkt[2]].append(Ether(pkt[3]))
    return net_session


def dns_analyze(udp_session):
    dns_cname = defaultdict(list)
    dns_query_result = defaultdict(list)

    for ip_, port_sessions in udp_session.items():
        for port_, packet in port_sessions.items():
            for p in packet:
                if p.haslayer(DNSRR):
                    i = 0
                    while True:
                        try:
                            if p[DNSRR][i].type == 5:
                                dns_cname[p[DNSRR][i].rrname.decode()].append(p[DNSRR][i].rdata.decode())
                            elif p[DNSRR][i].type == 1:
                                dns_query_result[p[DNSRR][i].rrname.decode()].append(p[DNSRR][i].rdata)
                            i += 1
                        except:
                            break

    return dns_cname, dns_query_result


def tcp_session_analyze(tcp_session, dns_query_result, dns_cname):
    # 用于存放最终的分析结果
    analyze_result = nesteddict()
    print_cache = []

    for ip_, port_sessions in tcp_session.items():
        for port_, session_packet in port_sessions.items():
            # print('\n' + '=' * 100)
            # print('Session: {} {}'.format(ip_, port_))
            print_cache.append('\n' + '=' * 100 + '\n')
            print_cache.append('Session: {} {}\n'.format(ip_, port_))
            analyze_result[ip_, port_]['dns'] = []

            for cname, resolve_ip in dns_query_result.items():
                for ip in ip_:
                    if ip in resolve_ip:
                        for qname, cname_list in dns_cname.items():
                            if cname in cname_list:
                                # print('Service domain: {}'.format(qname))
                                print_cache.append('Service domain: {}\n'.format(qname))
                                analyze_result[ip_, port_]['dns'].append(qname)
            # print('\n' + '-' * 100)
            print_cache.append('\n' + '-' * 100 + '\n')

            # 记录每个会话的三次握手包，包括seq，pkt.time
            handshake = {}

            for sbd, seq_nextseq in session_packet.items():
                # session based on direction. For example: sbd => ('52.85.149.120', '172.24.1.80', 443, 50935)
                # print(sbd, ': ', end='')
                print_cache.append(str(sbd) + ': ')
                nextseq = seq_nextseq[0][0]
                packet_lost = 0
                expect_nextseq = []
                total_wait_time = [0]
                for i, sn in enumerate(seq_nextseq):
                    if sn[5] == int('0x002', 16) or sn[5] == int('0x0c2', 16):
                        handshake['syn'] = (sn[0], sn[2])
                    elif sn[5] == int('0x012', 16) or sn[5] == int('0x052'):
                        handshake['syn_ack'] = (sn[0], sn[2])
                    elif sn[5] == int('0x010', 16) and handshake.get('syn') and handshake['syn'][0] + 1 == sn[0]:
                        handshake['ack'] = (sn[0], sn[2])
                    if sn[0] == nextseq:
                        # print('.', end='')
                        print_cache.append('.')
                        nextseq = sn[1]

                    elif sn[0] > nextseq:
                        # print('?', end='')
                        print_cache.append('?')
                        expect_nextseq.append((seq_nextseq[i - 1][1], sn[0], sn[2]))
                        nextseq = sn[1]

                    elif sn[0] < nextseq:
                        for en in expect_nextseq:
                            if en[0] <= sn[0] < en[1]:
                                # time 相差3ms以上，表示丢包
                                if microseconds_minus(sn[2], en[2]) > 0.003:
                                    total_wait_time.append(microseconds_minus(sn[2], en[2]))
                                    # print(">", end='')
                                    print_cache.append('>')
                                    packet_lost += 1
                                # time 相差3ms以内，表示乱序
                                elif microseconds_minus(sn[2], en[2]) <= 0.003:
                                    # print("<", end='')
                                    print_cache.append('<')

                packet_lost_percent = packet_lost / len(seq_nextseq) * 100
                total_wait_time = max(total_wait_time)
                total_time = microseconds_minus(seq_nextseq[len(seq_nextseq) - 1][2], seq_nextseq[0][2])
                total_len = sum(l[3] for l in seq_nextseq)

                # unit: Kbps
                avg_transmission_rate = total_len / 1024 / total_time * 8 if total_time else 0

                session_result = dict(zip(['pkt_lost', 'total_wait_time', 'total_time', 'total_len', 'avg_rate'],
                                          [packet_lost_percent, total_wait_time, total_time, total_len,
                                           avg_transmission_rate]))

                analyze_result[ip_, port_][sbd]['result'] = session_result

                """
                print()
                print(' | packets lost: {}'.format(str(packet_lost_percent) + '%'))
                print(' | Total packet retransmit wait time {}'.format(str(total_wait_time)))
                print(' | Total time {}s'.format(str(total_time)))
                print(' | Total len {}'.format(str(total_len)))
                print(' | Average transmission rate {}'.format(str(avg_transmission_rate) + 'Kbps'))
                """

                print_cache.append('\n')
                print_cache.append(' | packets lost: {}'.format(str(packet_lost_percent) + '%\n'))
                print_cache.append(' | Total packet retransmit wait time {}\n'.format(str(total_wait_time)))
                print_cache.append(' | Total time {}s\n'.format(str(total_time)))
                print_cache.append(' | Total len {}\n'.format(str(total_len)))
                print_cache.append(' | Average transmission rate {}\n'.format(str(avg_transmission_rate) + 'Kbps'))

                # print load content
                for x, snn in enumerate(seq_nextseq):
                    if snn[4]:
                        for line in snn[4].split('\n'):
                            if re.search(r'POST|GET\s+/|Host:', line):
                                # print('{}'.format(line))
                                print_cache.append('{}\n'.format(line))

            f = ['syn', 'syn_ack', 'ack'] - handshake.keys()
            # print(f)
            print_cache.append(str(handshake.keys()) + '\n')

            if not len(f):
                trans_rate = int(microseconds_minus(handshake.get('syn_ack')[1], handshake.get('syn')[1]) / 2 * 1000)

                # print('|| Transmition time: {}ms'.format(trans_rate))
                print_cache.append('|| Transmition time: {}ms\n'.format(trans_rate))

                # unit: ms
                analyze_result[ip_, port_]['handshake'] = trans_rate
            elif len(f) == 2 and 'syn' not in f:
                # print('|| Handshake not complete')
                print_cache.append('|| Handshake not complete\n')
                analyze_result[ip_, port_]['handshake'] = 'not complete'
            else:
                # print('|| Handshake packet is not entirely captured')
                print_cache.append('|| Handshake packet is not entirely captured\n')
                analyze_result[ip_, port_]['handshake'] = None

    return analyze_result, print_cache


def gen_tcp_alarm(tcp_result):
    problem_list = []

    # 'pkt_lost', 'total_wait_time', 'total_time', 'total_len', 'avg_rate'
    standard_dict = {
        'pkt_lost': [0, 0.3],
        'total_wait_time': [0, 0.2],
        'total_time': [0, 1000],
        'avg_rate': [0, 1024000]
    }
    problem_list.append('+-' * 50 + '\n')
    for k1 in tcp_result.keys():
        dns_print = False
        for k2 in tcp_result[k1].keys():
            if k2 != 'dns' and k2 != 'handshake':
                for k3, v in tcp_result[k1][k2].items():
                    for key, value in v.items():
                        if key in standard_dict:
                            if not standard_dict[key][0] <= value <= standard_dict[key][1]:
                                dns_print = True
                                problem_list.append('* {} {} {} {} {}\n'.format(k1, k2, k3, key, value))
            elif k2 == 'handshake':
                if (isinstance(tcp_result[k1][k2], str) and tcp_result[k1][k2] == 'not complete') or (
                        isinstance(tcp_result[k1][k2], float) and tcp_result[k1][k2] >= 50):
                    dns_print = True
                    # if result[k1][k2] is not None or result[k1][k2] == 'not complete' or result[k1][k2] >= 50:
                    problem_list.append('* {} {} {}\n'.format(k1, k2, tcp_result[k1][k2]))

        if tcp_result[k1]['dns'] and dns_print:
            problem_list.append(' \t** dns: \n\t' + str(tcp_result[k1]['dns']) + '\n')

        if dns_print:
            problem_list.append('.' * 100 + '\n')

    return problem_list


def do(file):
    p = read_pcap(pcap_file=file)
    sessions_ = build_session(p)
    dns_cname, dns_query_result = dns_analyze(sessions_['UDP'])

    result, print_detail = tcp_session_analyze(sessions_['TCP'], dns_query_result, dns_cname)

    # for line in print_cache:
    #     print(line, end='')

    if result:
        print_alarm = gen_tcp_alarm(result)
        return {'status': 'ok', 'content': {'detail': print_detail, 'verbose': print_alarm}}
    else:
        return {'status': 'fail', 'content': 'no tcp'}
