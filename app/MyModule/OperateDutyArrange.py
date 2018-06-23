from openpyxl import load_workbook
from ..models import DutyAttendedTime, DutySchedule, User, duty_schedule_status
from .. import logger, db
import re
from datetime import datetime, date, timedelta
from collections import defaultdict
from sqlalchemy import or_, and_
import time


def match_duty_range(**kwargs):
    name = kwargs.get('name')  # not used here now
    start_time = kwargs.get('start_time')
    stop_time = kwargs.get('stop_time')
    strict = kwargs.get('strict')
    duty_id_list = []

    logger.debug('start time: {}; stop time: {}'.format(start_time, stop_time))

    if start_time <= stop_time:
        day_adjust = 0
    else:
        day_adjust = 1

    logger.debug('day adjust is {}'.format(day_adjust))

    if strict:  # it's a boole
        duty_range = DutyAttendedTime.query.filter_by(start_time=start_time,
                                                      stop_time=stop_time,
                                                      status=1,
                                                      day_adjust=day_adjust).first()

        if duty_range:
            duty_id_list.append(duty_range.id)

    else:
        duty_range = DutyAttendedTime.query.filter(DutyAttendedTime.start_time.__le__(start_time),
                                                   DutyAttendedTime.stop_time.__ge__(stop_time),
                                                   DutyAttendedTime.status.__eq__(1),
                                                   DutyAttendedTime.day_adjust.__eq__(day_adjust)).all()

        duty_id_timedelta = defaultdict(list)
        for duty in duty_range:
            schedule_time_interval = datetime.combine(date.min, duty.stop_time) + \
                                     duty.day_adjust * timedelta(seconds=86400) - \
                                     datetime.combine(date.min, duty.start_time)

            real_time_interval = datetime.combine(date.min, stop_time) + \
                                 day_adjust * timedelta(seconds=86400) - \
                                 datetime.combine(date.min, start_time)

            duty_id_timedelta[duty.id] = schedule_time_interval - real_time_interval

        duty_id_list = sorted(duty_id_timedelta, key=duty_id_timedelta.__getitem__)

    return duty_id_list.pop(0) if duty_id_list else False


def read_duty_arrange(file_name, sheet_name, strict):
    wb = load_workbook(file_name, read_only=True)
    ws = wb[sheet_name]
    i = 1
    title = []
    row_list = []
    break_flag = False
    for row in ws.rows:
        cell_value_list = []
        if i == 1:  # 获取标题行
            for cell in row:
                logger.debug('cell value is {}'.format(cell.value))
                if re.search(r'date', cell.value):
                    title.append('date')

                elif re.search(r'\d+:\d+-\d+:\d+', cell.value):
                    logger.debug('The duty attend time may be {}'.format(cell.value))
                    try:
                        start_time, stop_time = cell.value.split('-')
                        start_time = datetime.strptime(start_time, "%H:%M").time()
                        stop_time = datetime.strptime(stop_time, "%H:%M").time()

                        duty_attend_time_id = match_duty_range(start_time=start_time,
                                                               stop_time=stop_time,
                                                               strict=strict)  # 此处应该是唯一值

                        if duty_attend_time_id:
                            title.append(duty_attend_time_id)
                        else:
                            break_flag = True
                            logger.warn('The time range formate error')
                            break
                    except Exception as e:
                        logger.error(e)
                        break_flag = True
                        break
            if 'date' not in title:
                break_flag = True

        else:
            for cell in row:
                logger.debug('cell value is {} type {}'.format(cell.value, type(cell.value)))
                if isinstance(cell.value, datetime):
                    duty_date = cell.value.date()
                    cell_value_list.append(duty_date)
                elif re.search(r'\d+/\d+/\d+', cell.value):
                    logger.debug('This cell may be a date {}'.format(cell.value))
                    try:
                        duty_date = datetime.strptime(cell.value, '%Y-%m-%d %H:%M:%S').date()
                        cell_value_list.append(duty_date)
                    except ValueError as e:
                        logger.error(e)
                        logger.error('Date format error {}'.format(cell.value))
                        cell_value_list.pop()  # 是否应该pop?
                        break
                elif re.search(r'\w+[,|，]?\w+', cell.value):
                    logger.debug('The duty engineer is(are) {}'.format(cell.value))
                    try:
                        # 半角逗号
                        duty_engineer = cell.value.split(',')
                    except Exception as e:
                        try:
                            # 全角逗号
                            duty_engineer = cell.value.split('，')
                        except Exception as e:
                            logger.error(e)
                            logger.error('Duty engineer format error {}'.format(cell.value))
                            cell_value_list.pop()
                            break

                    tmp_list = []
                    for engineer in duty_engineer:
                        eng = User.query.filter_by(username=engineer).first()
                        if eng:
                            tmp_list.append(eng.id)
                    cell_value_list.append(tmp_list)

        if break_flag:
            # The format of title is wrong
            break

        if cell_value_list:
            row_list.append(cell_value_list)

        i += 1  # i plus 1 for next row

    return title, row_list


