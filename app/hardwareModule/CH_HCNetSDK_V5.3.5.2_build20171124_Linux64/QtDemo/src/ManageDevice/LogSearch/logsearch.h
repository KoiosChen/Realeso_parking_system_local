/************************************************************************
 *        Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *        FileName                      :   logsearch.h
 *        Description                   :   log search.
 *        Modification History          :   none
 *        Version                       :   V1.0
 *        Time                          :   2009-11,12
 *        Author                        :   wanggongpu
 *        Descrp                        :
*************************************************************************/
#ifndef LOGSEARCH_H
#define LOGSEARCH_H

#include <QtGui/QWidget>
#include "ui_logsearch.h"
#include "DemoPublic.h"
#include "devicedata.h"
#include <QDialog>

class LogSearch : public QDialog
{
    Q_OBJECT

public:
    LogSearch(QList<DeviceData> *devicetree, int* userid, QDialog *parent = 0);
    ~LogSearch();
    void printTable(NET_DVR_LOG_V30*    LogData);

private slots:
    void on_comboBox_logtype_currentIndexChanged ( int index);
    void on_comboBox_maintype_currentIndexChanged ( int index);
    void on_comboBox_subtype_currentIndexChanged ( int index);

    void on_dateTimeEdit_start_dateTimeChanged ( const QDateTime & datetime );
    void on_dateTimeEdit_stop_dateTimeChanged ( const QDateTime & datetime );

    void on_pushButton_searchlog_clicked();
    void on_pushButton_exit_clicked();

private:
    Ui::LogSearchClass ui;
    int *m_lsuserid;
    QList<DeviceData> *m_lsdevicetree;

    int m_lsmode;
    int m_lsmaintype;
    int m_lssubtype;

    NET_DVR_TIME m_lsstarttime;
    NET_DVR_TIME m_lsstoptime;

};

#endif // LOGSEARCH_H

