/************************************************************************
 *        Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *        FileName                      :   shutdowndevice.h
 *        Description                   :   shutdown device
 *        Modification History          :   none
 *        Version                       :   V1.0
 *        Time                          :   2009-11,12
 *        Author                        :   wanggongpu
 *        Descrp                        :   
*************************************************************************/
#ifndef SHUTDOWNDEVICE_H
#define SHUTDOWNDEVICE_H

#include <QtGui/QWidget>
#include "ui_shutdowndevice.h"
#include "DemoPublic.h"

#include <QDialog>
class ShutdownDevice : public QDialog
{
    Q_OBJECT

public:
    ShutdownDevice(int* userid, QDialog *parent = 0);
    ~ShutdownDevice();

private slots:
    void on_pushButton_sure_clicked();
    void on_pushButton_cancel_clicked();

private:
    Ui::ShutdownDeviceClass ui;
    int *m_sdusrid;
};

#endif // SHUTDOWNDEVICE_H

