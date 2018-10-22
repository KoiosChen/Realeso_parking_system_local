from flask import redirect, session, url_for, render_template, request, jsonify, send_from_directory
from . import main
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
from sqlalchemy import or_, and_


# parking records search
@main.route('/fixed_parking_space', methods=['GET', 'POST'])
@login_required
@permission_required(Permission.MAN_ON_DUTY)
def fixed_parking_space():
    if request.method == 'GET':
        return render_template('fixed_parking_space.html')

    elif request.method == 'POST':
        logger.debug(request.form)
        company_name = '%' + request.form.get('query[company_name]') + '%'
        plate_or_space_number = '%' + request.form.get('query[plate_or_space_number]', '%').upper() + '%'
        search_date = request.form.get('query[search_date]')
        start_time = datetime(2000, 1, 1, 0, 0, 0)
        stop_time = datetime(2100, 12, 31, 23, 59, 59)
        logger.debug(
            'company name: {};  plate or space number: {}; date: {}'.format(company_name, plate_or_space_number,
                                                                            search_date))
        if search_date:
            start_time, stop_time = search_date.split(' - ')

            start_time = datetime.strptime(start_time.strip(), '%Y/%m/%d %H:%M:%S') \
                if search_date else datetime(2000, 1, 1, 0, 0, 0)

            stop_time = datetime.strptime(stop_time.strip(), '%Y/%m/%d %H:%M:%S') \
                if search_date else datetime(2100, 12, 31, 23, 59, 59)

            logger.debug('Searching fixed parking records from {} to {}'.format(start_time, stop_time))

        logger.debug('Searching datetime from {} to {}'.format(start_time, stop_time))

        page_start = (int(request.form.get('datatable[pagination][page]', '0')) - 1) * 10
        length = int(request.form.get('datatable[pagination][perpage]'))
        page_end = page_start + length

        logger.debug('pages from {} to {}, total {}'.format(page_start, page_end, length))

        if request.form.get('query[company_name]') or request.form.get(
                'query[plate_or_space_number]') or request.form.get('query[search_date]'):
            logger.debug('Searching records from database...')
            print(str(FixedParkingSpace.query.join(ParkingOrder).filter(

                or_(ParkingOrder.number_plate.like(plate_or_space_number),
                    FixedParkingSpace.specified_parking_space_code.like(plate_or_space_number)),

                FixedParkingSpace.company.like(company_name),

                and_(ParkingOrder.order_validate_start.__ge__(
                    start_time),
                    ParkingOrder.order_validate_stop.__le__(
                        stop_time)),
                ParkingOrder.status.__eq__(1)).order_by(FixedParkingSpace.create_time.desc())))

            data = [{'id': pr.uuid,
                     'number_plate': pr.fixed_order.number_plate,
                     'specified_parking_space_code': pr.specified_parking_space_code,
                     'company': pr.company,
                     'order_validate_start': pr.fixed_order.order_validate_start,
                     'order_validate_stop': pr.fixed_order.order_validate_stop}
                    for pr in
                    FixedParkingSpace.query.join(ParkingOrder).filter(

                        or_(ParkingOrder.number_plate.like(plate_or_space_number),
                            FixedParkingSpace.specified_parking_space_code.like(plate_or_space_number)),

                        FixedParkingSpace.company.like(company_name),

                        and_(ParkingOrder.order_validate_start.__ge__(
                            start_time),
                            ParkingOrder.order_validate_stop.__le__(
                                stop_time)),
                        ParkingOrder.status.__eq__(1)).order_by(FixedParkingSpace.create_time.desc()).all()]

            recordsTotal = len(data)
        else:
            data = [{'id': pr.uuid,
                     'number_plate': pr.fixed_order.number_plate,
                     'specified_parking_space_code': pr.specified_parking_space_code,
                     'company': pr.company,
                     'order_validate_start': pr.fixed_order.order_validate_start,
                     'order_validate_stop': pr.fixed_order.order_validate_stop}
                    for pr in
                    FixedParkingSpace.query.join(ParkingOrder).filter(ParkingOrder.status.__eq__(1)).order_by(
                        FixedParkingSpace.create_time.desc()).all()]

            recordsTotal = len(data)

            print(recordsTotal)

            print('data', data)

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


