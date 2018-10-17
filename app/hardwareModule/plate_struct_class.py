import ctypes
'''
包含SDK中定义的c语言结构体
'''
#定义结构体
class LPNET_DVR_DEVICEINFO_V30(ctypes.Structure):
    _fields_ = [
        ("sSerialNumber", ctypes.c_byte * 48),
        ("byAlarmInPortNum", ctypes.c_byte),
        ("byAlarmOutPortNum", ctypes.c_byte),
        ("byDiskNum", ctypes.c_byte),
        ("byDVRType", ctypes.c_byte),
        ("byChanNum", ctypes.c_byte),
        ("byStartChan", ctypes.c_byte),
        ("byAudioChanNum", ctypes.c_byte),
        ("byIPChanNum", ctypes.c_byte),
        ("byZeroChanNum", ctypes.c_byte),
        ("byMainProto", ctypes.c_byte),
        ("bySubProto", ctypes.c_byte),
        ("bySupport", ctypes.c_byte),
        ("bySupport1", ctypes.c_byte),
        ("bySupport2", ctypes.c_byte),
        ("wDevType", ctypes.c_uint16),
        ("bySupport3", ctypes.c_byte),
        ("byMultiStreamProto", ctypes.c_byte),
        ("byStartDChan", ctypes.c_byte),
        ("byStartDTalkChan", ctypes.c_byte),
        ("byHighDChanNum", ctypes.c_byte),
        ("bySupport4", ctypes.c_byte),
        ("byLanguageType", ctypes.c_byte),
        ("byVoiceInChanNum", ctypes.c_byte),
        ("byStartVoiceInChanNo", ctypes.c_byte),
        ("byRes3", ctypes.c_byte * 2),
        ("byMirrorChanNum", ctypes.c_byte),
        ("wStartMirrorChanNo", ctypes.c_uint16),
        ("byRes2", ctypes.c_byte * 2)]

#报警设备信息结构体
class NET_DVR_ALARMER(ctypes.Structure):
    _fields_ = [
        ("byUserIDValid", ctypes.c_byte),
        ("bySerialValid", ctypes.c_byte),
        ("byVersionValid", ctypes.c_byte),
        ("byDeviceNameValid", ctypes.c_byte),
        ("byMacAddrValid", ctypes.c_byte),
        ("byLinkPortValid", ctypes.c_byte),
        ("byDeviceIPValid", ctypes.c_byte),
        ("bySocketIPValid", ctypes.c_byte),
        ("lUserID", ctypes.c_long),
        ("sSerialNumber", ctypes.c_byte * 48),
        ("dwDeviceVersion", ctypes.c_uint32),
        ("sDeviceName", ctypes.c_char * 32),
        ("byMacAddr", ctypes.c_char * 6),
        ("wLinkPort", ctypes.c_uint16),
        ("sDeviceIP", ctypes.c_char * 128),
        ("sSocketIP", ctypes.c_char * 128),
        ("byIpProtocol", ctypes.c_byte),
        ("byRes2", ctypes.c_byte * 11)
    ]

class NET_VCA_RECT(ctypes.Structure):
    _fields_ = [
        ("fX", ctypes.c_float),
        ("fY", ctypes.c_float),
        ("fWidth", ctypes.c_float),
        ("fHeight", ctypes.c_float)
    ]

class NET_DVR_PLATE_INFO(ctypes.Structure):
    _fields_ = [
        ("byPlateType", ctypes.c_byte),
        ("byColor", ctypes.c_byte),
        ("byBright", ctypes.c_byte),
        ("byLicenseLen", ctypes.c_byte),
        ("byEntireBelieve", ctypes.c_byte),
        ("byRegion", ctypes.c_byte),
        ("byCountry", ctypes.c_byte),
        ("byArea", ctypes.c_byte),
        ("byPlateSize", ctypes.c_byte),
        ("byRes", ctypes.c_byte * 15),
        ("sPlateCategory", ctypes.c_char * 8),
        ("dwXmlLen", ctypes.c_short),
        ("pXmlBuf", ctypes.c_char_p),
        ("struPlateRect", NET_VCA_RECT),
        ("sLicense", ctypes.c_char * 16),
        ("byBelieve", ctypes.c_byte * 16)
    ]

