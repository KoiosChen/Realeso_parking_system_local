from ..models import ApiConfigure


def get_config(api_name):
    tmp = {}
    api = ApiConfigure.query.filter_by(api_name=api_name).all()
    for a in api:
        tmp[a.api_params] = a.api_params_value

    return tmp