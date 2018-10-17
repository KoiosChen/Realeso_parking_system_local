/*
 * Copyright(C) 2010,Hikvision Digital Technology Co., Ltd 
 * 
 * 文件名称：calarminptz.h
 * 描    述：
 * 当前版本：1.0
 * 作    者：潘亚东
 * 创建日期：2010年1月15日
 * 修改记录：
 */

#ifndef CALARMINPTZ_H
#define CALARMINPTZ_H

#include <QtGui/QDialog>
#include "ui_calarminptz.h"
#include "DemoPublic.h"

class CAlarmInPTZ : public QDialog
{
    Q_OBJECT

public:
    CAlarmInPTZ(QWidget *parent = 0);
    ~CAlarmInPTZ();

    int setUserID(long lUserID);
    int setAlarmInInfo(LPNET_DVR_ALARMINCFG_V30 pAlarmInInfo);

    //设置数据到界面
    int getAlarmInPTZInfo();
    //设置数据到设备上
    int setAlarmInPTZInfo();

public slots:
    void changeChanIndex(int index);
    void showEvent(QShowEvent * event);

    void on_rBtnPreset_clicked();
    void on_rBtnCruise_clicked();
    void on_rBtnPtzTrack_clicked();

    void on_btnSave_clicked();
    void on_btnExit_clicked();

private:
    Ui::CAlarmInPTZClass ui;
    long m_lUserID;

    LPNET_DVR_ALARMINCFG_V30 m_pAlarmInInfo;
};

#endif // CALARMINPTZ_H
