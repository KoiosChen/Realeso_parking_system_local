from flask import current_app
from . import db
from flask_login import UserMixin, AnonymousUserMixin
from werkzeug.security import generate_password_hash, check_password_hash
from . import login_manager
import datetime
from markdown import markdown
import bleach


@login_manager.user_loader
def load_user(user_id):
    return User.query.get(int(user_id))


class Permission:
    FOLLOW = 0x01
    COMMENT = 0x02
    WRITE_ARTICLES = 0x04
    MODERATE_COMMENTS = 0x08
    REGION_SUPPORT = 0x10
    MAN_ON_DUTY = 0x20
    NETWORK_MANAGER = 0x40
    ADMINISTER = 0x80


class MachineRoom(db.Model):
    __tablename__ = 'machineroom_list'
    id = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.String(24), unique=True, nullable=False)
    address = db.Column(db.String(100), unique=True, nullable=False)
    level = db.Column(db.Integer, nullable=True)
    status = db.Column(db.Integer, nullable=False, default=1)
    permit_value = db.Column(db.String(200))
    devices = db.relationship('Device', backref='machine_room')

    def __repr__(self):
        return '<Machine Room %r>' % self.name


class Device(db.Model):
    __tablename__ = 'device_list'
    id = db.Column(db.Integer, primary_key=True)
    device_name = db.Column(db.String(200), unique=True, nullable=False)
    ip = db.Column(db.String(16), nullable=False)
    login_name = db.Column(db.String(20))
    login_password = db.Column(db.String(20))
    machine_room_id = db.Column(db.Integer, db.ForeignKey('machineroom_list.id'))
    enable_password = db.Column(db.String(20), nullable=True)
    status = db.Column(db.Integer, nullable=False, default=1)
    community = db.Column(db.String(20), index=True)
    monitor_status = db.Column(db.SmallInteger)
    monitor_fail_date = db.Column(db.DateTime)
    monitor_rec_date = db.Column(db.DateTime)
    mib_model = db.Column(db.SmallInteger)
    vendor = db.Column(db.String(100))

    def __repr__(self):
        return '<device name %r>' % self.device_name


class SnmpInterface(db.Model):
    __tablename__ = 'snmp_interface'
    id = db.Column(db.Integer, primary_key=True)
    device_id = db.Column(db.Integer, index=True)
    snmp_sysname = db.Column(db.String(100))
    snmp_if_desc = db.Column(db.String(50), index=True)
    snmp_if_alias = db.Column(db.String(200))
    snmp_if_physical_status = db.Column(db.SmallInteger)
    snmp_if_protocal_status = db.Column(db.SmallInteger)
    snmp_last_down_time = db.Column(db.DateTime)
    snmp_last_rec_time = db.Column(db.DateTime)
    snmp_last_in_speed = db.Column(db.Float)
    snmp_last_out_speed = db.Column(db.Float)
    data_storage = db.Column(db.String(100), default='redis')
    data_path = db.Column(db.String(100), default='10')
    snmp_last_fetch_status = db.Column(db.String(100), default='Success')
    update_time = db.Column(db.DateTime)


class SnmpModels(db.Model):
    __tablename__ = 'snmp_models'
    id = db.Column(db.Integer, primary_key=True)
    vendor = db.Column(db.String(100), index=True)
    device_type = db.Column(db.String(100))
    oid_name = db.Column(db.String(50))
    oid = db.Column(db.String(100))


class Role(db.Model):
    __tablename__ = 'roles'
    id = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.String(64), unique=True)
    default = db.Column(db.Boolean, default=False, index=True)
    permissions = db.Column(db.Integer)
    users = db.relationship('User', backref='role', lazy='dynamic')

    @staticmethod
    def insert_roles():
        roles = {
            'User': (Permission.FOLLOW |
                     Permission.COMMENT |
                     Permission.WRITE_ARTICLES, True),
            'Moderator': (Permission.FOLLOW |
                          Permission.COMMENT |
                          Permission.WRITE_ARTICLES |
                          Permission.MODERATE_COMMENTS, False),
            'REGION': (Permission.FOLLOW |
                          Permission.COMMENT |
                          Permission.WRITE_ARTICLES |
                          Permission.MODERATE_COMMENTS |
                       Permission.REGION_SUPPORT, False),
            'MAN_ON_DUTY': (Permission.FOLLOW |
                       Permission.COMMENT |
                       Permission.WRITE_ARTICLES |
                       Permission.MODERATE_COMMENTS |
                       Permission.REGION_SUPPORT |
                            Permission.MAN_ON_DUTY, False),
            'SNOC': (Permission.FOLLOW |
                     Permission.COMMENT |
                     Permission.WRITE_ARTICLES |
                     Permission.MODERATE_COMMENTS |
                     Permission.REGION_SUPPORT |
                     Permission.MAN_ON_DUTY |
                     Permission.NETWORK_MANAGER, False),
            'Administrator': (0xff, False)
        }
        for r in roles:
            role = Role.query.filter_by(name=r).first()
            if role is None:
                role = Role(name=r)
            role.permissions = roles[r][0]
            role.default = roles[r][1]
            db.session.add(role)
        db.session.commit()

    def __repr__(self):
        return '<Role %r>' % self.name


