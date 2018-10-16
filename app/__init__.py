from flask import Flask
from flask_bootstrap import Bootstrap
from flask_mail import Mail
from flask_moment import Moment
from flask_sqlalchemy import SQLAlchemy
from flask_login import LoginManager
from config import config
from flask_apscheduler import APScheduler
from flask_session import Session
from flask_pagedown import PageDown
import logging
import redis
from collections import defaultdict
import queue
from flask_sqlalchemy import SQLAlchemy as SQLAlchemyBase
from sqlalchemy.pool import NullPool
from flask_socketio import SocketIO
import sys
import ctypes
from .hardwareModule.plate_struct_class import *
import platform
sys_info = platform.system()
if sys_info == "Linux":
    from fdfs_client.client import *
    fdfs_client = Fdfs_client('/etc/fdfs/client.conf')
else:
    fdfs_client = False


def nesteddict():
    """
    构造一个嵌套的字典
    :return:
    """
    return defaultdict(nesteddict)


class SQLAlchemy(SQLAlchemyBase):
    def apply_driver_hacks(self, app, info, options):
        super(SQLAlchemy, self).apply_driver_hacks(app, info, options)
        options['poolclass'] = NullPool
        options.pop('pool_size', None)


# 用于存放监控记录信息，例如UPS前序状态，需要配置持久化
redis_db = redis.Redis(host='localhost', port=6379, db=7)

bootstrap = Bootstrap()
mail = Mail()
moment = Moment()
db = SQLAlchemy()
login_manager = LoginManager()
scheduler = APScheduler()
sess = Session()
pagedown = PageDown()
socketio = SocketIO()
login_manager.session_protection = 'strong'
login_manager.login_view = 'auth.login'

# 用于处理订单建议书的队列
work_q = queue.Queue(maxsize=100)

logging.basicConfig(format='%(asctime)s %(message)s', datefmt='%m/%d/%Y %H:%M:%S')
logger = logging.getLogger()
hdlr = logging.FileHandler("log.txt")
formatter = logging.Formatter(fmt='%(asctime)s - %(module)s-%(funcName)s - %(levelname)s - %(message)s',
                              datefmt='%m/%d/%Y %H:%M:%S')
hdlr.setFormatter(formatter)
logger.addHandler(hdlr)
logger.setLevel(logging.DEBUG)

p_msg_cb_func = []

def create_app(config_name):
    app = Flask(__name__)
    app.config.from_object(config[config_name])
    config[config_name].init_app(app)

    bootstrap.init_app(app)
    sess.init_app(app)
    mail.init_app(app)
    moment.init_app(app)
    db.app = app
    db.init_app(app)
    db.create_scoped_session()
    login_manager.init_app(app)
    scheduler.init_app(app)
    scheduler.start()
    pagedown.init_app(app)
    socketio.init_app(app)

    from .main import main as main_blueprint
    app.register_blueprint(main_blueprint)

    from .auth import auth as auth_blueprint
    app.register_blueprint(auth_blueprint, url_prefix='/auth')

    return app
