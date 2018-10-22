from flask import session
from datetime import datetime, timedelta, date
from collections import defaultdict
from sqlalchemy import or_, and_
from ..models import ParkingRecords, ParkingOrder, OrderAndRecords
from .. import logger, db


def parking_rules():
    return {'free_minutes': timedelta(minutes=15),
            'start_minutes': timedelta(minutes=60),
            # 第一个计费周期价格，按照单价的倍数来计算
            'start_price': 1,
            'accounting_interval': timedelta(minutes=30),
            'max_fee_per_day': 80}


def datetime_format_date(dt):
    return date(dt.year, dt.month, dt.day)


def datetime_start(dt):
    return datetime(dt.year, dt.month, dt.day, 0, 0, 0)


def datetime_end(dt):
    return datetime_start(dt) + timedelta(days=1)


def spilt_date(date_dict, time_delta_in_date, start, stop, discount=1.0):
    logger.debug('split_date input: {} {}'.format(start, stop))
    date_dict[datetime_format_date(start)].append(start)
    date_dict[datetime_format_date(stop)].append(stop)
    starttime_end = datetime_end(start)
    endtime_start = datetime_start(stop)
    logger.debug('start time end {}  end time start {}'.format(starttime_end, endtime_start))

    order_time_delta = (stop - start) * discount
    logger.debug('Time delta of the order is {}'.format(order_time_delta))

    days = (endtime_start - starttime_end) / timedelta(days=1)
    logger.debug('days {}'.format(days))

    if days >= 0:
        if time_delta_in_date is not None:
            time_delta_in_date[datetime_format_date(start)].append((starttime_end - start) * discount)
            logger.debug(time_delta_in_date)
            time_delta_in_date[datetime_format_date(starttime_end)].append((stop - endtime_start) * discount)
            logger.debug(time_delta_in_date)

        date_dict[datetime_format_date(start)].append(datetime_end(start))
        date_dict[datetime_format_date(stop)].append(datetime_start(stop))

        for d in range(0, int(days) + 1):
            if time_delta_in_date is not None:
                time_delta_in_date[datetime_format_date(starttime_end + timedelta(days=d))].append(
                    timedelta(days=1) * discount)

            date_dict[datetime_format_date(starttime_end + timedelta(days=d))].append(
                starttime_end + timedelta(days=d))
            date_dict[datetime_format_date(starttime_end + timedelta(days=d))].append(
                starttime_end + timedelta(days=d + 1))
    else:
        if time_delta_in_date is not None:
            time_delta_in_date[datetime_format_date(start)].append(order_time_delta)

    return date_dict, time_delta_in_date


def bill_time(number_plate, exit_time, entry_time):
    """

    :param number_plate:
    :param exit_time:
    :param entry_time:
    :return:
    """
    logger.debug('Billing {}, exit time is {}'.format(number_plate, exit_time))

    if isinstance(exit_time, str):
        exit_time = datetime.strptime(exit_time, '%Y-%m-%d %H:%M:%S')

    entry_record = ParkingRecords.query.filter(ParkingRecords.number_plate.__eq__(number_plate),
                                               ParkingRecords.status.__eq__(0)).order_by(
        ParkingRecords.create_time.desc()).first()

    real_entry_time = entry_time if entry_time is not None else entry_record.entry_time

    free_order = None

    date_dict = defaultdict(list)

    date_dict, _ = spilt_date(date_dict, None, real_entry_time, exit_time)

    # 初始化time_list 列表，入场和出场时间为初始数值
    time_delta_in_date = defaultdict(list)

    all_related_orders = ParkingOrder.query.filter(ParkingOrder.number_plate.__eq__(number_plate),
                                                   ParkingOrder.status.__eq__(1),
                                                   or_(and_(ParkingOrder.order_validate_start.__le__(exit_time),
                                                            ParkingOrder.order_validate_stop.__ge__(exit_time)
                                                            ),
                                                       and_(ParkingOrder.order_validate_start.__le__(
                                                           real_entry_time),
                                                           ParkingOrder.order_validate_stop.__ge__(
                                                               real_entry_time)))
                                                   ).all()

    logger.debug(all_related_orders)

    if all_related_orders:
        # 查找所有有效订单，考虑可能出现所有订单。如果存在实际停车时间超出包月、预约等的时间：
        # 1. 如果没有免费停车的订单，则按照10元计费
        # 2. 如果有免费停车的订单，则按照免费订单对应的账户进行扣费
        try:
            for o in all_related_orders:
                if not OrderAndRecords.query.filter_by(record_id=entry_record.uuid, order_id=o.uuid).first():
                    order_and_record = OrderAndRecords(record_id=entry_record.uuid, order_id=o.uuid)
                    db.session.add(order_and_record)
            db.session.commit()
        except Exception as e:
            logger.error('订单记录关联失败 {}'.format(str(e)))
            db.session.rollback()

        record_relate_orders = OrderAndRecords.query.filter(
            OrderAndRecords.record_id.__eq__(entry_record.uuid)).all()

        logger.debug('record relate orders {}'.format(record_relate_orders))

        for ro in record_relate_orders:
            logger.debug('This related order type is {}'.format(ro.order.order_type))
            if ro.order.order_type in (1, 2, 3) and ro.order.status == 1:
                logger.debug('The order uuid is {}'.format(ro.order.uuid))
                date_dict, time_delta_in_date = spilt_date(date_dict, time_delta_in_date,
                                                           ro.order.order_validate_start if ro.order.order_validate_start > real_entry_time else real_entry_time,
                                                           ro.order.order_validate_stop if ro.order.order_validate_stop < exit_time else exit_time,
                                                           discount=ro.order.discount)

            elif ro.order.order_type == 4 and ro.order.status == 1:
                free_order = ro.order.uuid

    logger.debug('date dict: {} \n time delta: {}'.format(str(date_dict), str(time_delta_in_date)))

    return date_dict, time_delta_in_date, free_order


