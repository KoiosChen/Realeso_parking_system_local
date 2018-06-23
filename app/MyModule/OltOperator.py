import re

from app.MyModule import Telnet5680T
from .. import logger, db
from ..models import Device


def AnalysisONT(id):
    device_id = id
    device_info = Device.query.filter_by(id=device_id).first()
    tlnt = Telnet5680T.TelnetDevice(mac='', host=device_info.ip, username=device_info.login_name,
                                    password=device_info.login_password)
    if not device_info.status:
        logger.info('status %s. This device is not available' % device_info.status)
        return False

    # get the number of boards
    board_id_list = [line.strip().split()[0] for line in tlnt.check_board_info()]

    # Get the ONT info and insert or update to DB
    for bid in board_id_list:
        ont_list, port_list = tlnt.check_board_info(slot=bid)
        print(port_list)
        x = 0
        for i in ont_list:
            i = i.replace('b\'', '')
            if re.search(r'total of ONTs', str(i)):
                x += 1
                continue
            try:
                print(i.strip().split()[:9])
                f, s_p, ont_id, mac, control_flag, run_state, config_state, match_state, protect_side = i.strip().split()[:9]
                f = f.replace('/', '')
                s, p = s_p.split('/')
            except Exception as e:
                try:
                    print(i.split()[:8])
                    f_s_p, ont_id, mac, control_flag, run_state, config_state, match_state, protect_side = i.strip().split()[:8]
                    f, s, p = f_s_p.split('/')
                except Exception as e:
                    print(i.split()[:7])
                    f = '0'
                    s = bid
                    ont_id, mac, control_flag, run_state, config_state, match_state, protect_side = i.strip().split()[:7]
                    p = port_list[x]
                    print(p)

            ont_info = ONTDetail.query.filter_by(mac=mac).first()
            if ont_info:
                print('mac exist {} {}/{}/{}, ont_id:{}'.format(mac, ont_info.f, ont_info.s, ont_info.p, ont_info.ont_id))
                if ont_info.f == f and ont_info.s == s and ont_info.p == p and str(ont_info.ont_id) == ont_id:
                    ont_info.update_time = time.localtime()
                    ont_info.control_flag = control_flag
                    ont_info.run_state = run_state
                    ont_info.match_state = match_state
                else:
                    print('mac exist, but changed port or ont_id {}'.format(mac))
                    ont_info.ont_status = 2
                    ont_new = ONTDetail(device_id=id, f=f, s=s, p=p, ont_id=ont_id,
                                        mac=mac, control_flag=control_flag,
                                        run_state=run_state, config_state=config_state,
                                        match_state=match_state, protect_side=protect_side,
                                        pre_id=ont_info.id,
                                        create_time=time.localtime(), update_time=time.localtime())
                    db.session.add(ont_new)
            else:
                print(mac)
                if re.search(r'^[0-9a-fA-F]{4}\-[0-9a-fA-F]{4}\-[0-9a-fA-F]{4}', mac):
                    ont_info = ONTDetail(device_id=id, f=f, s=s, p=p, ont_id=ont_id,
                                         mac=mac, control_flag=control_flag,
                                         run_state=run_state, config_state=config_state,
                                         match_state=match_state, protect_side=protect_side,
                                         create_time=time.localtime(), update_time=time.localtime())
                else:
                    continue
            db.session.add(ont_info)
        try:
            db.session.commit()
        except Exception as e:
            print(e)
            db.session.rollback()

    tlnt.telnet_close()