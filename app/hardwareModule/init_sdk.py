import os
import datetime
import time
import json
from .plate_struct_class import *
from ..models import Camera
from .. import logger, redis_db, work_q, p_msg_cb_func, fdfs_client
import ctypes


# 撤防
def NET_DVR_CloseAlarmChan_V30(m_lAlarmHandle):
    res = callCpp("NET_DVR_CloseAarmChan_V30", m_lAlarmHandle)
    if res < 0:
        print("撤防失败:" + str(callCpp("NET_DVR_GetLastError")))
    else:
        print("撤防成功")


# 释放SDK 资源
def NET_DVR_Cleanup():
    res = callCpp("NET_DVR_Cleanup")
    if res < 0:
        print("SDK资源释放失败:" + str(callCpp("NET_DVR_GetLastError")))
    else:
        print("SDK资源成功释放！！")


####用户追注销
def NET_DVR_Logout_V30(lUserID):
    if callCpp("NET_DVR_Logout_V30", lUserID):
        print("用户已经成功注销")
    else:
        print("注销失败：" + str(callCpp("NET_DVR_GetLastError")))
    init_res = callCpp("NET_DVR_Cleanup")  # 释放SDK资源
    if init_res:
        print("SDK资源释放成功")
    else:
        error_info = callCpp("NET_DVR_GetLastError")
        print("错误信息：" + str(error_info))
        return False


# 回调函数接口
def MsgCallback(lCommand, net_dvr_alarm, struITSPlateResult, dwBuflen, pUser):
    print("回调函数执行，lcommand的值：" + str(lCommand))
    plate_info = {}  # plate的返回内容，是plate_json的‘data’的value
    if lCommand == 0x2800:  # 旧版本车牌识别
        device_IP = str(net_dvr_alarm.contents.sDeviceIP)
        plate_num = str(struITSPlateResult.contents.struPlateInfo.sLicense.decode('gb2312'))

        # print("车牌结构体中的变量，dwSize输出：" + str(struITSPlateResult.contents.dwSize))
        print("plate num=" + plate_num)
        time_table = []
        for i in struITSPlateResult.contents.byAbsTime:
            if i != 0:
                time_table.append(chr(i))
        string_time = ''.join(time_table)
        string_time_to_json = str(string_time[0:4]) + '-' + str(string_time[4:6]) + '-' + \
                              str(string_time[6:8]) + ' ' + str(string_time[8:10]) + ':' + \
                              str(string_time[10:12]) + ':' + str(string_time[12:14])

        print(str(string_time[0:4]) + '-' + str(string_time[4:6]) + '-' + \
              str(string_time[6:8]) + '-' + str(string_time[8:10]) + '-' + \
              str(string_time[10:12]) + '-' + str(string_time[12:14]) + '-' + str(string_time[14:]))

        print(struITSPlateResult.contents.dwPicLen)
        print(struITSPlateResult.contents.dwPicPlateLen)
        file_name = ''
        # 存全景图和车辆小图
        if struITSPlateResult.contents.dwPicLen > 0:  # & struITSPlateResult.contents.dwPicPlateLen > 0:
            full_pic_data = ctypes.create_string_buffer(b'\0', struITSPlateResult.contents.dwPicLen)

            for i in range(struITSPlateResult.contents.dwPicLen):
                full_pic_data[i] = struITSPlateResult.contents.pBuffer1[i]

            if fdfs_client:
                ret = fdfs_client.upload_by_buffer(full_pic_data, file_ext_name='jpg')
                if ret:
                    file_name = ret['Remote file_id'].decode()
                else:
                    logger.error("file storage fail {} at {}".format(plate_num, str(string_time[0:-3])))
                    file_name = ''
            else:
                file_name = './pic/full_pic/' + plate_num + '_' + str(string_time[0:-3]) + '.jpg'
                with open(file_name, 'wb') as f:
                    f.write(full_pic_data)
        # 存车牌图
        if struITSPlateResult.contents.dwPicPlateLen > 0:
            file_name = './pic/full_pic/' + plate_num \
                        + '_' + str(string_time[0:-3]) + '.jpg'
            plate_file_name = './pic/plate_pic/' + plate_num \
                              + '_' + str(string_time[0:-3]) + '.jpg'
            plate_pic_data = ctypes.create_string_buffer(b'\0', struITSPlateResult.contents.dwPicPlateLen)

            for i in range(struITSPlateResult.contents.dwPicPlateLen):
                plate_pic_data[i] = struITSPlateResult.contents.pBuffer2[i]

            if fdfs_client:
                ret = fdfs_client.upload_by_buffer(plate_pic_data, file_ext_name='jpg')
                if ret:
                    plate_file_name = ret['Remote file_id'].decode()
                else:
                    logger.error("file storage fail {} at {}".format(plate_num, str(string_time[0:-3])))
            else:
                plate_file_name = './pic/plate_pic/' + plate_num + '_' + str(string_time[0:-3]) + '.jpg'
                with open(plate_file_name, 'wb') as f:
                    f.write(plate_pic_data)

            # json字典注册
            plate_info['camera'] = device_IP
            plate_info['number_plate'] = plate_num
            plate_info['time'] = string_time_to_json
            plate_info['pic'] = file_name
            plate_info['plate_number_pic'] = plate_file_name
            work_q.put(plate_info)
            # plate_info_json_str = json.dumps(plate_json)

    if lCommand == 0x3050:
        device_IP = str(net_dvr_alarm.contents.sDeviceIP)
        plate_num = str(struITSPlateResult.contents.struPlateInfo.sLicense.decode('gb2312'))

    return 1