def add_duty_arrange(title, row_list):
    row_count = 0
    row_all = 0
    try:
        for row in row_list:
            date_time = row[0]
            zip_data = zip(title[1:], row[1:])
            for duty_data in zip_data:
                attended_time_id = duty_data[0]
                for priority_index, engineer in enumerate(duty_data[1]):
                    row_all += 1
                    test_exist = DutySchedule.query.filter_by(date_time=date_time,
                                                              userid=engineer,
                                                              attended_time_id=attended_time_id).first()
                    if not test_exist:
                        duty_add = DutySchedule(date_time=date_time,
                                                userid=engineer,
                                                attended_time_id=attended_time_id,
                                                duty_status=1,
                                                priority=priority_index,
                                                create_time=time.localtime())
                        db.session.add(duty_add)
                        db.session.commit()
                        row_count += 1
                    else:
                        logger.warn('{} {} {} is exist'.format(date_time, engineer, attended_time_id))
        logger.info('Totally import {} row(s)'.format(row_count))
        return '共%s条值班信息, 导入%s条' % (row_all, row_count)
    except Exception as e:
        return '系统繁忙'


def print_duty_schedule(**kwargs):
    check_month = kwargs.get('check_month') or date.today().month
    check_year = kwargs.get('check_year') or date.today().year
    this_month_begin = date(check_year, check_month, 1)
    d = this_month_begin
    if check_month == 12:
        check_year += 1
        next_month = 1
    else:
        next_month = check_month + 1
    this_month_end = date(check_year, next_month, 1) - timedelta(days=1)

    users = {u.id: {'username': u.username, 'phoneNum': u.phoneNum} for u in User.query.all()}

    duty_attended_time = DutyAttendedTime.query.filter_by(status=1).order_by(DutyAttendedTime.start_time).all()

    for x in duty_attended_time:
        logger.debug(x.attended_time_name + '' + str(x.id))

    intramonth_duty_schedule = DutySchedule.query.filter(and_(DutySchedule.date_time.__ge__(this_month_begin),
                                                         DutySchedule.date_time.__le__(this_month_end))).all()

    duty_id_set = set([i.attended_time_id for i in intramonth_duty_schedule])

    logger.debug("duty id set {}".format(duty_id_set))

    title = {}
    title['date'] = '日期'
    ti = 0
    duty_arrangement = []
    break_flag = False
    while d <= this_month_end:
        logger.debug(d)
        tmp_day_list = {}
        tmp_day_list['date'] = d.strftime('%Y-%m-%d')
        for d_time in duty_attended_time:
            logger.debug("id {} name {}".format(d_time.id, d_time.attended_time_name))
            append_content = d_time.attended_time_name + ':' + \
                             d_time.start_time.strftime('%H:%M') + '--' + d_time.stop_time.strftime('%H:%M')

            if d_time.id in duty_id_set and append_content not in title.values():
                tn = 't' + str(ti)
                title[tn] = append_content
                ti += 1
            elif append_content in title.values():
                for k, v in title.items():
                    if v == append_content:
                        tn = k
                        break
            else:
                continue

            if break_flag:
                break

            intraday_duty_schedule = DutySchedule.query.filter(DutySchedule.date_time.__eq__(d),
                                                               DutySchedule.attended_time_id.__eq__(d_time.id),
                                                               or_(DutySchedule.duty_status.__eq__(1),
                                                                   DutySchedule.duty_status.__eq__(5))).\
                order_by(DutySchedule.priority).all()

            tmp_member_info_list = []
            for member in intraday_duty_schedule:
                member_info = users[int(member.userid)]['username'] + '(' + users[int(member.userid)]['phoneNum'] + ')'
                tmp_member_info_list.append(member_info)

            tmp_day_list[tn] = ', '.join(tmp_member_info_list)
        duty_arrangement.append(tmp_day_list)
        d += timedelta(days=1)

    logger.debug(title)
    logger.debug(duty_arrangement)

    return title, duty_arrangement


