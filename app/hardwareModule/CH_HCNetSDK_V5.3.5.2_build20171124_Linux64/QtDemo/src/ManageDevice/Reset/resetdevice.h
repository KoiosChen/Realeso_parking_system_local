/************************************************************************
 *        Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *        FileName                      :   resetdevice.h
 *        Description                   :   reset device
 *        Modification History          :   none
 *        Version                       :   V1.0
 *        Time                          :   2009-11,12
 *        Author                        :   wanggongpu
 *        Descrp                        :   
*************************************************************************/
#ifndef RESETDEVICE_H
#define RESETDEVICE_H

#include <QtGui/QWidget>
#include "ui_resetdevice.h"
#include "DemoPublic.h"

#include <QDialog>

class ResetDevice : public QDialog
{
    Q_OBJECT

public:
    ResetDevice(int* userid, QDialog *parent = 0);
    ~ResetDevice();
 
private slots:
    void on_pushButton_sure_clicked();
    void on_pushButton_cancel_clicked();
    
private:
    Ui::ResetDeviceClass ui;
    int *m_rsusrid;
};

#endif // RESETDEVICE_H

