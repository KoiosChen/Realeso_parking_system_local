/************************************************************************
 *        Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *        FileName                      :   timing.h
 *        Description                   :   timing  device
 *        Modification History          :   none
 *        Version                       :   V1.0
 *        Time                          :   2009-11,12
 *        Author                        :   wanggongpu
 *        Descrp                        :   
*************************************************************************/
#ifndef TIMING_H
#define TIMING_H

#include <QtGui/QWidget>
#include "ui_timing.h"
#include <QList>
#include <QString>
#include "devicedata.h"
#include "channeldata.h"
#include "DemoPublic.h"
#include <QDialog>
class Timing : public QDialog
{
    Q_OBJECT
public:
    Timing(int channelnum, QList<DeviceData> * tree, QDialog *parent = 0);
    ~Timing();

private slots:
    void on_pushButton_timing_clicked();
    void on_pushButton_exit_clicked();
    void timingDevice(QTreeWidgetItem *item, int column);
    void on_dateTimeEdit_dateTimeChanged ( const QDateTime & datetime );

private:
    Ui::TimingClass ui;
    int m_timingchannelnum;
    QList<DeviceData> *m_qlistdevicetree;
    int m_timinguserid[256];
    int m_index;
    NET_DVR_TIME m_timingtime;
    
};

#endif // TIMING_H

