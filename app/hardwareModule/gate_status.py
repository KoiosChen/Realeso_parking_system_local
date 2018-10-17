import os
import ctypes
import datetime
import time
from .plate_struct_class import NET_DVR_BARRIERGATE_CFG
from .init_sdk import callCpp
from .. import logger

'''
1、获取设备目前道闸控制参数
2、开关道闸
'''

#道闸控制接口
#gate cond:0-关闭道闸,1-开启道闸,2-停止道闸 3-锁定道闸,4~解锁道闸
def NET_BARRIERGATE_CONTROL(lUserID, gate_cond):
    NET_DVR_BARRIERGATE_CTRL = ctypes.c_uint32(3128)
    m_struGateCFG = NET_DVR_BARRIERGATE_CFG()
    m_struGateCFG.dwSize = ctypes.sizeof(NET_DVR_BARRIERGATE_CFG)
    m_struGateCFG.dwChannel = ctypes.c_uint32(1) #通道号
    m_struGateCFG.byLaneNo = ctypes.c_byte(1) #道闸号（0-表示无效值(设备需要做有效值判断),1-道闸1）
    m_struGateCFG.byBarrierGateCtrl = gate_cond #0-关闭道闸,1-开启道闸,2-停止道闸 3-锁定道闸,4~解锁道闸
    p_struGateCFG = ctypes.byref(m_struGateCFG)
    res = callCpp("NET_DVR_RemoteControl", ctypes.c_uint32(lUserID), NET_DVR_BARRIERGATE_CTRL, p_struGateCFG,\
                  ctypes.sizeof(NET_DVR_BARRIERGATE_CFG))
    if res == 1:
        logger.debug('开闸配置成功')
        return True
    else:
        logger.error('set barrier gate failed res ' + str(res))
        control_res = callCpp("NET_DVR_GetLastError")
        logger.error('control res' + str(control_res))
        return False