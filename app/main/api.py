from flask import request, jsonify
from .. import logger, redis_db, db
from . import main
from ..decorators import permission_ip
from ..Camera.camera import parking_scheduler
from ..models import ParkingRecords, Camera, ParkingOrder, ParkingLot
from ..Billing.pay_fee import do_pay, check_out
from ..ParkingLot.free_parking_space import logically_free_parking_space
from datetime import datetime, timedelta
import json
from ..gate.gate_operator import open_gate


@main.route('/vehical_access', methods=['POST'])
@permission_ip
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
@permission_ip
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
            # 0 用于返回停车场剩余车位数, 此处的parking_Lot_id，先由action 2 来获取停车场id
            current_remaining_parking_space, temporary_for_reserved_plates = \
                logically_free_parking_space(data.get('parking_lot_id'))
            return jsonify({'status': 'true',
                            'data': {'current_remaining_parking_space': current_remaining_parking_space,
                                     'temporary_for_reserved_plates': temporary_for_reserved_plates}})
        if 1 in action:
            # 1 用于查询对应车牌的最后停车记录
            last_parking_record = ParkingRecords.query.filter_by(number_plate=number_plate, status=0).order_by(
                ParkingRecords.entry_time.desc()).first()
            if last_parking_record:
                return jsonify({'status': 'true', 'data': {'uuid': last_parking_record.uuid,
                                                           'number_plate': last_parking_record.number_plate,
                                                           'entry_time': last_parking_record.entry_time,
                                                           'entry_unit_price': last_parking_record.entry_unit_price,
                                                           'exit_time': last_parking_record.exit_time,
                                                           'fee': last_parking_record.fee,
                                                           'create_time': last_parking_record.create_time}})
            else:
                return jsonify({'status': 'false', 'data': {}, 'message': '获取该车牌{}最后入场（未出）记录失败'.format(number_plate)})

        if 2 in action:
            # 2 用于返回本地停车场相关参数信息
            parking_info = ParkingLot.query.filter_by(status=1).first()
            if parking_info:
                return jsonify({'status': 'true', 'data': {'id': parking_info.id,
                                                           'name': parking_info.name,
                                                           'address': parking_info.address,
                                                           'floors': parking_info.floors,
                                                           'parking_space_totally': parking_info.parking_space_tatally,
                                                           'free_minutes': parking_info.free_minutes,
                                                           'start_minutes': parking_info.start_minutes,
                                                           'pay_interval': parking_info.pay_interval,
                                                           'effective_duration': parking_info.effective_duration}})
            else:
                return jsonify({'status': 'false', 'data': {}, 'message': '停车场信息获取失败'})

        if 3 in action:
            # 3 用于返回对应车牌号已下发，未消费的订单，含有效期
            the_parking_orders = ParkingRecords.query.filter_by(number_plate=number_plate, status=1).all()
            catalog = ['uuid', 'number_plate', 'order_type', 'order_validate_start', 'order_validate_stop', 'reserved',
                       'create_time', 'update_time', 'discount']
            if the_parking_orders:
                return jsonify({'status': 'true',
                                'data': [dict(zip(catalog, [r.uuid, r.number_plate, r.order_type,
                                                            r.order_validate_start,
                                                            r.order_validate_stop, r.reserved,
                                                            r.create_time, r.update_time, r.discount]))
                                         for r in the_parking_orders]})
            else:
                return jsonify({'status': 'false', 'data': {}, 'message': '获取{}对应未消费订单失败'.format(number_plate)})

        if 4 in action:
            # 4 用于返回对应车牌所有停车记录
            catalog = ['uuid', 'number_plate', 'entry_time', 'entry_unit_price', 'exit_time', 'fee', 'status',
                       'create_time']
            all_parking_record = ParkingRecords.query.filter(number_plate=number_plate).all()
            if all_parking_record:
                return jsonify({'status': 'true', 'data': [dict(zip(catalog, [r.uuid, r.number_plate, r.entry_time,
                                                                              r.entry_unit_price, r.exit_time,
                                                                              r.fee,
                                                                              r.status, r.create_time])) for r in
                                                           all_parking_record]})

            else:
                return jsonify({'status': 'false', 'data': {}, 'message': '{}车牌对应停车记录获取失败'.format(number_plate)})

        return jsonify(parking_status_result)
    except Exception as e:
        logger.error(e)
        return jsonify({'status': 'false', 'message': 'job fail for ' + str(e), 'data': {}})


@main.route('/parking_order', methods=['POST'])
@permission_ip
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
@permission_ip
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
                return jsonify({"status": "paid",
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
@permission_ip
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
                                assert open_gate(parking_record.uuid, direction=1, operate_source=32), "开闸失败"
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
