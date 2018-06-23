from flask import redirect, session, url_for, render_template, flash, request, jsonify, send_from_directory
from flask_login import login_required
from ..models import *
from ..decorators import permission_required
from .. import db, logger, scheduler
from .forms import AreaConfigForm, AreaModal
from . import main
import time
from ..MyModule import OperateDutyArrange
from ..MyModule.UploadFile import uploadfile
from werkzeug.utils import secure_filename
import json
from bs4 import BeautifulSoup
import datetime
import os
import re
import requests


def get_device_info(machine_room_id):
    """
    :param machine_room_id:
    :return:
    """
    device_info = Device.query.filter_by(machine_room_id=machine_room_id).all()
    logger.debug('device list: {} '.format(device_info))
    return device_info if device_info else False


def allowed_file(filename):
    return '.' in filename and \
           filename.rsplit('.', 1)[1].lower() in ALLOWED_EXTENSIONS


def gen_file_name(filename, path=UPLOAD_FOLDER):
    """
    If file was exist already, rename it and return a new name
    """

    i = 1
    while os.path.exists(os.path.join(path, filename)):
        name, extension = os.path.splitext(filename)
        filename = '%s_%s%s' % (name, str(i), extension)
        i += 1

    return filename


IGNORED_FILES = set(['.gitignore'])


@main.route('/', methods=['GET', 'POST'])
@login_required
@permission_required(Permission.MAN_ON_DUTY)
def index():
    return render_template("/dashboard.html")


@main.route('/print_duty_schedule', methods=['GET', 'POST'])
@login_required
@permission_required(Permission.MAN_ON_DUTY)
def print_duty_schedule():
    logger.info('User {} is checking duty schedule'.format(session['LOGINNAME']))

    return render_template('print_duty_schedule.html',
                           duty_schedule_status=duty_schedule_status)


@main.route('/print_duty_schedule_api', methods=['POST'])
@login_required
@permission_required(Permission.MAN_ON_DUTY)
def print_duty_schedule_api():
    logger.debug('selected_month')
    params = request.get_data()
    jl = params.decode('utf-8')
    j = json.loads(jl)
    print(int(j.get('selected_month')))
    title, duty_arrangement = OperateDutyArrange.print_duty_schedule(check_year=2017,
                                                                     check_month=int(j.get('selected_month')))

    posted_body = {'title': title,
                   'duty_arrangement': duty_arrangement,
                   }

    return jsonify(json.dumps(posted_body, ensure_ascii=False))


@main.route('/upload_duty', methods=['GET', 'POST'])
@login_required
@permission_required(Permission.ADMINISTER)
def upload_duty():
    logger.info('User {} is uploading duty schedule'.format(session['LOGINNAME']))

    if request.method == 'POST':
        files = request.files['file']

        if files:
            filename = secure_filename(files.filename)
            filename = gen_file_name(filename)
            mime_type = files.content_type

            if not allowed_file(files.filename):
                result = uploadfile(name=filename, type=mime_type, size=0, not_allowed_msg="File type not allowed")

            else:
                # save file to disk
                uploaded_file_path = os.path.join(UPLOAD_FOLDER, filename)
                files.save(uploaded_file_path)

                # create thumbnail after saving
                if mime_type.startswith('image'):
                    pass

                # get file size after saving
                size = os.path.getsize(uploaded_file_path)

                # return json for js call back
                result = uploadfile(name=filename, type=mime_type, size=size)

            return json.dumps({"files": [result.get_file()]}, ensure_ascii=False)

    if request.method == 'GET':
        # get all file in ./data directory
        files = [f for f in os.listdir(UPLOAD_FOLDER) if
                 os.path.isfile(os.path.join(UPLOAD_FOLDER, f)) and f not in IGNORED_FILES]

        file_display = []

        for f in files:
            size = os.path.getsize(os.path.join(UPLOAD_FOLDER, f))
            file_saved = uploadfile(name=f, size=size)
            file_display.append(file_saved.get_file())

        return json.dumps({"files": file_display}, ensure_ascii=False)

    return redirect(url_for('upload_duty_schedule'))


