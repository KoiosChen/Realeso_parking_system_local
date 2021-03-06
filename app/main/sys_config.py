from flask import redirect, session, url_for, render_template, flash, request, jsonify
from flask_login import login_required
from ..models import *
from ..decorators import permission_required
from .. import db, logger
from .forms import DeviceForm, UserModal
from . import main
import time
from ..MyModule.GetConfig import get_config
from ..MyModule.SeqPickle import get_pubkey, update_crypted_licence
from ..my_func import get_machine_room_by_area
import json
from sqlalchemy import or_


@main.route('/parameterSetting', methods=['GET', 'POST'])
@login_required
@permission_required(Permission.NETWORK_MANAGER)
def parameterSetting():
    wechat_config = get_config('wechat')
    callapi_config = get_config('callapi')
    scheduler_config = get_config('scheduler')
    alarmpolicy_config = get_config('alarmpolicy')
    cacti_config = get_config('Cacti')

    return render_template('parameterSetting.html',
                           wechat_config=wechat_config,
                           callapi_config=callapi_config,
                           scheduler_config=scheduler_config,
                           alarmpolicy_config=alarmpolicy_config,
                           cacti_config=cacti_config)


@main.route('/parking_lot_management', methods=['GET', 'POST'])
@login_required
@permission_required(Permission.NETWORK_MANAGER)
def parking_lot_management():
    return True


@main.route('/parking_lot_add', methods=['POST'])
@login_required
@permission_required(Permission.NETWORK_MANAGER)
def parking_lot_add():
    params = request.get_data()
    jl = params.decode('utf-8')
    j = json.loads(jl)
    print(j)
    machine_room_name = j.get('machine_room_name')
    machine_room_address = j.get('machine_room_address')
    machine_room_level = j.get('machine_room_level')

    if ParkingLot.query.filter(or_(ParkingLot.name.__eq__(machine_room_name),
                                   ParkingLot.address.__eq__(machine_room_address))).all():
        return jsonify(json.dumps({'status': 'False'}))
    else:
        last_machine_room = ParkingLot.query.order_by(ParkingLot.id.desc()).first()
        if last_machine_room:
            permit_value = hex(int(last_machine_room.permit_value, 16) << 1)
        else:
            permit_value = hex(1)
        new_machine_room = ParkingLot(name=machine_room_name,
                                      address=machine_room_address,
                                      level=machine_room_level,
                                      status='1',
                                      permit_value=permit_value)
        db.session.add(new_machine_room)
        db.session.commit()
        return jsonify(json.dumps({'status': 'OK'}))


@main.route('/parking_lot_delete', methods=['POST'])
@login_required
@permission_required(Permission.NETWORK_MANAGER)
def parking_lot_delete():
    params = request.get_data()
    jl = params.decode('utf-8')
    j = json.loads(jl)
    print(j)
    sc_id = j.get('sc_id')

    delete_target = ParkingLot.query.filter_by(id=sc_id).first()
    delete_target.status = '0'

    if delete_target:
        db.session.add(delete_target)
        db.session.commit()
        return jsonify(json.dumps({'status': 'OK'}))
    else:
        return jsonify(json.dumps({'status': 'False'}))


@main.route('/devices_management', methods=['GET', 'POST'])
@login_required
@permission_required(Permission.NETWORK_MANAGER)
def devices_management():
    status_dict = {'0': '已删除', '1': '运行中', '2': '待定'}
    machine_dict = {m.id: m.name for m in ParkingLot.query.filter(ParkingLot.status.__ne__('0')).all()}
    print(machine_dict)
    if request.method == 'GET':
        logger.info('User {} is checking device list'.format(session['LOGINNAME']))
        machine_room_list = [{'id': m.id, 'name': m.name} for m in
                             ParkingLot.query.filter(ParkingLot.status.__ne__('0')).all()]
        return render_template('devices_management.html', machine_room_list=machine_room_list)
    elif request.method == 'POST':
        page_start = (int(request.form.get('datatable[pagination][page]', '0')) - 1) * 10
        length = int(request.form.get('datatable[pagination][perpage]'))

        data = []
        for sc in Camera.query.filter(Camera.machine_room_id.isnot(None)).order_by(Camera.id).offset(page_start).limit(
                length):
            print(sc)
            print(ParkingLot.query.filter_by(id=sc.machine_room_id).first())
            if sc.machine_room_id in machine_dict and ParkingLot.query.filter_by(
                    id=sc.machine_room_id).first().status != 0:
                data.append({'id': sc.id,
                             'device_name': sc.device_name,
                             'device_ip': sc.ip,
                             'machine_room': machine_dict[sc.machine_room_id],
                             'device_status': status_dict[str(sc.status)],
                             })

        recordsTotal = Camera.query.filter(Camera.machine_room_id.isnot(None)).count()
        print(recordsTotal)

        rest = {
            "meta": {
                "page": int(request.form.get('datatable[pagination][page]')),
                "pages": int(recordsTotal) / int(length),
                "perpage": int(length),
                "total": int(recordsTotal),
                "sort": "asc",
                "field": "ShipDate"
            },
            "data": data
        }
        return jsonify(rest)


