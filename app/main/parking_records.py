from flask import session, render_template, request, jsonify
from flask_login import login_required
from ..models import *
from ..decorators import permission_required
from .. import db, logger, redis_db
from . import main
import json
import datetime
from uuid import uuid1
from ..Billing.pay_fee import check_out, do_pay
from datetime import datetime, timedelta


# parking records search
@main.route('/parking_records', methods=['GET', 'POST'])
@login_required
@permission_required(Permission.MAN_ON_DUTY)
def parking_records():
    if request.method == 'GET':
        return render_template('parking_records.html')

    elif request.method == 'POST':
        logger.debug(request.form)
        record_status = request.form.get('query[record_status]')
        search_content = '%' + request.form.get('query[search_content]', '%').upper() + '%'
        search_date = request.form.get('query[search_date]')
        start_time = datetime(2000, 1, 1, 0, 0, 0)
        stop_time = datetime(2100, 12, 31, 23, 59, 59)
        logger.debug('status {} content {} date {}'.format(record_status, search_content, search_date))
        if search_date:
            start_time, stop_time = search_date.split(' - ')

            start_time = datetime.strptime(start_time.strip(), '%Y/%m/%d %H:%M:%S') \
                if search_date else datetime(2000, 1, 1, 0, 0, 0)

            stop_time = datetime.strptime(stop_time.strip(), '%Y/%m/%d %H:%M:%S') \
                if search_date else datetime(2100, 12, 31, 23, 59, 59)

            logger.debug('Searching parking records from {} to {}'.format(start_time, stop_time))

        logger.debug('Searching datetime from {} to {}'.format(start_time, stop_time))

        page_start = (int(request.form.get('datatable[pagination][page]', '0')) - 1) * 10
        length = int(request.form.get('datatable[pagination][perpage]'))
        page_end = page_start + length

        logger.debug('pages from {} to {}, the total of the page is {}'.format(page_start, length, page_end))

        if request.form.get('query[record_status]') or request.form.get('query[search_content]') or request.form.get(
                'query[search_date]'):
            logger.debug('searching content...')
            data = [{'id': pr.uuid,
                     'number_plate': pr.number_plate,
                     'entry_time': pr.entry_time,
                     'entry_gate': pr.entry_camera_id,
                     'exit_time': pr.exit_time,
                     'exit_gate': pr.exit_camera_id,
                     'status': '已出场' if pr.status else '未出场',
                     'entry_plate_number_pic': pr.entry_plate_number_pic}
                    for pr in ParkingRecords.query.filter(ParkingRecords.number_plate.like(search_content),
                                                          ParkingRecords.status.like(
                                                              record_status if record_status else '%'),
                                                          ParkingRecords.create_time.between(start_time,
                                                                                             stop_time)).order_by(
                    ParkingRecords.create_time.desc()).all()]

            recordsTotal = len(data)
        else:
            data = [{'id': pr.uuid,
                     'number_plate': pr.number_plate,
                     'entry_time': pr.entry_time,
                     'entry_gate': pr.entry_camera_id,
                     'exit_time': pr.exit_time,
                     'exit_gate': pr.exit_camera_id,
                     'status': '已出场' if pr.status else '未出场',
                     'entry_plate_number_pic': pr.entry_plate_number_pic}
                    for pr in ParkingRecords.query.order_by(ParkingRecords.create_time.desc()).all()]

            print('data', data)

            recordsTotal = len(data)

        rest = {
            "meta": {
                "page": int(request.form.get('datatable[pagination][page]')),
                "pages": int(int(recordsTotal) / int(length)),
                "perpage": int(length),
                "total": int(recordsTotal),
            },
            "data": data[page_start: page_end]
        }
        return jsonify(rest)


@main.route('/parking_entry_number_plate_update', methods=['POST'])
@login_required
@permission_required(Permission.MAN_ON_DUTY)
def parking_entry_number_plate_update():
    uuid = request.form.get('uuid')
    print(uuid)
    number_plate = request.form.get('number_plate')
    print(number_plate)

    logger.info('User {} is update parking record {}\'s info'.format(session['LOGINNAME'], uuid))

    if number_plate:
        try:
            old_record = ParkingRecords.query.filter(ParkingRecords.uuid.__eq__(uuid),
                                                     ParkingRecords.exit_time.is_(None)).first()
            print('old record ', old_record)
            if not old_record:
                return jsonify({"status": "false", "content": "记录已匹配不得修改"})
            logger.info(
                'User {} is update parking record from {} to {}'.format(session['LOGINNAME'], old_record.number_plate,
                                                                        number_plate))

            print('modify entry number')
            modification = NumberPlateModification(uuid=str(uuid1()),
                                                   relate_record_id=old_record.uuid,
                                                   type=0,
                                                   before_number=old_record.number_plate,
                                                   after_number=number_plate.upper(),
                                                   registrar_id=session['SELFID'],
                                                   create_time=datetime.now())

            old_record.number_plate = number_plate.upper()

            db.session.add(modification)
            db.session.add(old_record)
            db.session.commit()
            return jsonify({"status": "true", "content": "更新成功"})
        except Exception as e:
            db.session.rollback()
            logger.error('update entry number fail for {}'.format(e))
            return jsonify({"status": "false", "content": "更新失败"})

    else:
        return jsonify({"status": "false", "content": "无更新"})


