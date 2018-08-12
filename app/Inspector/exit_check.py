from ..Billing.pay_fee import check_out
from ..models import ParkingRecords, ParkingAbnormalExitRecords
from datetime import datetime
from .. import db, logger, redis_db, socketio
from ..gate.gate_operator import open_gate
from uuid import uuid1
import json
from ..Billing.pay_fee import do_pay, check_fee
import requests


def exit_check(parking_info, operate_source):
    number_plate = parking_info.get('number_plate')
    exit_time = parking_info.get('time')
    if isinstance(exit_time, str):
        exit_time = datetime.strptime(exit_time, '%Y-%m-%d %H:%M:%S')

    entry_record = ParkingRecords.query.filter(ParkingRecords.number_plate.__eq__(number_plate),
                                               ParkingRecords.status.__eq__(0)).order_by(
        ParkingRecords.create_time.desc()).first()

    if not entry_record:
        logger.debug('cannot find parking records')
        try:
            if not ParkingAbnormalExitRecords.query.filter_by(number_plate=number_plate,
                                                              exit_time=exit_time,
                                                              camera_id=parking_info.get('camera')).all():
                logger.debug('insert abnormal exit record')
                abnormal_exit = ParkingAbnormalExitRecords(uuid=str(uuid1()),
                                                           number_plate=number_plate,
                                                           camera_id=parking_info.get('camera'),
                                                           exit_time=exit_time,
                                                           exit_pic=parking_info.get('pic'),
                                                           exit_plate_number_pic=parking_info.get('plate_number_pic'),
                                                           create_time=datetime.now())
                db.session.add(abnormal_exit)
                db.session.commit()
        except Exception as e:
            logger.error("Update abnormal exit record fail for {}".format(str(e)))
            db.session.rollback()

        logger.debug('emit to ws_test without entry info')

        socketio.emit('ws_test', {'status': 'false',
                                  'content': {
                                      'number_plate': parking_info.get('number_plate'),
                                      'entry_time': '',
                                      'exit_time': str(parking_info.get('time')),
                                      'entry_unit_price': '',
                                      'entry_pic': '',
                                      'entry_plate_number_pic': '',
                                      'exit_pic': parking_info.get('exit_pic'),
                                      'exit_plate_number_pic': parking_info.get(
                                          'exit_plate_number_pic'),
                                      'fee': '',
                                      'totally_time': '',
                                      'parking_record_id': ''}
                                  }, namespace='/test')
        return "未找到入场记录"

    else:
        """
        如果是订单停车。或者入场时没有任何订单的临时停车，在出场的时候，都会重新检查是否有合适的订单
        目前允许单次预约的客户在入场之后再点预约
        """
        if entry_record.fee is not None and exit_time <= entry_record.exit_validate_before and operate_source >= 40:
            logger.debug('已付费出场')

            entry_record.exit_time = exit_time
            entry_record.exit_pic = parking_info.get('pic')
            entry_record.exit_plate_number_pic = parking_info.get('plate_number_pic')
            entry_record.exit_camera_id = parking_info.get('camera')
            db.session.add(entry_record)
            db.session.commit()
            open_gate(entry_record.uuid, action=1, operate_source=operate_source + 2)
            return "{}}已付费{}}，正常出场".format(number_plate, entry_record.fee)

        elif entry_record.fee is not None and exit_time > entry_record.exit_validate_before and operate_source >= 40:
            print(exit_time, entry_record.exit_validate_before)
            total_fee, total_time, free_order = check_out(number_plate, exit_time, entry_record.entry_unit_price)
            logger.debug('超时未出场，需重新付费，需再付费{} {}'.format(total_fee, total_time))
            return "{}已付费{}，离场最晚时间为{}，当前时间{}，超时未出场，请再付费{}".format(number_plate, entry_record.fee,
                                                                  entry_record.exit_validate_before, exit_time,
                                                                  total_fee)
        else:
            entry_record.exit_time = exit_time
            entry_record.exit_pic = parking_info.get('pic')
            entry_record.exit_plate_number_pic = parking_info.get('plate_number_pic')
            entry_record.exit_camera_id = parking_info.get('camera')
            entry_record.operate_source = 12

            db.session.add(entry_record)
            db.session.commit()

            parking_info['status'] = 1
            redis_db.set(parking_info['camera'], json.dumps(parking_info))

            checkout_amount, pay_time, free_order = check_out(number_plate=number_plate, exit_time=exit_time,
                                                              entry_price=entry_record.entry_unit_price)

            if free_order or checkout_amount == 0:
                do_pay(entry_record.uuid, fee=0, operate_source=operate_source + 1)
                open_gate(entry_record.uuid, action=1, operate_source=operate_source + 2)
                if free_order:
                    redis_db.set('free_order_' + free_order, entry_record.uuid)
                    url = 'http://api.realeso.com/free_order'

                    headers = {
                        'Content-Type': 'application/json',
                        'Cache-Control': 'no-store'
                    }

                    value = {
                        'uuid': free_order,
                        'parking_record_uuid': entry_record.uuid,
                        'fee': checkout_amount
                    }

                    response = requests.post(url, value, headers=headers)
                    logger.debug(response.content)

            logger.debug('emit data to ws_test')

            socketio.emit('ws_test', {'status': 'true',
                                      'content': {
                                          'number_plate': entry_record.number_plate,
                                          'entry_time': str(entry_record.entry_time),
                                          'exit_time': str(entry_record.exit_time),
                                          'entry_unit_price': str(entry_record.entry_unit_price) + '元',
                                          'entry_pic': entry_record.entry_pic,
                                          'entry_plate_number_pic': entry_record.entry_plate_number_pic,
                                          'exit_pic': entry_record.exit_pic,
                                          'exit_plate_number_pic': entry_record.exit_plate_number_pic,
                                          'fee': str(checkout_amount) + '元',
                                          'totally_time': str(pay_time),
                                          'parking_record_id': entry_record.uuid}
                                      }, namespace='/test')

            return "{}找到入场记录{}，应付费{}元，已发送到收银台，可在服务中心、或者通过电子支付完成后出场".format(number_plate, entry_record.uuid,
                                                                           checkout_amount)
