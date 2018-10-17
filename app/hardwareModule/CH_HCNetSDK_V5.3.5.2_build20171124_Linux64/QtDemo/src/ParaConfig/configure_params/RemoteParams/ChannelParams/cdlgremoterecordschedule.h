#ifndef CDLGREMOTERECORDSCHEDULE_H
#define CDLGREMOTERECORDSCHEDULE_H

#include <QtGui/QDialog>
#include <QtGui/QLineEdit>
#include <QtGui/QComboBox>
#include "ui_cdlgremoterecordschedule.h"
#include "DemoPublic.h"

/**
 * 这个类中没有操作设备，只是设置时间安排值
 * **/

class CDlgRemoteRecordSchedule : public QDialog
{
    Q_OBJECT

public:
    CDlgRemoteRecordSchedule(QWidget *parent = 0);
    ~CDlgRemoteRecordSchedule();

    //给结构体赋值个地址
    int SetStruRecord(NET_DVR_RECORD_V30 *pstruRecord);

    //event
    void showEvent ( QShowEvent * event );

public slots:
    void on_btnCopyDay_clicked();
    void on_btnSave_clicked();

    int comboWeekDayChanged(int iWeekDay);

    int clickRecordAllDay();


private:
    Ui::CDlgRemoteRecordScheduleClass ui;

    QLineEdit m_editStartHour[MAX_TIMESEGMENT_V30];
    QLineEdit m_editStartMin[MAX_TIMESEGMENT_V30];
    QLineEdit m_editEndHour[MAX_TIMESEGMENT_V30];
    QLineEdit m_editEndMin[MAX_TIMESEGMENT_V30];
    QComboBox m_comboRecordType[MAX_TIMESEGMENT_V30]; //录像类型

    //录像参数信息
    NET_DVR_RECORD_V30 *m_pstruRecord;

    //拷贝当前设置到某一天
    int copyDaySetting(int iWeekNo);
    int setEachWidget(int iWeekDay);

};

#endif // CDLGREMOTERECORDSCHEDULE_H
