from flask import request, jsonify
from . import main
from ..models import AlarmRecord, PonAlarmRecord, Permission, PiRegister, User, PcapOrder, SyslogAlarmConfig, Syslog
from .. import redis_db, logger, db, socketio
import json
import re
from flask_login import current_user
import time
import requests
from collections import defaultdict
import datetime
from sqlalchemy import and_, func
from ..MyModule.Counter import manage_key
from ..MyModule.HashContent import md5_content
from flask_socketio import SocketIO, emit


@socketio.on('connect', namespace='/test')
def test_connect():
    emit('my response', {'data': 'Connected', 'count': 0})


@socketio.on('my event', namespace='/test')
def test_message(message):
    emit('my response', {'data': message['data'], 'count': 2})