# Add parking records
@main.route('/add_fixed_parking_record', methods=['POST'])
@login_required
@permission_required(Permission.MAN_ON_DUTY)
def add_fixed_parking_record():
    data = request.json
    new_plate_number = data.get('new_plate_number')
    new_space_number = data.get('new_space_number')
    new_company_name = data.get('new_company_name')
    dateTimeRange = data.get('new_validate_period')
    registrar_id = session['SELFID']

    start_time, stop_time = dateTimeRange.split(' - ')

    start_time = datetime.strptime(start_time.strip(), '%Y/%m/%d %H:%M:%S') \
        if dateTimeRange else datetime(2000, 1, 1, 0, 0, 0)

    stop_time = datetime.strptime(stop_time.strip(), '%Y/%m/%d %H:%M:%S') \
        if dateTimeRange else datetime(2100, 12, 31, 23, 59, 59)

    logger.debug('add fixed parking record: {}'.format(str(data)))

    old_fixed_order_by_number_plate = FixedParkingSpace.query.join(ParkingOrder).filter(
        ParkingOrder.number_plate.__eq__(new_plate_number),
        ParkingOrder.status.__eq__(1),
        FixedParkingSpace.status.__eq__(1)).first()

    old_fixed_order = FixedParkingSpace.query.join(ParkingOrder).filter(
        ParkingOrder.number_plate.__eq__(new_plate_number),
        ParkingOrder.status.__eq__(1),
        FixedParkingSpace.status.__eq__(1),
        or_(ParkingOrder.order_validate_stop.__lt__(
            start_time),
            ParkingOrder.order_validate_start.__gt__(
                stop_time))).first()

    if old_fixed_order_by_number_plate and not old_fixed_order:
        return jsonify({'status': 'false', 'content': "新固定车位增加失败，时间段重复"})

    try:
        new_uuid = str(uuid1())
        new_order = ParkingOrder(uuid=new_uuid,
                                 number_plate=new_plate_number.upper(),
                                 order_validate_start=start_time,
                                 order_validate_stop=stop_time + timedelta(hours=23, minutes=59, seconds=59),
                                 status=1,
                                 order_type=1,
                                 reserved=1,
                                 update_time=datetime.now(),
                                 create_time=datetime.now())
        db.session.add(new_order)
        db.session.commit()
        new_record = FixedParkingSpace(uuid=str(uuid1()),
                                       specified_parking_space_code=new_space_number.upper(),
                                       company=new_company_name,
                                       order_uuid=new_uuid,
                                       registrar_id=registrar_id,
                                       create_time=datetime.now())

        db.session.add(new_record)
        db.session.commit()
        logger.info('User {} add fixed parking record {} success'.format(registrar_id, new_uuid))
        return jsonify({'status': 'true', 'content': "固定车位添加成功"})
    except Exception as e:
        logger.error('User {} fail for {}'.format(registrar_id, e))
        db.session.rollback()
        return jsonify({'status': 'false', 'content': "固定车位添加失败"})


@main.route('/fixed_parking_record_delete', methods=['POST'])
@login_required
@permission_required(Permission.FOLLOW)
def fixed_parking_record_delete():
    data = request.json
    print(data)
    uuid = data.get('uuid')
    try:
        fixed_parking_record_tobe_deleted = FixedParkingSpace.query.filter_by(uuid=uuid).first()
        order_related = ParkingOrder.query.filter_by(uuid=fixed_parking_record_tobe_deleted.order_uuid).first()
        fixed_parking_record_tobe_deleted.status = 0
        order_related.status = 0
        logger.debug('User {} is setting fixed parking record({})\'s status to 0'.format(session['LOGINNAME'], uuid))
        db.session.add(fixed_parking_record_tobe_deleted)
        db.session.add(order_related)
        db.session.commit()
        logger.info('Fixed parking record and related order is set to be 0')
        return jsonify({'status': 'true', 'content': '固定车位记录删除成功'})
    except Exception as e:
        logger.error('Delete user fail:{}'.format(e))
        db.session.rollback()
        return jsonify({'status': 'false', 'content': '固定车位记录删除失败'})


@main.route('/fixed_parking_record_update', methods=['POST'])
@login_required
@permission_required(Permission.FOLLOW)
def fixed_parking_record_update():
    data = request.json
    logger.debug(str(data))
    uuid = data.get('uuid')

    update_plate_number = data.get('update_plate_number').upper()
    update_space_number = data.get('update_space_number').upper()
    update_company_name = data.get('update_company_name')
    update_dateTimeRange = data.get('update_dateTimeRange')

    start_time = datetime(2000, 1, 1, 0, 0, 0)
    stop_time = datetime(2100, 12, 31, 23, 59, 59)

    if update_dateTimeRange:
        start_time, stop_time = update_dateTimeRange.split(' - ')

        start_time = datetime.strptime(start_time.strip(), '%Y/%m/%d %H:%M:%S') \
            if update_dateTimeRange else datetime(2000, 1, 1, 0, 0, 0)

        stop_time = datetime.strptime(stop_time.strip(), '%Y/%m/%d %H:%M:%S') \
            if update_dateTimeRange else datetime(2100, 12, 31, 23, 59, 59)

        logger.debug('Searching fixed parking records from {} to {}'.format(start_time, stop_time))

    logger.info('User {} is update {}\'s info'.format(session['LOGINNAME'], uuid))

    flag = False
    record_tobe_updated = FixedParkingSpace.query.filter_by(uuid=uuid).first()

    if update_plate_number and update_plate_number != record_tobe_updated.fixed_order.number_plate:
        record_tobe_updated.fixed_order.number_plate = update_plate_number
        flag = True
    if update_space_number and update_space_number != record_tobe_updated.specified_parking_space_code:
        record_tobe_updated.specified_parking_space_code = update_space_number
        flag = True
    if update_company_name and update_company_name != record_tobe_updated.company:
        record_tobe_updated.company = update_company_name
        flag = True
    if update_dateTimeRange and start_time != record_tobe_updated.fixed_order.order_validate_start:
        record_tobe_updated.fixed_order.order_validate_start = start_time
        flag = True
    if update_dateTimeRange and stop_time != record_tobe_updated.fixed_order.order_validate_stop:
        record_tobe_updated.fixed_order.order_validate_stop = stop_time  + timedelta(hours=23, minutes=59, seconds=59)
        flag = True

    if flag:
        try:
            db.session.add(record_tobe_updated)
            db.session.commit()
            logger.info('Fixed parking record {} is updated'.format(uuid))
            return jsonify({"status": "true", "content": "更新成功"})
        except Exception as e:
            logger.error('Fixed parking record {} update fail for {}'.format(uuid, e))
            db.session.rollback()
            return jsonify({"status": "false", "content": "数据更新失败"})
    else:
        return jsonify({"status": "false", "content": "无更新"})
