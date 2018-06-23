import telnetlib
import re
import time


class TelnetDevice:
    def __init__(self, host, username, password):
        self.timeout = 5
        self.command_timeout = 0.5
        self.command_interval = 0.5

        try:
            self.tn = telnetlib.Telnet(host, 23, self.timeout)
            self.tn.set_debuglevel(0)
            print('login')
            time.sleep(self.command_interval)

            self.tn.expect([re.compile(b'Username:'), ], self.timeout)
            print('input username')

            self.tn.write(username.encode('utf-8') + b'\n')
            time.sleep(self.command_interval)

            self.tn.expect([b'Password:', ], self.command_timeout)
            print('input password')

            self.tn.write(password.encode('utf-8') + b'\n\n\n')
            time.sleep(self.command_interval)
            self.tn.write(b'\n')
            time.sleep(self.command_interval)
            self.tn.write(b'\n')
            time.sleep(self.command_interval)
            self.tn.expect([b'>', ], self.command_timeout)
            time.sleep(self.command_interval)
        except Exception as e:
            print('Error: {}, telnet {} fail'.format(e, host))

    def get_result(self, stop, patern1='', patern2=''):
        f = True
        result_list = []
        line_result = ''
        while f:
            result = str(self.tn.read_until(b'Total users', timeout=3)).split('\\r\\n')
            for i in result:
                # This if ... else ... is used for page tuning and break the while
                if re.search(r'More', str(i)):
                    self.tn.write(b' ')
                    time.sleep(self.command_interval)
                elif re.search(stop, str(i)) and not re.search(r'display', str(i)):
                    f = False

                # This if ... else ... is used for tuning output format
                if re.search(r'\\x1b\[37D', str(i)):
                    tmp = [l for l in str(i).split('\\x1b[37D')]
                    for context in tmp:
                        result_list.append(context)
                else:
                    result_list.append(i)

        for line in result_list:
            if re.search(patern1, str(line)) and re.search(patern2, str(line)):
                line_result = line
                break
        return result_list, line_result

    def display_trans_verbose(self, int_type, int_num):
        self.tn.write(b'display transceiver interface ' + int_type.encode('utf8') + int_num.encode('utf8') + b'verbose')
        partern = r'dBM'
        result, __ = self.get_result(stop=r'\w+>', patern1=partern, patern2=partern)
        return [re.findall(r':-\w+', line) for line in result if not re.search(r'Threshold', line)]

    def telnet_close(self):
        self.tn.close()

if __name__ == '__main__':
    t = TelnetDevice(host='172.30.2.4', username='monitor', password='Gwbnsh@408')
    y = t.display_trans_verbose('XGigabitEthernet', '0/0/18')
    for yy in y:
        print(yy)

    '''
    fsp, ont_id, result = t.find_by_mac(mac='0007-2680-3C83')
    if fsp:
        t.go_into_interface_mode(fsp)
        p = fsp.split('/')[2]
        for i in t.check_register_info(p, id=ont_id):
            print(i)
    else:
        print('not find mac')
    '''