class NET_DVR_VEHICLE_INFO(ctypes.Structure):
    _fields_ = [
        ("dwIndex", ctypes.c_short),
        ("byVehicleType", ctypes.c_byte),
        ("byColorDepth", ctypes.c_byte),
        ("byColor", ctypes.c_byte),
        ("byRadarState", ctypes.c_byte),
        ("wSpeed", ctypes.c_short),
        ("wLength", ctypes.c_short),
        ("byIllegalType", ctypes.c_byte),
        ("byVehicleLogoRecog", ctypes.c_byte),
        ("byVehicleSubLogoRecog", ctypes.c_byte),
        ("byVehicleModel", ctypes.c_byte),
        ("byCustomInfo", ctypes.c_byte * 16),
        ("wVehicleLogoRecog", ctypes.c_short),
        ("byRes3", ctypes.c_byte * 14)
    ]

class NET_DVR_TIME_V30(ctypes.Structure):
    _fields_ = [
        ("wYear", ctypes.c_short),
        ("byMonth", ctypes.c_byte),
        ("byDay", ctypes.c_byte),
        ("byHour", ctypes.c_byte),
        ("byMinute", ctypes.c_byte),
        ("bySecond", ctypes.c_byte),
        ("byRes", ctypes.c_byte),
        ("wMilliSec", ctypes.c_short),
        ("byRes1", ctypes.c_byte * 2)
    ]

class NET_ITS_PICTURE_INFO(ctypes.Structure):
    _fields_ = [
        ("dwDataLen", ctypes.c_short),
        ("byType", ctypes.c_byte),
        ("byDataType", ctypes.c_byte),
        ("byCloseUpType", ctypes.c_byte),
        ("byPicRecogMode", ctypes.c_byte),
        ("dwRedLightTime", ctypes.c_short),
        ("byAbsTime", ctypes.c_byte * 32),
        ("struPlateRect", NET_VCA_RECT),
        ("struPlateRecgRect", NET_VCA_RECT),
        ("pBuffer", ctypes.c_char_p),
        ("dwUTCTime", ctypes.c_short),
        ("byCompatibleAblity", ctypes.c_byte),
        ("byRes2", ctypes.c_byte * 7)
    ]

class NET_DVR_PLATE_RESULT(ctypes.Structure):
    _fields_ = [
        ("dwSize", ctypes.c_ulong),
        ("byResultType", ctypes.c_byte),
        ("byChanIndex", ctypes.c_byte),
        ("wAlarmRecordID", ctypes.c_short),
        ("dwRelativeTime", ctypes.c_ulong),
        ("byAbsTime", ctypes.c_byte * 32),
        ("dwPicLen", ctypes.c_ulong),
        ("dwPicPlateLen", ctypes.c_ulong),
        ("dwVideoLen", ctypes.c_ulong),
        ("byTrafficLight", ctypes.c_byte),
        ("byPicNum", ctypes.c_byte),
        ("byDriveChan", ctypes.c_byte),
        ("byVehicleType", ctypes.c_byte),
        ("dwBinPicLen", ctypes.c_ulong),
        ("dwCarPicLen", ctypes.c_ulong),
        ("dwFarCarPicLen", ctypes.c_ulong),
        ("pBuffer3", ctypes.POINTER(ctypes.c_char)),
        ("pBuffer4", ctypes.POINTER(ctypes.c_char)),
        ("pBuffer5", ctypes.POINTER(ctypes.c_char)),
        ("byRelaLaneDirectionType", ctypes.c_byte),
        ("byCarDirectionType", ctypes.c_byte),
        ("byRes3", ctypes.c_byte * 6),
        ("struPlateInfo", NET_DVR_PLATE_INFO),
        ("struVehicleInfo", NET_DVR_VEHICLE_INFO),
        ("pBuffer1", ctypes.POINTER(ctypes.c_char)),
        ("pBuffer2", ctypes.POINTER(ctypes.c_char))
    ]