class Post(db.Model):
    __tablename__ = 'posts'
    id = db.Column(db.Integer, primary_key=True)
    body = db.Column(db.Text)
    alarm_id = db.Column(db.Integer)
    timestamp = db.Column(db.DateTime, index=True, default=datetime.datetime.now)
    author_id = db.Column(db.Integer)
    body_html = db.Column(db.Text)

    @staticmethod
    def on_changed_body(target, value, oldvalue, initiator):
        allowed_tags = ['a', 'abbr', 'acronym', 'b', 'blockquote', 'code',
                        'em', 'i', 'li', 'ol', 'pre', 'strong', 'ul',
                        'h1', 'h2', 'h3', 'p', 'img']
        attrs = {
            '*': ['class'],
            'a': ['href', 'rel'],
            'img': ['src', 'alt', 'width', 'height'],
        }
        target.body_html = bleach.linkify(bleach.clean(
            markdown(value, output_format='html'),
            tags=allowed_tags, attributes=attrs, strip=True))

db.event.listen(Post.body, 'set', Post.on_changed_body)


class ApiConfigure(db.Model):
    __tablename__ = 'api_configure'
    id = db.Column(db.Integer, primary_key=True)
    api_name = db.Column(db.String(20), nullable=False)
    api_params = db.Column(db.String(100), nullable=False)
    api_params_value = db.Column(db.String(200))


class User(UserMixin, db.Model):
    __tablename__ = 'users'
    id = db.Column(db.Integer, primary_key=True)
    email = db.Column(db.String(64), unique=True, index=True)
    phoneNum = db.Column(db.String(15), unique=True)
    username = db.Column(db.String(64), index=True)
    role_id = db.Column(db.Integer, db.ForeignKey('roles.id'))
    area = db.Column(db.Integer)
    duty = db.Column(db.Integer)
    permit_machine_room = db.Column(db.String(200))
    password_hash = db.Column(db.String(128))
    status = db.Column(db.SmallInteger)

    def __init__(self, **kwargs):
        super(User, self).__init__(**kwargs)
        if self.role is None:
            if self.email == current_app.config['FLASKY_ADMIN']:
                self.role = Role.query.filter_by(permissions=0xff).first()
            if self.role is None:
                self.role = Role.query.filter_by(default=True).first()

    @property
    def password(self):
        raise AttributeError('password is not a readable attribute')

    @password.setter
    def password(self, password):
        self.password_hash = generate_password_hash(password)

    def verify_password(self, password):
        return check_password_hash(self.password_hash, password)

    def can(self, permissions):
        return self.role is not None and \
               (self.role.permissions & permissions) == permissions

    def is_administrator(self):
        return self.can(Permission.ADMINISTER)

    def is_moderate(self):
        return self.can(Permission.MODERATE_COMMENTS)

    def is_region(self):
        return self.can(Permission.REGION_SUPPORT)

    def is_manonduty(self):
        return self.can(Permission.MAN_ON_DUTY)

    def is_snoc(self):
        return self.can(Permission.NETWORK_MANAGER)

    def __repr__(self):
        return '<User %r>' % self.username


class AccountInfo(db.Model):
    __tablename__ = 'account'
    id = db.Column(db.Integer, primary_key=True)
    username = db.Column(db.String(30), nullable=False, index=True)
    password = db.Column(db.String(40))
    interface = db.Column(db.String(10), index=True)
    sub_int = db.Column(db.String(5), index=True)
    ip = db.Column(db.String(20))
    mac = db.Column(db.String(30), index=True)
    bas_name = db.Column(db.String(20))
    create_time = db.Column(db.DateTime)
    update_time = db.Column(db.DateTime)

    def __repr__(self):
        return '<ACCOUNT INFO -> USERNAME: %r>' % self.username


