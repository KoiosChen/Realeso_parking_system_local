from flask import current_app
from . import db
from flask_login import UserMixin, AnonymousUserMixin
from werkzeug.security import generate_password_hash, check_password_hash
from . import login_manager
from datetime import time


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


class PermissionIP(db.Model):
    __tablename__ = 'permission_ip'
    ip = db.Column(db.String(20), primary_key=True)
    remarks = db.Column(db.String(200))
    create_time = db.Column(db.DateTime)


class ApiConfigure(db.Model):
    __tablename__ = 'api_configure'
    id = db.Column(db.Integer, primary_key=True)
    api_name = db.Column(db.String(20), nullable=False)
    api_params = db.Column(db.String(100), nullable=False)
    api_params_value = db.Column(db.String(200))


class ParkingLot(db.Model):
    __tablename__ = 'parking_lot'
    id = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.String(24), unique=True, nullable=False)
    address = db.Column(db.String(100), unique=True, nullable=False)
    floors = db.Column(db.String(10))
    floors_desc = db.Column(db.String(200))
    parking_space_totally = db.Column(db.Integer, nullable=False)
    free_minutes = db.Column(db.SmallInteger, nullable=False, default=15)
    start_minutes = db.Column(db.SmallInteger, nullable=False, default=60)
    pay_interval = db.Column(db.SmallInteger, nullable=False, default=30)
    effective_duration = db.Column(db.SmallInteger, nullable=False, default=20)
    status = db.Column(db.Integer, nullable=False, default=1)
    permit_value = db.Column(db.String(200))
    devices = db.relationship('Camera', backref='parking_lot')
    parking_lot_detail = db.relationship('ParkingLotDetail', backref='parking_lot_detail')

    def __repr__(self):
        return '<Parking lot %r>' % self.name


class ParkingLotDetail(db.Model):
    __tablename__ = 'parking_lot_detail'
    id = db.Column(db.Integer, primary_key=True)
    parking_lot_id = db.Column(db.Integer, db.ForeignKey('parking_lot.id'))
    name = db.Column(db.String(100), nullable=False, index=True)
    value = db.Column(db.String(100), nullable=False, index=True)

    def __repr__(self):
        return '<Parking lot detail for %r>' % self.name


class Camera(db.Model):
    __tablename__ = 'camera'
    device_number = db.Column(db.String(100), primary_key=True)
    device_name = db.Column(db.String(200), unique=True, nullable=False)
    device_type = db.Column(db.SmallInteger)
    parking_lot_id = db.Column(db.Integer, db.ForeignKey('parking_lot.id'))
    status = db.Column(db.Integer, nullable=False, default=1)
    vendor = db.Column(db.String(100))
    # 摄像头关联的闸机编号
    gate_id = db.Column(db.String(100))
    description = db.Column(db.String(200))
    parking_record_entry_camera = db.relationship('ParkingRecords', backref='entry_camera',
                                                  foreign_keys='ParkingRecords.entry_camera_id')
    parking_record_exit_camera = db.relationship('ParkingRecords', backref='exit_camera',
                                                 foreign_keys='ParkingRecords.exit_camera_id')

    def __repr__(self):
        return '<device name %r>' % self.device_name


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


class User(UserMixin, db.Model):
    __tablename__ = 'users'
    id = db.Column(db.Integer, primary_key=True)
    phoneNum = db.Column(db.String(15), unique=True, index=True)
    username = db.Column(db.String(64), index=True)
    role_id = db.Column(db.Integer, db.ForeignKey('roles.id'))
    duty = db.Column(db.SmallInteger, db.ForeignKey('job_desc.job_id'))
    password_hash = db.Column(db.String(128))
    status = db.Column(db.SmallInteger)
    cashier = db.relationship('ParkingRecords', backref='cashier')
    registrar = db.relationship('FixedParkingSpace', backref='registrar')
    blacklist = db.relationship('BlackList', backref='registrar')
    modifier = db.relation('NumberPlateModification', backref='modifier')

    def __init__(self, **kwargs):
        super(User, self).__init__(**kwargs)
        if self.role is None:
            if self.phoneNum == current_app.config['FLASKY_ADMIN']:
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


