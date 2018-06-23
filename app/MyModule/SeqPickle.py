from ..models import aes_key
import time
import rsa
from .. import logger, redis_db
from .SchedulerControl import scheduler_pause, scheduler_resume
from .AESCryptor import encrypt, decrypt
import binascii
import json


class Seq:
    def __init__(self, pkl_name):
        """
        self.file = CONFIG_FILE_PATH + pkl_name
        if os.path.exists(self.file):
            seq_pkl = open(self.file, 'rb')
            if len(seq_pkl.read()) > 0:
                seq_pkl.seek(0)
                self.last_seq = pickle.load(seq_pkl)
                seq_pkl.close()
                self.init = False
            else:
                self.init = True
                self.last_seq = 0
                seq_pkl.close()
        else:
            self.init = True
            self.last_seq = 0
        """
        self.file = 'seq_pickle_' + pkl_name
        if redis_db.get(self.file):
            self.last_seq = redis_db.get('seq_pickle_' + pkl_name).decode()
            self.init = False
        else:
            self.init = True
            self.last_seq = 0

    def update_seq(self, seq):
        """
        seq_pkl = open(self.file, 'wb')
        pickle.dump(seq, seq_pkl)
        seq_pkl.close()
        """
        redis_db.set(self.file, seq)

    @property
    def load_seq(self):
        """
        if os.path.exists(self.file):
            seq_pkl = open(self.file, 'rb')
            if len(seq_pkl.read()) > 0:
                seq_pkl.seek(0)
                seq_load = pickle.load(seq_pkl)
                seq_pkl.close()
                self.init = False
                return seq_load
            else:
                seq_pkl.close()
                return 0
        else:
            self.init = True
            return 0
        """
        if redis_db.get(self.file):
            self.init = False
            return redis_db.get(self.file).decode()
        else:
            self.init = True
            return 0

    def init_today_seq(self):
        pass


def checkLicence(init='0'):
    """

    :param init:
    :return:
    """
    check_interval = 30
    check_licence = Seq('licence.pkl')
    if check_licence.init:
        init_json = init_licence(init)
        init_ = binascii.b2a_hex(init_json.encode()).decode()
        check_licence.update_seq(init_)
        if init == '0':
            scheduler_pause()
        return init_
    else:
        lp = json.loads(binascii.a2b_hex(check_licence.load_seq.encode()).decode())

        if lp['expire_in'] == 0 or lp['expire_date'] <= lp['start_date'] or lp['status'] == '0':
            logger.critical('The licence has been expired, '
                            'pls ask Koios to buy new licence if your still want to use R2D2.')
            lp['status'] = '0'
            lp['expire_in'] = 0
            scheduler_pause()
        else:
            logger.info('The licence is in service. The time left now is {}'.format(lp['expire_in']))
            lp['expire_in'] -= check_interval
            logger.info('The time left now is {}'.format(lp['expire_in']))

        check_licence.update_seq(binascii.b2a_hex(json.dumps(lp).encode()).decode())
        return lp


def init_licence(status):
    # 产生私钥
    init_days = 7
    (pubkey, privkey) = rsa.newkeys(2048)
    pubkey = encrypt(pubkey.save_pkcs1().decode(), aes_key).decode()
    privkey = encrypt(privkey.save_pkcs1().decode(), aes_key).decode()
    print(pubkey, privkey)

    init_data = {"init_date": time.time(),
                 "start_date": time.time(),
                 "expire_in": init_days * 86400 if status != '0' else 0,
                 "expire_date": time.time() + init_days * 86400,
                 "pubkey": pubkey,
                 "privkey": privkey,
                 "rules": "",
                 "author": "Koios",
                 "status": status}

    # return init_data
    return json.dumps(init_data)


def update_crypted_licence(crypt_licence):
    new_licence = binascii.a2b_hex(crypt_licence.encode())
    privkey = get_loaded_privkey()
    newone = rsa.decrypt(new_licence, privkey).decode()
    dic_new = json.loads(newone)

    lic_pkl = Seq('licence.pkl')
    if lic_pkl.init:
        lic_pkl.update_seq(binascii.b2a_hex(init_licence('0').encode()).decode())
        return False
    else:
        licence = json.loads(binascii.a2b_hex(lic_pkl.load_seq.encode()).decode())

        dic_new['privkey'] = licence['privkey']
        dic_new['pubkey'] = licence['pubkey']
        lic_pkl.update_seq(binascii.b2a_hex(json.dumps(dic_new).encode()).decode())

        # 如果许可证状态不可用,说明这时需要resume scheduler
        if licence['expire_in'] <= 0 or licence['expire_date'] <= licence['start_date'] or licence['status'] == '0':
            scheduler_resume()
        return True


def get_pubkey():
    lic_pkl = Seq('licence.pkl')
    if lic_pkl.init:
        return False
    else:
        licence = json.loads(binascii.a2b_hex(lic_pkl.load_seq.encode()).decode())
        decrypted_licence = decrypt(licence['pubkey'].encode(), aes_key)
        return (licence['expire_date'],
                licence['expire_in'],
                decrypted_licence.decode())


def get_loaded_privkey():
    lic_pkl = Seq('licence.pkl')
    if lic_pkl.init:
        logger.critical('Licence is not exist!')
        return False
    else:
        licence = json.loads(binascii.a2b_hex(lic_pkl.load_seq.encode()).decode())
        return rsa.PrivateKey.load_pkcs1(decrypt(licence['privkey'], aes_key))


def gen_rsa():
    lic_pkl = Seq('licence.pkl')
    if lic_pkl.init:
        lic_pkl.update_seq(binascii.b2a_hex(init_licence('0').encode()).decode())
        return False
    else:
        (pubkey, privkey) = rsa.newkeys(1024)
        licence = json.loads(binascii.a2b_hex(lic_pkl.load_seq.encode()).decode())
        licence['pubkey'] = pubkey.save_pkcs1().decode()
        licence['privkey'] = privkey.save_pkcs1().decode()
        lic_pkl.update_seq(binascii.b2a_hex(json.dumps(licence).encode()).decode())
        return True


def gen_new_licence(**kwargs):
    init_date = time.time()
    start_date = init_date
    expire_date = init_date + kwargs.get('expire_in') * 86400
    new_lic = {'init_date': init_date,
               'start_date': start_date,
               'expire_in': kwargs.get('expire_in') * 86400,
               'expire_date': expire_date,
               'author': 'Koios',
               'status': '1'}
    encoded_new_lic = json.dumps(new_lic)
    loaded_pub_key = rsa.PublicKey.load_pkcs1(kwargs.get('pub_key'))
    encrypted_lic = rsa.encrypt(encoded_new_lic.encode(), loaded_pub_key)
    return binascii.b2a_hex(encrypted_lic).decode()

