/*
 * Copyright(C) 2010,Custom Co., Ltd 
 *    FileName: realplay.h
 * Description: 实时预览
 *     Version: 1.0
 *      Author: wanggongpu
 * Create Date: 2009-11,12
 * Modification History：
 *    2010-6-25 panyadong 给播放窗口加边框
 */
#ifndef REALPLAY_H
#define REALPLAY_H

#include <QtGui/QWidget>
#include <QModelIndex>
#include "ui_realplay.h"
#include <QFrame>
#include <QMenu>
#include <QList>

#include "channeldata.h"
#include "devicedata.h"
#include "DemoPublic.h"
#include "ptzcruise.h"
#include "ptzpreset.h"
#include "common.h"
#include "logtable.h"
#include "treemodel.h"
#include "realplay.h"
#include "FramePlayWnd.h"

//以后通过参数配置模块对实时录像存储路径进行设置，这里作为默认
#define SAVE_REALDATA_FILEPATH "./REALPLAY_DOWNLOAD_FILE/"

//Max num of windows used to preview.
#define REALPLAY_MAX_NUM 36
  
class RealPlay : public QWidget
{
    Q_OBJECT

public:
    friend class QtClientDemo;
	
    RealPlay(TreeModel**  model, const QModelIndex * index, QList<DeviceData>* tree,
		int* userid, NET_DVR_DEVICEINFO_V30* devinfo, int* channelnum,  
		int* channelmode, LogTable * table, QWidget *parent = 0);
    ~RealPlay();
	void DrawAreaInit();
	
    void startRealPlay(int index);
    void stopRealPlay();
	int  realPlayEncapseInterface(int devicerow, int channelrow, NET_DVR_CLIENTINFO *clientinfo);
	void stopRealPlayEncapseInterface();


	void setVideoEffect();
    void getVideoEffect();
    void ptzControlRealPlay(int command,int flag);

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    //(combox changed)
    void comboWinNumChanged(int iWindowNumIndex);

    //click play windows.
    void OnSingleClickPlayWindow();

    void RealPlayContextResponse(const QPoint &pos);
    void RealPlayFullFrameRep();
    void RealPlayFullScreenRep();
	void RealPlayNormalFrameRep();

    void on_comboBox_windowsnum_currentIndexChanged (int  index);
    void on_comboBox_qualitydemand_currentIndexChanged (int  index);
    void on_comboBox_ptzspeed_currentIndexChanged (int  index);
    void on_comboBox_ptzcruisepoint_currentIndexChanged (int  index);
    void on_comboBox_ptzcruisepath_currentIndexChanged (int  index);
    //play
    void on_pushButton_realplay_clicked();
    void on_pushButton_savefile_clicked();
    void on_pushButton_scrashPic_clicked();
    void on_pushButton_forceIframe_clicked();
    void on_pushButton_up_clicked();
    void on_pushButton_down_clicked();
    void on_pushButton_left_clicked();
    void on_pushButton_right_clicked();
    void on_pushButton_narrowfocus_clicked();
    void on_pushButton_expandfocus_clicked();
    void on_pushButton_nearfocus_clicked();    
    void on_pushButton_farfocus_clicked();
    void on_pushButton_largeaperture_clicked();
    void on_pushButton_smallaperture_clicked();
    void on_pushButton_ptzcruisepointcall_clicked();
    void on_pushButton_ptzcruisepointset_clicked();
    void on_pushButton_ptzcruisepathcall_clicked();
    void on_pushButton_ptzcruisepathset_clicked();
    void on_pushButton_trackmemorystart_clicked();  
    void on_pushButton_trackmemorystop_clicked();
    void on_pushButton_trackmemoryrun_clicked();
    void on_horizontalSlider_bright_valueChanged ( int value);  
    void on_horizontalSlider_contrast_valueChanged ( int value); 
    void on_horizontalSlider_saturation_valueChanged ( int value);
    void on_horizontalSlider_hue_valueChanged ( int value);
    void on_horizontalSlider_volume_valueChanged ( int value);
    void on_pushButton_defaultparameter_clicked();
 
private:
    Ui::RealPlayClass ui;
    
    QFrame *m_pframePlay;    //Window which is used to contain playWnd or is used to play.
    CFramePlayWnd *m_pSelectedPlayWnd;
    CFramePlayWnd *m_framePlayWnd[REALPLAY_MAX_NUM]; //playWnd

	LogTable * m_rplogtable;
    QList<DeviceData>* m_qlistdevicetree;
	int *m_rpuserid;
    NET_DVR_DEVICEINFO_V30* m_rpdeviceinfo;
    int* m_rpchannelnum;
    int *m_channellinkmode;
    //set viedeo quality
    int m_rpcurrentrealhandle;
    
#ifdef __linux__
    //draw area initial
    INITINFO m_rpinitinfo;  
#endif

    QMenu * m_rpmenu;
    //after begin value is 1 while stop is 0
    int m_rpstartstopflag;
    int m_rpsavestopflag;
    //realplay window num equal (index+1)*(index+1)
    int m_rpwindownumindex;
    int m_rpwindownumindexbackup;
    //vedio parameter
    int m_rpbrightvalue;
    int m_rpcontrastvalue;
    int m_rpsaturationvalue;
    int m_rphuevalue;
    int m_rpvolumvalue;
    //PTZ module
    int m_rpptzspeed;
    int m_ptzupswitch;
    int m_ptzdownswitch;
    int m_ptzrghtswitch;
    int m_ptzleftswitch;
    int m_ptznarrowswitch;
    int m_ptzexpandswitch;
    int m_ptzfarswitch;
    int m_ptznearswitch;
    int m_ptzbigswitch;
    int m_ptzsmallswitch;

	int m_ptzpresetpointindex;
	int m_ptzcruisepathindex;
	PtzPreset * m_ptzpreset;
	PtzCruise * m_ptzcruise;
	int m_ptzcruiserunning;
    //when set slider min/max value no messagebox
    int m_rpsliderflag;
	const QModelIndex * m_rpmodelindex;
	//used to close realplay handle point to device
	TreeModel**  m_rpmodel;
	int m_rpuseridbackup;
	int m_rpfirstrealhandle;
	//int m_rperrorflag;
};
#endif // REALPLAY_H

