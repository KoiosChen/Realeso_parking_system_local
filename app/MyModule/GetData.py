from . import MyDao


class GetData:
    def __init__(self, db_info):
        """

        Args:
            db_info:

        Returns:

        """
        self.db_info = db_info
        try:
            # connect to data base, the argument in SqlExceute is the connect info
            self.t = MyDao.SqlExcute(self.db_info)
        except Exception as e:
            print(e)
            exit()

    def get_result(self, **kwargs):
        data_list = []
        # execute the sql (sql_command, value(dict))
        query = kwargs['query']
        data = kwargs.get('data')
        catalog = kwargs.get('catalog')
        self.t.cursor_excute(query, data, kwargs.get('sql_join'))
        if not kwargs.get('notzip'):
            for line in self.t.cursor:
                xyz = zip(catalog, line)
                zyx = dict(xyz)
                # print(zyx)
                data_list.append(zyx)

            return data_list
        else:
            return self.t.cursor

    def close_cursor(self):
        self.t.cursor_close()

    def close_db(self):
        self.t.connect_close()


if __name__ == '__main__':
    db_info = 'Cacti'
    getdata = GetData(db_info=db_info)
    getdata.t.cursor.execute('set names latin1')
    catalog = ('description', 'hostname', 'status', 'status_fail_date')
    ddd = getdata.get_result(query='test', catalog=catalog)
    print(ddd)