class Log(db.Model):
    __tablename__ = 'log'
    id = db.Column(db.Integer, primary_key=True)
    operator = db.Column(db.String(64), index=True, nullable=False)
    machine_room_id = db.Column(db.Integer, nullable=False)
    mac = db.Column(db.String(28), nullable=False)
    customer_number = db.Column(db.String(64), nullable=False)
    type = db.Column(db.SmallInteger, nullable=False)
    create_time = db.Column(db.DateTime, nullable=False)


class AreaMachineRoom(db.Model):
    __tablename__ = 'area_machine_room'
    id = db.Column(db.Integer, primary_key=True)
    area_id = db.Column(db.Integer, index=True, nullable=False)
    permit_machine_room = db.Column(db.Integer, nullable=False)


class Area(db.Model):
    __tablename__ = 'area'
    id = db.Column(db.Integer, primary_key=True)
    area_name = db.Column(db.String(30), index=True, nullable=False)
    area_desc = db.Column(db.String(200))
    area_machine_room = db.Column(db.String(200))

    def __repr__(self):
        return '<Area info: %r>' % self.area_name


class SyncEvent(db.Model):
    __tablename__ = 'sync_event'
    event_id = db.Column(db.Integer, primary_key=True)
    sub_id = db.Column(db.Integer, index=True)
    sync_func = db.Column(db.String(100), index=True)
    sync_device = db.Column(db.Integer)
    start_time = db.Column(db.DateTime)
    stop_time = db.Column(db.DateTime)
    sync_status = db.Column(db.SmallInteger)
    remark = db.Column(db.String(200), nullable=True)

    def __reduce__(self):
        return '<SyncEvent INFO -> %r>' % self.id + ' ' + self.sync_func


class CallRecordDetail(db.Model):
    __tablename__ = 'call_record_detail'
    id = db.Column(db.Integer, primary_key=True)
    phoneNum = db.Column(db.String(20), index=True)
    respCode = db.Column(db.String(10), index=True)
    callId = db.Column(db.String(40), index=True)
    createDateInResp = db.Column(db.String(15), index=True)
    create_time = db.Column(db.DateTime)
    call_group = db.Column(db.String(32), index=True)


class VoiceNotifyCallBack(db.Model):
    __tablename__ = 'voice_notify_callback'
    id = db.Column(db.Integer, primary_key=True)
    phoneNum = db.Column(db.String(15), index=True)
    state = db.Column(db.String(5))
    callId = db.Column(db.String(40))
    create_time = db.Column(db.DateTime)


class DutySchedule(db.Model):
    __tablename__ = 'duty_schedule'
    id = db.Column(db.Integer, primary_key=True)
    date_time = db.Column(db.Date)
    userid = db.Column(db.String(100), nullable=False)
    attended_time_id = db.Column(db.SmallInteger, nullable=False)
    duty_status = db.Column(db.SmallInteger, nullable=False)
    priority = db.Column(db.SmallInteger, nullable=False, default=0)
    create_time = db.Column(db.DateTime)

    def __repr__(self):
        return '<Duty schedule date: %r>' % self.date_time


class JobDescription(db.Model):
    __tablename__ = 'job_desc'
    job_id = db.Column(db.SmallInteger, primary_key=True)
    job_name = db.Column(db.String(20))
    job_desc = db.Column(db.String(100))
    alarm_type = db.Column(db.String(20))


class DutyAttendedTime(db.Model):
    __tablename__ = 'duty_attended_time'
    id = db.Column(db.Integer, primary_key=True)
    attended_time_name = db.Column(db.String(64), nullable=False)
    start_time = db.Column(db.Time, nullable=False)
    stop_time = db.Column(db.Time, nullable=False)
    day_adjust = db.Column(db.SmallInteger, nullable=True, default=0)
    status = db.Column(db.SmallInteger, nullable=False)
    create_time = db.Column(db.DateTime, nullable=False)

    def __repr__(self):
        return '<Duty attended time name: %r>' % self.attended_time_name


class TokenRecord(db.Model):
    __tablename__ = 'token_record'
    unique_id = db.Column(db.String(128), primary_key=True)
    token = db.Column(db.String(512), nullable=False)
    expire = db.Column(db.String(10))
    create_time = db.Column(db.DateTime)


class AlarmRecord(db.Model):
    __tablename__ = 'alarm_record'
    id = db.Column(db.Integer, primary_key=True)
    content = db.Column(db.String(4096))
    content_md5 = db.Column(db.String(32), index=True)
    alarm_type = db.Column(db.SmallInteger, nullable=True, default=0)
    alarm_level = db.Column(db.SmallInteger, nullable=True, default=1)
    state = db.Column(db.SmallInteger)
    lastCallId = db.Column(db.String(128), index=True)
    calledTimes = db.Column(db.SmallInteger)
    create_time = db.Column(db.DateTime)
    call_group = db.Column(db.String(32), index=True)