@main.route("/show_image/<string:filename>", methods=['GET'])
def show_image(filename):
    return render_template('show_image.html',
                           filename=filename)


@main.route("/data/<string:filename>", methods=['GET'])
@login_required
@permission_required(Permission.NETWORK_MANAGER)
def get_file(filename):
    return send_from_directory(os.path.join(UPLOAD_FOLDER), filename=filename)


@main.route('/upload_duty_schedule', methods=['GET', 'POST'])
@login_required
@permission_required(Permission.ADMINISTER)
def upload_duty_schedule():
    return render_template('upload_duty_schedule.html')


@main.route("/delete/<string:filename>", methods=['DELETE'])
@login_required
@permission_required(Permission.ADMINISTER)
def delete(filename):
    file_path = os.path.join(UPLOAD_FOLDER, filename)

    if os.path.exists(file_path):
        try:
            os.remove(file_path)

            return json.dumps({filename: 'True'})
        except:
            return json.dumps({filename: 'False'})


@main.route("/import_duty/<string:filename>", methods=['GET'])
@login_required
@permission_required(Permission.ADMINISTER)
def import_duty(filename):
    file_path = os.path.join(UPLOAD_FOLDER, filename)
    logger.debug('file_path is {}'.format(file_path))

    if os.path.exists(file_path):
        try:
            logger.debug('do job')
            title, row_list = OperateDutyArrange.read_duty_arrange(file_path, 'Sheet1', True)
            import_result = OperateDutyArrange.add_duty_arrange(title, row_list)
            return jsonify(json.dumps({'status': import_result}, ensure_ascii=False))
        except:
            return jsonify(json.dumps({'status': '系统繁忙'}, ensure_ascii=False))


@main.route('/all_user', methods=['POST'])
@login_required
@permission_required(Permission.MAN_ON_DUTY)
def all_user():
    logger.info('User {} is getting all user dictionary'.format(session['LOGINNAME']))
    all_user = User.query.filter(User.status.__eq__(1), User.phoneNum.__ne__(None)).all()
    all_user_dict = {}
    for user in all_user:
        all_user_dict[user.id] = {'username': user.username, 'phoneNum': user.phoneNum}

    return jsonify(json.dumps(all_user_dict))


@main.route('/check_appointed_time_duty_engineer', methods=['POST'])
@login_required
@permission_required(Permission.MAN_ON_DUTY)
def check_appointed_time_duty_engineer():
    params = request.get_data()
    jl = params.decode('utf-8')
    j = json.loads(jl)
    print(j)

    logger.info('User {} is checking appointed time duty engineer'.format(session['LOGINNAME']))
    start_time, stop_time = re.findall(r'(\d+:\d+)--(\d+:\d+)', j.get('selected_duty_time'))[0]
    duty_attended_time_id = DutyAttendedTime.query.filter_by(start_time=start_time, stop_time=stop_time).first()
    duty_engineer = DutySchedule.query.filter_by(
        date_time=datetime.datetime.strptime(j.get('selected_date'), '%Y-%m-%d'),
        attended_time_id=duty_attended_time_id.id).all()

    r_json = {}
    for e in duty_engineer:
        username = User.query.get(e.userid)
        r_json[e.userid] = {'username': username.username,
                            'phoneNum': username.phoneNum,
                            'duty_status': duty_schedule_status[e.duty_status]}

    r = json.dumps(r_json, ensure_ascii=False)

    return jsonify(r)


@main.route('/operate_duty_arrange', methods=['POST'])
@login_required
@permission_required(Permission.ADMINISTER)
def operate_duty_arrange():
    params = request.get_data()
    jl = params.decode('utf-8')
    j = json.loads(jl)
    logger.debug(j)

    logger.info('User {} is operate duty arrange'.format(session['LOGINNAME']))

    result = OperateDutyArrange.change_duty_schedule_status(**j)

    r_json = {'status': result}

    r = json.dumps(r_json, ensure_ascii=False)

    return jsonify(r)


