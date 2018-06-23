from flask_wtf import Form
from flask import session
from wtforms.validators import DataRequired, Email, Length, Regexp, EqualTo, IPAddress, Optional, NumberRange
from wtforms import StringField, SubmitField, PasswordField, SelectField, SelectMultipleField, DateTimeField, \
    RadioField, TextAreaField
from flask_pagedown.fields import PageDownField
from ..models import Role, Area, JobDescription
from ..my_func import get_machine_room_by_area, get_device_name
from flask_wtf.file import FileField, FileAllowed, FileRequired


class BaseForm(Form):
    machine_room_name = SelectField(label='请选择上联机房:')
    mac = StringField('请输入ONU MAC地址:', validators=[DataRequired(),
                                                   Regexp('^[0-9a-fA-F]{4}\-[0-9a-fA-F]{4}\-[0-9a-fA-F]{4}',
                                                          0, '无效的MAC地址')])

    customer_number = StringField('请输入用户账号', validators=[DataRequired()])
    submit = SubmitField('提交')

    def __init__(self):
        super(BaseForm, self).__init__()
        self.machine_room_name.choices = get_machine_room_by_area(session.get('permit_machine_room'))


class DeviceForm(Form):
    machine_room_name = SelectField(label='请选择上联机房:')
    device_name = StringField('device name', validators=[DataRequired()])
    ip = StringField('IP', validators=[DataRequired(), IPAddress()])
    login_name = StringField('Login Name', default='monitor')
    login_password = PasswordField('Login Password', default='shf-k61-906')
    enable_password = PasswordField('Enable Password')
    status = SelectField('Status', choices=[('1', '1'), ('2', '2'), ('3', '3')])
    submit = SubmitField('submit')

    def __init__(self):
        super(DeviceForm, self).__init__()
        print(session.get('permit_machine_room'))
        self.machine_room_name.choices = get_machine_room_by_area(session.get('permit_machine_room'))


class AreaConfigForm(Form):
    area_name = StringField('请输入大区名称:', validators=[DataRequired()])
    area_machine_room = SelectMultipleField('请选择可管辖机房', validators=[DataRequired()])
    submit = SubmitField('提交')

    def __init__(self):
        super(AreaConfigForm, self).__init__()
        self.area_machine_room.choices = get_machine_room_by_area(session.get('permit_machine_room'))


class ManualSync(Form):
    device_name = SelectMultipleField(label='请选择OLT设备:')
    sync_action = RadioField(label='请选择同步内容:', choices=[('1', '同步CEVLAN'), ('2', '同步SERVICE PORT'),
                                                        ('3', '同步ONU基础信息'), ('4', '同步ONU光衰及下线原因'),
                                                        ('5', 'MAC LEARNED BY ONU')])
    submit = SubmitField('开始同步')

    def __init__(self):
        super(ManualSync, self).__init__()
        self.device_name.choices = get_device_name()


class UserModal(Form):
    duty_choice = [(jd.job_id, jd.job_name) for jd in JobDescription.query.all()]

    username = StringField('用户名', validators=[Regexp('^[\u4E00-\u9FA5]*$', 0,
                                                     '用户名只能为中文 ')])
    password = PasswordField('请输入密码')
    phone_number = StringField('电话', validators=[NumberRange(1, 9, '仅数字')])
    role = SelectField('请选择角色', default='0')
    area = SelectField('所属大区', default='0')
    machine_room_name = SelectMultipleField('请选择可管理的机房:')
    duty = SelectField('职务', choices=duty_choice, default='0')

    def __init__(self):
        super(UserModal, self).__init__()
        role_c = [(str(k.id), k.name) for k in Role.query.all()]
        role_c.append(('0', None))
        self.role.choices = role_c
        self.machine_room_name.choices = get_machine_room_by_area(session.get('permit_machine_room'))
        a = [(str(a.id), a.area_name) for a in Area.query.all()]
        a.append(('0', None))
        self.area.choices = a


class AreaModal(Form):
    area_name = StringField('大区名:')
    area_desc = StringField('大区描述:')
    machine_room_name = SelectMultipleField('请选择可管理的机房:')

    def __init__(self):
        super(AreaModal, self).__init__()
        self.machine_room_name.choices = get_machine_room_by_area(session.get('permit_machine_room'))


class PostForm(Form):
    body = PageDownField('故障说明', validators=[DataRequired()])


class UploadForm(Form):
    file = FileField('选择文件上传（pcap, pcapng）',
                     validators=[FileAllowed(['pcap', 'pcapng'], '只能上传抓包文件'), FileRequired('文件未选择')])
    submit = SubmitField('上传')