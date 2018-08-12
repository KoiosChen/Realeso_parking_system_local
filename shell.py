#!/usr/bin/env python
import os
import multiprocessing
from app import create_app, db, scheduler, logger
from app.models import User, Role,Camera, DutyAttendedTime, DutySchedule, CONFIG_FILE_PATH
from flask_script import Manager, Shell
from flask_migrate import Migrate, MigrateCommand

from app.MyModule import AddDutyMember, OperateDutyArrange, WechatAlarm
from app.MyModule import SendMail, SeqPickle, SchedulerControl

__author__ = 'Koios'

app = create_app(os.getenv('FLASK_CONFIG') or 'default')
manager = Manager(app)
migrate = Migrate(app, db)


def make_shell_context():
    return dict(app=app, db=db, User=User, Role=Role, Device=Camera,
                DutyAttendedTime=DutyAttendedTime, DutySchedule=DutySchedule,
                AddDutyMember=AddDutyMember, OperateDutyArrange=OperateDutyArrange, WechatAlarm=WechatAlarm,
                sendmail=SendMail,
                SeqPickle=SeqPickle)


manager.add_command("shell", Shell(make_context=make_shell_context))
manager.add_command('db', MigrateCommand)


if __name__ == '__main__':
    manager.run()
