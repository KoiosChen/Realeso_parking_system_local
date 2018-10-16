#!/usr/bin/env python
import os
from app import create_app, db, logger, redis_db
from flask_script import Manager
from flask_migrate import Migrate
from app.MyModule import SeqPickle, AllocateQueueWork
from OpenSSL import SSL
from app.hardwareModule.init_sdk import do_init, init_overtime, register_callback, login_camera, set_alarm
import sys

__author__ = 'Koios'

app = create_app(os.getenv('FLASK_CONFIG') or 'default')
manager = Manager(app)
migrate = Migrate(app, db)

# 检查许可, 如果传入的参数为'1', 则用户若删除licence.pkl文件, 每次重启服务都会产生一个新的licence.pkl文件, 并可以使用7天
init_status = '1'
SeqPickle.checkLicence(init_status)
if init_status == '0':
    # 如果init_status 是0, 表示默认不支持用户使用, 停止所有计划任务
    # SchedulerControl.scheduler_pause()
    pass
else:
    # 根据数据库配置修改scheduler计划, 用户覆盖默认配置文件中的配置
    # SchedulerControl.scheduler_modify()
    pass

# 初始化摄像头
logger.info('Start to init hik sdk')
init_flag = False
try:
    assert do_init(), "sdk初始化失败"
    assert init_overtime(), "初始化超时时间失败"
    assert register_callback(), "注册回调函数失败"
    init_flag = True
except Exception as e:
    logger.error(e)
    redis_db.set('hardware_sdk_init_status', 0)

# 登陆摄像头并且布防
if init_flag:
    try:
        assert login_camera(), "登陆摄像头失败"
        # 布防并且配置道闸参数
        lUserIDs = set_alarm()
        if not lUserIDs:
            logger.error("布防失败")
            redis_db.set('hardware_sdk_work_status', 0)
        else:
            redis_db.set('hardware_sdk_work_status', 1)
    except Exception as e:
        logger.error(e)
        redis_db.set('hardware_sdk_work_status', 0)
else:
    redis_db.set('hardware_sdk_work_status', 0)

# 启动调度程序
AllocateQueueWork.allocate_worker(thread_num=10)


if __name__ == '__main__':
    manager.run()