class Area(db.Model):
    __tablename__ = 'area'
    id = db.Column(db.Integer, primary_key=True)
    area_name = db.Column(db.String(30), index=True, nullable=False)
    area_desc = db.Column(db.String(200))
    area_parking_log = db.Column(db.String(200))

    def __repr__(self):
        return '<Area info: %r>' % self.area_name


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
    user_duty = db.relationship('User', backref='user_duty', lazy='dynamic')


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


class AnonymousUser(AnonymousUserMixin):
    def can(self, permissions):
        return False

    def is_administrator(self):
        return False


class ParkingOrder(db.Model):
    """
    从互联网模块获取数据，然后将获取到的UUID作为list post给互联网端 uuid, number_plate, order_type, order_validate_start, order_validate_stop,reserved, status, create_time, update_time, discount
    """
    __tablename__ = 'parking_orders'

    # 从互联网模块获取的订单号
    uuid = db.Column(db.String(50), primary_key=True)

    # 车牌
    number_plate = db.Column(db.String(10), index=True, nullable=False)

    # 1: 常租（常租信息在本地), 2: 包月, 3: 预约单次, 4: 充值消费, 5: 折扣
    order_type = db.Column(db.SmallInteger, index=True, nullable=False)

    # 订单的有效时长，例如常租用户、包月用户的有效期，是一个长期的订单。其它的，如单次的预约订单，则为订单当天的日期范围。
    # 免费停车、单次预约等单次停车的车辆，每个订单均单独一条记录
    order_validate_start = db.Column(db.DateTime, nullable=False, index=True)
    order_validate_stop = db.Column(db.DateTime, nullable=False, index=True)

    # 是否需要常租车位。包月、预约单次为1， 充值消费、折扣为0
    reserved = db.Column(db.SmallInteger, nullable=False, index=True, default=0)

    # 1 有效； 2 失效  本地模块生成字段
    status = db.Column(db.SmallInteger, index=True, default=1)

    # 订单创建时间， 本地模块生成字段
    create_time = db.Column(db.DateTime, nullable=False, index=True)

    # 订单状态更新时间， 本地模块生成字段
    update_time = db.Column(db.DateTime, nullable=False, index=True)

    # 默认不需要互联网模块提供，默认是1， 就是该时段全免；如果是打折，例如8折，则专递0.2
    discount = db.Column(db.Float, default=1.0)

    order_and_record = db.relationship('OrderAndRecords', backref='order')

    fixed = db.relation('FixedParkingSpace', backref='fixed_order')


class ParkingRecords(db.Model):
    """
    记录进出场信息. 如果parking_order_id为空，则表示是临时停车。
    parking_order_id, 可在进场的时候不填，如果出场的时候获取订单记录，特别是免费停车的记录等，则按照订单进行收费
    fee 用于存放此次停车消费金额，免费停车、临时停车不可为空（ >= 0）
    """
    __tablename__ = 'parking_records'
    uuid = db.Column(db.String(50), primary_key=True)

    number_plate = db.Column(db.String(10), index=True, nullable=False)

    entry_time = db.Column(db.DateTime, index=True, nullable=False)
    entry_camera_id = db.Column(db.String(100), db.ForeignKey('camera.device_number'))
    # 何时写入目前未定
    entry_pic = db.Column(db.String(100), index=True)
    entry_plate_number_pic = db.Column(db.String(100), index=True)
    entry_unit_price = db.Column(db.Float, index=True, nullable=False, default=10)

    # 出场填写
    exit_time = db.Column(db.DateTime, index=True, nullable=True)
    # 何时写入目前未定
    exit_pic = db.Column(db.String(100), index=True)
    exit_plate_number_pic = db.Column(db.String(100), index=True)
    exit_camera_id = db.Column(db.String(100), db.ForeignKey('camera.device_number'))

    # 出场填写
    cashier_id = db.Column(db.Integer, db.ForeignKey('users.id'))
    fee = db.Column(db.Float)

    # 订单状态 1 出场 0 未出场
    status = db.Column(db.SmallInteger, default=0, nullable=False)

    # 0、 未匹配；
    # XY, X : {1: '岗亭'，2: '服务中心', 3: '电子支付', 4:'来源摄像头'}  Y: {0: '表示来源，无操作', 1: '付费未出场', 2: '开闸', 3: '异常'}
    # X 来源于用户权限
    operate_source = db.Column(db.SmallInteger, default=0)

    exit_validate_before = db.Column(db.DateTime)

    create_time = db.Column(db.DateTime)

    record_and_order = db.relationship('OrderAndRecords', backref='record')