# 获取所有的库文件到一个列表
def add_so(path, so_list):
    files = os.listdir(path)
    for file in files:
        if not os.path.isdir(path + file):
            if file.endswith(".so"): so_list.append(path + file)
        else:
            add_so(path + file + "/", so_list)


def callCpp(func_name, *args):
    # path = "./hik_sdk_x64/"
    path = "/Users/Peter/python/realeso_parking_local/app/hardwareModule/CH_HCNetSDK_V5.3.5.2_build20171124_Linux64/"
    # path = "./hik_sdk/"
    so_list = []
    add_so(path, so_list)
    for so_lib in so_list:
        lib = ctypes.cdll.LoadLibrary(so_lib)
        try:

            try:
                value = eval("lib.%s" % func_name)(*args)
                # print("调用的库："+so_lib)
                print("执行成功,返回值：" + str(value))
                return value
            except Exception as e:
                print(e)
                continue
        except:
            # print("库文件载入失败："+so_lib)
            continue
    print("没有找到接口！")
    return False


##设置回调,由于在回调函数这里char*指针传不过去，因此改成了由C++去实现
def NET_DVR_SetDVRMessageCallBack_V30(p_cb_func):
    if callCpp("NET_DVR_SetDVRMessageCallBack_V30", p_cb_func, None):
        print("设置回调函数成功！")
        return True
    else:
        print("回调函数设置错误：" + str(callCpp("NET_DVR_GetLastError")))
        return False
    # iListenHandle = callCpp("NET_DVR_StartListen_V30", sLocalIP,wLocalPort,CALLFUNC(MSGCallBack),pUserData)


