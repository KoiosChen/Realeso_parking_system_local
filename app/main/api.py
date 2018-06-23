from flask import request, jsonify
from . import main
from ..models import AlarmRecord, PonAlarmRecord, Permission, PiRegister, User, PcapOrder, SyslogAlarmConfig, Syslog
from .. import redis_db, logger, db
import json
import re
from flask_login import current_user
import time
import requests
from collections import defaultdict
import datetime
from sqlalchemy import and_, func
from ..MyModule.Counter import manage_key
from ..MyModule.HashContent import md5_content


def nesteddict():
    """
    构造一个嵌套的字典
    :return:
    """
    return defaultdict(nesteddict)


@main.route('/submit_pcap_order', methods=["POST"])
def submit_pcap_order():
    data = request.get_json()
    submit_order = PcapOrder(id=data['id'],
                             account_id=data['accountId'],
                             login_name=data['login_name'],
                             username=data['username'],
                             question_description=data['question'],
                             create_time=time.localtime())
    db.session.add(submit_order)
    db.session.commit()
    return jsonify({'status': 'ok', 'content': '工单已提交，请携带对应设备上门'})


@main.route('/pi_register', methods=["POST"])
def pi_register():
    registerInfo = request.json['sysid'].strip()
    print(registerInfo)
    if not registerInfo:
        return jsonify({'status': 'fail', 'content': '未提交正确的信息'})
    else:
        register_record = PiRegister.query.filter_by(sysid=registerInfo.strip()).first()
        print(register_record)
        if not register_record:
            return jsonify({'status': 'fail', 'content': '此设备未绑定'})
        else:
            userinfo = User.query.filter_by(email=register_record.username, status=1).first()
            if not userinfo:
                return jsonify({'status': 'fail', 'content': '绑定用户不存在或者已经失效'})
            else:
                print(userinfo)
                account_dict = nesteddict()
                processing_orders = PcapOrder.query.filter_by(status=1, login_name=register_record.username).all()
                headers = {'Content-Type': 'application/json', "encoding": "utf-8"}
                send_sms_url = 'http://127.0.0.1:54322/get_customer_info'

                for o in processing_orders:
                    send_content = {"account_id": o.account_id, "loginName": "admin", "_hidden_param": True}
                    r = requests.post(send_sms_url, data=json.dumps(send_content, ensure_ascii=False).encode('utf-8'),
                                      headers=headers)
                    result = r.json()
                    if result['status'] == 'OK':
                        account_dict[o.account_id] = \
                            {"password": result['content']['customerListInfo']['customerList'][0]['password'],
                             "question": o.question_description,
                             "order_id": o.id}
                        print(result)

                register_record.times += 1
                register_record.last_register_time = time.localtime()
                db.session.add(register_record)
                db.session.commit()
                return jsonify(
                    {'status': 'ok', 'content': account_dict, 'url': {'r2d2_url': 'http://192.168.2.112:54321',
                                                                      'onu_url': 'http://192.168.2.112:54322',
                                                                      'iperf_server': '192.168.2.112'}})


@main.route('/delete_alarm_record', methods=['POST'])
def delete_alarm_record():
    """
    用于删除告警记录。alarm_record表不删除，只是将alarm_type修改为999；如果是alarm_type 为4， 那么要删除pon_alarm_record中的记录
    POST的是
    :return:
    """
    try:
        if not current_user.can(Permission.NETWORK_MANAGER):
            logger.warn('This user\'s action is not permitted!')
            return jsonify({'status': 'Fail', 'content': '此账号没有权限删除告警记录'})
        print('delete')
        alarm_id = request.json
        print(alarm_id)

        id = alarm_id['alarm_id']

        print(id)

        print('start check')

        alarm_record = AlarmRecord.query.filter_by(id=id).first()

        print(alarm_record)
        print(alarm_record.alarm_type)

        if alarm_record.alarm_type == 4 or alarm_record.alarm_type == 3:
            print(alarm_record.content)
            try:
                ontid = [int(i) for i in eval(re.findall(r'(\{*.+\})', alarm_record.content)[0])]
            except Exception as e:
                ontid = ['PON']
            ip = re.findall(r'(\d+\.\d+\.\d+\.\d+)', alarm_record.content)[0]
            f, s, p = re.findall(r'(\d+/\d+/\d+)', alarm_record.content)[0].split('/')
            print(f, s, p, ontid, ip)
            for ont in ontid:
                pon_alarm_record = PonAlarmRecord.query.filter_by(ip=ip, frame=f, slot=s, port=p, ontid=ont).first()
                if not pon_alarm_record:
                    continue
                db.session.delete(pon_alarm_record)
                db.session.commit()

        alarm_record.alarm_type = 999
        db.session.add(alarm_record)
        db.session.commit()

        return jsonify({'status': 'OK', 'content': '记录已删除'})

    except Exception as e:
        print(e)
        return jsonify({'status': 'Fail', 'content': str(e)})