@main.route('/device_add', methods=['GET', 'POST'])
@login_required
@permission_required(Permission.NETWORK_MANAGER)
def device_add():
    data = request.json
    print(data)
    device_name = data.get('device_name')
    device_ip = data.get('device_ip')
    machine_room = data.get('machine_room')

    try:
        device = Camera(device_name=device_name,
                        ip=device_ip,
                        login_name='monitor',
                        login_password='shf-k61-906',
                        enable_password='',
                        machine_room=ParkingLot.query.filter_by(id=machine_room).first(),
                        status='1')
        db.session.add(device)
        db.session.commit()
        logger.info('User {} add device {}  in machine room {} successful'.
                    format(session.get('LOGINNAME'), device_name,
                           ParkingLot.query.filter_by(id=machine_room).first()))
        return jsonify({'status': 'OK', 'content': "设备添加成功"})
    except Exception as e:
        # 但是此处不能捕获异常
        logger.error('User {} add device {}  in machine room {} fail, because {}'.
                     format(session.get('LOGINNAME'), device_name,
                            ParkingLot.query.filter_by(id=machine_room).first(), e))
        db.session.rollback()
        return jsonify({'status': 'False', 'content': "设备添加失败"})


@main.route('/device_delete', methods=['POST'])
@login_required
@permission_required(Permission.NETWORK_MANAGER)
def device_delete():
    params = request.get_data()
    jl = params.decode('utf-8')
    j = json.loads(jl)
    print(j)
    sc_id = j.get('sc_id')

    delete_target = Camera.query.filter_by(id=sc_id).first()
    delete_target.status = '0'

    if delete_target:
        db.session.add(delete_target)
        db.session.commit()
        return jsonify(json.dumps({'status': 'OK'}))
    else:
        return jsonify(json.dumps({'status': 'False'}))


@main.route('/licence_control', methods=['GET', 'POST'])
@login_required
@permission_required(Permission.NETWORK_MANAGER)
def licence_control():
    expire_date, expire_in, pubkey = get_pubkey()
    expire_date = time.strftime('%Y-%m-%d', time.localtime(expire_date))
    pubkey = pubkey.replace('\n', '\r\n')
    return render_template('licence_control.html',
                           expire_date=expire_date,
                           expire_in=expire_in,
                           pubkey=pubkey)


@main.route('/params_config', methods=['GET', 'POST'])
@login_required
@permission_required(Permission.NETWORK_MANAGER)
def params_config():
    wechat_config = get_config('wechat')
    callapi_config = get_config('callapi')
    scheduler_config = get_config('scheduler')
    alarmpolicy_config = get_config('alarmpolicy')
    cacti_config = get_config('Cacti')

    return render_template('params_config.html',
                           wechat_config=wechat_config,
                           callapi_config=callapi_config,
                           scheduler_config=scheduler_config,
                           alarmpolicy_config=alarmpolicy_config,
                           cacti_config=cacti_config)


@main.route('/update_config', methods=['POST'])
@login_required
@permission_required(Permission.NETWORK_MANAGER)
def update_wechat_config():
    params = request.get_data()
    jl = params.decode('utf-8')
    j = json.loads(jl)
    logger.info('User {} is updating api configuration {}'.format(session['LOGINNAME'], j))

    api_params = j.get('api_params').strip()
    api_params_value = j.get('api_params_value').strip()
    api_name = j.get('api_name').strip()
    print(api_params_value)

    update_api_params = ApiConfigure.query.filter_by(api_name=api_name, api_params=api_params).first()

    update_api_params.api_params_value = api_params_value
    db.session.add(update_api_params)
    db.session.commit()

    return jsonify(json.dumps({'status': 'OK'}))