class OrderAndRecords(db.Model):
    __tablename__ = 'order_and_records'
    id = db.Column(db.Integer, primary_key=True)
    record_id = db.Column(db.String(50), db.ForeignKey('parking_records.uuid'), index=True)
    order_id = db.Column(db.String(50), db.ForeignKey('parking_orders.uuid'), index=True)

    def __repr__(self):
        return '<Order and Records: %r>' % self.id


class FixedParkingSpace(db.Model):
    """
    每次登记产生一条新的记录，如果车牌有旧记录，则新记录的开始时间不能早于最后一条老记录的结束时间
    """
    __tablename__ = 'fixed_parking_space'
    uuid = db.Column(db.String(50), primary_key=True)

    # 指定停车位编号
    specified_parking_space_code = db.Column(db.String(10))
    company = db.Column(db.String(100))
    # 租用地址
    room = db.Column(db.String(20))

    order_uuid = db.Column(db.String(50), db.ForeignKey('parking_orders.uuid'), index=True, unique=True)

    status = db.Column(db.SmallInteger, default=1)

    # 登记员名字从users表获取
    registrar_id = db.Column(db.Integer, db.ForeignKey('users.id'))
    create_time = db.Column(db.DateTime)


class BlackList(db.Model):
    __tablename__ = 'black_list'
    uuid = db.Column(db.String(50), primary_key=True)
    number_plate = db.Column(db.String(20), index=True, nullable=False)
    reason = db.Column(db.String(200))
    order_validate_start = db.Column(db.DateTime, nullable=False, index=True)
    order_validate_stop = db.Column(db.DateTime, nullable=False, index=True)
    # 登记员名字从users表获取
    registrar_id = db.Column(db.Integer, db.ForeignKey('users.id'))
    create_time = db.Column(db.DateTime)


class ParkingAbnormalExitRecords(db.Model):
    __tablename__ = 'parking_abnormal_exit_records'
    uuid = db.Column(db.String(50), primary_key=True)
    number_plate = db.Column(db.String(20), index=True, nullable=False)
    camera_id = db.Column(db.String(100), db.ForeignKey('camera.device_number'), nullable=False)
    exit_time = db.Column(db.DateTime, nullable=False)
    exit_pic = db.Column(db.String(100), nullable=False)
    exit_plate_number_pic = db.Column(db.String(100), nullable=False)
    create_time = db.Column(db.DateTime)


class NumberPlateModification(db.Model):
    __tablename__ = 'number_plate_modification'
    uuid = db.Column(db.String(50), primary_key=True)
    # type 0 入场记录， 1 出场记录
    type = db.Column(db.SmallInteger)
    relate_record_id = db.Column(db.String(50))
    before_number = db.Column(db.String(20))
    after_number = db.Column(db.String(20))
    # 登记员名字从users表获取
    registrar_id = db.Column(db.Integer, db.ForeignKey('users.id'))
    create_time = db.Column(db.DateTime)


login_manager.anonymous_user = AnonymousUser
PATH_PREFIX = '/Users/Peter/python/r2d2/app/'
CONFIG_FILE_PATH = PATH_PREFIX + 'config_file/'
UPLOAD_FOLDER = PATH_PREFIX + 'UploadFile/'
CACTI_PIC_FOLDER = PATH_PREFIX + '/static/cacti_pic/'

# 凡是有订单信息的，包括常租固定车位、包月等都属于订单park，无任何订单记录的是临时停车
PARKING_TYPE = {1: '临时',
                2: '订单',
                3: '固定',
                4: '非固定'}

# 夜间停车配置，从晚上19点到次日7点30
NIGHT_PARKING = {'start': time(19, 00, 00), 'end': time(7, 30, 0)}

# 法定假日停车时间配置，从早上7点30至晚间19点
STATUTORY_HOLIDAY = {'start': time(7, 30, 0), 'end': time(19, 00, 00)}

DEVICE_TYPE = {11: '入口闸机',
               12: '出口闸机',
               21: '入口摄像头',
               22: '出口摄像头'}

duty_schedule_status = {1: '正常',
                        2: '调休',
                        3: '事假',
                        4: '调班',
                        # 5: '加班',
                        6: '管理员删除',
                        7: '新增'}

aes_key = 'koiosr2d2c3p0000'

EXIT_GATE = 'g002'
