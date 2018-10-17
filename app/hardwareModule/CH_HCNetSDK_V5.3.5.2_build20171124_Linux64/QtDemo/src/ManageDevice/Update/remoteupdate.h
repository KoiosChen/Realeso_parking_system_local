/************************************************************************
 *        Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *        FileName                      :   remoteupdate.h
 *        Description                   :   remote update
 *        Modification History          :   none
 *        Version                       :   V1.0
 *        Time                          :   2009-11,12
 *        Author                        :   wanggongpu
 *        Descrp                        :   
*************************************************************************/
#ifndef REMOTEUPDATE_H
#define REMOTEUPDATE_H

#include <qtimer.h>
#include <QtGui/QDialog>
#include "ui_remoteupdate.h"
#include "DemoPublic.h"
class RemoteUpdate : public QDialog
{
    Q_OBJECT

public:
    RemoteUpdate(int* userid,  QWidget *parent = 0);
    ~RemoteUpdate();

private slots:
	void on_comboBox_currentIndexChanged (int  index);
	void on_pushButton_setnetworktype_clicked();
    void on_pushButton_browse_clicked();                
    void on_pushButton_update_clicked();                
    void on_pushButton_cancel_clicked();        
    void on_lineEdit_textChanged(); 
	//timer doing
	void OnTimer();

private:
    Ui::RemoteUpdateClass ui;
    QString m_updatefilepath;
    int* m_updateuserid ;
	int m_updatenetworktype;
	int m_hUpgrade;
	QTimer * m_timer;
};

#endif // REMOTEUPDATE_H

