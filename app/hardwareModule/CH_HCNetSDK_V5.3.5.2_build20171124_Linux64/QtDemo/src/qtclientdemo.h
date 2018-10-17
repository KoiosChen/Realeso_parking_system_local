/**  @file qtclientdemo.h
 *   @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved. 
 *   @brief 
 *   
 *   @author wanggongpu
 *   @modify by panyadong
 *   @date 2012-5-22
 *   
 *   @note:
 */

#ifndef QTCLIENTDEMO_H
#define QTCLIENTDEMO_H

//qt module header files
#include <QtGui/QMainWindow>
#include "ui_qtclientdemo.h"
#include <QTimer>
#include <QList>
#include <QMenu>
#include <QString>
#include <QFrame>

//mainwindow
#include "addnode.h"
#include "channelattr.h"
#include "deviceattr.h"
#include "channeldata.h"
#include "devicedata.h"
//config
#include "cconfigureparams.h"

//realplay
#include "realplay.h"
//playback
#include "playback.h"
//exit
#include "exitdemo.h"
//management module subfile
#include "timing.h"
#include "remoteupdate.h"
#include "formatdisk.h"
#include "rebootdevice.h"
#include "shutdowndevice.h"
#include "logsearch.h"
#include "devicestate.h"
#include "resetdevice.h"
//other module subfile
#include "deploy.h"
#include "devicepan.h"
#include "importconfig.h"
#include "serialtransfer.h"
#include "audiobroadcast.h"
#include "audiointercom.h"
#include "audiotransfer.h"
//log and alarm module
#include "logtable.h"
#include "alarmtable.h"
//SDK export header file
#include "DemoPublic.h"
#include "common.h"
#include "treemodel.h"

//typedef void(CALLBACK *fExceptionCallBack)(DWORD    dwType,LONG     lUserID,LONG     lHandle,void     *pUser);


//fExceptionCallBack  ExceptionCallBack2;

#if defined(_WIN32)

void  __stdcall ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser);
#else
//void CALLBACK ExceptionCallBack(unsigned int dwType, int lUserID, int lHandle, void *pUser);
#endif


class QtClientDemo : public QMainWindow
{
    Q_OBJECT

public:
	friend class RealPlay;
	friend void __stdcall MessCallBack_V30(int lCommand, NET_DVR_ALARMER *pAlarmer, char *pAlarmInfo, unsigned int dwBufLen, void*pUser);
	friend unsigned int __stdcall MessCallBack(int lCommand, char *sDVRIP, char *pBuf, unsigned int dwBufLen);
	friend void __stdcall SerialDataCallBack( int lSerialHandle, char *pRecvDataBuffer, unsigned int dwBufSize, unsigned int dwUser );
	friend void __stdcall fVoiceDataCallBack(int lVoiceComHandle, char *pRecvDataBuffer, unsigned int dwBufSize, unsigned char byAudioFlag,  void *pUser);



    QtClientDemo(QWidget *parent = 0);
    ~QtClientDemo();

    //for tree content to file and contrary
    QList<DeviceData> getListFromArray(QByteArray & data);
    QString getStringFromList(QList<DeviceData> &data);
    void showDeviceTree(const QString &nodedata);
	void addInterfaceCalledInfoToLog(int isbool, int returnvalue, char* interfacename, char* deviceip, char* errorinfo);
	bool setDeviceMultiCast(int iUserID, QString sMultiCast);

    //Get SDK verison and Set to the label.
    void SetSDKVersionToLabel();

private slots:
    //for show the current date and time
    void updateTime();

    //tree view display
    void OnDoubleClickTree(const QModelIndex &);
    void showTreeContextMenu(const QPoint &);   
	void pressedTreeView(const QModelIndex &);
	void enterview();
	
    void addDeviceSureClicked();
    void addDeviceCancelClicked();
    void loginDevice();
    void logoutDevice();
    void deleteDevice();
    void showDevAttrDial();
    void getJPEGpic();
    void showChannAttrDial();
    void modfDevAttrSureClicked();
    void modfDevAttrCancelClicked();
    void modfChanAttrSureClicked();
    void modfChanAttrCancelClicked();

