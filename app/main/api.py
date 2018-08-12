from flask import request, jsonify
from .. import logger, redis_db, db
from . import main
from ..decorators import permission_ip
from ..Camera.camera import parking_scheduler
from ..models import ParkingRecords, Camera, ParkingOrder
from ..Billing.pay_fee import do_pay, check_out
from datetime import datetime, timedelta
import json
from ..gate.gate_operator import open_gate


@main.route('/vehical_access', methods=['POST'])
@permission_ip()
def vehical_access():
    """
    车辆进出调度
    :return:
    """

    try:
        logger.debug('Receive data from api - vehical access. Request data: {}.'.format(str(request.json)))
        request_data = request.json['data']
        if not request_data:
            raise Exception("传递空值")

        r = parking_scheduler(request_data)

        return jsonify(r)
    except Exception as e:
        logger.error(e)
        return jsonify({'code': 'fail', 'message': 'job fail for ' + str(e), 'data': ''})


@main.route('/parking_status', methods=['POST'])
@permission_ip()
def parking_status():
    """
    停车状态请求
    :return:
    """

    try:
        logger.debug('Received data from api - parking status. Request data: {}.'.format(str(request.json)))
        data = request.json['data']
        if not data:
            raise Exception("传递空值")

        # 计划出场的车牌号
        number_plate = data.get('number_plate')

        # 目前以服务器本地时间为准
        exit_time = datetime.now()

        # action为list
        action = data.get('action')

        parking_status_result = {}

        if 0 in action:
            pass
        if 1 in action:
            # 1 用于查询对应车牌的最后停车记录
            pass
        if 2 in action:
            # 2 用于返回付费后有效的出场时间，例如付费后20分钟内需离场，则在return的dict中增加此参数
            pass
        if 3 in action:
            # 3 用于返回对应车牌号已下发，未消费的订单，含有效期
            pass
        if 4 in action:
            # 4 用于返回对应车牌所有停车记录
            pass


        return jsonify(parking_status_result)
    except Exception as e:
        logger.error(e)
        return jsonify({'code': 'fail', 'message': 'job fail for ' + str(e), 'data': ''})


@main.route('/parking_order', methods=['POST'])
@permission_ip()
def parking_order():
    """
    只允许PermissionIP中涉及的服务器访问
    :return:
    """

    try:
        logger.debug('Receive data from api {}.'.format(str(request.json)))
        data = request.json['data']
        if not data:
            raise Exception("传递空值")

        order = ParkingOrder(uuid=data.get('uuid'),
                             number_plate=data.get('number_plate'),
                             order_type=data.get('order_type'),
                             order_validate_start=data.get('order_validate_start'),
                             order_validate_stop=data.get('order_validate_stop'),
                             reserved=data.get('reserved'),
                             create_time=datetime.now(),
                             update_time=datetime.now())
        db.session.add(order)
        db.session.commit()

        return jsonify({'status': 'true',
                        'message': 'Parking order {} has been inserted into local system'.format(data.get('uuid')),
                        'data': {"uuid": data.get("uuid")}})
    except Exception as e:
        logger.error(e)
        return jsonify({'status': 'false', 'message': 'Parking order insert fail for ' + str(e)})


