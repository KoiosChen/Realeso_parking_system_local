from ..models import ParkingOrder, ParkingLot, Camera, FixedParkingSpace
from .. import db, logger, redis_db
from datetime import datetime


def logically_free_parking_space(entry_gate):
    """
    # 获取对应停车场的信息，停车场信息必须录入
    parking_lot_info = Camera.query.filter(Camera.device_name.__eq__(entry_gate)).first()

    # 获取当前有效期内的固定车位数量
    fixed = FixedParkingSpace.query.join(ParkingOrder).filter(ParkingOrder.order_validate_start.__le__(datetime.now()),
                                                              ParkingOrder.order_validate_stop.__ge__(
                                                                  datetime.now()),
                                                              ParkingOrder.status.__eq__(1),
                                                              FixedParkingSpace.status.__eq__(1)).count()

    # 查询包月、单次预约的订单信息

    # 计算当前剩余临时车位 逻辑空余车位 = 总车位 - 常租车位 - 保留的临时车位数 - 包月车位 - 当前时段已预约车位

    temporary_parking_space = parking_lot_info.parking_lot.parking_space_totally - fixed

    free = (parking_lot_info.parking_space_totally - parking_lot_info.parking_space_fixed) * 0.6 - 1

    """

    return 1
