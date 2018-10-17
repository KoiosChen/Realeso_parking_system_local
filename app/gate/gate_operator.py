from ..models import ParkingRecords, Camera
from .. import logger, db, redis_db
from ..hardwareModule.gate_status import NET_BARRIERGATE_CONTROL
from ..hardwareModule.LEDControlTCP import *
import json


def open_gate(parking_record_id, direction=0, operate_source=None, debug=False):
    """

    :param parking_record_id: 停车订单id
    :param direction: 0表示入场， 1表示出场
    :param operate_source: 操作源，用来表示开闸操作来源于岗亭、在线支付还是其他
    :return:
    """
    try:
        parking_record = ParkingRecords.query.filter_by(uuid=parking_record_id).first()

        # 当没有闸机的时候，使debug为True，这样就能跳过实际的开闸操作
        if debug:
            logger.debug('open gate test')
            parking_record.operate_source = operate_source
            parking_record.status = direction
            if direction == 0:
                logger.debug('open entry gate {}'.format(parking_record.entry_camera_id))
            else:
                logger.debug('open exit gate {}'.format(parking_record.exit_camera_id))
                redis_db.delete(parking_record.exit_camera_id)
            db.session.add(parking_record)
            db.session.commit()
            return True

        # 以下为正常的开闸操作
        lUserIDs = redis_db.get('lUserIDs')
        if lUserIDs:
            lUserIDs = json.loads(lUserIDs.decode())
        else:
            logger.error("无登陆记录")
            return False

        parking_record.operate_source = operate_source
        parking_record.status = direction
        if direction == 0:
            if parking_record.entry_camera_id in lUserIDs.keys():
                logger.info('open entry gate {}'.format(parking_record.entry_camera_id))
                assert NET_BARRIERGATE_CONTROL(lUserIDs.get(parking_record.entry_camera_id), 0), "开闸失败"
                try:
                    LED_char_show_voice_broadcast(parking_record.entry_camera_id.led_id,
                                                  "{} 欢迎光临".format(parking_record.number_plate), 1,
                                                  "{} 欢迎光临".format(parking_record.number_plate))
                except Exception as e:
                    pass

            else:
                logger.error("入闸{}无登陆记录".format(parking_record.entry_camera_id))
                return False
        else:
            if parking_record.exit_camera_id in lUserIDs.keys():
                logger.info("open exit gate {}".format(parking_record.exit_camera_id))
                assert NET_BARRIERGATE_CONTROL(lUserIDs.get(parking_record.exit_camera_id), 0), "开闸失败"
                try:
                    LED_char_show_voice_broadcast(parking_record.exit_camera_id.led_id,
                                                  "{} 一路顺风".format(parking_record.number_plate), 1,
                                                  "{} 一路顺风".format(parking_record.number_plate))
                except Exception as e:
                    pass

                redis_db.delete(parking_record.exit_camera_id)
            else:
                logger.error("出闸{}无登陆记录".format(parking_record.exit_camera_id))
                return False
        db.session.add(parking_record)
        db.session.commit()
        return True
    except Exception as e:
        logger.error("open gate fail for {} ".format(e))
        db.session.rollback()
        return False
