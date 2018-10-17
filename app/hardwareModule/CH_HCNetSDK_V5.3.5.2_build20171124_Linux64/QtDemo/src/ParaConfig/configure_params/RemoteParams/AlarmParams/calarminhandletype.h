/*
 * Copyright(C) 2010,Hikvision Digital Technology Co., Ltd 
 * 
 * 文件名称：calarminhandletype.h
 * 描    述：
 * 当前版本：1.0
 * 作    者：潘亚东
 * 创建日期：2010年1月15日
 * 修改记录：
 */

#ifndef CALARMINHANDLETYPE_H
#define CALARMINHANDLETYPE_H

#include <QtGui/QDialog>
#include "ui_calarminhandletype.h"
#include "DemoPublic.h"
#include <QtGui/QLineEdit>

class CAlarmInHandleType : public QDialog
{
    Q_OBJECT

public:
    CAlarmInHandleType(QWidget *parent = 0);
    ~CAlarmInHandleType();

    int setUserID(long lUserID);
    int setAlarmInInfo(LPNET_DVR_ALARMINCFG_V30 pAlarmInInfo);

    //设置数据到界面
    int getAlarmInHandleInfo();
    //设置数据到设备上
    int setAlarmInHandleInfo();

public slots:
    void showEvent(QShowEvent * event);
    //星期被改变
    void comboWeekDayChanged(int iWeekDay);
    void on_checkbo08_clicked();

    //保存参数
    void on_btnSave_clicked();


private:
    Ui::CAlarmInHandleTypeClass ui;
    long m_lUserID;

    LPNET_DVR_ALARMINCFG_V30 m_pAlarmInInfo;
    //触发的录像通道, 数组编号代表：通道ID - 起始通道号
    QCheckBox *m_pCheckChannel[MAX_CHANNUM_V30];
    //触发的报警输出
    QCheckBox *m_pcheckAlarmOut[MAX_ALARMOUT_V30];

    QLineEdit m_editStartHour[MAX_TIMESEGMENT_V30]; //布防开始时间(小时)
    QLineEdit m_editStartMin[MAX_TIMESEGMENT_V30];   //布防开始时间(分钟)
    QLineEdit m_editEndHour[MAX_TIMESEGMENT_V30];  //布防结束时间(小时)
    QLineEdit m_editEndMin[MAX_TIMESEGMENT_V30];    //布防结束时间(分钟)

    int setAlarmTypeWidget();
    int getAlarmTypeWidget();//获取报警处理方式
};

#endif // CALARMINHANDLETYPE_H
