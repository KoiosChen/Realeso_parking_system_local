# -*- coding:utf-8 -*-
# Created on 02/20/2017
# By Peter Chen
# coding=utf-8

from .GetConfig import get_config
import datetime
import random
from ..models import CACTI_PIC_FOLDER
import requests
from .. import logger


def get_cacti_pic(action, **kwargs):
    try:
        """
    
        :param action: 'cacti_view_pic_url', ...  which stored in table api_configure
        :param kwargs: according to the action params, for exmaple: graph_id, rra_id. All of these params' type is string.
        :return: none
        """

        params = get_config(kwargs.get('db_info'))

        filename = datetime.datetime.now().strftime('%Y%m%d%H%M%S') + str(random.randint(1, 20)) + '.png'

        filename_with_prefix = CACTI_PIC_FOLDER + filename

        if action == 'cacti_view_pic_url':
            graph_id = kwargs.get('graph_id')
            rra_id = kwargs.get('rra_id')
            pic_url = params[action] + "&local_graph_id=" + str(graph_id) + "&rra_id=" + str(rra_id)

        else:
            return False

        req_session = requests.Session()

        headers = {'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; WOW64; rv:38.0) Gecko/20100101 Firefox/38.0',
                   'Referer': params['headers']}

        values = {'action': 'login', 'login_username': params['login_username'], 'login_password': params['login_password']}

        req_session.post(params['login_url'], data=values, headers=headers)

        # get pic
        img_response = req_session.get(pic_url)

        with open(filename_with_prefix, 'wb') as file_object:
            file_object.write(img_response.content)

        return params['return_pic_url'] + "/show_image/" + filename
    except Exception as e:
        logger.error('Get Cacti picture fail for {}'.format(e))
        return False
