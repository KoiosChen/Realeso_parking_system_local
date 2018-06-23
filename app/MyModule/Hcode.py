from MyModule import MyDao, MyExcel


class HCODE:
    """
    This module is used to get the hcode with province code and city code
    """
    def __init__(self):
        self.db_info = '192.168.168.56_flask_report'
        self.query = 'HCODE'
        try:
            # connect to database, the argument in SqlExceute is the connect info
            self.t = MyDao.SqlExcute(self.db_info)
            self.t.cursor_excute(self.query, '')
            self.result = self.t.cursor
            self.hcode_result = {}
            self.province_code_name = {}
            self.city_code_name = {}
            self.city_province = {}
            self.isp_name = {}
            self.isp_code = {}
            zip_info = ('p_code', 'p_name', 'c_code', 'c_name', 'isp_name', 'isp_code')
            for line in self.result:
                self.hcode_result[line[0]] = dict(zip(zip_info, [k for k in line[1:]]))
                self.province_code_name[line[1]] = line[2]
                self.city_code_name[line[3]] = line[4]
                self.city_province[line[3]] = line[1]
                self.isp_name[line[6]] = line[5]
        except Exception as e:
            print(e)
            exit()

    def get_mobile_hcode_info(self, hcode):
        return self.hcode_result.get(hcode) or None

    def get_province_code_name(self, p_code):
        p_code = int(p_code)
        return self.province_code_name.get(p_code) or None

    def get_city_code_name(self, c_code):
        c_code = int(c_code)
        return self.city_code_name.get(c_code) or None

    def get_city_province(self, c_code):
        c_code = int(c_code)
        return {'city_name': self.city_code_name.get(c_code),
                'province_code': self.city_province.get(c_code),
                'province_name': self.province_code_name.get(self.city_province.get(c_code))}

    def get_isp(self, hcode):
        if self.hcode_result.get(hcode):
            return self.isp_name.get(self.hcode_result.get(hcode).get('isp_code')) or None
        else:
            return None

if __name__ == '__main__':
    ha = HCODE()
    for key, value in ha.province_code_name.items():
        print(key, value)
    y = ha.get_mobile_hcode_info('1381773')
    print(y['p_code'])


