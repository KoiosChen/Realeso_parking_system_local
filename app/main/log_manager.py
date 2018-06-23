from flask import session, render_template, request, jsonify
from flask_login import login_required
from ..models import *
from ..decorators import permission_required
from .. import db, logger
from .forms import PostForm
from . import main
import json
import datetime


@main.route('/echarts_1', methods=['GET'])
def echarts_1():
    return render_template('echart_1.html')


@main.route('/metronic', methods=['GET'])
def metronic():
    return render_template('metronic/index.html')


@main.route('/newsyslog', methods=['GET'])
def newsyslog():
    device_list = {device.ip: device.device_name for device in Device.query.all()}

    syslog_ip = [[ip[0], ip[0] + '(' + str(device_list.get(ip[0])) + ')']
                 for ip in Syslog.query.with_entities(Syslog.device_ip).group_by(Syslog.device_ip).all()]

    sys_serverty = [value for value in syslog_serverty.values()]
    logger.info('User {} is checking syslog record'.format(session['LOGINNAME']))
    return render_template('newsyslog.html', syslog_ip=syslog_ip, sys_serverty=sys_serverty)


@main.route('/syslog_search', methods=['POST'])
@login_required
@permission_required(Permission.NETWORK_MANAGER)
def syslog_search():
    device_list = {device.ip: device.device_name for device in Device.query.all()}

    print(request.form)
    device_ip = '%' + request.form.get('query[device_ip]', '') + '%'
    logmsg = '%' + request.form.get('query[logmsg]', '') + '%'
    search_date = request.form.get('query[search_date]', None)
    serverty = '%' + request.form.get('query[serverty]', '') + '%'
    logger.debug('{} {} {} {}'.format(device_ip, logmsg, search_date, serverty))
    start_time = datetime.datetime(2000, 1, 1, 0, 0, 0)
    stop_time = datetime.datetime(2100, 12, 31, 23, 59, 59)
    if search_date:
        start_time, stop_time = search_date.split(' - ')

        start_time = datetime.datetime.strptime(start_time.strip(), '%Y/%m/%d %H:%M:%S') \
            if search_date else datetime.datetime(2000, 1, 1, 0, 0, 0)

        stop_time = datetime.datetime.strptime(stop_time.strip(), '%Y/%m/%d %H:%M:%S') \
            if search_date else datetime.datetime(2100, 12, 31, 23, 59, 59)

        logger.debug('search syslog from {} to {}'.format(start_time, stop_time))

    page_start = (int(request.form.get('datatable[pagination][page]', '0')) - 1) * 10 + 1
    length = int(request.form.get('datatable[pagination][perpage]'))

    if request.form.get('query[device_ip]') or request.form.get('query[logmsg]') or search_date or request.form.get(
            'query[serverty]'):
        logger.debug('{} {} {} {} {}'.format(device_ip, logmsg, start_time, stop_time, serverty))
        logger.debug('search syslog')
        data = [{'id': syslog.id,
                 'device_name': device_list.get(syslog.device_ip),
                 'device_ip': syslog.device_ip,
                 'logmsg': syslog.logmsg,
                 'serverty': syslog.serverty,
                 'logtime': syslog.logtime}
                for syslog in Syslog.query.filter(Syslog.device_ip.like(device_ip),
                                                  Syslog.logmsg.like(logmsg),
                                                  Syslog.logtime.between(start_time, stop_time),
                                                  Syslog.serverty.like(serverty)).order_by(
                Syslog.logtime.desc()).offset(page_start).limit(length)]
        recordsTotal = Syslog.query.filter(Syslog.device_ip.like(device_ip),
                                           Syslog.logmsg.like(logmsg),
                                           Syslog.logtime.between(start_time, stop_time),
                                           Syslog.serverty.like(serverty)).count()
    else:
        data = [{'id': syslog.id,
                 'device_name': device_list.get(syslog.device_ip),
                 'device_ip': syslog.device_ip,
                 'logmsg': syslog.logmsg,
                 'serverty': syslog.serverty,
                 'logtime': syslog.logtime}
                for syslog in Syslog.query.order_by(Syslog.logtime.desc()).offset(page_start).limit(length)]
        recordsTotal = Syslog.query.count()

    # rest = {'draw': 1,
    #         'recordsTotal': recordsTotal,
    #         'recordsFiltered': recordsTotal,
    #         'data': data
    #         }
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


