/************************************************************************
 *        Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *        FileName                      :   ptzcruise.h
 *        Description                   :   预览云台巡航路径设置相关操作
 *        Modification History          :   none
 *        Version                       :   V1.0
 *        Time                          :   2009-2010
 *        Author                        :   wanggongpu
 *        Descrp                        :   none
*************************************************************************/
#ifndef PTZCRUISE_H
#define PTZCRUISE_H

#include <QtGui/QDialog>
#include "ui_ptzcruise.h"
#include "DemoPublic.h"

class PtzCruise : public QDialog
{
    Q_OBJECT
		
public:
    PtzCruise(int realhandle,QWidget *parent = 0);
    ~PtzCruise();

private slots:
	void on_pushButton_addcruisepoint_clicked();
	void on_pushButton_deletecruisepoint_clicked();
	void on_pushButton_exit_clicked();
	void on_comboBox_ptzcruisepath_currentIndexChanged(int index);
	void on_comboBox_ptzcruisepoint_currentIndexChanged(int index);
	void on_comboBox_ptzcruisepresetpoint_currentIndexChanged(int index);
	void on_comboBox_ptzcruisetime_currentIndexChanged(int index);
	void on_comboBox_ptzcruisespeed_currentIndexChanged(int index);
	
private:
    Ui::PtzCruiseClass ui;
	int m_rpcurrentrealhandle ;
	int m_ptzcruisepathindex;
	int m_ptzcruisepointindex;
	int m_ptzcruisepresetpointindex;
	int m_ptzcruisetimeindex;
	int m_ptzcruisespeedindex;
};

#endif // PTZCRUISE_H

