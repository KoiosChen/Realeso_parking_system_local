import os

basedir = os.path.abspath(os.path.dirname(__file__))


class Config:
    SECRET_KEY = os.environ.get('SECRET_KEY') or 'hard to guess string'
    SQLALCHEMY_COMMIT_ON_TEARDOWN = True
    SQLALCHEMY_TRACK_MODIFICATIONS = True
    SESSION_TYPE = 'redis'
    SESSION_KEY_PREFIX = 'flask_session:'
    SESSION_PERMANENT = True
    SESSION_USE_SIGNER = True

    SQLALCHEMY_POOL_RECYCLE = 1800
    FLASKY_ADMIN = 'peter.chen@mbqianbao.com'

    UPLOADED_FILES_DEST = '/Users/Peter/Desktop/uploads'

    # FLASKY_ADMIN = os.environ.get('FLASKY_ADMIN')

    @staticmethod
    def init_app(app):
        pass


class DevelopmentConfig(Config):
    DEBUG = True
    DB_USERNAME = os.environ.get('DEV_DATABASE_USERNAME') or 'peter'
    DB_PASSWORD = os.environ.get('DEV_DATABASE_PASSWORD') or '123123'
    DB_HOST = os.environ.get('DEV_DATABASE_HOST') or '127.0.0.1'
    DB_DATABASE = os.environ.get('DEV_DATABASE_DATABASE') or 'r2d2'
    SQLALCHEMY_DATABASE_URI = 'mysql+pymysql://' + DB_USERNAME + ':' + DB_PASSWORD + '@' + DB_HOST + '/' + DB_DATABASE


class TestingConfig(Config):
    DEBUG = False
    DB_USERNAME = os.environ.get('TEST_DATABASE_USERNAME') or 'peter'
    DB_PASSWORD = os.environ.get('TEST_DATABASE_PASSWORD') or 'ftp123buzhidao'
    DB_HOST = os.environ.get('TEST_DATABASE_HOST') or '127.0.0.1'
    DB_DATABASE = os.environ.get('TEST_DATABASE_DATABASE') or 'r2d2'
    SQLALCHEMY_DATABASE_URI = 'mysql+pymysql://' + DB_USERNAME + ':' + DB_PASSWORD + '@' + DB_HOST + '/' + DB_DATABASE

    JOBS = [

        {
            'id': 'ups',
            'func': 'app.r2d2.UpsMonitor:ups_monitor',
            'args': (),
            'trigger': 'interval',
            'seconds': 300,
        },

        {
            'id': 'cacti',
            'func': 'app.r2d2.CactiMonitor:cacti_db_monitor',
            'args': (),
            'trigger': 'interval',
            'seconds': 300,
        },

        {
            'id': 'polling',
            'func': 'app.r2d2.ScheduleWorker:unalarmed_polling',
            'args': (),
            'trigger': 'interval',
            'seconds': 300,
        },

        {
            'id': 'check_licence',
            'func': 'app.MyModule.SeqPickle:checkLicence',
            'args': (),
            'trigger': 'interval',
            'seconds': 30,
        },
    ]

    SCHEDULER_VIEWS_ENABLED = True


class ProductionConfig(Config):
    DB_USERNAME = os.environ.get('DATABASE_USERNAME') or 'peter'
    DB_PASSWORD = os.environ.get('DATABASE_PASSWORD') or '123123'
    DB_HOST = os.environ.get('DATABASE_HOST') or '127.0.0.1'
    DB_DATABASE = os.environ.get('DATABASE_DATABASE') or 'r2d2'
    SQLALCHEMY_DATABASE_URI = 'mysql+pymysql://' + DB_USERNAME + ':' + DB_PASSWORD + '@' + DB_HOST + '/' + DB_DATABASE


config = {
    'development': DevelopmentConfig,
    'testing': TestingConfig,
    'production': ProductionConfig,
    'default': DevelopmentConfig
}