@main.route('/update_licence', methods=['POST'])
@login_required
@permission_required(Permission.ADMINISTER)
def update_licence():
    if request.form.get('new_licence') and update_crypted_licence(request.form.get('new_licence')):
        expire_date, expire_in, pubkey = get_pubkey()
        expire_date = time.strftime('%Y-%m-%d', time.localtime(expire_date))
        pubkey = pubkey.replace('\n', '\r\n')
        return jsonify(
            json.dumps({'status': 'OK', 'expire_date': expire_date, 'expire_in': expire_in, 'pubkey': pubkey}))
    else:
        return jsonify(json.dumps({'status': 'FAIL'}))


@main.route('/user_register', methods=['POST'])
@login_required
@permission_required(Permission.NETWORK_MANAGER)
def user_register():
    logger.debug('register new user')
    data = request.json
    username = data.get('username')
    phone = data.get('phone')
    password = data.get('password')
    role_select = data.get('role_select')
    duty_select = data.get('duty_select')
    print(username, phone, password, role_select, duty_select)

    try:
        user_role = Role.query.filter_by(id=role_select).first()
        user = User(username=username,
                    phoneNum=phone,
                    password=password,
                    role=user_role,
                    duty=duty_select,
                    status=1)

        db.session.add(user)
        db.session.commit()
        logger.info('User {} register success'.format(username))
        return jsonify({'status': 'ok', 'content': "用户添加成功"})
    except Exception as e:
        logger.error('user register {} fail for {}'.format(username, e))
        db.session.rollback()
        return jsonify({'status': 'fail', 'content': "用户添加失败，请联系网管"})


@main.route('/user_management', methods=['GET', 'POST'])
@login_required
@permission_required(Permission.NETWORK_MANAGER)
def user_management():
    if request.method == 'GET':
        logger.info('User {} is checking user list'.format(session['LOGINNAME']))
        role = [(str(k.id), k.name) for k in Role.query.all()]
        duty_choice = [(str(jd.job_id), jd.job_name) for jd in JobDescription.query.all()]

        return render_template('user_management.html', role=role, duty_choice=duty_choice)

    elif request.method == 'POST':
        page_start = (int(request.form.get('datatable[pagination][page]', '0')) - 1) * 10
        length = int(request.form.get('datatable[pagination][perpage]'))

        print(page_start, length)

        data = [{'id': u.id,
                 'username': u.username,
                 'phonenum': u.phoneNum,
                 'role': u.role.name,
                 'duty': u.user_duty.job_name
                 }
                for u in User.query.filter(User.status.__eq__(1)).order_by(User.id).offset(page_start).limit(length)]

        recordsTotal = User.query.filter_by(status=1).count()

        rest = {
            "meta": {
                "page": int(request.form.get('datatable[pagination][page]')),
                "pages": int(recordsTotal) / int(length),
                "perpage": int(length),
                "total": int(recordsTotal),
                "sort": "asc",
                "field": "ShipDate"
            },
            "data": data
        }
        return jsonify(rest)


@main.route('/userinfo_update', methods=['POST'])
@login_required
@permission_required(Permission.FOLLOW)
def userinfo_update():
    password = request.form.get('pass')
    username = request.form.get('username')
    area = request.form.get('area')
    role = request.form.get('role')
    duty = request.form.get('duty')
    id = request.form.get('id')
    phone_number = request.form.get('phone_number')

    logger.info('User {} is update {}\'s info'.format(session['LOGINNAME'], id))
    logger.debug('p{password} u{username} a{area} r{role} d{duty} i{id} p{phone_number}'.format_map(vars()))
    print(type(role))
    flag = False
    if id == session.get('SELFID') or Role.query.filter_by(id=session['ROLE']).first().permissions >= 127:
        userinfo_tobe_changed = User.query.filter_by(id=id).first()

        if password:
            userinfo_tobe_changed.password = password
            flag = True
            print('password')
            print(userinfo_tobe_changed.password_hash)
        if username != userinfo_tobe_changed.username:
            userinfo_tobe_changed.username = username
            flag = True
            print('username')
        if area != 'null' and area != userinfo_tobe_changed.area:
            userinfo_tobe_changed.area = area
            flag = True
            print('area')
        if role != 'null' and role != userinfo_tobe_changed.role:
            userinfo_tobe_changed.role_id = role
            flag = True
            print('role')
        if duty != 'null' and duty != userinfo_tobe_changed.duty:
            userinfo_tobe_changed.duty = duty
            flag = True
            print('duty')
        if phone_number != userinfo_tobe_changed.phoneNum:
            userinfo_tobe_changed.phoneNum = phone_number
            flag = True
            print('phone_number')

        if flag:
            try:
                db.session.add(userinfo_tobe_changed)
                db.session.commit()
                logger.info('Userinfo of user id {} is changed'.format(id))
                return jsonify({"status": "ok", "content": "更新成功"})
            except Exception as e:
                logger.error('Userinfo change fail: {}'.format(e))
                db.session.rollback()
                return jsonify({"status": "fail", "content": "数据更新失败，可能存在数据冲突"})
        else:
            return jsonify({"status": "fail", "content": "无更新"})
    else:
        logger.info('This user do not permitted to alter user info')
        return jsonify({"status": "fail", "content": "无权限更新"})


