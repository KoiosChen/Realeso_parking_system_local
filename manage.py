#!/usr/bin/env python
import os
import multiprocessing
from app import create_app, db
from flask_script import Manager
from flask_migrate import Migrate
from app.MyModule import py_syslog
from app.MyModule import SeqPickle, SchedulerControl
from app.MyModule import AllocateQueueWork

__author__ = 'Koios'

app = create_app(os.getenv('FLASK_CONFIG') or 'default')
manager = Manager(app)
migrate = Migrate(app, db)

# 启动syslog服务
syslog_process = multiprocessing.Process(target=py_syslog.py_syslog)
syslog_process.daemon = True
syslog_process.start()

# 启动调度程序
AllocateQueueWork.allocate_worker(thread_num=1)

# 检查许可, 如果传入的参数为'1', 则用户若删除licence.pkl文件, 每次重启服务都会产生一个新的licence.pkl文件, 并可以使用7天
init_status = '1'
SeqPickle.checkLicence(init_status)
if init_status == '0':
    # 如果init_status 是0, 表示默认不支持用户使用, 停止所有计划任务
    SchedulerControl.scheduler_pause()
else:
    # 根据数据库配置修改scheduler计划, 用户覆盖默认配置文件中的配置
    SchedulerControl.scheduler_modify()

if __name__ == '__main__':
    manager.run()