@main.route('/today_syslog_counter', methods=['POST'])
def today_syslog_counter():
    """
    用于统计返回当天syslog数量
    :return:
    """
    try:
        logger.debug('in today_syslog_counter api')
        now = datetime.datetime.now()
        yesterday = now - datetime.timedelta(days=1)

        todaySyslogCounter = Syslog.query.filter(
            and_(Syslog.logtime.__ge__(datetime.datetime(now.year, now.month, now.day, 0, 0, 0, 0)),
                 Syslog.logtime.__le__(
                     datetime.datetime(now.year, now.month, now.day, now.hour, now.minute, now.second,
                                       now.microsecond)))).count()

        yesterdaySyslogCounter = Syslog.query.filter(
            and_(Syslog.logtime.__ge__(
                datetime.datetime(yesterday.year, yesterday.month, yesterday.day, 0, 0, 0, 0)),
                Syslog.logtime.__le__(
                    datetime.datetime(yesterday.year, yesterday.month, yesterday.day, yesterday.hour,
                                      yesterday.minute, yesterday.second,
                                      yesterday.microsecond)))).count()

        print('today syslog counter:', todaySyslogCounter)
        return jsonify({'status': 'Success', 'content': todaySyslogCounter,
                        'compare': int(todaySyslogCounter) - int(yesterdaySyslogCounter)})

    except Exception as e:
        print(e)
        return jsonify({'status': 'Fail', 'content': 0})


@main.route('/alarm_counter', methods=['POST'])
def alarm_counter():
    """
    用于统计返回当天告警出发数量
    :return:
    """
    try:
        now = datetime.datetime.now()

        todayAlarmCounter = AlarmRecord.query.filter(
            and_(AlarmRecord.create_time.__ge__(datetime.datetime(now.year, now.month, now.day, 0, 0, 0, 0)),
                 AlarmRecord.create_time.__le__(
                     datetime.datetime(now.year, now.month, now.day, now.hour, now.minute, now.second,
                                       now.microsecond)))).count()

        print('today alarm counter', todayAlarmCounter)

        return jsonify({'status': 'Success', 'content': todayAlarmCounter if todayAlarmCounter else 0})

    except Exception as e:
        print(e)
        return jsonify({'status': 'Fail', 'content': 0})


@main.route('/wechat_counter', methods=['POST'])
def wechat_counter():
    """
    用于统计返回当天微信告警发送成功百分比
    :return:
    """
    try:
        wechat_presend = redis_db.get(manage_key(key='wechatPreSend', date_type='today'))

        wechat_sent = redis_db.get(manage_key(key='wechatSent', date_type='today'))

        if wechat_presend and wechat_sent:
            send = int(wechat_presend.decode())
            sent = int(wechat_sent.decode())
            percent_success_send = int((sent / send) * 10000) / 100

            print(send, sent, percent_success_send)

            return jsonify({'status': 'Success',
                            'content': str(percent_success_send) if percent_success_send <= 100 else 100 + '%'})
        else:
            return jsonify({'status': 'Fail', 'content': 0})

    except Exception as e:
        logger.error(e)
        return jsonify({'status': 'Fail', 'content': 0})


@main.route('/realTimeSyslogRate', methods=['POST'])
def realTimeSyslogRate():
    """
    用于统计返回当天告警出发数量
    :return:
    """
    try:
        pre_data_ = int(request.form.get('pre_data', '0'))
        interval_ = int(request.form.get('pre_data'))
        logger.debug('getting real time syslog receiving rate')
        syslog_counter_today = redis_db.get(manage_key(key='syslog_recv_counter', date_type='today'))
        if not syslog_counter_today:
            redis_db.set(manage_key(key='syslog_recv_counter', date_type='today'), '1')
            syslog_counter_today = '1'
        else:
            syslog_counter_today = syslog_counter_today.decode()
        print('today\'s syslog counter is:', syslog_counter_today)
        return jsonify({'status': 'Success',
                        'content': (int(syslog_counter_today) - pre_data_) / (
                                    interval_ / 1000) if syslog_counter_today and interval_ and pre_data_ != 0 else 0,
                        'pre_data': syslog_counter_today if syslog_counter_today else 0
                        })

    except Exception as e:
        logger.error('get real time syslog receive rate fail for {}'.format(e))
        return jsonify({'status': 'Fail', 'content': 0})


@main.route('/latest_fifth_alarms', methods=['POST'])
def latest_fifth_alarms():
    """
    最新5条告警信息
    :return:
    """
    try:
        now = datetime.datetime.now()

        latest_alarms = AlarmRecord.query.filter(
            and_(AlarmRecord.create_time.__ge__(datetime.datetime(now.year, now.month, now.day, 0, 0, 0, 0)),
                 AlarmRecord.create_time.__le__(
                     datetime.datetime(now.year, now.month, now.day, 23, 59, 59, 59)))).order_by(
            AlarmRecord.create_time.desc()).limit(5)

        fifth = [[info.content, info.create_time] for info in latest_alarms]
        logger.debug('the fifth is {}'.format(fifth))

        if fifth:
            return jsonify({'status': 'Success',
                            'content': fifth
                            })
        else:
            return jsonify({'status': 'Fail',
                            'content': [['当前无记录', datetime.datetime.now()]]
                            })

    except Exception as e:
        logger.error('get real time syslog receive rate fail for {}'.format(e))
        return jsonify({'status': 'Fail', 'content': 0})


