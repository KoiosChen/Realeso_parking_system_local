from flask_socketio import emit
from .. import socketio, redis_db
from datetime import datetime
import json


def websocket():
    socketio.emit('ws_test', json.loads(redis_db.get('c002').decode()), namespace='/test')