class UpsInfo(db.Model):
    __tablename__ = 'ups_info'
    id = db.Column(db.Integer, primary_key=True)
    oid_dict = db.Column(db.String(200))
    ip = db.Column(db.String(24))
    name = db.Column(db.String(20))
    vendeor = db.Column(db.String(20))
    community = db.Column(db.String(20))


class PonAlarmRecord(db.Model):
    __tablename__ = 'pon_alarm_record'
    id = db.Column(db.Integer, primary_key=True)
    device_name = db.Column(db.String(100))
    ip = db.Column(db.String(64))
    frame = db.Column(db.String(2))
    slot = db.Column(db.String(2))
    port = db.Column(db.String(3))
    ontid = db.Column(db.String(3), default='PON')
    fail_times = db.Column(db.Integer)
    status = db.Column(db.SmallInteger)
    last_fail_time = db.Column(db.DateTime)
    last_recovery_time = db.Column(db.DateTime)
    alarmed_flag = db.Column(db.SmallInteger, default=0)
    create_time = db.Column(db.DateTime)


class CallTimeRange(db.Model):
    __tablename__ = 'call_time_range'
    id = db.Column(db.Integer, primary_key=True)
    range_name = db.Column(db.String(100))
    start_time = db.Column(db.Time, nullable=False)
    stop_time = db.Column(db.Time, nullable=False)
    day_adjust = db.Column(db.Integer, default=0)
    valid_alarm_type = db.Column(db.String(20))
    status = db.Column(db.SmallInteger, default=1)


class SyslogAlarmConfig(db.Model):
    __tablename__ = 'syslog_alarm_config'
    id = db.Column(db.Integer, primary_key=True)
    alarm_type = db.Column(db.String(10))
    alarm_name = db.Column(db.String(100))
    alarm_level = db.Column(db.String(10))
    alarm_status = db.Column(db.SmallInteger)
    alarm_keyword = db.Column(db.String(100))
    create_time = db.Column(db.DateTime)


class Syslog(db.Model):
    __tablename__ = 'syslog'
    id = db.Column(db.Integer, primary_key=True)
    facility = db.Column(db.String(20))
    serverty = db.Column(db.String(20))
    device_ip = db.Column(db.String(40))
    logmsg = db.Column(db.String(600))
    logtime = db.Column(db.DateTime)


class SyslogEmerg(db.Model):
    __tablename__ = 'syslog_emerg'
    id = db.Column(db.Integer, primary_key=True)
    facility = db.Column(db.String(20))
    serverty = db.Column(db.String(20))
    device_ip = db.Column(db.String(40))
    logmsg = db.Column(db.String(600))
    logtime = db.Column(db.DateTime)


class SyslogAlert(db.Model):
    __tablename__ = 'syslog_alert'
    id = db.Column(db.Integer, primary_key=True)
    facility = db.Column(db.String(20))
    serverty = db.Column(db.String(20))
    device_ip = db.Column(db.String(40))
    logmsg = db.Column(db.String(600))
    logtime = db.Column(db.DateTime)


class SyslogCrit(db.Model):
    __tablename__ = 'syslog_crit'
    id = db.Column(db.Integer, primary_key=True)
    facility = db.Column(db.String(20))
    serverty = db.Column(db.String(20))
    device_ip = db.Column(db.String(40))
    logmsg = db.Column(db.String(600))
    logtime = db.Column(db.DateTime)


class SyslogErr(db.Model):
    __tablename__ = 'syslog_err'
    id = db.Column(db.Integer, primary_key=True)
    facility = db.Column(db.String(20))
    serverty = db.Column(db.String(20))
    device_ip = db.Column(db.String(40))
    logmsg = db.Column(db.String(600))
    logtime = db.Column(db.DateTime)


class SyslogWarn(db.Model):
    __tablename__ = 'syslog_warn'
    id = db.Column(db.Integer, primary_key=True)
    facility = db.Column(db.String(20))
    serverty = db.Column(db.String(20))
    device_ip = db.Column(db.String(40))
    logmsg = db.Column(db.String(600))
    logtime = db.Column(db.DateTime)


class SyslogNotice(db.Model):
    __tablename__ = 'syslog_notice'
    id = db.Column(db.Integer, primary_key=True)
    facility = db.Column(db.String(20))
    serverty = db.Column(db.String(20))
    device_ip = db.Column(db.String(40))
    logmsg = db.Column(db.String(600))
    logtime = db.Column(db.DateTime)