# alarm manager
@main.route('/alarm_record', methods=['GET', 'POST'])
@login_required
@permission_required(Permission.MAN_ON_DUTY)
def alarm_record():
    if request.method == 'GET':
        operator = [(str(user_info.id), user_info.username) for user_info in User.query.all()]
        modal_form = PostForm()
        logger.info('User {} is checking alarm record'.format(session['LOGINNAME']))
        return render_template('alarmRecordsSearch.html',
                               operator=operator,
                               modal_form=modal_form)
    elif request.method == 'POST':
        logger.debug(request.form)
        select_handler = request.form.get('query[select_handler]', '%')
        search_content = '%' + request.form.get('query[search_content]', '%') + '%'
        search_date = request.form.get('query[search_date]')
        start_time = datetime.datetime(2000, 1, 1, 0, 0, 0)
        stop_time = datetime.datetime(2100, 12, 31, 23, 59, 59)
        logger.debug('{} {} {}'.format(select_handler, search_content, search_date))
        if search_date:
            start_time, stop_time = search_date.split(' - ')

            start_time = datetime.datetime.strptime(start_time.strip(), '%Y/%m/%d %H:%M:%S') \
                if search_date else datetime.datetime(2000, 1, 1, 0, 0, 0)

            stop_time = datetime.datetime.strptime(stop_time.strip(), '%Y/%m/%d %H:%M:%S') \
                if search_date else datetime.datetime(2100, 12, 31, 23, 59, 59)

            logger.debug('Searching alarm log from {} to {}'.format(start_time, stop_time))

        logger.debug('Searching datetime from {} to {}'.format(start_time, stop_time))

        page_start = (int(request.form.get('datatable[pagination][page]', '0')) - 1) * 10 + 1
        length = int(request.form.get('datatable[pagination][perpage]'))
        page_end = page_start + length

        print(page_start, length, page_end)

        posted_body = Post.query.order_by(Post.timestamp.desc()).all()
        all_user_dict = {user.id: {'username': user.username, 'phoneNum': user.phoneNum}
                         for user in User.query.filter_by(status=1).all()}
        pl = []
        post_info = {}
        for p in posted_body:
            if p.alarm_id not in pl:
                pl.append(p.alarm_id)
                post_info[p.alarm_id] = {'author_id': p.author_id, 'timestamp': p.timestamp}

        call_record = {r.callId: r.phoneNum
                       for r in CallRecordDetail.query.filter(CallRecordDetail.respCode.__eq__(000000)).all()}

        if request.form.get('query[select_handler]') or request.form.get('query[search_content]') or request.form.get(
                'query[search_date]'):
            if request.form.get('query[select_handler]') and Post.query.filter_by(author_id=select_handler).all():
                data = [{'id': ui.id,
                         'alarm_content': ui.content,
                         'handler': all_user_dict[post_info[ui.id]['author_id']]['username'] if ui.id in post_info and
                                                                                                post_info[ui.id][
                                                                                                    'author_id'] in all_user_dict else '',
                         'handle_time': post_info[ui.id]['timestamp'] if ui.id in post_info else '',
                         # call_record[ui.lastCallId] if ui.lastCallId in call_record else '',
                         'alarm_time': ui.create_time}
                        for ui in AlarmRecord.query.filter(AlarmRecord.content.like(search_content),
                                                           AlarmRecord.alarm_type.__ne__(999),
                                                           AlarmRecord.create_time.between(start_time,
                                                                                           stop_time)).order_by(
                        AlarmRecord.create_time.desc()).all()
                        if ui.id in post_info and post_info[ui.id]['author_id'] == int(select_handler)]

            elif request.form.get('query[select_handler]') and not Post.query.filter_by(author_id=select_handler).all():
                # 防止异常
                logger.debug('search alarm error without post data')
                data = []

            else:
                logger.debug('searching content...')
                data = [{'id': ui.id,
                         'alarm_content': ui.content,
                         'handler': all_user_dict[post_info[ui.id]['author_id']]['username'] if ui.id in post_info and
                                                                                                post_info[ui.id][
                                                                                                    'author_id'] in all_user_dict else '',
                         'handle_time': post_info[ui.id]['timestamp'] if ui.id in post_info else '',
                         # call_record[ui.lastCallId] if ui.lastCallId in call_record else '',
                         'alarm_time': ui.create_time}
                        for ui in AlarmRecord.query.filter(AlarmRecord.content.like(search_content),
                                                           AlarmRecord.alarm_type.__ne__(999),
                                                           AlarmRecord.create_time.between(start_time,
                                                                                           stop_time)).order_by(
                        AlarmRecord.create_time.desc()).all()]

            recordsTotal = len(data)
        else:
            data = [{'id': ui.id,
                     'alarm_content': ui.content,
                     'handler': all_user_dict[post_info[ui.id]['author_id']]['username'] if ui.id in post_info and
                                                                                            post_info[ui.id][
                                                                                                'author_id'] in all_user_dict else '',
                     'handle_time': post_info[ui.id]['timestamp'] if ui.id in post_info else '',
                     # call_record[ui.lastCallId] if ui.lastCallId in call_record else '',
                     'alarm_time': ui.create_time}
                    for ui in AlarmRecord.query.filter(AlarmRecord.alarm_type.__ne__(999)).order_by(
                    AlarmRecord.create_time.desc()).all()]
            recordsTotal = len(data)

        rest = {
            "meta": {
                "page": int(request.form.get('datatable[pagination][page]')),
                "pages": int(int(recordsTotal) / int(length)),
                "perpage": int(length),
                "total": int(recordsTotal),
            },
            "data": data[page_start-1: page_end]
        }
        return jsonify(rest)


