import datetime
from ..models import DutySchedule, DutyAttendedTime, User
from .. import logger, db


class ManageDutySchedule:
    def __init__(self, manage_date):
        self.manage_date = manage_date or datetime.datetime.now().date()
        self.manage_duty_schedule = DutySchedule.query.filter_by(date_time=self.manage_date).all()
        self.duty_time_schedule = DutyAttendedTime.query.all()

    def duty_exist(self, **kwargs):
        return True if DutySchedule.query.filter_by(date_time=kwargs.get('date_time'),
                                                    userid=kwargs.get('userid'),
                                                    attended_time_id=kwargs.get('attended_time_id'),
                                                    duty_status=1).first() else False

    def add_duty_member(self, **kwargs):
        duty_member = kwargs.get('duty_member')
        user = User.query.filter_by(username=duty_member).first()

        if user:
            userid = user.id
        else:
            logger.info('The user you wanna add is not exist')
            return False

        duty_time = kwargs.get('duty_time')

        attended_time_id = None
        for time_schedule in self.duty_time_schedule:
            if time_schedule.start_time <= duty_time <= time_schedule.stop_time:
                attended_time_id = time_schedule.id

        if not attended_time_id:
            logger.info('There is no suitable time')
            return False

        if not self.duty_exist(date_time=self.manage_date,
                               userid=userid,
                               attended_time_id=attended_time_id,
                               duty_status=1):

            duty_add = DutySchedule(date_time=self.manage_date,
                                    userid=userid,
                                    attended_time_id=attended_time_id,
                                    duty_status=1,
                                    create_time=datetime.datetime.now())
            db.session.add(duty_add)
            db.session.commit()
        else:
            logger.info('The duty your want to add is existed!')

    def delete_duty_member(self):
        pass