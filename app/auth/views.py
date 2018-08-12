from flask import render_template, redirect, request, url_for, flash, session, jsonify, json
from flask_login import login_user, logout_user, login_required
from ..models import User, Area
from . import auth
from .. import logger


@auth.route('/login', methods=['GET', 'POST'])
def login():
    if request.method == 'POST':
        phoneNum = request.form.get('phoneNum')
        password = request.form.get('password')
        remember_me = request.form.get('remember_me')
        logger.warning('Somebody is trying to login as {}'.format(phoneNum))
        user = User.query.filter_by(phoneNum=phoneNum, status=1).first()
        if user is not None and user.verify_password(password):
            session.permanent = True
            logger.warning('Username is {}'.format(user.username))
            this_user = User.query.filter_by(phoneNum=phoneNum).first()
            session['LOGINUSER'] = phoneNum
            session['LOGINNAME'] = this_user.username
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
