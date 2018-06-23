from flask_wtf import Form
from wtforms.validators import DataRequired, Email, Length
from wtforms import StringField, BooleanField, SubmitField, PasswordField


class LoginForm(Form):
    email = StringField('Email', validators=[DataRequired(), Length(1, 64), Email()])
    password = PasswordField('Password', validators=[DataRequired()])
    remember_me = BooleanField('keep me logged in')
    submit = SubmitField('Log In')
