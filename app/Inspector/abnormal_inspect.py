from ..models import ParkingRecords
from .. import logger
from sqlalchemy import or_


def entry_inspect(number_plate):
    # 判断是否正常离场，若未正常离场，则不允许进入，需要人工干预
    abnormal_flag = ParkingRecords.query.filter(
        ParkingRecords.number_plate.__eq__(number_plate),
        or_(ParkingRecords.status.__eq__(0),
            ParkingRecords.exit_time.is_(None))).order_by(ParkingRecords.create_time.desc()).first()

    if abnormal_flag:
        logger.info(
            '车牌 {} 未正常离场，上次入场时间为 {}， 记录编号为 {}'.format(number_plate, abnormal_flag.entry_time, abnormal_flag.uuid))

    return True if abnormal_flag else False
