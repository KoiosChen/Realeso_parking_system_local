# -*- coding:utf-8 -*-
# Created on 07/27/2016
# By Peter Chen
# coding=utf-8

import json
import requests
import time
from ..models import TokenRecord
from .. import db, logger
import datetime
from .GetConfig import get_config
from .Counter import count


class WechatAlarm:
    def __init__(self):
        variable = {}
        tmp = get_config('wechat')

        # 记录微信发送总数
        count(key='wechatPreSend')

        # 记录微信当天发送总数
        count(key='wechatPreSend', date_type='today')

        variable['corpid'] = tmp['corpid']
        variable['corpsecret'] = tmp['corpsecret']
        self.expire_time = int(tmp['expire_time'])  # seconds
        self.default_agentid = tmp['default_agentid']
        self.get_token_url = 'https://qyapi.weixin.qq.com/cgi-bin/gettoken?corpid=%s&corpsecret=%s'
        self.send_sms_url = 'https://qyapi.weixin.qq.com/cgi-bin/message/send?access_token=%s'
        self.headers = {'Content-Type': 'application/json', "encoding": "utf-8"}

        if_token = TokenRecord.query.filter_by(unique_id=variable['corpid']).first()

        if if_token:
            if (datetime.datetime.now() - if_token.create_time).seconds < (int(if_token.expire) - self.expire_time):
                self.access_token = if_token.token
            else:
                db.session.delete(if_token)
                self.access_token = self.get_token(variable)
        else:
            self.access_token = self.get_token(variable)

    def get_token(self, variable):
        r = requests.get(self.get_token_url % (variable['corpid'], variable['corpsecret']))
        js = r.json()
        logger.debug('Accessing %s ' % r.url)
        if js.get('errcode') == 0:
            logger.debug('Get access token %s successful' % js)
            access_token = js.get('access_token')
            expires_in = js.get('expires_in')
            token_record = TokenRecord(unique_id=variable['corpid'], token=access_token, expire=expires_in,
                                       create_time=time.localtime())
            db.session.add(token_record)
            db.session.commit()
            return access_token
        else:
            logger.error('Get access token fail')
            return False

    def init_text(self, content, agentid=None):
        content = content
        print(content)
        send_content = {
            "touser": "@all",
            "toparty": "",
            "totag": "",
            "msgtype": "text",
            "agentid": self.default_agentid if agentid is None else agentid,
            "text": {
                "content": content
            },
            "safe": "0"
        }

        return send_content

    def news_msg(self, **kwargs):
        title = kwargs.get('title')
        description = kwargs.get('description')
        web_url = kwargs.get('web_url')
        picurl = kwargs.get('picurl')
        send_content = {
            "touser": "@all",
            "toparty": "",
            "totag": "",
            "msgtype": "news",
            "agentid": self.default_agentid if not kwargs.get('agentid') else kwargs.get('agentid'),
            "news": {
                "articles": [
                    {
                        "title": title,
                        "description": description,
                        "url": web_url,
                        "picurl": picurl
                    }
                ]
            },
            "safe": "0"
        }

        return send_content

    def sendMsg(self, send_content):
        try:
            r = requests.post(self.send_sms_url % self.access_token,
                              data=json.dumps(send_content, ensure_ascii=False).encode('utf-8'), headers=self.headers)
            result = r.json()
            logger.debug(result)

            # 记录微信发送成功数量
            count('wechatSent')
            count('wechatSent', date_type='today')
        except Exception as e:
            logger.error('wechat send fail for {}'.format(e))


