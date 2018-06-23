from .models import MachineRoom, Device
from . import logger
from flask import flash, session


def get_machine_room_by_area(permit_machine_room):
    logger.debug('get machine room param: {}'.format(permit_machine_room))
    logger.debug('the session id is {}'.format(session.sid))
    if not permit_machine_room:
        flash('获取权限异常,请尝试注销后完全关闭页面并重新登陆')
        permit_machine_room = '0x0'
    return [(str(k.id), k.name)
            for k in MachineRoom.query.filter(MachineRoom.status != 0).all()
            if int(k.permit_value, 16) & int(permit_machine_room, 16) == int(k.permit_value, 16)]


def get_device_name():
    return [(str(k.id), k.device_name) for k in Device.query.all()]


def get_device_info(machine_room_id):
    """
    :param machine_room_id:
    :return:
    """
    device_info = Device.query.filter_by(machine_room_id=machine_room_id).all()
    return device_info if device_info else False