@main.route('/call_callback', methods=['POST'])
def callback():
    logger.debug('callback')
    resp = request.get_data()
    logger.debug(resp)
    soup = BeautifulSoup(resp, 'lxml')
    logger.info('call to {} status {}'.format(soup.called.string, soup.state.string))
    callback = VoiceNotifyCallBack(phoneNum=str(soup.called.string),
                                   state=str(soup.state.string),
                                   callId=str(soup.callid.string),
                                   create_time=time.localtime())

    db.session.add(callback)
    db.session.commit()

    call_record = CallRecordDetail.query.filter_by(callId=str(soup.callid.string)).first()

    alarm_record = AlarmRecord.query.filter_by(call_group=call_record.call_group).all()

    for r in alarm_record:

        # update state in the alarm record
        if soup.state.string == '0':
            r.state = 9
        else:
            r.state = 3

        db.session.add(r)
    db.session.commit()

    return '', 200


@main.route('/areainfo_update', methods=['GET', 'POST'])
@login_required
@permission_required(Permission.NETWORK_MANAGER)
def areainfo_update():
    area_id = request.form.get('area_id')
    area_name = request.form.get('area_name')
    area_desc_list = []
    area_machine_room = request.form.get('machine_room_name')
    logger.debug('area_name {} machine_room {}'.format(area_name, area_machine_room))
    if area_machine_room != 'null':
        area_machine_room = area_machine_room.split(',')
    logger.debug(area_machine_room)

    areainfo_tobe_updated = Area.query.filter_by(id=area_id).first()

    if areainfo_tobe_updated.area_machine_room == '0xffffffffff':
        return redirect(url_for('.area_config', update_result=3))

    if area_name:
        areainfo_tobe_updated.area_name = area_name

    if area_machine_room != 'null':
        logger.debug('area_machine_room {}'.format(area_machine_room))
        permit_machineroom = 0
        for mr in area_machine_room:
            permit_value = MachineRoom.query.filter_by(id=mr).first()
            area_desc_list.append(permit_value.name)
            if permit_value:
                permit_machineroom |= int(permit_value.permit_value, 16)

        areainfo_tobe_updated.area_machine_room = hex(permit_machineroom)

        logger.info('The hex of the permitted machine room is {}'.format(hex(permit_machineroom)))

        area_desc = ','.join(area_desc_list)
        areainfo_tobe_updated.area_desc = area_desc

    if area_name or area_machine_room:
        try:
            db.session.add(areainfo_tobe_updated)
            db.session.commit()
            logger.info('update area info successful')
            update_result = 1
        except Exception as e:
            logger.error(e)
            update_result = 2
    else:
        update_result = 4

    return redirect(url_for('.area_config', update_result=update_result))


@main.route('/gps_location', methods=['GET', 'POST'])
@login_required
@permission_required(Permission.NETWORK_MANAGER)
def gps_location():
    return render_template('GPS.html')


@main.route('/test', methods=['GET', 'POST'])
def test():
    if request.method == 'GET':
        return render_template('test.html')
    elif request.method == 'POST':
        print(request.form.get('length'))
        recordsTotal = len(AlarmRecord.query.all())
        recordsFiltered = recordsTotal
        print(recordsTotal)
        draw = request.form.get('draw')
        print(draw)
        print(request.form.get('start'))
        page_start = int(request.form.get('start', '0'))
        page_end = page_start + int(request.form.get('length'))
        data = [[str(ui.id), ui.content, ui.create_time] for ui in AlarmRecord.query.all()]
        rest = {'draw': int(draw),
                'recordsTotal': recordsTotal,
                'recordsFiltered': recordsFiltered,
                'data': data[page_start:page_end],
                }
        return jsonify(rest)


@main.route('/modify_scheduler_server', methods=['POST'])
def modify_scheduler_server():
    PermissionIP = ['127.0.0.1']
    if request.headers.get('X-Forwarded-For', request.remote_addr) not in PermissionIP:
        return jsonify(json.dumps({'status': 'Permission deny'}))
    logger.info('User {} is modifying scheduler configuration'.format(session['LOGINNAME']))
    params = request.get_data()
    jl = params.decode('utf-8')
    j = json.loads(jl)

    scheduler_id = j.get('scheduler_id')
    interval = float(j.get('interval').strip())
    try:
        if interval:
            scheduler.pause_job(id=scheduler_id)
            scheduler.modify_job(id=scheduler_id, trigger='interval', seconds=interval)
            scheduler.resume_job(id=scheduler_id)
            return jsonify(json.dumps({'status': '%s OK' % scheduler_id}))
        else:
            scheduler.pause_job(id=scheduler_id)
            return jsonify(json.dumps({'status': '%s 暂停' % scheduler_id}))
    except Exception as e:
        return jsonify(json.dumps({'status': '%s 提交计划任务失败' % scheduler_id}))


