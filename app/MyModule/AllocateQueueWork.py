import threading
from .. import logger, work_q, redis_db
import json
import uuid
from ..MyModule.py_scapy import do
from ..MyModule.HashContent import md5_content
import os


class StartThread(threading.Thread):
    def __init__(self, q):
        threading.Thread.__init__(self)
        self.queue = q

    def run(self):
        while True:
            file_info = self.queue.get()
            print(file_info)

            file = os.path.join(file_info['filepath'], file_info['filename'])
            file_tmp = os.path.join(file_info['filepath'], 'tmp')
            if not os.path.exists(file_tmp):
                os.mkdir(file_tmp)
            this_uuid = md5_content(file)

            print(this_uuid)

            redis_db.set(this_uuid, json.dumps({"filename": file_info['filename'], "filepath": file_info['filepath'], "status": 1}))

            did = do(file=file)

            if did['status'] == 'ok':
                with open(os.path.join(file_tmp, this_uuid+'.txt'), 'w') as pcap_result:
                    for line in did['content']['verbose']:
                        pcap_result.write(line)

                redis_db.set(this_uuid, json.dumps(
                    {"filename": file_info['filename'], "filepath": file_info['filepath'], "status": 2}))
            else:
                redis_db.set(this_uuid, json.dumps(
                    {"filename": file_info['filename'], "filepath": file_info['filepath'], "status": 3}))

            self.queue.task_done()


def allocate_worker(thread_num=1):
    """
    用来处理上传的抓包文件，线程池默认共1个线程
    :return:
    """

    for threads_pool in range(thread_num):
        t = StartThread(work_q)
        t.setDaemon(True)
        t.start()
