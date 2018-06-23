from pysnmp.entity.rfc3413.oneliner import cmdgen


class Snmp(object):
    """A basic SNMP session"""
    def __init__(self, oid="sysDescr", Version=2, host=None, community=None):
        self.oid = oid
        self.version = Version - 1
        self.destHost = host
        self.community = community
        self.cg = cmdgen.CommandGenerator()

    def query(self, qoid=None):
        self.oid = self.oid if qoid is None else qoid
        """Creates SNMP query session"""
        try:
            errorIndication, errorStatus, errorIndex, varBinds = self.cg.getCmd(
              cmdgen.CommunityData('netagent', self.community, self.version),
                cmdgen.UdpTransportTarget((self.destHost, 161)),
                self.oid
            )
            result = varBinds
        except Exception as err:
            print(err)
            result = None
        return result

    def snmp_query_string(self, _oid):
        return str(self.query(qoid=_oid)[0][1])

    def query_bulk(self, N=0, R=100, oid=None):
        self.oid = self.oid if oid is None else oid
        """Creates SNMP query session"""
        try:
            print(self.destHost, self.community, self.oid)
            errorIndication, errorStatus, errorIndex, varBinds = self.cg.bulkCmd(
              cmdgen.CommunityData('netagent', self.community, self.version),
                cmdgen.UdpTransportTarget((self.destHost, 161)), N, R,
                self.oid
            )
            # logger.debug(varBinds)
            result = varBinds
        except Exception as err:
            result = None
        return result

    def snmp_query_bulk_string(self, _oid):
        return {str(k[0][0]).split('.')[-1]: str(k[0][1]) for k in self.query_bulk(N=0, R=1, oid=_oid)}

