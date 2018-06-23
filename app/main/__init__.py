from flask import Blueprint

main = Blueprint('main', __name__)

from . import views, errors, api, walle, log_manager, sys_config, websocket
