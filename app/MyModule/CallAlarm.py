import requests
import base64
import time
import hashlib
import json
from .GetConfig import get_config


class CallAlarm:
    def __init__(self):
        tmp = get_config('callapi')

        SoftVersion = tmp['SoftVersion']
        accountid = tmp['accountid']
        account_token = tmp['account_token']

        self.appId = tmp['appId']

        timestamp = time.strftime('%Y%m%d%H%M%S', time.localtime())

        author_str = accountid + ':' + timestamp
        sig_str = accountid + account_token + timestamp

        authorization = base64.b64encode(author_str.encode('utf-8'))
        m2 = hashlib.md5()
        m2.update(sig_str.encode('utf-8'))
        sig = m2.hexdigest().upper()

        self.request_url = 'https://api.ucpaas.com/' + SoftVersion + '/Accounts/' + accountid + '/Calls/voiceNotify?sig=' + sig

        self.headers = {
            'Accept': 'application/json',
            'Content-Type': 'application/json;charset=utf-8',
            'Content-Length': '256',
            'Authorization': authorization
        }

    def do_call(self, call_number, content):
        send_content = {
            'voiceNotify': {
                'appId': self.appId,
                'to': call_number,
                'type': '1',
                'toSerNum': '02180251599',
                'content': content,
                'playTimes': '2'
            }
        }
        r = requests.post(self.request_url,
                          data=json.dumps(send_content, ensure_ascii=False).encode('utf-8'),
                          headers=self.headers)
        return r

if __name__ == '__main__':
    call = CallAlarm()
    r = call.do_call('13817730962', '528793')
    print(r.json())
