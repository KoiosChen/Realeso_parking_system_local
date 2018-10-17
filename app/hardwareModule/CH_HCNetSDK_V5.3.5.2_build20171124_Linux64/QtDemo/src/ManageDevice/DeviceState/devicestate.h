/************************************************************************
 *        Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *        FileName                      :  
 *        Description                   :   
 *        Modification History          :   
 *        Version                       :   V1.0
 *        Time                          :   2009-11,12
 *        Author                        :   wanggongpu
 *        Descrp                        :   
*************************************************************************/
#ifndef DEVICESTATE_H
#define DEVICESTATE_H
#include <QDialog>
#include <QList>
#include <QtGui/QWidget>
#include "ui_devicestate.h"
#include "DemoPublic.h"
#include "devicedata.h"

class DeviceState : public QDialog
{
    Q_OBJECT

public:
    DeviceState(QList<DeviceData> *devicetree, int* userid, QDialog *parent = 0);
    ~DeviceState();
    void printDVRState(LPNET_DVR_WORKSTATE pworkstate);
    void insertChannelState( LPNET_DVR_WORKSTATE pworkstate);
    void insertDiskState( LPNET_DVR_WORKSTATE pworkstate);

private slots:
    void on_pushButton_refresh_clicked();
    void on_pushButton_exit_clicked();


private:
    Ui::DeviceStateClass ui;
    int* m_dsuserid;
    QList<DeviceData> *m_dsdevicetree;
    NET_DVR_WORKSTATE m_dsworkstate;
};
#endif // DEVICESTATE_H

