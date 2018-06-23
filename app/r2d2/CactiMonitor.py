from .. import db, logger
from ..MyModule import AlarmPolicy, GetData, GetCactiPic


def cacti_db_monitor(db_info=None):
    db_info = db_info or 'Cacti'
    getdata = GetData.GetData(db_info=db_info)
    getdata.t.cursor.execute('set names latin1')
    catalog = ('id', 'description', 'hostname', 'status', 'status_fail_date')
    host_offline = getdata.get_result(query='host_offline', catalog=catalog)

    alarm_content = []

    for info in host_offline:
        # 解决部分Cacti表中无法取到status_fail_date的问题
        try:
            offline_time = ' 于 ' + info['status_fail_date'].strftime('\'%Y-%m-%d %H:%M:%S\'')
        except Exception as e:
            logger.error('未取得status_fail_date {} error: {}'.format(info['status_fail_date'], e))
            offline_time = ' '

        alarm_content.append(
            'id: ' + str(info['id']) + ' ' + info['description'] + offline_time + ' 离线' + ', IP: ' + info[
                'hostname'] + '\n\n')

    catalog2 = ('id', 'name', 'thold_hi', 'thold_low', 'thold_alert', 'host_id', 'graph_id', 'rra_id')
    thold_alert = getdata.get_result(query='thold_alert', catalog=catalog2)

    for alert in thold_alert:
        pic_url = GetCactiPic.get_cacti_pic(action='cacti_view_pic_url',
                                            graph_id=alert['graph_id'],
                                            rra_id='5',
                                            db_info=db_info)
        # 解决图片取不到抛异常造成基础告警无法发出
        pic_url = pic_url if pic_url else ''
        alarm_content.append('id: ' + str(alert['id']) +
                             ' ' + alert['name'] +
                             '产生阀值告警。\n>>>图片链接:' + pic_url + '\n\n')

    if host_offline or thold_alert:
        try:
            AlarmPolicy.alarm(alarm_content=alarm_content, alarm_type='0')
        except Exception as e:
            logger.error(e)
    else:
        logger.info('There is no host offline or thold alert')

    getdata.close_cursor()
    getdata.close_db()
