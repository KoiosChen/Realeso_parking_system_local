from . import HashContent, SeqPickle, GetConfig
from .DoAlarm import *
from .. import db, logger
from ..models import AlarmRecord, CallRecordDetail, OntAccountInfo
import time
import random
import datetime
from sqlalchemy import and_


def alarm(**kwargs):
    """
    用于处理告警信息，并发出告警
    :param alarm_content:  告警内容
    :param alarm_type: 告警类型
    :return:  no return
    """
    api_config = GetConfig.get_config('alarmconfig')
    alarm_content = kwargs.get('alarm_content')
    alarm_type = kwargs.get('alarm_type')

    # 这里存在的问题是,当产生的告警有网工接听电话之后,再次轮询到这个故障,并且未解决的时候,会再次发出告警,这个时候可以考虑在一定时间内再次
    # 发现这个告警的时候, 增加一个已接听故障重复告警间隔
    alarm_now = ''
    alarm_not_fixed = ''
    record = []
    not_fixed_record = []
    call_group = HashContent.md5_content(
        time.strftime('%Y-%m-%d %H:%M:%S', time.localtime()) + str(random.randint(1, 20)))
    call_interval = datetime.timedelta(minutes=api_config.get('call_interval') or 480)
    call_again = datetime.timedelta(minutes=api_config.get('call_again') or 60)

    for alarm in alarm_content:
        content_md5 = HashContent.md5_content(alarm.split('>>>')[0])
        logger.debug('Alarm  content {}'.format(alarm))
        logger.debug('The md5 is : {}'.format(content_md5))

        # find_md5 = db.session.query(AlarmRecord).filter_by(content_md5=content_md5).order_by(AlarmRecord.id.desc()).first()
        find_md5 = AlarmRecord.query.filter(AlarmRecord.content_md5.__eq__(content_md5),
                                            AlarmRecord.alarm_type.__ne__('999')).order_by(
            AlarmRecord.id.desc()).first()

        if find_md5:
            logger.debug('{} {}'.format(find_md5.id, find_md5.content_md5))
            end_num = int(call_interval / call_again)
            # seqnum = SeqPickle.Seq('tmp/' + find_md5.content_md5 + '.pkl')
            seqnum = SeqPickle.Seq(find_md5.content_md5)
            start_num = int(seqnum.last_seq) + 1

        if not find_md5 \
                or ((find_md5 and find_md5.state == 9)
                    and datetime.datetime.now() - find_md5.create_time > call_interval):
            logger.info('The alarm is not exist, or finished( not sovled in 60min). start to create a new alarm')
            if find_md5:
                time_delta = datetime.datetime.now() - find_md5.create_time
                logger.debug('Time delta {}'.format(time_delta))

            # generate a new alarm record
            alarm_record = AlarmRecord(content=alarm,
                                       content_md5=content_md5,
                                       state=1,
                                       alarm_type=alarm_type,
                                       create_time=time.localtime(),
                                       call_group=call_group)

            db.session.add(alarm_record)
            record.append(alarm_record)

            # 查找是否存在该条告警的附加信息 20170714 目前仅有批量光猫下线对应的信息
            alarm_attachment = OntAccountInfo.query.filter_by(hash_id=content_md5).first()

            # 如果告警存在附加信息，则添加在告警内容后, 此处可能存在字符过多不能发送的问题
            if alarm_attachment:
                alarm += '\n附加信息：' + alarm_attachment.account_info
            alarm_now += alarm
            db.session.commit()

        elif (find_md5 and find_md5.state == 9) \
                and int(((datetime.datetime.now() - find_md5.create_time) / call_again)) in range(start_num, end_num):
            logger.info('The alarm has been confirmed, '
                        'but not be solved, '
                        'call again to the engineer who answered the phone just now')

            not_fixed_record.append(find_md5)
            # 查找是否存在该条告警的附加信息 20170714 目前仅有批量光猫下线对应的信息
            alarm_attachment = OntAccountInfo.query.filter_by(hash_id=content_md5).first()

            # 如果告警存在附加信息，则添加在告警内容后, 此处可能存在字符过多不能发送的问题
            if alarm_attachment:
                alarm_a = '\n附加信息：' + alarm_attachment.account_info
            else:
                alarm_a = ''

            alarm_not_fixed += find_md5.content + alarm_a

            if start_num < end_num:
                seqnum.update_seq(start_num)
        else:
            logger.info('The alarm exist and not finished yet')

    alarm_now += '\n' + time.strftime('%Y-%m-%d %H:%M:%S', time.localtime()) + '\nCacti 自动告警\n'
    alarm_not_fixed += '\n' + time.strftime('%Y-%m-%d %H:%M:%S', time.localtime()) + '\nCacti 自动告警\n'

    # 用于提交之前的数据
    if len(record) > 0:
        send_wechat(alarm_now)

        call_info = {'index': 0,
                     'alarm_record': record,
                     'type': 0,
                     'alarm_type': alarm_type,
                     'call_group': call_group
                     }

        phone_call(**call_info)

    if len(not_fixed_record) > 0:
        send_wechat(alarm_not_fixed)

        call_info = {'index': 0,
                     'alarm_record': not_fixed_record,
                     'type': 99,
                     'alarm_type': alarm_type,
                     'call_group': call_group
                     }

        # phone_call(**call_info)
    db.session.expire_all()
    db.session.close()
    return True


