/************************************************************************
 *        Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *        FileName                      :   serialtransfer.h
 *        Description                   :   serial transfer
 *        Modification History          :   none
 *        Version                       :   V1.0
 *        Time                          :   2009-11,12
 *        Author                        :   wanggongpu
 *        Descrp                        :   
*************************************************************************/
#ifndef SERIALTRANSFER_H
#define SERIALTRANSFER_H

#include <QtGui/QWidget>
#include "ui_serialtransfer.h"
#include "DemoPublic.h"

#include <QList>
#include "devicedata.h"
#include <QDialog>

#ifdef _WIN32

#elif  defined(__linux__)

#ifndef __stdcall
#define __stdcall
#endif

#endif

class SerialTransfer : public QDialog
{
    Q_OBJECT

public:
	friend void __stdcall SerialDataCallBack( int lSerialHandle, 
                         char *pRecvDataBuffer,
                         unsigned int dwBufSize,
                         unsigned int dwUser );
    SerialTransfer(QList<DeviceData> *devicetree, int *userid, QDialog *parent = 0);
    ~SerialTransfer();

private slots:
    void on_comboBox_channeltype_currentIndexChanged ( int index);
    void on_comboBox_channelnum_currentIndexChanged ( int index);
    void on_comboBox_sendinterface_currentIndexChanged ( int index);
    
    void on_pushButton_send_clicked();
    void on_pushButton_startserial_clicked();
    void on_pushButton_stopserial_clicked();
    void on_pushButton_exit_clicked();

    void on_lineEdit_textChanged ( const QString & text );
    

    
public:
    Ui::SerialTransferClass ui;
    QList<DeviceData> * qtreelist;
    int *m_stuserid;
    int m_stserialtype;
    int m_st485channelnum;
    int m_stinterfaceindex;
    int m_sthandle;

    QString m_stsenddata;
};

#endif // SERIALTRANSFER_H

