#ifndef CUSERPARAMS_H
#define CUSERPARAMS_H

#include <QtGui/QDialog>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include "ui_cuserparams.h"
#include "DemoPublic.h"

class CUserParams : public QDialog
{
    Q_OBJECT

public:
    CUserParams(QWidget *parent = 0);
    ~CUserParams();

    int setUserID(long lUserID);

    int getAllInfo();
    int setAllInfo();

public slots:
    void userIDChanged(int iUserID);
    void localRightTypeChanged(int iLocalRightType);       //本地通道
    void remoteRightTypeChanged(int iRemoteRightType); //远程通道

private:
    Ui::CUserParamsClass ui;
    long m_lUserID;
    QCheckBox *m_pcheckLocalRight[MAX_RIGHT];    //本地权限
    QCheckBox *m_pcheckRemoteRight[MAX_RIGHT]; //远程权限
    int m_iArryChannel[MAX_CHANNUM_V30];//the index is  combox index, the value is channel ID
    QCheckBox *m_pcheckLocalChannel[MAX_CHANNUM_V30];    //本地通道权限
    QCheckBox *m_pcheckRemoteChannel[MAX_CHANNUM_V30]; //远程通道权限
    QPushButton  *m_ptest;
    int m_iStartChan;            // 开始通道号.
    int m_iSimChannelNum;  //实际可用模拟通道个数
    QLineEdit *m_pEditMAC[MACADDR_LEN]; //物理地址

    //event
    void showEvent ( QShowEvent * event );
    //建立check权限与check数组之间的对应关系
    void connectArrayAndWidget();
    //get the channel and IP channel. (Used by myself.)
    int getChannelNum();
};

#endif // CUSERPARAMS_H
