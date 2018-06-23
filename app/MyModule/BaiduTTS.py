# 2016-12-03
# Baidu API for TTS
import requests
import json
import time


class BaiduTTS:
    def __init__(self):
        oath_url = 'https://openapi.baidu.com/oauth/2.0/token'
        self.headers = {
            'Content-Type': 'application/json',
            'Cache-Control': 'no-store'
        }

        value = {
            'grant_type': 'client_credentials',
            'client_id': 'eG3uALX4VqzcVOclbLxs05QRj5YOSgqq',
            'client_secret': 'ElZwfZbt8B1Rns3spGK6zUG5oPi6HkxC'
        }
        print(json.dumps(value, ensure_ascii=False).encode('utf-8'))
        r = requests.post(oath_url, value, headers=self.headers)
        js = r.json()
        self.access_token = js.get('access_token')
        print(self.access_token)

    def text2voice(self, text):
        tts_url = 'http://tsn.baidu.com/text2audio'
        tts_data = {
            'tex': text,
            'lan': 'zh',
            'tok': self.access_token,
            'ctp': '1',
            'cuid': '60:03:08:a1:c5:ac',
            'per': '1'
        }

        tts_response = requests.post(tts_url, tts_data, headers=self.headers)
        print(tts_response.headers)
        mp3 = open('/Users/Peter/text2voice_%s.mp3' % time.strftime('%Y%m%d%H%M%S', time.localtime()), 'wb')
        mp3.write(tts_response.content)
        mp3.close()
