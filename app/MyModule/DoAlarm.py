from . import WechatAlarm
from . import PhoneNumber
from . import CallAlarm
from collections import defaultdict
from ..models import CallRecordDetail
from .. import logger, db
import time


def analysis_resp(**kwargs):
    phoneNum = kwargs.get('phoneNum')
    resp = kwargs.get('resp')
    call_group = kwargs.get('call_group')
    alarm_record = kwargs.get('alarm_record')

    if resp.json()['resp']['respCode'] == '000000':
        voiceNotify = resp.json()['resp']['voiceNotify']
        logger.info('call {} successful createDate: {}: vocieNotify: {}'
                    .format(phoneNum, voiceNotify['createDate'], voiceNotify['callId']))

        alarm_new_inst = CallRecordDetail(phoneNum=phoneNum,
                                          respCode=resp.json()['resp']['respCode'],
                                          callId=voiceNotify['callId'],
                                          createDateInResp=voiceNotify['createDate'],
                                          create_time=time.localtime(),
                                          call_group=call_group)

        for r in alarm_record:
            r.lastCallId = voiceNotify['callId']
            if r.calledTimes is None:
                r.calledTimes = 1
            else:
                r.calledTimes += 1

        db.session.add(alarm_new_inst)
        db.session.commit()

        return True

    else:
        print('call {} fail respCode: {}'.format(phoneNum, resp.json()['resp']['respCode']))
        fail_inst = CallRecordDetail(phoneNum=phoneNum,
                                     respCode=resp.json()['resp']['respCode'],
                                     create_time=time.localtime(),
                                     call_group=call_group)
        db.session.add(fail_inst)
        db.session.commit()

        return False


def send_wechat(content):
    logger.info('send a wechat message')
    wechat = WechatAlarm.WechatAlarm()
    c = wechat.init_text(content)
    wechat.sendMsg(c)


def phone_call(**kwargs):
    """

    :param i:  the index of number list
    :param alarm_record:  the object of alarm_records
    :return:  nothing
    """

    i = kwargs.get('index')
    alarm_record = kwargs.get('alarm_record')
    call_group = kwargs.get('call_group')
    type = kwargs.get('type')
    alarm_type = kwargs.get('alarm_type')

    numberList = PhoneNumber.get_number_list(alarm_type=alarm_type)

    if numberList:
        call = CallAlarm.CallAlarm()
        if type != 99:
            for phoneNum in numberList[i:len(numberList)]:
                resp = call.do_call(phoneNum, '528793')
                all_resp_info = {'phoneNum': phoneNum,
                                 'resp': resp,
                                 'call_group': call_group,
                                 'alarm_record': alarm_record}
                if analysis_resp(**all_resp_info):
                    break

        elif type == 99:
            phone_list = defaultdict(list)
            for rr in alarm_record:
                if rr.lastCallId:
                    phoneNum = CallRecordDetail.query.filter_by(callId=rr.lastCallId).first()
                    phone_list[phoneNum.phoneNum].append(rr)
                else:
                    phone_list['0'].append(rr)
            logger.debug(phone_list)

            for phone, alarm_obj in phone_list.items():
                logger.debug(alarm_obj)
                logger.debug(type(alarm_obj))
                if phone != '0':
                    resp = call.do_call(phone, '528793')
                    # for ar in alarm_obj:
                    logger.debug(alarm_obj)
                    all_resp_info = {'phoneNum': phone,
                                     'resp': resp,
                                     'call_group': call_group,
                                     'alarm_record': alarm_obj}
                    analysis_resp(**all_resp_info)
                else:
                    call_info = {'index': 0,
                                 'alarm_record': alarm_obj,
                                 'type': 0,
                                 'call_group': call_group
                                 }
                    phone_call(**call_info)
    else:
        for rr in alarm_record:
            rr.state = 9
            db.session.add(rr)
        db.session.commit()
        db.session.expire_all()
        db.session.close()
        logger.warn('There is no phone number to be called in this time {}'.
                    format(time.strftime('%Y-%m-%d %H:%M:%S', time.localtime())))
