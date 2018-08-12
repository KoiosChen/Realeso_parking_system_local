from flask import Blueprint

main = Blueprint('main', __name__)

from . import views, errors, api, sys_config, parking_records, websocket, fixed_parking_space
