#ifndef CHIDEALARM_H
#define CHIDEALARM_H

#include <QtGui/QDialog>
#include <QtGui/QCheckBox>
#include <QtGui/QLineEdit>
#include "ui_chidealarm.h"
#include "DemoPublic.h"

class CHideAlarm : public QDialog
{
    Q_OBJECT

public:
    CHideAlarm(QWidget *parent = 0);
    ~CHideAlarm();

    //设置信号丢失报警参数
    int setStruHideAlarm(NET_DVR_HIDEALARM_V30 *pstruHideAlarm);
    //设置报警输出个数
    int setAlarmOutNum(int iAlarmOutNum);
    //设置用户ID
    int setUserID(long lUserID);

public slots:
    //保存参数
    void on_btnSave_clicked();
    //星期被改变
    void comboWeekDayChanged(int iWeekDay);
    //是否触发报警输出
    void checkAlarmOut();

private:
    Ui::CHideAlarmClass ui;

    long m_lUserID;                    //登陆时获取的ID值
    int m_iAlarmOutNum;            //报警输出个数

    NET_DVR_HIDEALARM_V30 *m_pstruHideAlarm; //遮挡报警参数

    QLineEdit m_editStartHour[MAX_TIMESEGMENT_V30]; //布防开始时间(小时)
    QLineEdit m_editStartMin[MAX_TIMESEGMENT_V30];   //布防开始时间(分钟)
    QLineEdit m_editEndHour[MAX_TIMESEGMENT_V30];  //布防结束时间(小时)
    QLineEdit m_editEndMin[MAX_TIMESEGMENT_V30];    //布防结束时间(分钟)
    QCheckBox *m_pcheckAlarmOut[MAX_ALARMOUT_V30]; //触发的报警输出

    //event
    void showEvent ( QShowEvent * event );
    //在table中产生iAlarmOutNum个报警输出checkbo
    int createAlarmOutCheck();
    int getAlarmOutInfo(int iAlarmOut, QCheckBox *pQCheckAlarmOut);

    //设置每个控件的值
    int setEachWidgetValue();
    //获取每个控件的值
    int getEachWidgetValue();
};

#endif // CHIDEALARM_H
