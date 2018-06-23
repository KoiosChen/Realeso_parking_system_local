from .. import redis_db
from datetime import date


def manage_key(key, date_type=None):
    return key + '_' + str(date.today()) if date_type == 'today' else key


def count(key, date_type=None, num=1):

    __key = manage_key(key, date_type)

    this_counter = redis_db.get(__key)

    if this_counter:
        redis_db.set(__key, int(this_counter.decode()) + num)
    else:
        redis_db.set(__key, num)