class SyslogInfo(db.Model):
    __tablename__ = 'syslog_info'
    id = db.Column(db.Integer, primary_key=True)
    facility = db.Column(db.String(20))
    serverty = db.Column(db.String(20))
    device_ip = db.Column(db.String(40))
    logmsg = db.Column(db.String(600))
    logtime = db.Column(db.DateTime)


class SyslogDebug(db.Model):
    __tablename__ = 'syslog_debug'
    id = db.Column(db.Integer, primary_key=True)
    facility = db.Column(db.String(20))
    serverty = db.Column(db.String(20))
    device_ip = db.Column(db.String(40))
    logmsg = db.Column(db.String(600))
    logtime = db.Column(db.DateTime)


class OntAccountInfo(db.Model):
    # 用途改为存放附加告警信息
    __tablename__ = 'ont_account_info'
    id = db.Column(db.Integer, primary_key=True)
    hash_id = db.Column(db.String(256), nullable=False)
    account_info = db.Column(db.String(256))


class PiRegister(db.Model):
    __tablename__ = 'pi_register'
    sysid = db.Column(db.String(100), primary_key=True)
    username = db.Column(db.String(50), index=True, nullable=False)
    times = db.Column(db.Integer, default=0)
    last_register_time = db.Column(db.DateTime)
    status = db.Column(db.SmallInteger)

    def __repr__(self):
        return '<Pi Register: %r>' % self.sysid


class PcapOrder(db.Model):
    __tablename__ = 'pcap_order'
    id = db.Column(db.String(128), primary_key=True)
    account_id = db.Column(db.String(20), index=True, nullable=False)
    login_name = db.Column(db.String(50), index=True, nullable=False)
    username = db.Column(db.String(20), index=True, nullable=False)
    question_description = db.Column(db.String(1024))
    # status:
    # 0->this user hasn't bind to a Pi
    # 1->just created
    # 2->processing
    # 3->recapture
    # 4->order finished
    status = db.Column(db.SmallInteger, default=1)
    create_time = db.Column(db.DateTime)


class PcapResult(db.Model):
    __tablename__ = 'pcap_result'
    id = db.Column(db.String(128), primary_key=True)
    sysid = db.Column(db.String(100), index=True)
    pcap_order_id = db.Column(db.String(128), index=True)
    pcap_filepath = db.Column(db.String(200))
    r2d2_filepath = db.Column(db.String(200))
    result_description = db.Column(db.String(1024))
    speedtest = db.Column(db.String(50))
    pingtest = db.Column(db.String(100))
    create_time = db.Column(db.DateTime)


class AnonymousUser(AnonymousUserMixin):
    def can(self, permissions):
        return False

    def is_administrator(self):
        return False

login_manager.anonymous_user = AnonymousUser
PATH_PREFIX = '/Users/Peter/python/r2d2/app/'
CONFIG_FILE_PATH = PATH_PREFIX + 'config_file/'
UPLOAD_FOLDER = PATH_PREFIX + 'UploadFile/'
CACTI_PIC_FOLDER = PATH_PREFIX + '/static/cacti_pic/'


alarm_record_state = {1: '呼叫失败',
                      2: '未定义',
                      3: '被叫未接听',
                      8: '已达最大呼叫次数, 并且未接听',
                      9: '呼叫成功',
                      99: '未呼叫'}


duty_schedule_status = {1: '正常',
                        2: '调休',
                        3: '事假',
                        4: '调班',
                        # 5: '加班',
                        6: '管理员删除',
                        7: '新增'}

ALLOWED_EXTENSIONS = set(['pcap', 'pcapng'])

syslog_serverty = {0: "emergency",
                   1: "alert",
                   2: "critical",
                   3: "error",
                   4: "warning",
                   5: "notice",
                   6: "info",
                   7: "debug"
                 }
syslog_facility = {0: "kernel",
                   1: "user",
                   2: "mail",
                   3: "daemaon",
                   4: "auth",
                   5: "syslog",
                   6: "lpr",
                   7: "news",
                   8: "uucp",
                   9: "cron",
                   10: "authpriv",
                   11: "ftp",
                   12: "ntp",
                   13: "security",
                   14: "console",
                   15: "cron",
                   16: "local 0",
                   17: "local 1",
                   18: "local 2",
                   19: "local 3",
                   20: "local 4",
                   21: "local 5",
                   22: "local 6",
                   23: "local 7"
                 }
aes_key = 'koiosr2d2c3p0000'
max_ont_down_in_sametime = 4

temp_threshold = {'min': 20, 'max': 30}
humi_threshold = {'min': 15, 'max': 70}
