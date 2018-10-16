from flask import session, render_template, request, jsonify, send_from_directory
from flask_login import login_required
from ..models import *
from ..decorators import permission_required
from .. import db, logger, scheduler
from . import main
from ..MyModule import OperateDutyArrange
import json
import os
import requests


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


@main.route('/', methods=['GET'])
@login_required
@permission_required(Permission.MAN_ON_DUTY)
def index():
    return render_template("/parking_cashier.html")


@main.route('/parking_cashier', methods=['GET'])
def parking_cashier():
    return render_template("/parking_cashier.html")


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


@main.route("/show_image/<string:filename>", methods=['GET'])
def show_image(filename):
    return render_template('show_image.html',
                           filename=filename)


@main.route("/data/<string:filename>", methods=['GET'])
@login_required
@permission_required(Permission.NETWORK_MANAGER)
def get_file(filename):
    return send_from_directory(os.path.join(UPLOAD_FOLDER), filename=filename)


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


@main.route('/gps_location', methods=['GET', 'POST'])
@login_required
@permission_required(Permission.NETWORK_MANAGER)
def gps_location():
    return render_template('GPS.html')


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