#####布防
def NET_DVR_SetupAlarmChan_V41(lUserID):
    struAlarmParam = NET_DVR_SETUPALARM_PARAM()
    struAlarmParamSize = ctypes.sizeof(struAlarmParam)
    print("结构体大小：" + str(struAlarmParamSize))
    struAlarmParam.dwSize = struAlarmParamSize  # 结构体大小
    struAlarmParam.beLevel = 1  # 0 一级布防，1 二级布防
    # struAlarmParam.byAlarmInfoType = 1#智能交通设备有效
    struAlarmParam.byFaceAlarmDetection = 1  # 人脸侦测报警
    struAlarmParam.byAlarmInfoType = 0  # 上传报警信息类型: 0- 老报警信息(NET_DVR_PLATE_RESULT), 1- 新报警信息(NET_ITS_PLATE_RESULT)
    struAlarmParam_p = ctypes.byref(struAlarmParam)
    m_lAlarmHandle = callCpp("NET_DVR_SetupAlarmChan_V41", lUserID, struAlarmParam_p)
    print("布防结果：" + str(m_lAlarmHandle))
    if m_lAlarmHandle < 0:
        print("错误信息：" + str(callCpp("NET_DVR_GetLastError")))
        return False
    else:
        print("布防成功")
        return True


# 配置出入口道闸控制参数
def NET_BARRIERGATE_config(lUserID):
    ITC_POST_SINGLEIO_TYPE = ctypes.c_uint32(0x2)
    NET_DVR_GET_ENTRANCE_PARAMCFG = ctypes.c_uint32(3126)
    NET_DVR_SET_ENTRANCE_PARAMCFG = ctypes.c_uint32(3127)
    m_struGateCond = NET_DVR_BARRIERGATE_COND()
    m_struEntrance = NET_DVR_ENTRANCE_CFG()
    relay_param = NET_DVR_RELAY_PARAM()
    relay_param.byAccessDevInfo = ctypes.c_byte(1)
    v_control = NET_DVR_VEHICLE_CONTROL()
    dwStatus = ctypes.c_uint32(0)
    p_dwStatus = ctypes.byref(dwStatus)
    m_struGateCond.byLaneNo = ctypes.c_byte(1)
    print(ctypes.sizeof(m_struGateCond))
    print(ctypes.sizeof(m_struEntrance))
    print(ctypes.sizeof(NET_DVR_ENTRANCE_CFG))
    size_out_buf = ctypes.sizeof(NET_DVR_ENTRANCE_CFG)
    print(size_out_buf)
    print(ctypes.sizeof(relay_param))
    print(ctypes.sizeof(NET_DVR_RELAY_PARAM))
    print(ctypes.sizeof(v_control))
    print(ctypes.sizeof(NET_DVR_VEHICLE_CONTROL))
    p_struGateCond = ctypes.byref(m_struGateCond)
    p_struEntrance = ctypes.byref(m_struEntrance)
    # 获取道闸参数
    res = callCpp("NET_DVR_GetDeviceConfig", ctypes.c_int32(lUserID), NET_DVR_GET_ENTRANCE_PARAMCFG, \
                  ctypes.c_uint32(1), p_struGateCond, ctypes.c_uint32(ctypes.sizeof(NET_DVR_BARRIERGATE_COND)), \
                  p_dwStatus, p_struEntrance, ctypes.c_uint32(size_out_buf))

    if res == 1:
        if (dwStatus.value != 0):
            print('get entrance config failed status error' + str(dwStatus))
            # return -2
    else:
        print('get entrance config failed res' + str(res))
        get_res = callCpp("NET_DVR_GetLastError")
        print('get_res ' + str(get_res))
        # return -1

    # 配置道闸参数
    m_struEntrance.byEnable = ctypes.c_byte(1)  # 使能：0-关闭，1-打开
    m_struEntrance.byBarrierGateCtrlMode = ctypes.c_byte(1)  # 道闸控制模式：0-相机自动控制，1-平台外部控制
    m_struEntrance.dwRelateTriggerMode = ITC_POST_SINGLEIO_TYPE
    # 0-不接入设备，1-开道闸，2-关道闸，3-停道闸，4-报警信号，5-常亮灯
    m_struEntrance.struRelayRelateInfo[0].byAccessDevInfo = ctypes.c_byte(1)

    p_struGateCond = ctypes.byref(m_struGateCond)
    p_struEntrance = ctypes.byref(m_struEntrance)

    # 写回道闸参数
    res = callCpp("NET_DVR_SetDeviceConfig", ctypes.c_uint32(lUserID), NET_DVR_SET_ENTRANCE_PARAMCFG, \
                  ctypes.c_uint32(1), p_struGateCond, ctypes.sizeof(NET_DVR_BARRIERGATE_COND), \
                  p_dwStatus, p_struEntrance, ctypes.c_uint32(size_out_buf))
    if res == 1:
        if dwStatus != 0:
            print('set barrier gate cond error dwstatus ' + str(dwStatus))
            return -2
    else:
        print('set barrier gate cond error res ' + str(res))
        set_res = callCpp("NET_DVR_GetLastError")
        print('set res ' + str(set_res))
        return -1