# 新报警消息检测结果
class NET_ITS_PLATE_RESULT(ctypes.Structure):
    _fields_ = [
        ("dwSize", ctypes.c_uint32), # 结构长度
        ("dwMatchNo", ctypes.c_uint32), # 匹配序号,由(车辆序号,数据类型,车道号)组成匹配码
        ("byGroupNum", ctypes.c_byte), # 图片组数量（一辆过车相机多次抓拍的数量，代表一组图片的总数，用于延时匹配数据）
        ("byPicNo", ctypes.c_byte), # 连拍的图片序号（接收到图片组数量后，表示接收完成;接收超时不足图片组数量时，根据需要保留或删除）
        ("bySecondCam", ctypes.c_byte), # 是否第二相机抓拍（如远近景抓拍的远景相机，或前后抓拍的后相机，特殊项目中会用到）
        ("byFeaturePicNo", ctypes.c_byte), # 闯红灯电警，取第几张图作为特写图,0xff-表示不取
        ("byDriveChan", ctypes.c_byte), # 触发车道号
        ("byVehicleType", ctypes.c_byte), #车辆类型，参考VTR_RESULT
        ("byDetSceneID", ctypes.c_byte), #检测场景号[1,4], IPC默认是0
        #车辆属性，按位表示，0 - 无附加属性(普通车)，bit1 - 黄标车(类似年检的标志)，bit2 - 危险品车辆，值：0 - 否，1 - 是
        #该节点已不再使用, 使用下面的byYellowLabelCar和byDangerousVehicles判断是否黄标车和危险品车
        ("byVehicleAttribute", ctypes.c_byte),
        ("wIllegalType", ctypes.c_uint16), #违章类型采用国标定义
        ("byIllegalSubType", ctypes.c_byte * 8), #违章子类型
        ("byPostPicNo", ctypes.c_byte), #违章时取第几张图片作为卡口图,0xff-表示不取
        #通道号(有效，报警通道号和所在设备上传报警通道号一致，在后端和所接入的 通道号一致)
        ("byChanIndex", ctypes.c_byte),
        ("wSpeedLimit", ctypes.c_uint16), #限速上限（超速时有效）
        ("byChanIndexEx", ctypes.c_byte), #byChanIndexEx*256+byChanIndex表示真实通道号。
        ("byRes2", ctypes.c_byte),
        ("struPlateInfo", NET_DVR_PLATE_INFO), #车牌信息结构
        ("struVehicleInfo", NET_DVR_VEHICLE_INFO), #车辆信息
        ("byMonitoringSiteID", ctypes.c_byte * 48), #监测点编号
        ("byDeviceID", ctypes.c_byte * 48), #设备编号
        ("byDir", ctypes.c_byte), #监测方向，1-上行（反向），2-下行(正向)，3-双向，4-由东向西，5-由南向北,6-由西向东，7-由北向南，8-其它
        ("byDetectType", ctypes.c_byte), #检测方式,1-地感触发，2-视频触发，3-多帧识别，4-雷达触发
        #关联车道方向类型，参考ITC_RELA_LANE_DIRECTION_TYPE
        #该参数为车道方向参数，与关联车道号对应，确保车道唯一性。
        ("byRelaLaneDirectionType", ctypes.c_byte),
        ("byCarDirectionType", ctypes.c_byte), #车辆具体行驶的方向，0表示从上往下，1表示从下往上（根据实际车辆的行驶方向来的区分）
        #当wIllegalType参数为空时，使用该参数。若wIllegalType参数为有值时，以wIllegalType参数为准，该参数无效。
        ("dwCustomIllegalType", ctypes.c_uint16), #违章类型定义(用户自定义)
        # 为0~数字格式时，为老的违章类型，wIllegalType、dwCustomIllegalType参数生效，赋值国标违法代码。
        # 为1~字符格式时，pIllegalInfoBuf参数生效。老的违章类型，wIllegalType、dwCustomIllegalType参数依然赋值国标违法代码
        ("pIllegalInfoBuf", ctypes.POINTER(ctypes.c_byte)), #违法代码字符信息结构体指针；指向NET_ITS_ILLEGAL_INFO
        ("byIllegalFromatType", ctypes.c_byte), #违章信息格式类型； 0~数字格式， 1~字符格式
        ("byPendant", ctypes.c_byte), #0-表示未知,1-车窗有悬挂物，2-车窗无悬挂物
        ("byDataAnalysis", ctypes.c_byte), #0-数据未分析, 1-数据已分析
        ("byYellowLabelCar", ctypes.c_byte), #0-表示未知, 1-非黄标车,2-黄标车
        ("byDangerousVehicles", ctypes.c_byte), #0-表示未知, 1-非危险品车,2-危险品车
        #以下字段包含Pilot字符均为主驾驶，含Copilot字符均为副驾驶
        ("byPilotSafebelt", ctypes.c_byte), #0-表示未知,1-系安全带,2-不系安全带
        ("byCopilotSafebelt", ctypes.c_byte), #0-表示未知,1-系安全带,2-不系安全带
        ("byPilotSunVisor", ctypes.c_byte), #0-表示未知,1-不打开遮阳板,2-打开遮阳板
        ("byCopilotSunVisor", ctypes.c_byte), #0-表示未知, 1-不打开遮阳板,2-打开遮阳板
        ("byPilotCall", ctypes.c_byte), #0-表示未知, 1-不打电话,2-打电话
        #0-开闸，1-未开闸 (专用于历史数据中相机根据黑白名单匹配后，是否开闸成功的标志)
        ("byBarrierGateCtrlType", ctypes.c_byte),
        ("byAlarmDataType", ctypes.c_byte), #0-实时数据，1-历史数据
        ("struSnapFirstPicTime", NET_DVR_TIME_V30), #端点时间(ms)（抓拍第一张图片的时间）
        ("dwIllegalTime", ctypes.c_uint32), #违法持续时间（ms） = 抓拍最后一张图片的时间 - 抓拍第一张图片的时间
        ("dwPicNum", ctypes.c_uint32), #图片数量（与picGroupNum不同，代表本条信息附带的图片数量，图片信息由struVehicleInfoEx定义
        ("struPicInfo", NET_ITS_PICTURE_INFO * 6) #图片信息,单张回调，最多6张图，由序号区分
    ]