def unit_price():
    """
    单价计算要修改为动态价格
    :return:
    """
    return 10


def do_pay(parking_record_id, fee, operate_source=None):
    try:
        logger.debug("Pay for the parking record {}".format(parking_record_id))
        parking_record = ParkingRecords.query.filter_by(uuid=parking_record_id).first()
        if parking_record.fee is None:
            parking_record.fee = fee
        else:
            parking_record.fee += fee
        parking_record.operate_source = operate_source
        if session.get('SELFID'):
            parking_record.cashier_id = session.get('SELFID')
        db.session.add(parking_record)
        db.session.commit()
        return True
    except Exception as e:
        db.session.rollback()
        logger.error('Do pay fail for {} '.format(e))
        return False


def check_fee(pay_time, flag, start_flag=True):
    rules = parking_rules()
    logger.debug('checking fee, the first day flag is {} the start flag is {}'.format(flag, start_flag))
    if pay_time <= rules['free_minutes'] and flag:
        return 0
    if rules['free_minutes'] < pay_time <= rules['start_minutes'] + rules['free_minutes'] and flag and start_flag:
        return rules['start_price']
    elif pay_time > timedelta(seconds=1):
        # 首个计费时长
        sms = rules['start_minutes'].total_seconds() if flag else 0

        # 所需付费时长
        logger.debug('Total pay time is {}'.format(pay_time.total_seconds()))
        logger.debug(
            'First day free time + start minutes: {}'.format(str(rules['start_minutes'] + rules['free_minutes'])))
        pts = pay_time.total_seconds() - (rules['free_minutes'].total_seconds() if flag else 0)

        logger.debug('pay time in second: {} start minutes in second: {}'.format(pts, sms))

        # 首个计费时长后最小计费周期
        interval = rules['accounting_interval'].total_seconds()

        logger.debug('time count {}'.format(
            (rules['start_price'] if flag and rules['start_minutes'] else 0) + interval / 3600 * (
                    (pts - sms) // interval + (1 if (pts - sms) % interval else 0))
        ))

        return (rules['start_price'] if flag and rules['start_minutes'] else 0) + interval / 3600 * (
                (pts - sms) // interval + (1 if (pts - sms) % interval else 0))
    else:
        return 0


def check_out(number_plate, exit_time, entry_price):
    """

    :param number_plate:
    :param exit_time:
    :param entry_price:  这个变量可删除（2018-10-18）
    :return:
    """

    if isinstance(exit_time, str):
        exit_time = datetime.strptime(exit_time, '%Y-%m-%d %H:%M:%S')

    entry_record = ParkingRecords.query.filter(ParkingRecords.number_plate.__eq__(number_plate),
                                               ParkingRecords.status.__eq__(0)).order_by(
        ParkingRecords.create_time.desc()).first()

    # 如果exit_validate_before存在，则表明已经付费
    if entry_record and entry_record.exit_validate_before and entry_record.exit_validate_before >= exit_time:
        return None, None, None

    # 如果出场时间大于有效时间，则重新计算应付费用， normal为False，则表示不计算免费停车15分钟及首小时10元
    elif entry_record and entry_record.exit_validate_before and entry_record.exit_validate_before < exit_time:
        do_over_entry_time = entry_record.exit_validate_before
        normal = False
    else:
        # do_over_entry_time为None，则在bill_time中会根据车牌号获取实际的入场时间
        do_over_entry_time = None
        normal = True

    date_dict, timedelta_in_date, free_order = bill_time(number_plate, exit_time, do_over_entry_time)

    total_fee = 0
    total_time = timedelta()

    # 获取数据库中的停车规则
    rules = parking_rules()

    # 判断是否为订单类型的停车用户
    order_flag = False if timedelta_in_date else True

    # 获取停车首日日期
    min_date = min([k for k in date_dict.keys()])

    logger.debug('The min date（first day) is {}'.format(min_date))

    """
    计算每天所需计费时长，如果当天计费时长所产生的单价大于设置的当天封顶价，则当天价格按照封顶价计算
    如果当天未达到封顶价，则将时间计入计费时间内
    如果第一天没有达到封顶价，则check_fee_flag为True
    """

    daily_duration = {}

    # 计算最终费用时，确定是否包含第一天
    check_fee_flag = True

    for k, v in date_dict.items():
        logger.debug(k)
        logger.debug(v)
        logger.debug(timedelta_in_date.get(k))
        # tmax是当天最大的计时时间，一般为零点，如果是出场当天，则为出场时间
        tmax = max(v) if max(v) <= exit_time else exit_time

        # sum的部分是免费时长，就是当天停车时间，减去免费的订单时间总和
        pay_time = tmax - min(v) - sum([] if not timedelta_in_date.get(k) else timedelta_in_date.get(k), timedelta())

        # 矫正负计费，一般不应该出现这种情况
        pay_time = timedelta(seconds=0) if pay_time < timedelta(seconds=0) else pay_time

        logger.debug("Date: {}, 总计费时长 {}".format(k, pay_time))

        total_time += pay_time

        daily_duration[k] = pay_time

    '''
    check_fee_flag 的作用是，如果第一天就已经是封顶计费了，那么第一天的时长就不会加到total_pay_time中，也就是从第二天开始计费,
    这个时候，如果还有需要计费的时间，一定是第一天之后的，那么就没有首小时不满一小时按一小时计费的规则，同样不会有起始免费15分钟的规则

    normal 目前始终为true

    order_flag，只要有订单就为false


    '''

    logger.debug("daily duration is {}".format(daily_duration))
    logger.debug("the min date is {}".format(min_date))

    logger.debug("The length of daily duration is {}".format(len(daily_duration)))
    if len(daily_duration) > 1:
        for dk, dv in daily_duration.items():
            logger.debug("daily_duration's key is {}, value is {}".format(dk, dv))
            if dk == min_date:
                logger.debug("Find the min date in daily duration")
                if dv < rules['free_minutes'] + rules['start_minutes']:
                    logger.debug(
                        "The first day's parking time less than {} + {}, it is {}".format(rules['free_minutes'],
                                                                                          rules['start_minutes'], dv))
                    if dv + daily_duration[dk + timedelta(days=1)] <= rules['free_minutes'] + rules['start_minutes']:
                        logger.debug(
                            "The next day's parking time is not enough to lend the minutes to the first day, it is {}".format(
                                daily_duration[dk + timedelta(days=1)]))
                        daily_duration[dk] += daily_duration[dk + timedelta(days=1)]
                        daily_duration[dk + timedelta(days=1)] = timedelta(seconds=0)
                    else:
                        logger.debug(
                            "The next day's parking time is enough to lend the minutes to the first day, it is {}".format(
                                daily_duration[dk + timedelta(days=1)]))
                        daily_duration[dk] = rules['free_minutes'] + rules['start_minutes']
                        daily_duration[dk + timedelta(days=1)] -= rules['free_minutes'] + rules['start_minutes'] - dv
                    break

        logger.debug("******New daily duration is {}".format(daily_duration))

    logger.debug("Start to check the daily fee")
    for dk, dv in daily_duration.items():
        if dk == min_date:
            billing_time = check_fee(dv, flag=True & normal & order_flag, start_flag=True & normal & order_flag)
        # 确认当天停车费用是否超过当天封顶金额
        else:
            billing_time = check_fee(dv, flag=False, start_flag=False)

        if billing_time * entry_price >= rules['max_fee_per_day']:
            total_fee += rules['max_fee_per_day']
        else:
            total_fee += billing_time * entry_price

    return total_fee, total_time, free_order
