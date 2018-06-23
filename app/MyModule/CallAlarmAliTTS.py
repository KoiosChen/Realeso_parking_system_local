# -*- coding: utf-8 -*-

import time
import hashlib
import requests

url = 'http://gw.api.taobao.com/router/rest'
port = '80'
appkey = '23554988'
secret = 'a9e4ecbf270eb79a72f7650daed5fbef'
method = 'alibaba.aliqin.fc.tts.num.singlecall'
timestamp = time.strftime('%Y-%m-%d %H:%M:%S', time.localtime())
sign_method = 'md5'
v = '2.0'
called_num = '17317955572'
called_show_num = '01053912804'
tts_code = 'TTS_33265088'

tts_param = "{'alarmcontent': '这是阿里的TTS语音通知'}"

headers = {
    'Content-Type': 'application/x-www-form-urlencoded',
    'charset': 'utf-8'
}


value = {
    'method': method,
    'app_key': appkey,
    'sign_method': sign_method,
    'timestamp': timestamp,
    'v': v,
    'tts_param': tts_param,
    'called_num': called_num,
    'called_show_num': called_show_num,
    'tts_code': tts_code
}

sortedParameters = sorted(value.items(), key=lambda parameters: parameters[0])

combinstring = ''

for (k, v) in sortedParameters:
    combinstring += k + v


combinstring = secret + combinstring + secret

print(combinstring)

m2 = hashlib.md5()
m2.update(combinstring.encode('utf-8'))
sig = m2.hexdigest().upper()
print(sig)

value['sign'] = sig

print(value)

r = requests.post(url, value, headers=headers)
print(r, r.content)