@main.route('/post_body', methods=['POST'])
@login_required
@permission_required(Permission.MAN_ON_DUTY)
def post_body():
    alarm_id = request.form.get('alarm_id')
    body = request.form.get('body')
    print('view', body)

    post = Post(body=body, author_id=str(session['SELFID']), alarm_id=alarm_id)
    db.session.add(post)
    db.session.commit()
    return json.dumps({"status": 'OK'}, ensure_ascii=False)


@main.route('/get_alarm_detail_info', methods=['POST'])
@login_required
@permission_required(Permission.MAN_ON_DUTY)
def get_alarm_detail_info():
    params = request.get_data()
    jl = params.decode('utf-8')
    j = json.loads(jl)
    alarm_record = AlarmRecord.query.get(j.get('alarm_id'))

    call_record = {r.callId: r.phoneNum
                   for r in CallRecordDetail.query.filter(CallRecordDetail.respCode.__eq__(000000)).all()}

    alarm_detail_info = {
        'phoneNum': call_record[alarm_record.lastCallId] if alarm_record.lastCallId in call_record.keys() else None,
        'call_status': alarm_record_state[alarm_record.state],
        'times': alarm_record.calledTimes}

    return jsonify(json.dumps(alarm_detail_info, ensure_ascii=False))


@main.route('/get_attachment', methods=['POST'])
@login_required
@permission_required(Permission.MAN_ON_DUTY)
def get_attachment():
    params = request.get_data()
    jl = params.decode('utf-8')
    j = json.loads(jl)
    print(j)
    alarm_record = AlarmRecord.query.get(j.get('id'))
    print(alarm_record.content_md5)

    attachment = OntAccountInfo.query.filter_by(hash_id=alarm_record.content_md5).first()

    if attachment:
        return jsonify({'status': 'OK', 'data': attachment.account_info})
    else:
        return jsonify({'status': 'Fail', 'data': 'Null'})


@main.route('/get_posted_body', methods=['POST'])
@login_required
@permission_required(Permission.MAN_ON_DUTY)
def get_posted_body():
    params = request.get_data()
    jl = params.decode('utf-8')
    j = json.loads(jl)
    posted_body = {}
    posts = Post.query.filter_by(alarm_id=j.get('alarm_id')).order_by(Post.timestamp.desc()).all()
    all_user = User.query.filter_by(status=1).all()
    all_user_dict = {}
    for user in all_user:
        all_user_dict[user.id] = {'username': user.username, 'phoneNum': user.phoneNum}
    i = 0
    for p in posts:
        posted_body[i] = {'username': all_user_dict[p.author_id]['username'],
                          'phoneNum': all_user_dict[p.author_id]['phoneNum'],
                          'body': p.body,
                          'body_html': p.body_html,
                          'timestamp': datetime.datetime.strftime(p.timestamp, '%Y-%m-%d %H:%M:%S')}
        i += 1
    print(posted_body)
    return jsonify(json.dumps(posted_body, ensure_ascii=False))
