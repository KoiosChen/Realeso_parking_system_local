import datetime
import re
import threading
from collections import defaultdict
from .. import db, logger
from ..MyModule import AlarmPolicy, Telnet5680T, SendMail, HashContent, requestVerboseInfo
from ..models import PonAlarmRecord, Device, OntAccountInfo, max_ont_down_in_sametime


def pon_alarm_in_time_range(start_time, end_time):
    logger.debug('Start to alarm today\'s pon fail record')
    to_be_alarmed = PonAlarmRecord.query.filter(PonAlarmRecord.last_fail_time.between(start_time, end_time),
                                                PonAlarmRecord.status.__eq__(0),
                                                PonAlarmRecord.ontid.__eq__("PON")).all()

    if to_be_alarmed:
        alarm_list = []
        for olarm in to_be_alarmed:
            device_name = olarm.device_name if olarm.device_name else 'OLT'
            c = '\n' + device_name + '( ' + olarm.ip + ' ) 的' + \
                str(olarm.frame) + '/' + str(olarm.slot) + '/' + str(olarm.port) + \
                ' 无收光, 疑似断线.\n'
            alarm_list.append(c)

            hash_id = HashContent.md5_content(c)

            if not OntAccountInfo.query.filter_by(hash_id=hash_id).first():

                # 此处可根据该端口注册的信息，查询用户注册结果来判断涉及的社区
                ont_verbose = requestVerboseInfo.request_ontinfo(device_ip=olarm.ip,
                                                                 fsp=[str(olarm.frame), str(olarm.slot),
                                                                      str(olarm.port)],
                                                                 ontid_list='all')

                if ont_verbose.get('status') == 'OK':
                    account_info_list = ont_verbose['content']  # list
                    community_list = []
                    for account_info in account_info_list:
                        if 'customerListInfo' in account_info and account_info['customerListInfo']['customerList']:
                            customer_info = account_info['customerListInfo']['customerList'][0]
                            logger.debug(customer_info)
                            community_list.append(customer_info['communityName'])

                    attach = str(set(community_list))

                    add_verbose_info = OntAccountInfo(hash_id=hash_id, account_info=attach)
                    db.session.add(add_verbose_info)
                    db.session.commit()

            logger.warning('{} 此端口断线过 {} 次'.format(c, str(olarm.fail_times)))

        if len(alarm_list) > 0:
            AlarmPolicy.alarm(alarm_content=alarm_list, alarm_type='3')
        else:
            logger.info('There is no alarm')
    else:
        logger.info('There is no olt alarm now')

    db.session.expire_all()
    db.session.close()


