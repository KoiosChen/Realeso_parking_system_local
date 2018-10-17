/***************************************************************************************
 *      Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *      FileName        :       devicedata.cpp
 *      Description     :       设备数据相关操作
 *      Modification    :
 *      Version         :       V1.0.0
 *      Time            :       2009-10,11
 *      Author          :       wanggp@hikvision.com
 ****************************************************************************************/
#include "devicedata.h"

/************************************************************************
 *        Function            :  DeviceData
 *        Description         :  构造函数
 *        Input               :  无
 *        Output              :  无
 *        Return              :  无
*************************************************************************/
DeviceData::DeviceData()
{
    m_irealplaying =0;
    m_iuserid = -1;
    memset(&m_deviceinfo,0,sizeof(NET_DVR_DEVICEINFO_V30));
    m_qdevicename = "";
    m_qip = "";
    m_qiport = 0;
    m_qusername = "";
    m_qpassword = "";
	m_ideployed = -1;
	m_multiCast = "234.5.6.7";
}

/************************************************************************
 *        Function            :  ~DeviceData
 *        Description         :  析构函数
 *        Input               :  无
 *        Output              :  无
 *        Return              :  无
*************************************************************************/
DeviceData::~DeviceData()
{

}

/************************************************************************
 *        Function            :  setRealPlayLabel
 *        Description         :  设置正在用预览标记
 *        Input               :  value
 *        Output              :  无
 *        Return              :  无
*************************************************************************/
void DeviceData::setRealPlayLabel(int value)
{
    m_irealplaying =value;
}

/************************************************************************
 *        Function            :  getRealPlayLabel
 *        Description         :  获取是否有通道正在预览标记位值
 *        Input               :  无
 *        Output              :  无
 *        Return              :  m_irealplaying
*************************************************************************/
int DeviceData::getRealPlayLabel()
{
    return m_irealplaying;   
}

/************************************************************************
 *        Function            :  setUsrID
 *        Description         :  设置用户ID
 *        Input               :  id
 *        Output              :  无
 *        Return              :  无
*************************************************************************/
void DeviceData::setUsrID(int id)
{
    m_iuserid = id;
}

/************************************************************************
 *        Function            :  getUsrID
 *        Description         :  获取用户ID
 *        Input               :  无
 *        Output              :  无
 *        Return              :  无
*************************************************************************/
int DeviceData::getUsrID()
{
    return m_iuserid;
}

/************************************************************************
 *        Function            :  setDeviceName
 *        Description         :  设置设备名称
 *        Input               :  devicename
 *        Output              :  无
 *        Return              :  无
*************************************************************************/
void DeviceData::setDeviceName(QString devicename)
{
    m_qdevicename = devicename;
}

/************************************************************************
 *        Function            :  getDeviceName
 *        Description         :  获取设备名称
 *        Input               :  无
 *        Output              :  无
 *        Return              :  m_qdevicename
*************************************************************************/
QString DeviceData::getDeviceName()
{
    return m_qdevicename;
}

/************************************************************************
 *        Function            :  setIP
 *        Description         :  设置IP
 *        Input               :  ip
 *        Output              :  无
 *        Return              :  无
*************************************************************************/
void DeviceData::setIP(QString  ip)
{
    m_qip = ip;
}

/************************************************************************
 *        Function            :  getIP
 *        Description         :  获取IP
 *        Input               :  无
 *        Output              :  无
 *        Return              :  m_qip
*************************************************************************/
QString DeviceData::getIP()
{
    return m_qip;
}

/************************************************************************
 *        Function            :  setPort
 *        Description         :  设置端口值
 *        Input               :  port
 *        Output              :  无
 *        Return              :  无
*************************************************************************/
void DeviceData::setPort(int port)
{
    m_qiport = port;
}

/************************************************************************
 *        Function            :  getPort
 *        Description         :  获取端口值
 *        Input               :  无
 *        Output              :  无
 *        Return              :  m_qiport
*************************************************************************/
int DeviceData::getPort()
{
    return m_qiport;
}

/************************************************************************
 *        Function            :  setUsrName
 *        Description         :  设置用户名
 *        Input               :  usrname
 *        Output              :  无
 *        Return              :  无
*************************************************************************/
void DeviceData::setUsrName(QString usrname)
{
    m_qusername = usrname;
}

QString DeviceData::getUsrName()
{
    return m_qusername;
}

/************************************************************************
 *        Function            :  setPasswd
 *        Description         :  设置密码
 *        Input               :  passwd
 *        Output              :  无
 *        Return              :  无
*************************************************************************/
void DeviceData::setPasswd(QString passwd)
{
    m_qpassword = passwd;
}

/************************************************************************
 *        Function            :  getPasswd
 *        Description         :  获取密码
 *        Input               :  无
 *        Output              :  无
 *        Return              :  字符串型密码
*************************************************************************/
QString DeviceData::getPasswd()
{
    return m_qpassword;
}

/************************************************************************
 *        Function            :  modifyChannelDataChild
 *        Description         :  修改通道内容
 *        Input               :  channel
 *        Output              :  无
 *        Return              :  0成功-1失败
*************************************************************************/
int DeviceData::modifyChannelDataChild(ChannelData *channel)
{
    for ( QList<ChannelData >::Iterator it = m_qlistchanneldata.begin(); it != 
    m_qlistchanneldata.end(); ++it )
    {
        if ((*it).m_ichannelnum == channel->m_ichannelnum)
        {
            (*it).m_qchannelname = channel->m_qchannelname;
            (*it).m_eprotocoltype = channel->m_eprotocoltype;
            (*it).m_estreamtype = channel->m_estreamtype;
            return 0;
        }
    }
    return -1;
}

NET_DVR_DEVICEINFO_V30 DeviceData::getDeviceInfo()
{

    return m_deviceinfo;
}

void DeviceData::setDeployState(int deployed)
{
	m_ideployed = deployed;
}

int  DeviceData::getDeployState()
{
	return m_ideployed;
}

/************************************************************************
*        Function            :  setMultiCast
*        Description         :  设置多播地址
*        Input               :  passwd
*        Output              :  无
*        Return              :  无
*************************************************************************/
void DeviceData::setMultiCast(QString multiCast)
{
	m_multiCast = multiCast;
}

/************************************************************************
*        Function            :  getMultiCast
*        Description         :  获取多播地址
*        Input               :  无
*        Output              :  无
*        Return              :  多播地址
*************************************************************************/
QString DeviceData::getMultiCast()
{
	return m_multiCast;
}



