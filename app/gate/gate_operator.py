from ..models import ParkingRecords
from .. import logger, db, redis_db
from ..LED.gate_led import entry_led


def open_gate(parking_record_id, action=0, operate_source=None):
    try:
        parking_record = ParkingRecords.query.filter_by(uuid=parking_record_id).first()
        parking_record.operate_source = operate_source
        parking_record.status = action
        if action == 0:
            print('open the gate {}'.format(parking_record.entry_camera_id))
            entry_led("{} 欢迎光临".format(parking_record.number_plate))
        else:
            print("open the gate {}".format(parking_record.exit_camera_id))
            entry_led("{} 欢迎再来".format(parking_record.number_plate))
            redis_db.delete(parking_record.exit_camera_id)
        db.session.add(parking_record)
        db.session.commit()
    except Exception as e:
        logger.error("open gate fail for {} ".format(e))
        db.session.rollback()