@main.route('/fee_online', methods=['POST'])
@permission_ip()
def fee_online():
    """
    json: {"uuid": "965a417a-9489-11e8-b1e6-8c8590c5a545",
           "number_plate": "沪A589R0"}
    只允许PermissionIP中涉及的服务器访问
    :return: fee -> 总共需要支付的费用 , time_need_pay -> 打折或者免去包月（预约）等折扣（免费）时长后还需付费的时长，
             free_order_uuid -> 对应的免费停车订单ID（充值消费订单）
    """
    try:
        now_time = datetime.now()
        data = request.json['data']
        if not data:
            raise Exception("传递空值")

        parking_record = ParkingRecords.query.filter(ParkingRecords.number_plate.__eq__(data.get('number_plate')),
                                                     ParkingRecords.status.__eq__(0)).first()
        if parking_record:
            fee, time_need_pay, free_order_uuid = check_out(number_plate=data.get('number_plate'),
                                                            exit_time=now_time,
                                                            entry_price=parking_record.entry_unit_price)
            if fee is None and time_need_pay is None and free_order_uuid is None:
                return jsonify({"status": "false",
                                "message": "Paid, hurry to exit the parking lot",
                                "data": {"uuid": data.get("uuid"),
                                         "fee": fee,
                                         "time_need_pay": str(time_need_pay),
                                         "free_order_uuid": str(free_order_uuid),
                                         "localtime": now_time}})
            else:
                return jsonify({"status": "true",
                                "message": "Fee application is normal",
                                "data": {"uuid": data.get("uuid"),
                                         "fee": fee,
                                         "time_need_pay": str(time_need_pay),
                                         "free_order_uuid": str(free_order_uuid),
                                         "localtime": now_time}})
        else:
            return jsonify(
                {"status": "false",
                 "message": "Cannot find the entry record of number plate {}".format(data.get('number_plate')),
                 "data": {"uuid": data.get("uuid"),
                          "localtime": now_time}})
    except Exception as e:
        logger.error(e)
        return jsonify({"status": "false", "message": str(e)})


@main.route('/pay_online', methods=['POST'])
@permission_ip()
def pay_online():
    """
    json: {"uuid": "965a417a-9489-11e8-b1e6-8c8590c5a545",
           "number_plate": "沪A589R0",
           "fee": 20.0,
           "paid_time": "2018-08-01 11:11:11"}  其中paid_time即为fee_online接口回复的localtime字段
    只允许PermissionIP中涉及的服务器访问
    :return:
    """

    try:
        logger.debug('Receive data from api pay online {}.'.format(str(request.json)))
        request_data = request.json['data']

        if not request_data:
            raise Exception("传递空值")

        parking_record = ParkingRecords.query.filter(
            ParkingRecords.number_plate.__eq__(request_data.get('number_plate')),
            ParkingRecords.status.__eq__(0)).first()

        if parking_record:
            fee, _, _ = check_out(number_plate=request_data.get('number_plate'),
                                  exit_time=request_data.get('paid_time'),
                                  entry_price=parking_record.entry_unit_price)
            if fee is None:
                return jsonify({'status': 'false',
                                'message': 'Already paid, hurry to exit the parking lot',
                                'data': {"uuid": request_data.get('uuid')}})
            else:

                parking_record.exit_validate_before = datetime.strptime(request_data.get("paid_time"),
                                                                        "%Y-%m-%d %H:%M:%S") + timedelta(minutes=20)
                pay_result = do_pay(parking_record.uuid, fee, operate_source=31)
                if pay_result:
                    # 如果支付成功，则查看是否车辆已在出口，若在出口则直接开闸
                    all_exit_cameras = Camera.query.filter_by(device_type=22).all()
                    for exit_camera in all_exit_cameras:
                        exit_record = redis_db.get(exit_camera.device_number)
                        if exit_record:
                            exit_record = json.loads(exit_record.decode())
                            if exit_record.get('number_plate') == request_data.get('number_plate'):
                                open_gate(parking_record.uuid, action=1, operate_source=32)
                                return jsonify(
                                    {'status': 'true',
                                     'message': 'Record {} {} Yuan succeed, gate opened}}'.format(
                                         request_data.get('number_plate'), fee),
                                     'data': {"uuid": request_data.get('uuid')}})
                    return jsonify(
                        {'status': 'true',
                         'message': 'Record {} {} Yuan succeed，not exit yet}}'.format(request_data.get('number_plate'),
                                                                                     fee),
                         'data': {"uuid": request_data.get('uuid')}})
                else:
                    return jsonify(
                        {'status': 'false',
                         'message': 'Record {} {} Yuan failed'.format(request_data.get('number_plate'), fee),
                         'data': {"uuid": request_data.get('uuid')}})
        else:
            return jsonify(
                {'status': 'false',
                 'message': 'Cannot find the entry record of number plate {}'.format(request_data.get('number_plate')),
                 'data': ''})
    except Exception as e:
        logger.error(e)
        return jsonify({'status': 'false', 'message': '回调失败：' + str(e), 'data': ''})