class USING_PLATE_INFO(ctypes.Structure):
    _fields_ = [
        ("using_plate_info", NET_DVR_PLATE_INFO),
        ("full_picture_path", ctypes.c_char * 128),
        ("plate_picture_path", ctypes.c_char * 128),
        ("byAbsTime", ctypes.c_byte * 32)
    ]

#####布防
class NET_DVR_SETUPALARM_PARAM(ctypes.Structure):
    _fields_ = [
        ("dwSize",ctypes.c_uint32),
        ("beLevel",ctypes.c_byte),
        ("byAlarmInfoType",ctypes.c_byte),
        ("byRetAlarmTypeV40",ctypes.c_byte),
        ("byRetDevInfoVersion",ctypes.c_byte),
        ("byRetVQDAlarmType",ctypes.c_byte),
        ("byFaceAlarmDetection",ctypes.c_byte),
        ("bySupport",ctypes.c_byte),
        ("byBrokenNetHttp",ctypes.c_byte),
        ("wTaskNo",ctypes.c_uint16),
        ("byDeployType",ctypes.c_byte),
        ("byRes1",ctypes.c_byte*3),
        ("byAlarmTypeURL",ctypes.c_byte),
        ("byCustomCtrl",ctypes.c_byte)
    ]

#IO输出口控制结构体
class NET_DVR_IO_OUTCFG(ctypes.Structure):
    _fields_ = [
        ("dwSize", ctypes.c_uint32),
        ("byDefaultStatus", ctypes.c_byte),
        ("byIoOutStatus", ctypes.c_byte),
        ("wAheadTime", ctypes.c_uint16),
        ("dwTimePluse", ctypes.c_uint32),
        ("dwTimeDelay", ctypes.c_uint32),
        ("byFreqMulti", ctypes.c_byte),
        ("byDutyRate", ctypes.c_byte),
        ("byRes", ctypes.c_byte * 2)
    ]

#道闸条件结构体
class NET_DVR_BARRIERGATE_COND(ctypes.Structure):
    _fields_ = [
        ("byLaneNo", ctypes.c_byte),
        ("byRes", ctypes.c_byte * 3)
    ]

#为了注册出入口配置结构体而注册的结构体
class NET_DVR_RELAY_PARAM(ctypes.Structure):
    _fields_ = [
        ("byAccessDevInfo", ctypes.c_byte),
        ("byRes", ctypes.c_byte * 3)
    ]

class NET_DVR_VEHICLE_CONTROL(ctypes.Structure):
    _fields_ = [
        ("byGateOperateType", ctypes.c_byte),
        ("byRes1", ctypes.c_byte),
        ("wAlarmOperateType", ctypes.c_uint16),
        ("byRes2", ctypes.c_byte * 8)
    ]