def alarmMonitor():
    """
    轮询告警
    :return:
    """
    numberList = PhoneNumber.get_number_list()
    max_called_times = 5

    # 查找未接听或未拨打成功的的告警
    alarm_record = AlarmRecord.query.filter(and_(AlarmRecord.state.__ne__(9), AlarmRecord.state.__ne__(8)),
                                            AlarmRecord.calledTimes.__lt__(max_called_times)). \
        order_by(AlarmRecord.create_time.desc()). \
        all()

    # 将未接听次数大于 max_called_times 的告警记录状态设为8
    not_answered_alarm_record = AlarmRecord.query. \
        filter(and_(AlarmRecord.state.__ne__(9), AlarmRecord.state.__ne__(8)),
               AlarmRecord.calledTimes.__ge__(max_called_times)).all()

    for noanswered in not_answered_alarm_record:
        noanswered.state = 8
        db.session.add(noanswered)
    db.session.commit()

    if alarm_record:
        # 产生一个即时的call_group, 若未呼叫的告警的call_group数量大于1, 则用新产生的call_group
        call_group = HashContent.md5_content(
            time.strftime('%Y-%m-%d %H:%M:%S', time.localtime()) + str(random.randint(1, 20)))

        # call_group计数
        call_group_count = []
        # 未告警的内容, 用于发送微信, 产生发送微信的文本, 并且在文末增加当前时间和提示语
        unalarm_content = ''

        for unalarm in alarm_record:
            unalarm_content += unalarm.content
            call_group_count.append(unalarm.call_group)
            logger.debug('{} {} {} {} {}'.
                         format(unalarm.id,
                                unalarm.lastCallId,
                                unalarm.content,
                                unalarm.content_md5,
                                unalarm.call_group))

        unalarm_content += '\n' + time.strftime('%Y-%m-%d %H:%M:%S', time.localtime()) + '\nCacti告警, 电话未接听再次提醒\n'

        # 未接听电话的告警暂时考虑不重复发送微信通知
        # send_wechat(unalarm_content)

        call_group_set = set(call_group_count)
        logger.debug('call group set is : {}.'.format(call_group_set))

        logger.debug('The last call id is {}.'.format(alarm_record[0].lastCallId))

        if len(call_group_set) == 1 and alarm_record[0].lastCallId is not None:
            logger.info('There is only one call group {}, and has a callid {}'.
                        format(call_group_set, alarm_record[0].lastCallId))

            call_record_by_group = CallRecordDetail.query.filter_by(call_group=alarm_record[0].call_group). \
                order_by(CallRecordDetail.id.desc()).all()

            for crgb in call_record_by_group:
                if crgb.phoneNum:
                    try:
                        i = numberList.index(crgb.phoneNum) + 1
                    except Exception as e:
                        i = 0
                    break

            # voice_callback = VoiceNotifyCallBack.query.filter_by(callId=alarm_record[0].lastCallId).first()
            # 这里做了修改,不从callback中取呼叫号码,而从呼叫记录中取被叫号码

            if not len(numberList[i:len(numberList)]):
                i = 0
            call_info = {'index': i,
                         'alarm_record': alarm_record,
                         'type': 0,
                         'call_group': call_group
                         }

        else:
            call_info = {'index': 0,
                         'alarm_record': alarm_record,
                         'type': 0,
                         'call_group': call_group
                         }

        # 2017-01-30 现在考虑是将再次呼叫的alarm_record的call_group都设置为新的call_group, 因为这样才能和新的
        # call_record_detail 关联起来。 但是这样就不能对应之前产生的呼叫记录, 无法知道之前呼叫失败的对应的号码, 但是
        # 可以新增一个call_history的表, 用来记录历史每次alarm_record.id 和  call_id 以及呼叫时间的对应记录
        for r in alarm_record:
            r.call_group = call_group
            db.session.add(r)
            db.session.commit()

        phone_call(**call_info)

    else:
        logger.info('no alarm')