def lots_ont_losi_alarm(start_time, end_time):
    """
    定时器调用，用于检测是否有pon口下光猫批量下线的情况
    :param start_time: 检索开始时间
    :param end_time: 检索结束时间
    :return:
    """
    ont_down_in_same_time = PonAlarmRecord.query.filter(PonAlarmRecord.last_fail_time.between(start_time, end_time),
                                                        PonAlarmRecord.status.__eq__(0),
                                                        PonAlarmRecord.ontid.__ne__("PON")).all()
    dict_ont_down_in_same_time = defaultdict(list)
    alarm_list = []
    if ont_down_in_same_time:
        for ont in ont_down_in_same_time:
            dict_ont_down_in_same_time[(ont.device_name,
                                        ont.ip, ont.frame, ont.slot, ont.port,
                                        ont.last_fail_time)].append(ont.ontid)
        for sametimedown_info, sametimedown_ontid in dict_ont_down_in_same_time.items():
            if len(set(sametimedown_ontid)) > max_ont_down_in_sametime:
                c = sametimedown_info[0] + '( ' + sametimedown_info[1] + ' ) 的' + \
                    str(sametimedown_info[2]) + '/' + str(sametimedown_info[3]) + '/' + str(sametimedown_info[4]) + \
                    '于' + sametimedown_info[5].strftime('%Y-%m-%d %H:%M:%S') + ' 同时因光的原因下线.共' + \
                    str(len(set(sametimedown_ontid))) + ' 台ONT。ONT ID:\n' + str(set(sametimedown_ontid)) + '\n\n'

                hash_id = HashContent.md5_content(c)

                if not OntAccountInfo.query.filter_by(hash_id=hash_id).first():
                    # 向founderbn_nmp项目提交查询请求，获取对应的ont用户信息
                    # 如果此条告警的附加信息已经存在，则不再进行更新
                    # requestVerboseInfo.request_ontinfo 返回的是json
                    ont_verbose = requestVerboseInfo.request_ontinfo(device_ip=sametimedown_info[1],
                                                                     fsp=sametimedown_info[2:5],
                                                                     ontid_list=list(set(sametimedown_ontid)))
                    if ont_verbose.get('status') == 'OK':

                        # 将该条告警信息以MD5的方式作为唯一标签，把对应的ontid信息存储到OntAccountInfo表中
                        # 在AlarmPolicy.alarm中增加alarm_attach_detail方法对OntAccountInfo表的查询（通过MD5），
                        # 如果有，则发送微信的时候添加对应的用户信息

                        account_info_list = ont_verbose['content']  # list
                        attach = ''  # 附件告警信息
                        for account_info in account_info_list:
                            if 'customerListInfo' in account_info and account_info['customerListInfo'].get(
                                    'customerList'):
                                customer_info = account_info['customerListInfo']['customerList'][0]
                                attach += '\n' + str(customer_info['accountId']) + ' ' + \
                                          customer_info['communityName'] + '/' + customer_info['aptNo'] + '\n'

                        add_verbose_info = OntAccountInfo(hash_id=hash_id, account_info=attach)
                        db.session.add(add_verbose_info)
                        db.session.commit()

                        db.session.expire_all()
                        db.session.close()

                # 添加到告警列表
                alarm_list.append(c)
                logger.debug(c)

    if len(alarm_list) > 0:  # 如果存在告警，则调用alarm方法
        AlarmPolicy.alarm(alarm_content=alarm_list, alarm_type='4')
    else:
        logger.info('There is no alarm')

    db.session.expire_all()
    db.session.close()


def per_ont_losi_alarm(start_time, end_time, alarm_times=100):
    """
    ont频繁上下线告警，目前仅做每天0点调用后发送邮件
    :param start_time:
    :param end_time:
    :param alarm_times: 累积下线次数
    :return:
    """
    ont_down_over_times = PonAlarmRecord.query.filter(PonAlarmRecord.last_fail_time.between(start_time, end_time),
                                                      PonAlarmRecord.fail_times.__ge__(alarm_times),
                                                      PonAlarmRecord.ontid.__ne__("PON")).all()
    alarm_list = []
    if ont_down_over_times:
        for ont in ont_down_over_times:
            c = '\n' + ont.device_name + '(' + ont.ip + ') 的' + \
                str(ont.frame) + '/' + str(ont.slot) + '/' + str(ont.port) + ' ONT ID:' + str(ont.ontid) \
                + '因光的原因累积断线超过' + str(alarm_times) + '次, 请尽快排查\n\n'

            ont_verbose = requestVerboseInfo.request_ontinfo(device_ip=ont.ip,
                                                             fsp=[ont.frame, ont.slot, ont.port],
                                                             ontid_list=[ont.ontid])
            attach = ''  # 附加告警信息
            if ont_verbose.get('status') == 'OK':

                # 将该条告警信息以MD5的方式作为唯一标签，把对应的ontid信息存储到OntAccountInfo表中
                # 在AlarmPolicy.alarm中增加alarm_attach_detail方法对OntAccountInfo表的查询（通过MD5），
                # 如果有，则发送微信的时候添加对应的用户信息

                account_info_list = ont_verbose['content']  # list

                for account_info in account_info_list:
                    if 'customerListInfo' in account_info and account_info['customerListInfo'].get(
                            'customerList'):
                        customer_info = account_info['customerListInfo']['customerList'][0]
                        attach += '\n' + str(customer_info['accountId']) + ' ' + \
                                  customer_info['communityName'] + '/' + customer_info['aptNo'] + ' ' + \
                                  str(customer_info['mobilePhone']) + '\n'

            # 告警后fail_times计数器清零
            ont.fail_times = 0
            db.session.add(ont)
            db.session.commit()

            alarm_list.append(c + '\n' + attach)
            logger.debug(c)

    SM = SendMail.sendmail(subject='ONT LOSi 告警汇总', mail_to='597796137@qq.com')
    content = '\n'.join(alarm_list)
    msg = SM.addMsgText(content, 'plain', 'GBK')
    SM.send(addmsgtext=msg)
    db.session.expire_all()
    db.session.close()


