import json
import requests


def request_ontinfo(**kwargs):
    ontinfo_url = 'http://127.0.0.1:5000/get_ontinfo'
    headers = {
        'Content-Type': 'application/json',
        'Cache-Control': 'no-store'
    }

    value = {
        'device_ip': kwargs.get('device_ip'),
        'fsp': kwargs.get('fsp'),
        'ontid_list': kwargs.get('ontid_list')
    }

    r = requests.post(ontinfo_url, json.dumps(value, ensure_ascii=False).encode('utf-8'), headers=headers)
    return r.json()


def request_interface_desc():
    pass