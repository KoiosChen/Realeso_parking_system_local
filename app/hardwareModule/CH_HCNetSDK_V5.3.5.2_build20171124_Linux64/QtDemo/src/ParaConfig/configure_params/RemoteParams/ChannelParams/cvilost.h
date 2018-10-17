#ifndef CVILOST_H
#define CVILOST_H

#include <QtGui/QDialog>
#include <QtGui/QLineEdit>
#include <QtGui/QCheckBox>
#include "DemoPublic.h"
#include "ui_cvilost.h"

using namespace Ui;

class CVILost : public QDialog
{
    Q_OBJECT

public:
    CVILost(QDialog *parent = 0);
    ~CVILost();

    //设置信号丢失报警参数
    int setStruVILost(NET_DVR_VILOST_V30 *pstruVILost);
    //设置报警输出个数
    int setAlarmOutNum(int iAlarmOutNum);
    //设置用户ID
    int setUserID(long lUserID);

    //event
    void showEvent ( QShowEvent * event );

public slots:
    //保存参数
    void on_btnSave_clicked();
    //星期被改变
    void comboWeekDayChanged(int iWeekDay);
    //是否触发报警输出
    void checkAlarmOut();

private:
    Ui::CVILostClass ui;
    long m_lUserID;   //登陆时获取的ID值
    NET_DVR_VILOST_V30 *m_pstruVILost; //视频信号丢失报警参数指针
    int m_iAlarmOutNum;                                   //报警输出个数
    QLineEdit m_editStartHour[MAX_TIMESEGMENT_V30]; //布防开始时间(小时)
    QLineEdit m_editStartMin[MAX_TIMESEGMENT_V30];   //布防开始时间(分钟)
    QLineEdit m_editEndHour[MAX_TIMESEGMENT_V30];  //布防结束时间(小时)
    QLineEdit m_editEndMin[MAX_TIMESEGMENT_V30];    //布防结束时间(分钟)

    QCheckBox *m_pcheckAlarmOut[MAX_ALARMOUT_V30]; //触发的报警输出

    //在table中产生iAlarmOutNum个报警输出checkbo
    int createAlarmOutCheck();
    int getAlarmOutInfo(int iAlarmOut, QCheckBox *pQCheckAlarmOut);

    //设置每个控件的值
    int setEachWidgetValue();
    //获取每个控件的值
    int getEachWidgetValue();
};

#endif // CVILOST_H
