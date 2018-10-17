#ifndef CREMOTEPARAMS_H
#define CREMOTEPARAMS_H

#include <QtGui/QWidget>
#include <QLineEdit>

#ifndef HPR_OK
#define HPR_OK 0
#endif

#ifndef HPR_ERROR
#define HPR_ERROR -1
#endif

#ifndef INT_LEN
#define INT_LEN 10  //the length of the string of 32 bit int
#endif

#include "cadvancednetparams.h"
#include "ui_cremoteparams.h"
#include "cnfsparams.h"
#include "czonedst.h"
#include "crtspparams.h"
#include "ccompressaudio.h"
#include "cchannelparams.h"
#include "cuserparams.h"
#include "catmparams.h"

#include "exceptionparams.h"
#include "cserialparams.h"
#include "calarmparamsdlg.h"
#include "devicedata.h"
class CRemoteParams : public QWidget {
    Q_OBJECT
public:
    CRemoteParams(QWidget *parent = 0);
    ~CRemoteParams();

    //kit function
    int editToInt(const QLineEdit *pEdit, int *pNum);
    int intToEdit(QLineEdit *pEdit, int iNum);
    int checkIP(char * ip);    //check ip
    int editToStr(const QLineEdit *pEdit, BYTE *pStr, int iLen);
    int editToIP(const QLineEdit *pEditIP, BYTE *pIP, int iLen);

    //***SDK function***//
    int setUserID(long lUserID);

    int getDeviceInfo();//get device params
    int setDeviceInfo();//change device params

    int getNetInfo();//Get net params;
    int setNetInfo();//Set net params;

    //视频缩放
    int getScaleInfo();
    int setScaleInfo();

public slots:
    void refreshParams(); //Update all params.
    void saveParams();     //Save all params.

    //combox
    void clickDHCP();      //CheckboDHCP is clicked.
    //button
    void clickAdvancedNet();  //The btnAdvancednet is clicked.
    void clickNFS();        //NFS
    void clickZoneDst();  // zone and dfs
    void clickRtsp();       //rtsp
    void clickCompressAudio(); //audio

    //click table pages
    void clickTabPage(int iTabIndex);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::CRemoteParamsClass *m_ui;
    long m_lUserID;
    long m_lChannel;

    //Net Parameters
    CAdvancedNetParams *m_pAdvancedNet;
    CNfsParams *m_pNfs;     //net file system
    CZoneDst *m_pZoneDst; //zone time
    CRtspParams *m_pRtsp; //Rtsp
    CCompressAudio *m_pCompressAudio; // Audio

    //Channel Parameters
    int m_iTabIndex;   //the index of pages
    CChannelParams *m_pChannelParams; // the pointer of ChannelParams table page
    CUserParams *m_pUserParams;           //用户参数窗口指针

    ExceptionParams *m_pExceptionParams;
    CSerialParams *m_pSerialParams;
    CAlarmParamsDlg *m_pAlarmParams;
    CATMParams *m_pATMParams;

    //event
    void showEvent ( QShowEvent * event );
};

#endif // CREMOTEPARAMS_H
