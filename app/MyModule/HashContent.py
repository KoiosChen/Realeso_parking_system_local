import hashlib


def md5_content(content):
    m2 = hashlib.md5()
    m2.update(content.encode('utf-8'))
    return m2.hexdigest().upper()


if __name__ == '__main__':
    print(md5_content('id: 844 2401-隆昌-S5700-1 - 流量 - GigabitEthernet0/0/8 [traffic_out]产生阀值告警。 告警值:2\n\n'))