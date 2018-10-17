 /***************************************************************************************
 *      Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *      FileName        :       channeldata.h
 *      Description     :       通道数据信息
 *      Modification    :       存储通道相关的有效数据，用于存取文件和界面显示，实时修改通道信息
 *      Version         :       V1.0.0
 *      Time            :       2009-10,11
 *      Author          :       panyd@hikvision.com  wanggp@hikvision.com
 *****************************************************************************************/
#ifndef CHANNELDATA_H_
#define CHANNELDATA_H_


#include <QString>
#include "devicedata.h"

enum PROTOCOL{ TCP = 0, UDP, MCAST, RTP, RTP_RTSP};
enum STREAMTYPE{ MAINSTREAM = 0, SUBSTREAM};


class ChannelData
{

public:
    friend class DeviceData;
    friend class QtClientDemo;
	friend class RealPlay;
    friend class PlayBack;
    ChannelData();
    ~ChannelData();

    void setChannelName(QString name);
    QString getChannelName();

    void setChannelNum(int num);
    int getChannelNum();

    void setProtocolType(PROTOCOL type);
    PROTOCOL getProtocolType();
    QString getProtocolTypeQstring();

    void setStreamType(STREAMTYPE type);
    STREAMTYPE getStreamType();
    QString getStreamTypeQstring();

    void setLinkMode();
    int getLinkMode();

	void setRealhandle(int num);
	int getRealhandle();


private:
    //通道名称
    QString m_qchannelname; 
    //通道号
    int m_ichannelnum;  
    //传输协议，有TCP,UDP，多播，和RTP等
    PROTOCOL m_eprotocoltype;
    //码流类型，分主码流和子码流
    STREAMTYPE m_estreamtype;  
    int m_ilinkmode;
	int m_irealhandle;
};
#endif /* CHANNELDATA_H_ */