def change_duty_schedule_status(**kwargs):
    print(kwargs)
    date_time = datetime.strptime(kwargs.get('intraday'), '%Y-%m-%d')
    action_id = int(kwargs.get('action_val'))
    attended_time_name = kwargs.get('duty_attended_time_val').split(':')[0]
    if attended_time_name == '0':
        return '未选择时间'
    else:
        start_time, stop_time = re.findall(r'(\d+:\d+)--(\d+:\d+)', kwargs.get('duty_attended_time_val'))[0]

    taget_engineer = kwargs.get('adjust_select_val')
    if taget_engineer == 'undefined':
        taget_engineer = None
    else:
        taget_engineer = int(taget_engineer)

    userid = int(kwargs.get('duty_engineer_val')) if action_id != 7 else taget_engineer

    attended_time_id = DutyAttendedTime.query.filter_by(start_time=start_time, stop_time=stop_time).first()

    if action_id != 7 and userid == 0:
        return '未选择值班人员'

    duty_schedule = DutySchedule.query.filter(DutySchedule.date_time.__eq__(date_time),
                                              DutySchedule.userid.__eq__(userid),
                                              DutySchedule.attended_time_id.__eq__(attended_time_id.id)).first()
    if action_id == 1 or action_id == 5 or action_id == 7:
        if duty_schedule and action_id != 7:
            duty_schedule.duty_status = action_id
            db.session.add(duty_schedule)
        elif duty_schedule and action_id == 7:
            return '值班人员已存在,可修改其状态'
        else:
            pri = DutySchedule.query.filter(DutySchedule.date_time.__eq__(date_time),
                                            DutySchedule.attended_time_id.__eq__(attended_time_id.id),
                                            or_(DutySchedule.duty_status.__eq__(1),
                                                DutySchedule.duty_status.__eq__(5))).all()
            pri_max = max(set([p.priority for p in pri])) + 1 if pri else 0
            new_duty_schedule = DutySchedule(date_time=date_time,
                                             userid=userid,
                                             attended_time_id=attended_time_id.id,
                                             duty_status=action_id if action_id != 7 else 1,
                                             priority=pri_max,
                                             create_time=time.localtime())
            db.session.add(new_duty_schedule)

    elif action_id == 2 or action_id == 3 or action_id == 6:
        duty_schedule.duty_status = action_id
        db.session.add(duty_schedule)

    elif action_id == 4:
        if duty_schedule.duty_status == 1 or duty_schedule.duty_status == 5:  # 当值班状态不是调休、事假、调班时才能调班
            duty_schedule.duty_status = action_id
            pri = DutySchedule.query.filter(DutySchedule.date_time.__eq__(date_time),
                                            DutySchedule.attended_time_id.__eq__(attended_time_id.id),
                                            or_(DutySchedule.duty_status.__eq__(1),
                                                DutySchedule.duty_status.__eq__(5))).all()
            pri_max = max(set([p.priority for p in pri])) + 1 if pri else 0
            add_duty = DutySchedule(date_time=date_time,
                                    userid=taget_engineer,
                                    attended_time_id=attended_time_id.id,
                                    duty_status=1,
                                    priority=pri_max,
                                    create_time=time.localtime())
            db.session.add(add_duty)
        else:
            logger.warn('Cannot alternate with other engineer')
            return '所选人员当班状态非正常班'

        db.session.add(duty_schedule)

    db.session.commit()
    db.session.close()
    return 'OK'


def shift_duty_work(**kwargs):
    pass


def duty_leave(**kwargs):
    pass