@main.route('/keyAlarmRanking', methods=['POST'])
def keyAlarmRanking():
    """
    用于统计返回当天关键字排名列表
    colors = {
        brand:      '#716aca',
        metal:      '#c4c5d6',
        light:      '#ffffff',
        accent:     '#00c5dc',
        primary:    '#5867dd',
        success:    '#34bfa3',
        info:       '#36a3f7',
        warning:    '#ffb822',
        danger:     '#f4516c'
    };
    :return:
    """
    try:
        logger.debug("start to rank the syslog key alarm records")
        colors = ['danger', 'warning', 'info', 'success', 'primary', 'accent', 'light', 'metal', 'brand']
        logger.debug('getting key alarm ranking')

        alarm_keys = SyslogAlarmConfig.query.filter(SyslogAlarmConfig.alarm_type.__ne__('filter')).all()
        all_keys = {manage_key(key=str(k.id) + '_' + md5_content(k.alarm_keyword), date_type='today'): k.alarm_name for
                    k
                    in alarm_keys}

        key_name_counter = []

        for k, v in all_keys.items():
            value = redis_db.get(k)

            if value:
                key_name_counter.append({'name': v,
                                         'value': value.decode()})

        if key_name_counter:

            sorted_counter = sorted(key_name_counter, key=lambda k: k['value'])

            counter_len = len(sorted_counter)

            counter_len = counter_len if counter_len <= 9 else 9

            print(sorted_counter[:counter_len])

            return jsonify({'status': 'Success', 'data': sorted_counter[:counter_len], 'name': [n['name'] for n in key_name_counter]})

        else:
            return jsonify({'status': 'Fail', 'content': '暂无告警数据'})

    except Exception as e:
        logger.error('get real time syslog receive rate fail for {}'.format(e))
        return jsonify({'status': 'Fail', 'content': 0})


@main.route('/syslog_ranking', methods=['POST'])
def syslog_ranking():
    """
    用于统计返回当天关键字排名列表
    colors = {
        brand:      '#716aca',
        metal:      '#c4c5d6',
        light:      '#ffffff',
        accent:     '#00c5dc',
        primary:    '#5867dd',
        success:    '#34bfa3',
        info:       '#36a3f7',
        warning:    '#ffb822',
        danger:     '#f4516c'
    };
    :return:
    """
    try:
        logger.debug("Starting to rank the syslog records")

        now = datetime.datetime.now()

        today_syslog_ranking = Syslog.query.with_entities(Syslog.device_ip.label('device_ip'),
                                                          func.count('*').label('count')).filter(
            and_(Syslog.logtime.__ge__(datetime.datetime(now.year, now.month, now.day, 0, 0, 0, 0)),
                 Syslog.logtime.__le__(
                     datetime.datetime(now.year, now.month, now.day, 23, 59, 59, 59)))).group_by(
            Syslog.device_ip).order_by(
            func.count('*').label('count').desc()).limit(10)

        target_device = [ranking_device.device_ip for ranking_device in today_syslog_ranking]

        if target_device:
            serverty_dict = {'error': [], 'critical': [], 'alert': [], 'emergency': []}

            legend = ['error', 'critical', 'alert', 'emergency']

            for log in today_syslog_ranking:
                target_devices_logs = Syslog.query.with_entities(Syslog.device_ip.label('device_ip'),
                                                                 Syslog.serverty.label('serverty'),
                                                                 func.count('*').label('count')).filter(
                    and_(Syslog.logtime.__ge__(datetime.datetime(now.year, now.month, now.day, 0, 0, 0, 0)),
                         Syslog.logtime.__le__(
                             datetime.datetime(now.year, now.month, now.day, 23, 59, 59,
                                               59)),
                         Syslog.device_ip.__eq__(log.device_ip))).group_by(Syslog.serverty).all()
                print('the target devices logs have: ', target_devices_logs)
                tmpList = []
                for one_log in target_devices_logs:
                    tmpList.append(one_log.serverty)
                    serverty_dict[one_log.serverty].append(one_log.count)

                for not_matched in list(set(legend) - set(tmpList)):
                    serverty_dict[not_matched].append(0)

            return jsonify({'status': 'Success',
                            'labels': target_device,
                            'error_list': serverty_dict['error'],
                            'critical_list': serverty_dict['critical'],
                            'alert_list': serverty_dict['alert'],
                            'emergency_list': serverty_dict['emergency']})
        else:
            return jsonify(
                {'status': 'Fail', 'labels': ['暂无Syslog记录'], 'error_list': ['0'], 'critical_list': ['0'],
                 'alert_list': ['0'],
                 'emergency_list': ['0']})

    except Exception as e:
        logger.error('get real time syslog receive rate fail for {}'.format(e))
        return jsonify({'status': 'Fail', 'content': 0})
