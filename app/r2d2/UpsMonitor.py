# -*- coding: utf-8 -*-
import json
import re
import time
from .. import db, logger, redis_db, snmp
from ..MyModule import AlarmPolicy
from ..models import UpsInfo


def ups_monitor():
    ups_status = {'1': 'unknown', '2': '（市电正常）online', '3': '(市电中断)onBattery', '4': 'onBoost', '5': 'sleeping',
                  '6': 'onBypass',
                  '7': 'rebooting', '8': 'standBy', '9': 'onBuck'}
    ups_info = UpsInfo.query.all()
    alarm_list = []
    for u in ups_info:
        if redis_db.get('ups_status_' + u.ip):
            pre_status = redis_db.get('ups_status_' + u.ip).decode()
        else:
            pre_status = None

        if redis_db.get('ups_power_left_' + u.ip):
            pre_power_left = redis_db.get('ups_status_' + u.ip).decode()
        else:
            pre_power_left = None

        logger.debug('Getting {} {} snmp info'.format(u.name, u.ip))
        snmp.destHost = u.ip
        snmp.community = u.community
        snmp_result = {}
        for key, oid in json.loads(u.oid_dict).items():
            logger.debug('Get {} {}'.format(key, oid))
            snmp.oid = oid
            result = snmp.query()
            try:
                result_value = re.findall(r'=\s+(\d+)', str(result[0]))[0]
                logger.debug('Get result {}'.format(result_value))
                snmp_result[key] = result_value
            except Exception as e:
                print(e)

        snmp_status = \
            ups_status.get(snmp_result.get('status')) if ups_status.get(snmp_result.get('status')) else 'unknown'
        snmp_power_left = int(snmp_result.get('power_left')) if snmp_result.get('power_left') else 80

        if snmp_result.get('status') != '2' and (pre_status == '2' or pre_status is None):
            s = u.name + ' UPS ' + u.ip + ' 于 ' + time.strftime("%Y-%m-%d %H:%M:%S",
                                                                time.localtime(time.time())) + ' ' + snmp_status + '\n'
            redis_db.set('ups_status_' + u.ip, snmp_result.get('status'))
            alarm_list.append(s)

        if snmp_result.get('status') != '2' and pre_power_left is not None and snmp_power_left < 80 <= int(
                pre_power_left):
            s = u.name + ' UPS ' + u.ip + ' 于 ' + time.strftime("%Y-%m-%d %H:%M:%S",
                                                                time.localtime(time.time())) + ' ' + '电力低于80%\n'
            redis_db.set('ups_power_left_' + u.ip, snmp_power_left)
            alarm_list.append(s)
        elif snmp_power_left >= 80 or pre_power_left is None:
            redis_db.set('ups_power_left_' + u.ip, snmp_power_left)

        if snmp_result.get('status') == '2' and pre_status != '2':
            logger.debug('pre_status {}'.format(pre_status))
            s = u.name + ' UPS ' + u.ip + ' 于 ' + time.strftime("%Y-%m-%d %H:%M:%S",
                                                                time.localtime(time.time())) + ' ' + snmp_status + '\n'
            redis_db.set('ups_status_' + u.ip, snmp_result.get('status'))
            alarm_list.append(s)

    if len(alarm_list) > 0:
        logger.info('UPS ALARM')
        AlarmPolicy.alarm(alarm_content=alarm_list, alarm_type='1')
    else:
        logger.info('There is no ups alarm')