    //manage module qmenu action slots
    void timingDevice();
    void updateDevice();
    void formatDisk();
    void rebootDevice();
    void shutdownDevice();
    void searchlogOnDevice();
    void deviceState();
    void setDeviceDefaultConfig();

    //other module slots
    void deploymentOrNotDevice();
    void manualSaveFile();
    void serialTransfer();
    void devicePanelSimulation();
    void importExortConfig();
    void auidoIntercom();
    void audioBroadcast();
    void audioTransfer();   
    
    //exit module
    void exitDemoSure();
    void exitDemoCancel();
    
    //top main buttons slots
    void on_Configure_pushButton_clicked();
    void on_Preview_pushButton_clicked();
    void on_PlayBack_pushButton_clicked();
    void on_Manage_pushButton_clicked();    
    void on_Other_pushButton_clicked();
    void on_Exit_pushButton_clicked();


	//log and alarm table
	void on_radioButton_locallog_pressed ();
	void on_radioButton_alarminfo_pressed ();
	
	void on_pushButton_logaramclear_clicked();
	void on_pushButton_logaramsave_clicked();
	void on_pushButton_logaramlistconf_clicked();
	void on_pushButton_logaramtest_clicked();
	

public:
    Ui::QtClientDemoClass ui;
	//QStandardItem * m_gstandarditem;
	TreeModel * m_gmodel;
    //The upper right corner of mainwindow show time and date
    QTimer *m_qtimer;
    QFrame m_gframe;
    //the left side show the device tree
    QList<DeviceData> m_qlistdevicedata;
    AddNode *m_addnewdevice;
    QMenu *m_qtreemenu ;
    int m_iposttreelevel;
    QModelIndex m_qtreemodelindex;
    //QString m_qtreedevicenodename;
    //QString m_qtreechannelname;
    DeviceAttr *m_deviceattr;
    ChannelAttr *m_channelattr;

    //sdk init flag and usrid devinfo and channel num etc. gloabal variaties
    int m_gsdkinit;
    NET_DVR_DEVICEINFO_V30 m_gcurrentdeviceinfo;
    int m_gcurrentuserid;
	int m_guseridbackup;
    int m_gcurrentchannelnum;
	int m_gchannelnumbackup;
    int m_gcurrentchannellinkmode;
 
    //the button on the main window tops
    int m_currentpagenum;
    //parameter config page 0
    int m_iparaconfhasclicked;

    //realplay page 1
    int m_irealplayhasclicked;
    RealPlay *m_realplay;

    //playback page 2
    int m_ipalybackhasclicked;
    PlayBack *m_playback;

    //manage module
    int m_imanagehasclicked;
    QMenu *m_qmenumanage ;
    Timing *m_timing;
    RemoteUpdate *m_update;
    FormatDisk *m_format;
    RebootDevice *m_reboot;
    ShutdownDevice *m_shutdown;
    LogSearch *m_searchlog;
    DeviceState *m_state;
    ResetDevice *m_reset;
        
    //other module 
    int m_iotherhasclicked;
    QMenu *m_qmenuother ;
    Deploy *m_deploy;
    DevicePan *m_devicepan;
    ImportConfig *m_import;
    SerialTransfer *m_serial;
    AudioBroadcast *m_audiobroadcast;
    AudioIntercom *m_audiointercom;
    AudioTransfer *m_audiotransfer;
        
    //exit module
    int m_iexithasclicked;
    ExitDemo *m_exitdemo;
	CConfigureParams m_ConfigureParams;

	//log alarm module
	
	int m_glogoralarm;//0-log 1-alarm
	LogTable*  m_logtable;
	AlarmTable* m_alarmtable;
	int m_loghasallocated;
	int m_alarmhasallocated;
	int m_glistenhandle;
	int m_gblanklabel;
};

#endif // QTCLIENTDEMO_H