def pon_state_check():
    """
    用于检测pon口最终状态,并根据最后下线用户的时间和下线原因,猜测pon口down的原因,如果猜测是光缆问题则告警
    :param:
    :return:
    """

    def __do_check(ip):
        """
        这个方法用来再次检查pon口状态, pon_state_check的调用,受计时器的控制,也就是在x时间之后再次检查pon口,如果还是down的状态
        则检查pon口下所有ont下线的原因,如果发现匹配时间上,ont下线的原因是LOSI,则认为是光的原因下线,状态是0, 如果是其它原因则状态为2
        :param ip:  需要检查的OLT的IP地址
        :return: 无返回
        """
        two_words = re.compile(r'([\w+\s+]*)\s+([\w+\-\s+:]*$)')
        reg_datetime = re.compile(r'(\d+-\d+-\d+\s+\d+:\d+:\d+)')
        reg_mac = re.compile(r'[0-9a-fA-F]{4}\-[0-9a-fA-F]{4}\-[0-9a-fA-F]{4}')
        logger.info('Start to check {} OLT'.format(ip))
        value = {}
        # 增加了status 为-1的状态，用于标识怀疑是pon口掉线的端口，通过do_check来确认
        pon_info = PonAlarmRecord.query.filter_by(status=-1, ip=ip, ontid='PON').all()
        for pon_obj in pon_info:
            logger.debug('fsp: {} {} {}'.format(pon_obj.frame, pon_obj.slot, pon_obj.port))
            value[(pon_obj.frame, pon_obj.slot, pon_obj.port)] = pon_obj

        try:
            device_info = Device.query.filter_by(ip=ip).first()

            t = Telnet5680T.TelnetDevice(mac='', host=device_info.ip, username=device_info.login_name,
                                         password=device_info.login_password)

            for fsp, db_obj in value.items():
                t.go_into_interface_mode('/'.join(fsp))
                result = t.display_port_state(fsp[2])

                # 第一步，确认该port状态，获取port状态、最后上线及下线时间
                for line in result:
                    if re.search(r'Port state', line):
                        logger.debug(line)
                        port_state = re.findall(two_words, line)[0][1]
                    elif re.search(r'Last up time', line):
                        logger.debug(line)
                        last_up_time = re.findall(two_words, line)[0][1]
                    elif re.search(r'Last down time', line):
                        logger.debug(line)
                        last_down_time = re.findall(two_words, line)[0][1]

                try:
                    last_fail_time = re.findall(reg_datetime, last_down_time)[0]
                except Exception as e:
                    last_fail_time = None

                try:
                    last_recovery_time = re.findall(reg_datetime, last_up_time)[0]
                except Exception as e:
                    last_recovery_time = None

                logger.debug('last fail time: {}'.format(last_fail_time))
                logger.debug('last recovery time: {}'.format(last_recovery_time))

                if port_state == 'Online':
                    logger.debug('port online')
                    db_obj.status = 1
                elif port_state == 'Offline':
                    """
                    第二步，如果port是offLine的，那么做如下判断
                    """
                    logger.debug('port offline')

                    ont_id_list = []
                    """
                    获取该port下所有注册的onu的ont id，并写入到ont_id_list中
                    """
                    for ont in t.display_ont_info(fsp[2]):
                        if re.search(reg_mac, ont):
                            logger.debug(ont)
                            ont_id_list.append(
                                re.findall(r'(\d+)\s+[0-9a-fA-F]{4}\-[0-9a-fA-F]{4}\-[0-9a-fA-F]{4}', ont)[0])
                    ont_count = len(set(ont_id_list))
                    pon_flag = 0
                    if ont_count > 0:
                        for oid in ont_id_list:
                            register_info = t.display_ont_detail_info(p=str(fsp[2]), ontid=oid)
                            ont_last_down_cause, ont_last_down_time = None, None
                            for line in register_info:
                                if re.search(r'Last down cause', line):
                                    ont_last_down_cause = line.split(':')[1].strip()
                                elif re.search(r'Last down time', line):
                                    try:
                                        ont_last_down_time = datetime.datetime.strptime(
                                            re.findall(reg_datetime, line)[0], "%Y-%m-%d %H:%M:%S")
                                    except Exception as e:
                                        ont_last_down_time = None

                                if ont_last_down_cause and ont_last_down_time:
                                    break

                            logger.debug("ont_last_down_cause is {}".format(ont_last_down_cause))
                            logger.debug("ont_last_down_time is {}".format(str(ont_last_down_time)))

                            if ont_last_down_time == datetime.datetime.strptime(last_fail_time,
                                                                                "%Y-%m-%d %H:%M:%S") \
                                    and ont_last_down_cause.upper() == 'LOSI':
                                pon_flag += 1
                                logger.debug(pon_flag)
                                if pon_flag == 2:
                                    # 如果有两个光猫是因为光的原因下线，并且时间匹配PON口下线时间，则判断确认PON口是光的原因下线
                                    logger.debug("满足条件跳出循环，此接口为LOSI原因下线，切光猫数大于2")
                                    break

                    db_obj.status = 0 if pon_flag >= 2 else 2

                db_obj.last_fail_time = last_fail_time
                db_obj.last_recovery_time = last_recovery_time

                db.session.add(db_obj)

            # 关闭连接的telnet session， 以免造成文件句柄耗尽
            t.telnet_close()

            db.session.commit()
            db.session.expire_all()
            db.session.close()

        except Exception as e:
            logger.error("Cannot find the ip {} in the device list, error {}".format(ip, str(e)))

    # start from here
    pon_check = [pon.ip for pon in PonAlarmRecord.query.filter_by(status=-1, ontid='PON').all()]

    r = []
    for ip in set(pon_check):
        logger.info('checking {}'.format(ip))
        i = threading.Thread(target=__do_check, args=(ip,))
        r.append(i)
        i.start()

    for t in r:
        logger.debug('######### {}'.format(t))
        t.join()

    return True


def olt_check():
    """
    定时器调用
    :return:
    """
    pon_state_check()

    today = datetime.datetime.today()
    start_time = datetime.datetime(today.year, today.month, today.day, 0, 0, 0)
    end_time = datetime.datetime(today.year, today.month, today.day, 23, 59, 59)

    pon_alarm_in_time_range(start_time=start_time, end_time=end_time)

    lots_ont_losi_alarm(start_time=start_time, end_time=end_time)


def per_ont_check():
    """
    定时器调用
    :return:
    """
    today = datetime.datetime.today()
    start_time = datetime.datetime(today.year, today.month, today.day, 0, 0, 0)
    end_time = datetime.datetime(today.year, today.month, today.day, 23, 59, 59)

    per_ont_losi_alarm(start_time=start_time, end_time=end_time)


def unalarmed_polling():
    """
    定时器调用
    :return:
    """
    AlarmPolicy.alarmMonitor()