def do_init():
    init_res = callCpp("NET_DVR_Init")  # SDK初始化
    if init_res:
        logger.info("SDK初始化成功")
        return True
    else:
        error_info = callCpp("NET_DVR_GetLastError")
        logger.error("SDK初始化错误：" + str(error_info))
        return False


def init_overtime():
    set_overtime = callCpp("NET_DVR_SetConnectTime", 5000, 4)  # 设置超时
    if set_overtime:
        logger.info("设置超时时间成功")
        return True
    else:
        error_info = callCpp("NET_DVR_GetLastError")
        logger.info("设置超时错误信息：" + str(error_info))
        return False


def register_callback():
    CALLFUNC = ctypes.CFUNCTYPE(ctypes.c_int, \
                                ctypes.c_long, \
                                ctypes.POINTER(NET_DVR_ALARMER), \
                                ctypes.POINTER(NET_DVR_PLATE_RESULT), \
                                ctypes.c_uint, \
                                ctypes.POINTER(ctypes.c_uint8))

    p_msg_cb_func.append(CALLFUNC(MsgCallback))
    if NET_DVR_SetDVRMessageCallBack_V30(p_msg_cb_func[0]):
        logger.info("注册回调函数成功")
        return True
    else:
        return False


# 用户注册设备
def NET_DVR_Login_V30(sDVRIP="192.168.3.65", wDVRPort=8000, sUserName="admin", sPassword="abc12345"):
    # 用户注册设备
    # c++传递进去的是byte型数据，需要转成byte型传进去，否则会乱码
    sDVRIP = bytes(sDVRIP, "ascii")
    sUserName = bytes(sUserName, "ascii")
    sPassword = bytes(sPassword, "ascii")
    DeviceInfo = LPNET_DVR_DEVICEINFO_V30()
    DeviceInfoRef = ctypes.byref(DeviceInfo)
    lUserID = callCpp("NET_DVR_Login_V30", sDVRIP, wDVRPort, sUserName, sPassword, DeviceInfoRef)
    print("登录结果：" + str(lUserID))
    if lUserID == -1:
        error_info = callCpp("NET_DVR_GetLastError")
        logger.error("登录错误信息：" + str(error_info))
        return False
    else:
        return lUserID


def login_camera():
    try:
        lUserIDs = {}
        cameras = Camera.query.filter_by(status=1).all()
        for camera in cameras:
            lUserID = NET_DVR_Login_V30(camera.device_number)
            assert lUserID, "登陆摄像头{}失败".format(camera.device_number)
            lUserIDs[camera.device_number] = lUserID
        redis_db.set("lUserIDs", json.dumps(lUserIDs))
        return True
    except Exception as e:
        logger.error(e)
        return False


def set_alarm():
    try:
        lUserIDs = redis_db.get('lUserIDs')
        if lUserIDs:
            lUserIDs = json.loads(lUserIDs.decode())
            for ip, lUserID in lUserIDs.items():
                assert NET_DVR_SetupAlarmChan_V41(lUserID=lUserID), "布防摄像头{}，lUserID: {}".format(ip, lUserID)
                NET_BARRIERGATE_config(lUserID)
            return True
        else:
            logger.error('无登陆后的User_id, 布防失败')
            return False
    except Exception as e:
        logger.error(e)
