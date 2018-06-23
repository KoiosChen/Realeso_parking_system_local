from flask import render_template, redirect, request, url_for, flash, session, jsonify, json
from flask_login import login_user, logout_user, login_required
from ..models import User, Area
from . import auth
from .. import logger


@auth.route('/login', methods=['GET', 'POST'])
def login():
    if request.method == 'POST':
        email = request.form.get('email')
        password = request.form.get('password')
        remember_me = request.form.get('remember_me')
        logger.warning('Somebody is trying to login as {}'.format(email))
        user = User.query.filter_by(email=email, status=1).first()
        if user is not None and user.verify_password(password):
            session.permanent = True
            logger.warning('Username is {}'.format(user.username))
            this_user = User.query.filter_by(email=email).first()
            this_user_area = Area.query.filter_by(id=this_user.area).first()
            if this_user.permit_machine_room == '0x0':
                session['permit_machine_room'] = this_user_area.area_machine_room
            else:
                session['permit_machine_room'] = this_user.permit_machine_room
            session['LOGINUSER'] = email
            session['LOGINNAME'] = this_user.username
            session['LOGINAREA'] = this_user.area
            session['ROLE'] = this_user.role_id
            session['DUTY'] = this_user.duty
            session['SELFID'] = this_user.id
            login_user(user, remember_me)
            return redirect(request.args.get('next') or url_for('main.index'))
        logger.warning('This email is not existed')
        flash('用户名密码错误')
        return jsonify({'status': 'ok', 'content': 'got files'})
    elif request.method == 'GET':
        return render_template('auth/login.html')


@auth.route('/logout')
@login_required
def logout():
    logout_user()
    logger.warning('User {} logout'.format(session.get('LOGINNAME')))
    session.clear()
    flash('You have been logged out.')
    return redirect(url_for('.login'))
