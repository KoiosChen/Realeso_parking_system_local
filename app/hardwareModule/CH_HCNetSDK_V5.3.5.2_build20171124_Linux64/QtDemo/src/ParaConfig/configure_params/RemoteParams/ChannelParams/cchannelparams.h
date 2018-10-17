#ifndef CCHANNELPARAMS_H
#define CCHANNELPARAMS_H

#include <QtGui/QWidget>
#include "ui_cchannelparams.h"
#include "DemoPublic.h"

#define KIT_SHOWSTRING_LEN 44
//using namespace Ui;

class CChannelParams : public QWidget
{
    Q_OBJECT

public:
    CChannelParams(QWidget *parent = 0);
    ~CChannelParams();

    //event
    void showEvent ( QShowEvent * event );

    int setUserID(long lUserID);
    int getChannelNum();   //get the channel and IP channel. (Used by myself.)

    //get all information
    int getAllInfo();
    int setAllInfo();

    int getCompressInfo(int iCompIndex); //Get compress parameters.
    int setCompressInfo(); //Set compress parameters.

    int getRecordInfo(); //Get record parameter.
    int setRecordInfo(); //Set record parameter.

    int getPictureInfo();  //获取图像参数
    int setPictureInfo();  //设置图像参数

    int getShowStringInfo();//获取字符叠加参数
    int setShowStringInfo();//设置字符叠加参数

public slots:
    //(combox changed)
    void comboChNoChanged ( int iChannelIndex );
    void comboCompChanged(int iCompIndex);
    void comboVideoBitChanged(int iVideoBitIndex);
    void comboStringNoChanged(int iStringNo);

    //(btn)
    void clickBtnRemoteRecord();  //配置录像时间计划.
    void clickBtnVILost();              //视频信号丢失报警参数设置
    void clickBtnShelter();               //遮挡区域
    void on_btnMotion_clicked();   //移动侦测
    void on_btnHide_clicked();       //遮挡报警

    //(click check)是否定时录像
    void clickCheckVideoRecord();
    //(click check)是否在屏幕上显示通道名称
    void clickCheckShowChanName();
    //(click check)是否设置信号丢失报警参数
    void clickCheckVILost();
    //(click check)是否启动遮挡
    void clickCheckShelter();
    //(click check)是否设置移动侦测
    void clickCheckMotion();
    //(click check)是否设置遮挡
    void clickCheckHide();
    //(click check)是否显示OSD
    void clickCheckOSD();
    //是否显示字符叠加
    void clickCheckShowString();

private:
    Ui::CChannelParamsClass ui;
    long m_lUserID;
    long m_lChannel;
    int m_iArryChannel[MAX_CHANNUM_V30];//the index is  combox index, the value is channel ID
    int m_iStartChan;            // start channel NO.
    int m_iSimChannelNum;  //实际可用模拟通道个数

    NET_DVR_RECORD_V30 m_struRecord;//录像参数定时录像的时间段设定
    BOOL m_bSetRecord; //表示录像参数中“设置”按钮有没有被点过；不选"定时录像"时，该值被设置为假

    NET_DVR_VILOST_V30 m_struVILost; //图像参数->信号丢失报警
    BOOL m_bVILost;                                   //信号丢失参数中“设置”按钮中有没有被点过

    NET_DVR_SHELTER  m_struShelter[MAX_SHELTERNUM]; //遮挡区域
    BOOL m_bShelter;  //遮挡区域按钮是否被点击过

    NET_DVR_MOTION_V30  m_struMotion; //移动侦测
    BOOL m_bMotion; //移动侦测按钮是否被点过

    NET_DVR_HIDEALARM_V30   m_struHide; //遮挡报警
    BOOL m_bHide; //遮挡报警的按钮是否被点过

    //显示字符

    //set the value of each compress widget.压缩参数
    int setEachCompWidget(LPNET_DVR_COMPRESSION_INFO_V30 pCompressInfo);
    //get the value of each compress widget.
    int getEachCompWidget(LPNET_DVR_COMPRESSION_INFO_V30 pCompressInfo);
};

#endif // CCHANNELPARAMS_H