@main.route('/parking_exit_record', methods=['GET', 'POST'])
@login_required
@permission_required(Permission.NETWORK_MANAGER)
def parking_exit_record():
    if request.method == 'GET':
        return render_template('parking_exit_record.html')
    elif request.method == 'POST':
        page_start = (int(request.form.get('datatable[pagination][page]', '0')) - 1) * 10
        length = int(request.form.get('datatable[pagination][perpage]'))
        page_end = page_start + length

        cameras = Camera.query.filter(Camera.device_type.__eq__(22)).all()

        exit_records = []

        for c in cameras:
            print(c)
            record = redis_db.get(c.device_number)
            print(record)
            if record:
                exit_records.append(json.loads(record.decode('utf-8')))

        data = [{'camera': exit_record.get('camera'),
                 'number_plate': exit_record.get('number_plate'),
                 'exit_pic': exit_record.get('plate_number_pic'),
                 'exit_time': exit_record.get('time')}
                for exit_record in exit_records]

        print('data', data)

        recordsTotal = len(data)

        rest = {
            "meta": {
                "page": int(request.form.get('datatable[pagination][page]')),
                "pages": int(int(recordsTotal) / int(length)),
                "perpage": int(length),
                "total": int(recordsTotal),
            },
            "data": data[page_start - 1: page_end]
        }
        return jsonify(rest)


@main.route('/parking_exit_number_plate_update', methods=['POST'])
@login_required
@permission_required(Permission.MAN_ON_DUTY)
def parking_exit_number_plate_update():
    camera = request.form.get('camera')
    print(camera)
    number_plate = request.form.get('number_plate')
    print(number_plate)

    logger.info('User {} is update parking record {}\'s info'.format(session['LOGINNAME'], camera))

    if number_plate:
        try:
            old_record = json.loads(redis_db.get(camera).decode('utf-8'))
            logger.info(
                'User {} is update parking record from {} to {}'.format(session['LOGINNAME'],
                                                                        old_record['number_plate'],
                                                                        number_plate))

            if old_record['status'] == 1:
                return jsonify({"status": "false", "content": "此出场记录已经被匹配，不得修改"})

            if not ParkingAbnormalExitRecords.query.filter_by(number_plate=old_record['number_plate'],
                                                              exit_time=old_record['time'],
                                                              camera_id=old_record['camera']).all():
                abnormal_exit = ParkingAbnormalExitRecords(uuid=str(uuid1()),
                                                           number_plate=old_record.get('number_plate'),
                                                           camera_id=old_record.get('camera'),
                                                           exit_time=old_record.get('time'),
                                                           exit_pic=old_record.get('exit_pic'),
                                                           exit_plate_number_pic=old_record.get(
                                                               'exit_plate_number_pic'),
                                                           create_time=datetime.now())
                db.session.add(abnormal_exit)
                db.session.commit()

            modification = NumberPlateModification(uuid=str(uuid1()),
                                                   relate_record_id='',
                                                   type=1,
                                                   before_number=old_record['number_plate'],
                                                   after_number=number_plate.upper(),
                                                   registrar_id=session['SELFID'],
                                                   create_time=datetime.now())

            old_record['number_plate'] = number_plate.upper()

            db.session.add(modification)
            db.session.commit()

            redis_db.set(camera, json.dumps(old_record))
            return jsonify({"status": "true", "content": "更新成功"})
        except Exception as e:
            db.session.rollback()
            logger.error('update exit number fail for {}'.format(e))
            return jsonify({"status": "fail", "content": "更新失败"})
    else:
        return jsonify({"status": "fail", "content": "无更新"})


@main.route('/center_pay_info', methods=['POST'])
@login_required
@permission_required(Permission.MAN_ON_DUTY)
def center_pay_info():
    id = request.form.get('id')
    number_plate = request.form.get('number_plate')

    now_time = datetime.now()

    logger.info('User {} is checking park record {}\'s info'.format(session['LOGINNAME'], id))

    entry_record = ParkingRecords.query.filter_by(uuid=id).first()
    print('entry fee', entry_record.fee)

    if entry_record.status == 1:
        return jsonify({'status': 'fail', 'content': '此记录车辆已出场'})
    elif entry_record.fee is not None and entry_record.exit_validate_before is not None and datetime.now() <= entry_record.exit_validate_before:
        return jsonify({'status': 'fail', 'content': '已缴费，请尽快离场'})

    checkout_amount, pay_time, free_order = check_out(number_plate=number_plate, exit_time=now_time,
                                                      entry_price=entry_record.entry_unit_price)
    print(checkout_amount, pay_time, free_order)
    return jsonify({'status': 'true',
                    'content': {'exit_time': str(now_time.strftime('%Y-%m-%d %H:%M:%S')),
                                'pay_fee': checkout_amount,
                                'parking_time': str(pay_time),
                                }
                    }
                   )


@main.route('/center_pay', methods=['POST'])
@login_required
@permission_required(Permission.MAN_ON_DUTY)
def center_pay():
    id = request.form.get('id')
    pay_fee = request.form.get('pay_fee')
    exit_time = request.form.get('exit_time')

    entry_record = ParkingRecords.query.filter_by(uuid=id).first()

    logger.info('User {} is update parking record {}\'s info'.format(session['LOGINNAME'], id))

    entry_record.exit_validate_before = datetime.strptime(exit_time, "%Y-%m-%d %H:%M:%S") + timedelta(minutes=20)

    db.session.add(entry_record)
    db.session.commit()

    pay_result = do_pay(id, pay_fee, operate_source=21)

    return jsonify({'status': 'true' if pay_result else 'false',
                    'content': '付费成功' if pay_result else '付费失败'})