@main.route('/user_delete', methods=['POST'])
@login_required
@permission_required(Permission.FOLLOW)
def user_delete():
    data = request.json
    user_id = data.get('id')
    user_tobe_deleted = User.query.filter_by(id=user_id).first()
    logger.debug('User {} is deleting user {}'.format(session['LOGINNAME'], user_tobe_deleted.username))
    if user_tobe_deleted.phoneNum == session['LOGINUSER']:
        return jsonify({'status': 'fail', 'content': '无权操作'})
    else:
        if Role.query.filter_by(id=session['ROLE']).first().permissions < Role.query.filter_by(
                name='SNOC').first().permissions:
            logger.debug(session['ROLE'])
            return jsonify({'status': 'fail', 'content': '无权操作'})
        else:
            logger.info('try to delete {}:{}:{}'
                        .format(user_tobe_deleted.id, user_tobe_deleted.username, user_tobe_deleted.phoneNum))
            try:
                db.session.delete(user_tobe_deleted)
                db.session.commit()
                logger.info('user is deleted')
                return jsonify({'status': 'true', 'content': '用户删除成功'})
            except Exception as e:
                logger.error('Delete user fail:{}'.format(e))
                return jsonify({'status': 'false', 'content': '用户删除失败'})


@main.route('/user_update', methods=['POST'])
@login_required
@permission_required(Permission.FOLLOW)
def user_update():
    data = request.json
    logger.debug(str(data))

    username = data.get('username_update')
    phone_number = data.get('phone_update')
    role = data.get('role_update')
    duty = data.get('duty_update')
    password = data.get('password_update')

    logger.info('User {} is update {}\'s info'.format(session['LOGINNAME'], id))
    logger.debug('p{password} u{username} r{role} d{duty} p{phone_number}'.format_map(vars()))
    print(type(role))
    flag = False
    if phone_number == session['LOGINUSER'] or Role.query.filter_by(id=session['ROLE']).first().permissions >= 127:
        userinfo_tobe_changed = User.query.filter_by(phoneNum=phone_number).first()

        if password:
            userinfo_tobe_changed.password = password
            flag = True
            logger.debug('new password is {}.'.format(userinfo_tobe_changed.password_hash))
        if username and username != userinfo_tobe_changed.username:
            userinfo_tobe_changed.username = username
            flag = True
            logger.debug('new username is {}'.format(username))
        if role and role != userinfo_tobe_changed.role:
            userinfo_tobe_changed.role_id = role
            flag = True
            logger.debug('new role is {}'.format(role))
        if duty and duty != userinfo_tobe_changed.duty:
            userinfo_tobe_changed.duty = duty
            flag = True
            logger.debug('new duty is {}'.format(duty))

        if flag:
            try:
                db.session.add(userinfo_tobe_changed)
                db.session.commit()
                logger.info('Userinfo of user id {} is changed'.format(id))
                return jsonify({"status": "true", "content": "更新成功"})
            except Exception as e:
                logger.error('Userinfo change fail: {}'.format(e))
                db.session.rollback()
                return jsonify({"status": "false", "content": "数据更新失败，可能存在数据冲突"})
        else:
            return jsonify({"status": "false", "content": "无更新"})
    else:
        logger.info('This user do not permitted to alter user info')
        return jsonify({"status": "false", "content": "无权限更新"})