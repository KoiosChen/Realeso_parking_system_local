#!/usr/bin/env python
import os
import multiprocessing
from app import create_app, db, scheduler, logger
from app.models import OntAccountInfo, Syslog, User, Role, MachineRoom, Device, AlarmRecord, DutyAttendedTime, DutySchedule, CONFIG_FILE_PATH
from flask_script import Manager, Shell
from flask_migrate import Migrate, MigrateCommand
from app.r2d2.CactiMonitor import *
from app.r2d2.ScheduleWorker import *
from app.r2d2.UpsMonitor import *
from app.r2d2.SyslogWorker import *
from app.MyModule import AlarmPolicy, PhoneNumber, AddDutyMember, OperateDutyArrange, WechatAlarm, GetCactiPic
from app.MyModule import SendMail, SeqPickle, SchedulerControl, requestVerboseInfo, Snmp, SnmpHW

__author__ = 'Koios'

app = create_app(os.getenv('FLASK_CONFIG') or 'production')
manager = Manager(app)
migrate = Migrate(app, db)


def make_shell_context():
    return dict(app=app, db=db, User=User, Role=Role, MachineRoom=MachineRoom, Device=Device,
                cacti=cacti_db_monitor, ups=ups_monitor,
                polling=unalarmed_polling, pon=pon_state_check, today=pon_alarm_in_time_range,
                alarm=AlarmPolicy, AlarmRecord=AlarmRecord, Snmp=Snmp, SnmpHW=SnmpHW,
                DutyAttendedTime=DutyAttendedTime, DutySchedule=DutySchedule, PhoneNumber=PhoneNumber,
                AddDutyMember=AddDutyMember, OperateDutyArrange=OperateDutyArrange, WechatAlarm=WechatAlarm,
                GetCactiPic=GetCactiPic, ont_alarm_in_time_range=lots_ont_losi_alarm, sendmail=SendMail,
                SeqPickle=SeqPickle, Syslog=Syslog, OntAccountInfo=OntAccountInfo, requestOntInfo=requestVerboseInfo)


manager.add_command("shell", Shell(make_context=make_shell_context))
manager.add_command('db', MigrateCommand)


if __name__ == '__main__':
    manager.run()
