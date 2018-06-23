def main():
    import sys
    if len(sys.argv) > 1:
        try:
            statistic_operate = sys.argv[1] or 'account_statistic'
        except IndexError:
            print('lost args')

        try:
            start_time = datetime.strptime(sys.argv[2], "%Y-%m-%d %H:%M:%S") or datetime(2016, 1, 1)
        except ValueError or IndexError:
            print('The date format should be %%Y-%%m-%%d %%H:%%M:%%S')
        except IndexError:
            print('lost args')

        try:
            stop_time = datetime.strptime(sys.argv[3], "%Y-%m-%d %H:%M:%S") or datetime(2099, 12, 31)
        except ValueError:
            print('The date format should be %%Y-%%m-%%d %%H:%%M:%%S')
        except IndexError:
            print('lost args')
        if statistic_operate == 'account_statistic':
            Account.account_statistic(start_time, stop_time)
        elif statistic_operate == 'channel_statistic':
            c = Channel.channel_statistic()
            c.channel_statistic(start_time, stop_time)
    else:
        print('no action')


class ReportTemplate:
    def __init__(self, **kwargs):
        try:
            self.conntect_db = GetData(db_info=kwargs['db_info'])
        except Exception as e:
            print(e)
            exit()

        self.report_action = kwargs['report_action']
        # used to store data from database
        self.raw_data = []

        # used to store data to be printed to excel
        self.print_list = []

        self.query_data = kwargs.get('query_data', default={})
        self.query = kwargs.get('query', default='')
        self.catalog = kwargs.get('catalog', default=[])
        self.title = kwargs.get('title', default=[])

    def query_init(self, query, title, query_data):
        self.query = query
        self.title = title
        self.query_data = query_data
        return self.conntect_db.get_mobile_hcode_info(data=self.query_data, catalog=self.catalog, query=self.query)

    def create_template(self, catalog):
        self.catalog = catalog

    def prepare_data(self, data):
        pass


def user_statistic(start_time, stop_time):
    pass