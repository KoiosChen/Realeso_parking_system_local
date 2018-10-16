from ..models import ParkingOrder, ParkingLot, Camera, FixedParkingSpace, ParkingRecords
from .. import db, logger, redis_db
from datetime import datetime


def logically_free_parking_space(parking_lot_id):
    # 获取对应停车场的信息，停车场信息必须录入
    parking_lot_info = ParkingLot.query.filter_by(id=parking_lot_id).first()

    # 获取当前有效期内的固定车位数量
    fixed = FixedParkingSpace.query.join(ParkingOrder).filter(ParkingOrder.order_validate_start.__le__(datetime.now()),
                                                              ParkingOrder.order_validate_stop.__ge__(datetime.now()),
                                                              ParkingOrder.status.__eq__(1),
                                                              FixedParkingSpace.status.__eq__(1)).count()

    # 查询包月、单次预约的订单信息

    # 计算当前剩余临时车位 逻辑空余车位 = 总车位 - 常租车位 - 保留的临时车位数 - 包月车位 - 当前时段已预约车位

    acctually_temporary_parking_space = parking_lot_info.parking_lot.parking_space_totally - fixed

    car_parking_plates = [plates.number_plate for plates in
                          ParkingRecords.query.filter(ParkingRecords.status.__eq__(0)).all()]

    reserved_plates = [plates.number_plate for plates in ParkingOrder.query.filter(ParkingOrder.reserved.__eq__(1),
                                                                                   ParkingOrder.order_validate_start.__le__(
                                                                                       datetime.now()),
                                                                                   ParkingOrder.order_validate_stop.__ge__(
                                                                                       datetime.now()),
                                                                                   ParkingOrder.status.__eq__(1)).all()]

    reserved_plates_already_in = set(car_parking_plates) & set(reserved_plates)



    temporary_parking_space = parking_lot_info.parking_space_totally - parking_lot_info.parking_space_fixed

    current_remaining_parking_space = temporary_parking_space - len(car_parking_plates)

    temporary_for_reserved_plates = current_remaining_parking_space + len(list(reserved_plates_already_in)) - len(reserved_plates)

    # free = (parking_lot_info.parking_space_totally - parking_lot_info.parking_space_fixed) * 0.6 - 1

    logger.info("Acctually temporary parking space: {acctually_temporary_parking_space} "
                "Car parking num: {car_parking_num} "
                "Reserved number plates: {reserved_plates}".format_map(vars()))

    return current_remaining_parking_space, temporary_for_reserved_plates
