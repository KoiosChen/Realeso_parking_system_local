/*
 * Copyright(C) 2010,Hikvision Digital Technology Co., Ltd 
 * 
 * 文件名称：cconfigureparams.h
 * 描    述：
 * 当前版本：1.0
 * 作    者：潘亚东
 * 创建日期：2010年2月1日
 * 修改记录：
 */


#ifndef CCONFIGUREPARAMS_H
#define CCONFIGUREPARAMS_H

#include <QtGui/QWidget>
#include <QMessageBox>
#include "cremoteparams.h"
#include "cipparams.h"
#include "charddiskparams.h"
#include "DemoPublic.h"


#ifndef HPR_OK
#define HPR_OK 0
#endif

#ifndef HPR_ERROR
#define HPR_ERROR 1
#endif

namespace Ui
{
    class CConfigureParams;
}

class CConfigureParams : public QWidget
{
    Q_OBJECT

public:
    CConfigureParams(QWidget *parent = 0);
    ~CConfigureParams();

    //event
    void showEvent ( QShowEvent * event );

public slots:
    void loadRemoteParams();          //远程参数
    void on_btnIPParams_clicked();             //IP接入配置
    void on_btnHardDiskParams_clicked(); //硬盘配置
    void GetDVRUserID();
    void SetDVRUserID(long lUserID);

    void showLoginBtn();

private:
    Ui::CConfigureParams *ui;
    QMessageBox m_MessageBox;

    QWidget *m_pCurrentWidget;
    CRemoteParams *m_pRemoteparams;
    CIPParams *m_pIPParams;
    CHardDiskParams *m_pHardDiskParams;

    long m_lUserID;  //user ID
};

#endif // CCONFIGUREPARAMS_H
