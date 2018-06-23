import requests
from ..models import temp_threshold, humi_threshold
from ..MyModule import AlarmPolicy
from .. import db, logger


def get_485_device_list():
    get_account_info_url = 'http://172.20.0.7:9001/Device/API_GetDevList'
    r = requests.get(get_account_info_url)
    return r.json()


def monitor_temp_humi_485():
    alarm_list = []
    for device in get_485_device_list():
        s = '{} {}: {} {}: {}'.format(device['DevName'],
                                      device['DevTempName'], device['DevTempValue'],
                                      device['DevHumiName'], device['DevHumiValue'])

        if not temp_threshold['min'] < float(device['DevTempValue']) < temp_threshold['max'] \
                or not humi_threshold['min'] < float(device['DevHumiValue']) < humi_threshold['max']:
            alarm_list.append(s)

    if alarm_list:
        logger.info('UPS ALARM')
        AlarmPolicy.alarm(alarm_content=alarm_list, alarm_type='1')
    else:
        logger.info('There is no ups alarm')

    db.session.expire_all()
    db.session.close()
