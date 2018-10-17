/************************************************************************
 *        Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *        FileName                      :   rebootdevice.h
 *        Description                   :   reboot
 *        Modification History          :   none
 *        Version                       :   V1.0
 *        Time                          :   2009-11,12
 *        Author                        :   wanggongpu
 *        Descrp                        :   
*************************************************************************/
#ifndef REBOOTDEVICE_H
#define REBOOTDEVICE_H

#include <QtGui/QDialog>
#include "ui_rebootdevice.h"
#include "DemoPublic.h"

class RebootDevice : public QDialog
{
    Q_OBJECT

public:
    RebootDevice(int *userid, QWidget *parent = 0);
    ~RebootDevice();
private slots:
    void on_pushButton_sure_clicked();
    void on_pushButton_cancel_clicked();
    
private:
    Ui::RebootDeviceClass ui;
    int *m_rbusrid;
};

#endif // REBOOTDEVICE_H

