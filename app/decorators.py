from functools import wraps
from flask import abort
from flask_login import current_user
from .models import Permission
from . import logger


def permission_required(permission):
    def decorator(f):
        @wraps(f)
        def decorated_function(*args, **kwargs):
            if not current_user.can(permission):
                logger.warn('This user\'s action is not permitted!')
                abort(403)
            return f(*args, **kwargs)
        return decorated_function
    return decorator


def admin_required(f):
    return permission_required(Permission.ADMINISTER)(f)
