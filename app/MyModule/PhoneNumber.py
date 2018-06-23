from ..models import DutyAttendedTime, User, DutySchedule, JobDescription, CallTimeRange
from .. import logger
from datetime import datetime, timedelta, date
import time
from sqlalchemy import or_
from .GetConfig import get_config


def valid_call(alarm_type, this_date_time):
    night_call = get_config('alarmpolicy')
    if night_call['night_call'] == '1':
        logger.debug('night call is {}'.format(night_call['night_call']))
        valid_time = CallTimeRange.query.filter_by(status=1).all()
        for target in valid_time:
            start_time = datetime.combine(date.today(), target.start_time)
            stop_time = datetime.combine(date.today(), target.stop_time)
            if target.day_adjust:
                if this_date_time < start_time and this_date_time < stop_time:
                    start_time -= timedelta(days=1)
                elif this_date_time > stop_time and this_date_time > start_time:
                    stop_time += timedelta(days=1)
                elif stop_time < this_date_time < start_time:
                    stop_time += timedelta(days=1)

            if start_time <= this_date_time <= stop_time:
                valid_alarm_type = target.valid_alarm_type

                if alarm_type == 'all':
                    return True
                elif valid_alarm_type:
                    if alarm_type in valid_alarm_type.split(','):
                        return True
    else:
        logger.debug('night call is {}, do not do alarm action'.format(night_call['night_call']))

    return False


def get_number_list(**kwargs):
    alarm_type = kwargs.get('alarm_type')
    this_date_time = datetime.now()
    now_date = datetime.now().date()
    numberList = []

    if valid_call(alarm_type, this_date_time):
        duty_schedule = DutyAttendedTime.query.all()
        tmp = get_config('alarmpolicy')
        final_number_list = tmp['final_number_list'].split(',')

        for duty in duty_schedule:
            start_time = datetime.combine(date.today(), duty.start_time)
            stop_time = datetime.combine(date.today(), duty.stop_time)
            if duty.day_adjust:
                if this_date_time < start_time and this_date_time < stop_time:
                    start_time -= timedelta(days=1)
                elif this_date_time > stop_time and this_date_time > start_time:
                    stop_time += timedelta(days=1)
                elif stop_time < this_date_time < start_time:
                    stop_time += timedelta(days=1)

            if start_time <= this_date_time <= stop_time:
                # 获取值班时间范围内的值班人员的清单
                logger.debug(duty.id)
                if this_date_time.date() == stop_time.date():
                    now_date -= timedelta(days=1)
                duty_member = DutySchedule.query.filter(DutySchedule.attended_time_id.__eq__(duty.id),
                                                        or_(DutySchedule.date_time.__eq__(now_date),
                                                            DutySchedule.date_time.__eq__(None)),
                                                        DutySchedule.duty_status.__eq__(1)).order_by(DutySchedule.priority).first()

                duty_member = duty_member.userid.split(',')
                logger.debug(duty_member)
                if duty_member:
                    for member in duty_member:
                        logger.debug(member)
                        user_prepare = User.query.filter_by(id=member, status=1).first()
                        if user_prepare:
                            logger.debug('user_prepare {}'.format(user_prepare.duty))
                            if alarm_type:
                                alarm_type_all = JobDescription.query.filter(
                                    JobDescription.job_id.__eq__(user_prepare.duty)).first()
                                if alarm_type in alarm_type_all.alarm_type.split(','):
                                    logger.debug('Call phone {}'.format(user_prepare.phoneNum))
                                    numberList.append(user_prepare.phoneNum)
                            else:
                                logger.debug('Call phone {}'.format(user_prepare.phoneNum))
                                numberList.append(user_prepare.phoneNum)
                else:
                    logger.warn('There is no corresponding duty on date {}'.
                                format(time.strftime('%Y-%m-%d %H:%M:%S', time.localtime())))

        if not len(numberList):
            if final_number_list:
                logger.warn('There is no suitable phone number to be called, use the default phone number list')
                numberList = final_number_list
            elif not final_number_list:
                logger.warn('There is no suitable phone number to be called, and no default phone number list')

        logger.debug(numberList)
        return list(set(numberList))
    else:
        logger.warn('There\'s no valid call time')
        return numberList


if __name__ == '__main__':
    get_number_list(alarm_type='1')
