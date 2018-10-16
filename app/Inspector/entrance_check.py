from ..ParkingLot.free_parking_space import logically_free_parking_space
from ..Billing.pay_fee import unit_price
from .abnormal_inspect import entry_inspect
from ..models import ParkingOrder, ParkingRecords, FixedParkingSpace, PARKING_TYPE, BlackList, Camera
from datetime import datetime
from sqlalchemy import and_
from .. import db, logger
from uuid import uuid1
from ..hardwareModule.LEDControlTCP import LED_char_show


def record_parking(parking_info, parking_type):
    try:
        camera_id = parking_info.get('camera')
        device = Camera.query.filter(Camera.device_number.__eq__(camera_id)).first()
        record_id = str(uuid1())
        current_price = unit_price()
        parking_record = ParkingRecords(uuid=record_id,
                                        number_plate=parking_info.get('number_plate'),
                                        entry_time=parking_info.get('time'),
                                        entry_camera_id=parking_info.get('camera'),
                                        entry_pic=parking_info.get('pic'),
                                        entry_plate_number_pic=parking_info.get('plate_number_pic'),
                                        entry_unit_price=current_price,
                                        create_time=datetime.now())

        db.session.add(parking_record)
        db.session.commit()
        # 获取对应停车场的信息，停车场信息必须录入
        parking_lot_info = Camera.query.filter(Camera.device_name.__eq__(camera_id)).first()

        current_remaining_parking_space, temporary_for_reserved_plates = logically_free_parking_space(
            parking_lot_info.parking_lot_id)
        if current_remaining_parking_space <= 0:
            LED_char_show(device.led_id, '车位已满', 60)
        elif temporary_for_reserved_plates <= 0:
            LED_char_show(device.led_id, '车位已满', 60)

        return {'status': True, 'content': PARKING_TYPE[parking_type] + '车辆入场', 'data': {'uuid': record_id}}
    except Exception as e:
        logger.error(
            '新增{}停车记录 {} 失败，原因为{}'.format(PARKING_TYPE[parking_type], parking_info.get('number_plate'), str(e)))
        db.session.rollback()
        return {'status': False, 'content': '入场失败，请联系物业'}


def entrance_check(parking_info):
    """
    被监听函数调用，由摄像头拍摄触发
    当车辆进入时，用于检测车辆牌照是否有预订单（包括常租、包月、预约按次、免费等）.同时，根据预制的算法，来获取实际空余车位以及逻辑空余车位
    如果时间段在晚上19点至次日7点，或者节假日7点至晚上19点，特殊情况除外
    逻辑空余车位 = 总车位 - 常租车位 - 保留的临时车位数 - 包月车位 - 当前时段已预约车位

    其余时段
    逻辑空余车位 = 总车位 - 常租车位
    :param parking_info:
    :return: True 表示可以正常开闸， False表示不可开炸
    """
    logger.debug('入场车辆信息为 {}'.format(parking_info))

    if entry_inspect(number_plate=parking_info['number_plate']):
        # 如果车辆异常入场，例如未正常出场，则直接返回错误，需要由管理员处理
        return {'status': False, 'content': '车辆未正常出场'}

    """
    确认即将入场的车辆是否为有效的保留车位用户
    1. 查找有效时间内的保留车位订单
    2. 查找有效期内的固定车位车辆
    """

    now_time = datetime.now()

    logger.debug('The vehical\'s entry time is {}'.format(now_time))

    # 订单类型的停车，只检索有效期内的
    reserved = ParkingOrder.query.filter(ParkingOrder.number_plate.__eq__(parking_info['number_plate']),
                                         ParkingOrder.status.__eq__(1),
                                         and_(ParkingOrder.order_validate_start.__le__(datetime.now()),
                                              ParkingOrder.order_validate_stop.__ge__(datetime.now()))).order_by(
        ParkingOrder.create_time.desc()).first()

    fixed_space = FixedParkingSpace.query.join(ParkingOrder).filter(
        ParkingOrder.number_plate.__eq__(parking_info['number_plate']),
        and_(ParkingOrder.order_validate_start.__le__(now_time),
             ParkingOrder.order_validate_stop.__ge__(now_time)
             ),
        ParkingOrder.status.__eq__(1),
        FixedParkingSpace.status.__eq__(1)
    ).first()

    logger.debug("Checking the Black list")

    blacklist = [b.number_plate for b in BlackList.query.filter(BlackList.order_validate_start.__le__(now_time),
                                                                BlackList.order_validate_stop.__ge__(now_time)).all()]

    if fixed_space:
        """
        如果为保留车位车辆，并且入场的时间在订单有效期内，直接允许进入
        """
        logger.info('车辆 {} 为固定车位用户'.format(fixed_space.fixed_order.number_plate))

        return record_parking(parking_info=parking_info,
                              parking_type=3)
    elif reserved and reserved.reserved == 1:
        logger.info('车辆 {} 为订单用户'.format(reserved.number_plate))

        return record_parking(parking_info=parking_info,
                              parking_type=2)

    # 确认剩余车位是否足够。 返回临时车位空余数量，以及保留车位空余数量
    parking_lot_info = Camera.query.filter(Camera.device_name.__eq__(parking_info.get('camera'))).first()

    current_remaining_parking_space, temporary_for_reserved_plates = logically_free_parking_space(
        parking_lot_info.parking_lot_id)

    """
    确认是否为临时车辆入场
    判断是否有临时停车位 
    """
    if parking_info['number_plate'] in blacklist:
        return {'status': False, 'content': '黑名单用户'}
    elif temporary_for_reserved_plates <= 0:
        return {'status': False, 'content': '临时车位不足，需先满足预约车辆入场'}
    elif current_remaining_parking_space <= 0:
        return {'status': False, 'content': '车位已满'}
    else:
        return record_parking(parking_info=parking_info,
                              parking_type=1)