#出入口配置结构体
class NET_DVR_ENTRANCE_CFG(ctypes.Structure):
    _fields_ = [
        ("dwSize", ctypes.c_uint32),
        ("byEnable", ctypes.c_byte),
        ("byBarrierGateCtrlMode", ctypes.c_byte),
        ("byRes1", ctypes.c_byte * 2),
        ("dwRelateTriggerMode", ctypes.c_uint32),
        ("dwMatchContent",  ctypes.c_uint32),
        ("struRelayRelateInfo", NET_DVR_RELAY_PARAM * 12),
        ("byGateSingleIO", ctypes.c_byte * 8),
        ("struVehicleCtrl", NET_DVR_VEHICLE_CONTROL * 8),
        ("byRes2", ctypes.c_byte * 64)
    ]

#道闸控制结构体
'''
若老的平台不支持byUnlock字段，该字段将赋值为0，通过“0-关闭道闸,1-开启道闸,2-停止道闸”中的任何一种操作皆可进行解锁。
若新平台支持byUnlock字段，需byUnlock字段赋值为1，并结合4~解锁道闸来进行解锁。byUnlock字段赋值为1后，“0-关闭道闸,1-开启道闸,2-停止道闸”操作将不可用于解锁。
'''
class NET_DVR_BARRIERGATE_CFG(ctypes.Structure):
    _fields_ = [
        ("dwSize", ctypes.c_uint32),
        ("dwChannel", ctypes.c_uint32), #通道号
        ("byLaneNo", ctypes.c_byte), #道闸号（0-表示无效值(设备需要做有效值判断),1-道闸1）
        ("byBarrierGateCtrl", ctypes.c_byte), #0-关闭道闸,1-开启道闸,2-停止道闸 3-锁定道闸,4~解锁道闸
        ("byEntranceNo", ctypes.c_byte), #出入口编号 [1,8]
        ("byUnlock", ctypes.c_byte), #启用解锁使能，0~为不启用，1~启用
        ("byRes", ctypes.c_byte * 12)
    ]

#std config结构体
class NET_DVR_STD_CONFIG(ctypes.Structure):
    _fields_ = [
        ("lpCondBuffer", ctypes.POINTER(ctypes.c_uint32)),
        ("dwCondSize", ctypes.c_uint32),
        ("lpInBuffer", ctypes.POINTER(ctypes.c_uint8)),
        ("dwInSize", ctypes.c_uint32),
        ("lpOutBuffer", ctypes.POINTER(ctypes.c_uint8)),
        ("dwOutSize", ctypes.c_uint32),
        ("lpStatusBuffer", ctypes.POINTER(ctypes.c_uint8)),
        ("dwStatusSize", ctypes.c_uint32),
        ("lpXmlBuffer", ctypes.POINTER(ctypes.c_uint8)),
        ("dwXmlSize", ctypes.c_uint32),
        ("byDataType", ctypes.c_byte),
        ("byRes", ctypes.c_byte * 23)
    ]

#LED屏幕显示参数
class NET_DVR_LEDDISPLAY_CFG(ctypes.Structure):
    _fields_ = [
        ("dwSize", ctypes.c_uint32),
        ("sDisplayInfo", ctypes.c_char * 512), #LED显示内容
        ("byDisplayMode", ctypes.c_byte), #显示方式:0~左移,1~右移,2~立即显示
        ("bySpeedType", ctypes.c_byte), #速度类型:0~快,1~中,2~慢
        ("byShowPlateEnable", ctypes.c_byte), #显示车牌使能，0~关闭，1~启用
        ("byRes1", ctypes.c_byte),
        ("dwShowTime", ctypes.c_uint32), #显示时长，1~60秒
        ("byRes", ctypes.c_byte * 128)
    ]

#语音播报控制参数
class NET_DVR_VOICEBROADCAST_CFG(ctypes.Structure):
    _fields_ = [
        ("dwSize", ctypes.c_uint32), #结构体大小
        ("sInfo", ctypes.c_byte * 128), #语音播报内容
        ("byBroadcastNum", ctypes.c_byte), #语音播报次数， 1~10次
        ("byIntervalTime", ctypes.c_byte), #语音播报间隔时间,1~5s
        ("byRes", ctypes.c_byte * 126)
    ]
