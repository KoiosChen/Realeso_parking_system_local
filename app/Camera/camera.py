from datetime import datetime
from ..Inspector import entrance_check, exit_check
from .. import socketio, redis_db, db, logger
from ..models import Camera, ParkingAbnormalExitRecords
from ..gate import gate_operator
from ..LED import gate_led
import json
from uuid import uuid1


def get_number_plate():
    return {"number_plate": "沪A589R0",
            "time": "2018-07-30 10:01:01",
            "camera": "c001",
            "exit_pic": "/home/peter/1.jpg",
            "exit_plate_number_pic": "http://www.w3school.com.cn/i/eg_tulip.jpg"}


def entry_test():
    check_result = entrance_check.entrance_check(get_number_plate())
    print(check_result)
    if check_result:
        print('open the entry gate')

    else:
        socketio.emit('cashier_entry_check', {'data': check_result}, namespace='/entry_check_socket')


def parking_scheduler(parking_info):
    """
    根据触发设备的类型，来确定调度给那个程序入口
    :param parking_info:
    :return:
    """
    camera_id = parking_info.get('camera')
    device = Camera.query.filter(Camera.device_number.__eq__(camera_id)).first()
    # enter
    if device.device_type == 21:
        check_result = entrance_check.entrance_check(parking_info)
        print(check_result)
        if check_result.get('status'):
            # 此处要修改，传入参数错误
            gate_operator.open_gate(check_result['data']['uuid'], action=0, operate_source=42)
            return {"status": True, "message": "{} 正常入场".format(parking_info.get('number_plate'))}
        else:
            gate_led.entry_led(check_result.get('content'))
            return {"status": False, "message": "不能进入，因为{}".format(check_result.get('content'))}
    # exit
    elif device.device_type == 22:
        old_record = redis_db.get(camera_id)
        if old_record:
            old_record = json.loads(old_record.decode())
            try:
                abnormal_exit = ParkingAbnormalExitRecords(uuid=str(uuid1()),
                                                           number_plate=old_record['number_plate'],
                                                           camera_id=old_record['camera'],
                                                           exit_pic=old_record['pic'],
                                                           exit_plate_number_pic=old_record['plate_number_pic'],
                                                           exit_time=old_record['time'],
                                                           create_time=datetime.now())
                db.session.add(abnormal_exit)
                db.session.commit()
            except Exception as e:
                logger.error('Insert abnormal exit record fail for {}'.format(str(e)))
                db.session.rollback()

        redis_db.set(camera_id, json.dumps(parking_info))
        exit_result = exit_check.exit_check(parking_info, operate_source=40)
        return {"status": True,
                "message": "{} 被摄像头{}拍摄到，{}".format(parking_info.get('number_plate'), parking_info.get('camera'),
                                                     exit_result)}
