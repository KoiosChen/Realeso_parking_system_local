import configparser
import os


def readcontent(file_handle):
    """
    It is used to read the content of the config file
    :param file_handle:
    :return:
    """
    content_array = []
    for content in file_handle:
        content = content.rstrip()
        # delete the null string.去掉最后一个换行符,如果所取得的字符串不是空字符,则为有数据行,如果是空字符,则为空行
        if content[:-1]:
            content_array.append(content)
    file_handle.close()
    return content_array


def readSubConfigFile(cfg_file, list_name):
    """
    :param cfg_file:
    :param list_name:
    :return:
    """
    """used to get the content of the file """

    cfgListName = readConfigFile(cfg_file, list_name)[list_name]['file']
    CF = open(cfgListName, 'r')
    return CF


def readConfigFile(cfg_file, list_name='all'):
    """

    Args:
        cfg_file: the path of the config file
        list_name: the head of a config section, the default value is all

    Returns:
        The director of key = value of the config file

    """

    '''
        this function is used to read config file
        which is used to store the file name of game, web, vedio, ISP and CDN
    '''
    if os.path.exists(cfg_file):
        try:
            cfg = configparser.ConfigParser()
            cfg.read(cfg_file)
            if list_name == 'all':
                return cfg
            else:
                temp_dict = {}
                temp_dict[list_name] = {}
                for key, value in cfg[list_name].items():
                    temp_dict[list_name].update({key: value})
                return temp_dict
        except KeyError:
            print('The key is not found')
            exit()
    else:
        print('Config file %s is not found' % cfg_file)
        exit()


if __name__ == '__main__':
    cfg_file = '../config_file/mysql_config.cfg'
    dict_conf = readConfigFile(cfg_file, '172.16.1.200_api')
    for key in dict_conf:
        for key2, value in dict_conf[key].items():
            print(key, key2, value)