from flask import redirect, session, url_for, render_template, flash, request, jsonify, send_from_directory
from flask_login import login_required
from .. import db, logger, redis_db, work_q
from . import main
from ..decorators import permission_required
from ..models import *
from werkzeug.utils import secure_filename
from ..MyModule.UploadFile import uploadfile
from .views import allowed_file, gen_file_name, IGNORED_FILES
import os
import json
from ..MyModule.HashContent import md5_content
from flask_socketio import SocketIO
import uuid


@main.route('/upload_pcap', methods=['GET'])
def upload_pcap():
    return render_template('upload_pcap.html')


@main.route('/join_pcap_queue', methods=['GET', 'POST'])
def join_pcap_queue():
    filename = request.form.get('filename')
    filepath = os.path.join(UPLOAD_FOLDER, md5_content(session['LOGINUSER']))
    this_uuid = md5_content(os.path.join(filepath, filename))
    print('the file\'s md5 is', this_uuid)
    redis_record = redis_db.get(this_uuid)
    if not redis_record:
        print('no record')
        work_q.put({'filename': filename,
                    'filepath': filepath})
        print('work end')
        return jsonify({"status": 'ok', "content": "已加入队列"})
    else:
        return jsonify({"status": 'fail', "content": "请勿重复加入队列"})


@main.route('/check_pcap_result', methods=['GET', 'POST'])
def check_pcap_result():
    filename = request.form.get('filename')
    filepath = os.path.join(UPLOAD_FOLDER, md5_content(session['LOGINUSER']))
    this_uuid = md5_content(os.path.join(filepath, filename))
    redis_record = redis_db.get(this_uuid)
    if not redis_record:
        return jsonify({"status": 'fail', "content": "订单不存在"})
    elif json.loads(redis_record.decode())['status'] == 1:
        return jsonify({"status": 'fail', "content": "正在处理中，请稍后"})
    elif json.loads(redis_record.decode())['status'] == 2:
        return jsonify({"status": 'ok', "content": "处理完成，查看结果"})
    elif json.loads(redis_record.decode())['status'] == 3:
        return jsonify({"status": 'fail', "content": "处理异常，请检查文件"})


@main.route('/do_upload_pcap', methods=['GET', 'POST'])
@login_required
@permission_required(Permission.ADMINISTER)
def do_upload_pcap():
    logger.info('User {} is uploading pcap packets'.format(session['LOGINNAME']))
    logger.info('{}'.format(md5_content(session['LOGINUSER'])))
    user_dir = md5_content(session['LOGINUSER'])
    if not os.path.exists(os.path.join(UPLOAD_FOLDER, user_dir)):
        os.makedirs(os.path.join(UPLOAD_FOLDER, user_dir))
    this_dir = os.path.join(UPLOAD_FOLDER, user_dir)

    if request.method == 'POST':
        print(request.files)
        files = request.files['file']

        if files:
            filename = secure_filename(files.filename)
            filename = gen_file_name(filename, path=this_dir)
            mime_type = files.content_type

            if not allowed_file(files.filename):
                result = uploadfile(name=filename, type=mime_type, size=0, not_allowed_msg="File type not allowed")

            else:
                # save file to disk
                uploaded_file_path = os.path.join(this_dir, filename)
                files.save(uploaded_file_path)

                # create thumbnail after saving
                if mime_type.startswith('image'):
                    pass

                # get file size after saving
                size = os.path.getsize(uploaded_file_path)

                # return json for js call back
                result = uploadfile(name=filename, type=mime_type, size=size, service_url='_pcap')

            return json.dumps({"files": [result.get_file()]}, ensure_ascii=False)

    if request.method == 'GET':
        # get all file in ./data directory
        files = [f for f in os.listdir(this_dir) if
                 os.path.isfile(os.path.join(this_dir, f)) and f not in IGNORED_FILES]

        file_display = []

        for f in files:
            size = os.path.getsize(os.path.join(this_dir, f))
            try_this_order = redis_db.get(md5_content(os.path.join(this_dir, f)))
            if try_this_order:
                print(try_this_order.decode())
            file_saved = uploadfile(name=f,
                                    size=size,
                                    service_url='_pcap',
                                    order_status=json.loads(try_this_order.decode())[
                                        'status'] if try_this_order else '')
            file_display.append(file_saved.get_file())
        print(file_display)

        return json.dumps({"files": file_display}, ensure_ascii=False)

    return redirect(url_for('main.upload_pcap'))


@main.route("/data_pcap/<string:filename>", methods=['GET'])
@login_required
@permission_required(Permission.NETWORK_MANAGER)
def data_pcap(filename):
    return send_from_directory(os.path.join(UPLOAD_FOLDER), filename=filename)


@main.route("/data_pcap_tmp/<string:filename>", methods=['GET'])
def data_pcap_tmp(filename):
    filepath = os.path.join(UPLOAD_FOLDER, md5_content(session['LOGINUSER']))
    file = os.path.join(filepath, filename)
    file_tmp = md5_content(file)
    print(file)
    print(os.path.join(filepath, 'tmp'))
    print(file_tmp)
    return send_from_directory(os.path.join(filepath, 'tmp'), filename=file_tmp+'.txt', as_attachment=True)


@main.route("/delete_pcap/<string:filename>", methods=['DELETE'])
@login_required
@permission_required(Permission.ADMINISTER)
def delete_pcap(filename):
    logger.info('{}'.format(md5_content(session['LOGINUSER'])))
    user_dir = md5_content(session['LOGINUSER'])

    this_dir = os.path.join(UPLOAD_FOLDER, user_dir)

    file_path = os.path.join(this_dir, filename)

    tmp_file_path = os.path.join(this_dir, 'tmp', md5_content(file_path))

    if os.path.exists(file_path):
        try:
            os.remove(file_path)
            r = redis_db.get(md5_content(file_path))
            print(md5_content(file_path))
            if r:
                redis_db.delete(md5_content(file_path))

            if os.path.exists(tmp_file_path):
                os.remove(tmp_file_path)

            return json.dumps({filename: 'True'})
        except:
            return json.dumps({filename: 'False'})

    else:
        return json.dumps({filename: 'False'})
