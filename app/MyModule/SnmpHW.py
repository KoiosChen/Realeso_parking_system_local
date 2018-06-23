from .Snmp import Snmp
from ..MyModule import GetData
from ..models import Device, SnmpInterface
from .. import db, logger, redis_db
import time
import threading

mib = {'sysContact': '1.3.6.1.2.1.1.4.0',
       'sysName': '1.3.6.1.2.1.1.5.0',
       'ifIndex': "1.3.6.1.2.1.2.2.1.1",
       'ifDesc': '1.3.6.1.2.1.2.2.1.2',
       'ifAlias': '1.3.6.1.2.1.31.1.1.1.18',
       'ifSpeed': "1.3.6.1.2.1.2.2.1.5",
       }


def doIfDescAlias():

    def _do(_d):
        s = GetHwSnmpInfo()
        s.match_int_desc(devices=_d)

    device = Device.query.filter(Device.status.__eq__('1'),
                                 Device.community.__ne__(None),
                                 Device.mib_model.__eq__('1')).all()
    for d in device:
        t = threading.Thread(target=_do, args=[[d],])
        t.start()


class GetHwSnmpInfo(Snmp):
    def __init__(self, **kwargs):
        Snmp.__init__(self, oid=kwargs.get('oid'),
                      Version=2,
                      host=kwargs.get('host'),
                      community=kwargs.get('community'))

    def get_device_community(self, db_info=None):
        """
        用于从Cacti的数据库中获取监控目标的community
        :param db_info:
        :return:
        """

        db_info = 'Cacti' if db_info is None else db_info
        getdata = GetData.GetData(db_info=db_info)
        getdata.t.cursor.execute('set names latin1')
        catalog = ('hostname', 'description', 'snmp_community', 'snmp_version', 'snmp_port', 'snmp_timeout')
        host_table = getdata.get_result(query='get_host_table', catalog=catalog)

        for h in host_table:
            print(h)
            device = Device.query.filter_by(ip=h['hostname']).first()
            if device:
                device.community = h['snmp_community']
            else:
                device = Device(ip=h['hostname'], device_name=h['description'], community=h['snmp_community'],
                                login_name='', login_password='')
            self.destHost = h['hostname']
            self.community = h['snmp_community']
            self.oid = '1.3.6.1.2.1.1.4.0'
            try:
                result = str(self.query()[0][1])
                print(result)
            except Exception as e:
                print('snmp timeout')
            db.session.add(device)
        db.session.commit()

    def match_int_desc(self, devices=None):
        """
        用于对应处制定设备（设备列表）中 interface -- description的对应关系
        :param devices: 这里的device是一个db的对象，所以传入的一定是Device的对象
        :return:
        """
        devices = Device.query.filter(Device.status.__eq__('1'),
                                      Device.community.__ne__(None),
                                      Device.mib_model.__eq__('1')).all() \
            if devices is None else devices
        for d in devices:
            self.destHost = d.ip
            self.community = d.community

            ifDesc = self.snmp_query_bulk_string(mib['ifDesc'])
            if not ifDesc:
                logger.debug('get interface description fail')
                continue

            ifAlias = self.snmp_query_bulk_string(mib['ifAlias'])
            if not ifAlias:
                logger.debug('get interface alias fail')
                continue

            sysname = self.snmp_query_string(mib['sysName'])

            if_desc_alias = {}
            for i, ifs in ifDesc.items():
                if_desc_alias[ifs] = ifAlias[i] if i in ifAlias.keys() else None
            print(if_desc_alias)
            for key, value in if_desc_alias.items():
                if key:
                    if_record = SnmpInterface.query.filter_by(device_id=d.id, snmp_if_desc=key).first()
                    if if_record:
                        if_record.snmp_if_alias = value
                        if_record.snmp_sysname = sysname
                        if_record.update_time = time.localtime()
                    else:
                        if_record = SnmpInterface(device_id=d.id, snmp_if_desc=key, snmp_if_alias=value,
                                                  snmp_sysname=sysname, update_time=time.localtime())

                    db.session.add(if_record)
                    db.session.commit()

    # def last_bandwidth(self):
    #
    #     for i, previous in r[0].items():
    #         if previous and r[1][i]:
    #             if int(r[1][i]) < int(previous):
    #                 diff = (int(r[1][i]) + 2 ** 32 - 1 - int(previous)) * 8 / (60 * 1024 * 1024)
    #             else:
    #                 diff = (int(r[1][i]) - int(previous)) * 8 / (60 * 1024 * 1024)
    #             diffDic[i] = diff