@main.route('/modify_scheduler', methods=['POST'])
@login_required
@permission_required(Permission.ADMINISTER)
def modify_scheduler():
    logger.info('User {} is modifying scheduler configuration'.format(session['LOGINNAME']))
    ms_url = "http://127.0.0.1:54322/modify_scheduler_server"
    params = request.get_data()

    headers = {
        'Accept': 'application/json',
        'Content-Type': 'application/json;charset=utf-8',
    }

    r = requests.post(ms_url, data=params, headers=headers)
    print(r.text)
    return r.text


@main.route('/update_callapi_config', methods=['POST'])
@login_required
@permission_required(Permission.NETWORK_MANAGER)
def update_callapi_config():
    """
    此处默认权限为NETWORK_MANAGER,这样会造成网络管理员直接访问此路径能够具备修改微信接口和语音接口参数的权限,是个漏洞,需要修改
    :return:
    """
    logger.info('User {} is updating callapi configuration'.format(session['LOGINNAME']))
    params = request.get_data()
    jl = params.decode('utf-8')
    j = json.loads(jl)
    print(j)

    api_params = j.get('api_params').strip()
    api_params_value = j.get('api_params_value').strip()
    print(api_params_value)

    update_api_params = ApiConfigure.query.filter_by(api_name='callapi', api_params=api_params).first()

    update_api_params.api_params_value = api_params_value
    db.session.add(update_api_params)
    db.session.commit()

    return jsonify(json.dumps({'status': 'OK'}))


@main.route('/area_config', methods=['GET', 'POST'])
@login_required
@permission_required(Permission.NETWORK_MANAGER)
def area_config():
    logger.info('User {} is checking area list'.format(session['LOGINNAME']))
    page = request.args.get('page', 1, type=int)
    update_result = request.args.get('update_result')
    flash_message = {1: '大区信息修改成功', 2: '大区信息修改失败', 3: '无权修改大区信息', 4: '未修改信息'}

    if update_result:
        session['update_result'] = update_result

    if session.get('update_result') and not update_result:
        flash(flash_message[int(session['update_result'])])
        session['update_result'] = ''
    form = AreaConfigForm()
    modal_form = AreaModal()

    if form.validate_on_submit():
        logger.info('User {} is configuring the machine room included in area {}'
                    .format(session['LOGINNAME'], form.area_machine_room.data, form.area_name.data))

        machine_room_list = form.area_machine_room.data
        permit_machineroom = 0
        area_desc_list = []
        for mr in machine_room_list:
            permit_value = MachineRoom.query.filter_by(id=mr).first()
            area_desc_list.append(permit_value.name)
            if permit_value:
                permit_machineroom |= int(permit_value.permit_value, 16)

        area_desc = ','.join(area_desc_list)
        logger.info('The hex of the permitted machine room is {}'.format(permit_machineroom))
        try:
            insert_area = Area(area_name=form.area_name.data,
                               area_desc=area_desc,
                               area_machine_room=hex(permit_machineroom))
            db.session.add(insert_area)
            db.session.commit()
            logger.info('Area config successful')
            flash('大区添加成功')
        except Exception as e:
            logger.error('config area fail for {}'.format(e))
            flash('插入数据失败')
        return redirect(url_for('.area_config'))

    POSTS_PER_PAGE = 10

    if page < 1:
        page = 1
    paginate = Area.query.order_by(Area.id).paginate(page, POSTS_PER_PAGE, False)

    object_list = paginate.items

    return render_template('area_config.html',
                           pagination=paginate,
                           object_list=object_list,
                           POSTS_PER_PAGE=POSTS_PER_PAGE,
                           form=form,
                           modal_form=modal_form)
