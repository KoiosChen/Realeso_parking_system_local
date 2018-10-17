/***************************************************************************************
 *      Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *      FileName        :       channeldata.cpp
 *      Description     :       通道数据相关操作
 *      Modification    :       none
 *      Version         :       V1.0.0
 *      Time            :       2009-10,11
 *      Author          :       wanggp@hikvision.com
 *****************************************************************************************/
#include "channeldata.h"

/************************************************************************
 *        Function            :  ChannelData
 *        Description         :  构造函数
 *        Input               :  无
 *        Output              :  无
 *        Return              :  无
*************************************************************************/
ChannelData::ChannelData()
{
    m_qchannelname = "";
    m_ichannelnum = -1;
	m_irealhandle = -1;
    m_eprotocoltype = TCP;
    m_estreamtype = MAINSTREAM;
}

/************************************************************************
 *        Function            :  ~ChannelData
 *        Description         :  析构函数
 *        Input               :  无
 *        Output              :  无
 *        Return              :  无
*************************************************************************/
ChannelData::~ChannelData()
{

}

/************************************************************************
 *        Function            :  setChannelName
 *        Description         :  设置通道名称
 *        Input               :  name
 *        Output              :  无
 *        Return              :  无
*************************************************************************/
void ChannelData::setChannelName(QString name)
{
    m_qchannelname = name;
}

/************************************************************************
 *        Function            :  getChannelName
 *        Description         :  获取通道名称
 *        Input               :  无
 *        Output              :  无
 *        Return              :  通道名称
*************************************************************************/
QString ChannelData::getChannelName()
{
    return m_qchannelname;
}

/************************************************************************
 *        Function            :  setChannelNum
 *        Description         :  设置通道号
 *        Input               :  num
 *        Output              :  无
 *        Return              :  无
*************************************************************************/
void ChannelData::setChannelNum(int num)
{
    m_ichannelnum = num;
}

/************************************************************************
 *        Function            :  getChannelNum
 *        Description         :  获取通道号
 *        Input               :  无
 *        Output              :  无
 *        Return              :  m_iChannelNum
*************************************************************************/
int ChannelData::getChannelNum()
{
    return m_ichannelnum;
}

/************************************************************************
 *        Function            :  setProtocolType
 *        Description         :  设置协议类型
 *        Input               :  type
 *        Output              :  无
 *        Return              :  无
*************************************************************************/
void ChannelData::setProtocolType(PROTOCOL type)
{
    m_eprotocoltype = type;
}

/************************************************************************
 *        Function            :  getProtocolType
 *        Description         :  获取协议类型
 *        Input               :  无
 *        Output              :  无
 *        Return              :  协议类型
*************************************************************************/
PROTOCOL ChannelData::getProtocolType()
{
    return m_eprotocoltype;
}

/************************************************************************
 *        Function            :  getProtocolTypeQstring
 *        Description         :  获取协议类型
 *        Input               :  无
 *        Output              :  无
 *        Return              :  字符型协议类型
*************************************************************************/
QString ChannelData::getProtocolTypeQstring()
{
    QString protocol;
    switch (m_eprotocoltype)
    {
    case TCP:
        protocol="TCP";
        break;
    case UDP:
        protocol="UDP";
        break;
    case MCAST :
        protocol="MCAST";
        break;
    case RTP:
        protocol="RTP";
        break;
    case RTP_RTSP:
        protocol = "RTP/RTSP";
    default :
        protocol = "error";
        break;
    }
    return protocol;
}

/************************************************************************
 *        Function            :  setStreamType
 *        Description         :  设置协议类型
 *        Input               :  协议类型
 *        Output              :  无
 *        Return              :  无
*************************************************************************/
void ChannelData::setStreamType(STREAMTYPE type)
{
    m_estreamtype = type;
}

/************************************************************************
 *        Function            :  getStreamType
 *        Description         :  获取主码流还是子码流类型
 *        Input               :  无
 *        Output              :  无
 *        Return              :  码流类型
*************************************************************************/
STREAMTYPE ChannelData::getStreamType()
{
    return m_estreamtype;
}

/************************************************************************
 *        Function            :  getStreamTypeQstring
 *        Description         :  设置主码流还是子码流类型
 *        Input               :  无
 *        Output              :  无
 *        Return              :  字符型码流类型
*************************************************************************/
QString ChannelData::getStreamTypeQstring()
{
    QString type;

    switch (m_estreamtype)
    {
    case MAINSTREAM:
        type="MAINSTREAM";
        break;
    case SUBSTREAM:
        type="SUBSTREAM";
        break;
    default :
        type = "error";
        break;
    }
    return type;
}


void ChannelData::setLinkMode()
{
    switch (m_estreamtype)
    {
    case MAINSTREAM:
            switch (m_eprotocoltype)
            {
            case TCP:
                m_ilinkmode =0x0;
                break;
            case UDP:
                m_ilinkmode=0x1;
                break;
            case MCAST :
                m_ilinkmode=0x2;
                break;
            case RTP:
                m_ilinkmode=0x3;
                break;
            case RTP_RTSP:
                m_ilinkmode=0x4;
                break;
            default :
                m_ilinkmode=0x0;
                break;
            }
            break;
    case SUBSTREAM:
            switch (m_eprotocoltype)
            {
            case TCP:
                m_ilinkmode =0x80000000;
                break;
            case UDP:
                m_ilinkmode=0x80000001;
                break;
            case MCAST :
                m_ilinkmode=0x80000002;
                break;
            case RTP:
                m_ilinkmode=0x80000003;
                break;
            case RTP_RTSP:
                m_ilinkmode=0x80000004;
                break;
            default :
                m_ilinkmode=0x80000000;
                break;
            }
            break;
        break;
    default :
        break;
    }
}

int ChannelData::getLinkMode()
{
    setLinkMode();
    return m_ilinkmode;
}


void ChannelData::setRealhandle(int num)
{
    m_irealhandle = num;
}

int ChannelData::getRealhandle()
{
    return m_irealhandle;
}