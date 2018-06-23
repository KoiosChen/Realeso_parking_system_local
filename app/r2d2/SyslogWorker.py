import datetime
import re
import time
from .. import db, logger
from ..MyModule import AlarmPolicy
from ..models import PonAlarmRecord, Device


def py_syslog_olt_monitor(host, logmsg):
    """
    处理py_syslog调用，用于处理olt发来的日志
    :param host:
    :param logmsg:
    :return:
    """
    logger.debug("olt syslog monitor {} {}".format(host, logmsg))
    device = Device.query.filter_by(ip=host).first()
    device_name = device.device_name if device else 'None'
    ont_down = re.compile(r'(FAULT).*?\n*.*?(fiber is broken).*?\n*.*?(EPON ONT)|'
                          r'The feed fiber is broken or OLT can not receive any expected')
    ont_up = re.compile(r'(RECOVERY CLEARED).*?\n*.*?(OLT can receive expected optical signals)|'
                        r'OLT can receive expected optical signals from ONT')
    try:
        fail = re.search(ont_down, logmsg)
        recovery = re.search(ont_up, logmsg)
        alert_time = datetime.datetime.strptime(
            re.findall(r'(\d+-\d+-\d+\s+\d+:\d+:\d+)', logmsg)[0],
            '%Y-%m-%d %H:%M:%S'
        )
        try:
            f, s, p, ontid = \
                re.findall(r'FrameID:\s+(\d+),\s+SlotID:\s+(\d+),\s+PortID:\s+(\d+),\s+ONT\s+ID:\s+(\d+)', logmsg)[0]
            pon_history = PonAlarmRecord.query.filter_by(ip=host, frame=f, slot=s, port=p, ontid=ontid).first()
        except Exception as e:
            logger.warning("split frame slot port error first time: {}, the msg is {}".format(e, logmsg))
            try:
                f, s, p = \
                    re.findall(r'FrameID:\s+(\d+),\s+SlotID:\s+(\d+),\s+PortID:\s+(\d+)', logmsg)[0]
                pon_history = PonAlarmRecord.query.filter_by(ip=host, frame=f, slot=s, port=p, ontid='PON').first()
                ontid = "PON"
            except Exception as e:
                logger.warning("split frame slot port error second time: {}, the msg is {}".format(e, logmsg))
                return False

        if pon_history:
            if fail:
                alarm_interval = datetime.timedelta(minutes=120)
                pon_history.last_fail_time = alert_time
                # 如果是PON口fail, 那么在计划任务中会再次检测PON口状态, 因此当写入新的syslog服务时,如果是fail 则status 为-1,
                # 表示此时失效,但是不确定
                pon_history.status = -1 if ontid == 'PON' else 0
                pon_history.fail_times += 1
            elif recovery:
                pon_history.last_recovery_time = alert_time
                pon_history.status = 1
            db.session.add(pon_history)
        else:
            if fail:
                ont_status = -1 if ontid == 'PON' else 0
                pon_new = PonAlarmRecord(device_name=device_name, ip=host,
                                         frame=f, slot=s, port=p, ontid=ontid, fail_times=1, status=ont_status,
                                         last_fail_time=alert_time, create_time=time.localtime())
                db.session.add(pon_new)

            elif recovery:
                pon_new = PonAlarmRecord(device_name=device_name, ip=host,
                                         frame=f, slot=s, port=p, ontid=ontid, fail_times=0, status=1,
                                         last_recovery_time=alert_time, create_time=time.localtime())

                db.session.add(pon_new)
        db.session.commit()
        db.session.expire_all()
        db.session.close()
    except Exception as e:
        logger.warning("Host {} log msg {} not match".format(host, logmsg))


def general_syslog_monitor(host, logmsg):
    """
    处理py_syslog发来的日志，处理除OLT外的日志
    :param host:
    :param logmsg:
    :return:
    """
    logger.debug("{} {}".format(host, logmsg))
    device = Device.query.filter_by(ip=host).first()
    device_name = device.device_name if device else 'None'
    n = logmsg.find('>')
    c = device_name + ' ' + host + \
        ' 产生syslog告警 ' + logmsg[26:] + '\n'

    AlarmPolicy.alarm(alarm_content=[c], alarm_type='2')
