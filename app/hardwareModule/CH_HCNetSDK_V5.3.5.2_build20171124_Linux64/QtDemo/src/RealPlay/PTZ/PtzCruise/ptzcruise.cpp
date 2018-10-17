/************************************************************************
 *        Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *        FileName                      :   ptzcruise.cpp
 *        Description                   :   云台巡航路径设置相关操作
 *        Modification History          :   none
 *        Version                       :   V1.0
 *        Time                          :   2009-11,12
 *        Author                        :   wanggongpu
 *        Descrp                        :   none
*************************************************************************/
#include "ptzcruise.h"
#include <QMessageBox>
#include <QString>
#include <QStringList>
#include <stdio.h>

/************************************************************************
 *        Function            :  PtzCruise
 *        Description         :  构造函数
 *        Input               :  int realhandle 预览句柄号,QWidget *parent
 								 QT widget基类
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
PtzCruise::PtzCruise(int realhandle,QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);
	m_rpcurrentrealhandle = realhandle;

	QStringList stringlistforpreset;
	char strTemp[10];
	for (int i=1;i<256;i++)
	{
		memset(strTemp, 0, sizeof(strTemp));
		sprintf(strTemp, "%d", i);
		stringlistforpreset <<strTemp;
	}
	ui.comboBox_ptzcruisepresetpoint->addItems(stringlistforpreset);
	ui.comboBox_ptzcruisetime->addItems(stringlistforpreset);

	QStringList stringlistforcruisespeed;
	//char strTemp[10];
	for (int i=1;i<16;i++)
	{
		memset(strTemp, 0, sizeof(strTemp));
		sprintf(strTemp, "%d", i);
		stringlistforcruisespeed <<strTemp;
	}
	ui.comboBox_ptzcruisespeed->addItems(stringlistforcruisespeed);

	QStringList stringlistforcruise;
	for (int i=1;i<32;i++)
	{
		memset(strTemp, 0, sizeof(strTemp));
		sprintf(strTemp, "%d", i);
		stringlistforcruise <<strTemp;
	}
	ui.comboBox_ptzcruisepath->addItems(stringlistforcruise);
	ui.comboBox_ptzcruisepoint->addItems(stringlistforcruise);

	m_ptzcruisepathindex = 1;
	m_ptzcruisepointindex = 1;
	m_ptzcruisepresetpointindex = 1;
	m_ptzcruisetimeindex = 1;
	m_ptzcruisespeedindex = 1;

}

/************************************************************************
 *        Function            :  ~PtzCruise
 *        Description         :  析构函数
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
PtzCruise::~PtzCruise()
{

}

/************************************************************************
 *        Function            :  on_pushButton_addcruisepoint_clicked
 *        Description         :  增加云台巡航预置点
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void PtzCruise::on_pushButton_addcruisepoint_clicked()
{
 	if (!NET_DVR_PTZCruise(m_rpcurrentrealhandle,FILL_PRE_SEQ, \
		m_ptzcruisepathindex, m_ptzcruisepointindex,m_ptzcruisepresetpointindex ))
    {
        QMessageBox::information(this,tr("NET_DVR_PTZControl Error"),\
                tr("SDK_LASTERROR=%1").arg(NET_DVR_GetLastError()));
    }
	if (!NET_DVR_PTZCruise(m_rpcurrentrealhandle,SET_SEQ_DWELL, \
		m_ptzcruisepathindex, m_ptzcruisepointindex,m_ptzcruisetimeindex ))
    {
        QMessageBox::information(this,tr("NET_DVR_PTZControl Error"),\
                tr("SDK_LASTERROR=%1").arg(NET_DVR_GetLastError()));
    }
	if (!NET_DVR_PTZCruise(m_rpcurrentrealhandle,SET_SEQ_SPEED, \
		m_ptzcruisepathindex, m_ptzcruisepointindex,m_ptzcruisespeedindex ))
    {
        QMessageBox::information(this,tr("NET_DVR_PTZControl Error"),\
                tr("SDK_LASTERROR=%1").arg(NET_DVR_GetLastError()));
    }
}

/************************************************************************
 *        Function            :  on_pushButton_deletecruisepoint_clicked
 *        Description         :  删除云台巡航预置点
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void PtzCruise::on_pushButton_deletecruisepoint_clicked()
{
 	if (!NET_DVR_PTZCruise(m_rpcurrentrealhandle,CLE_PRE_SEQ, \
		m_ptzcruisepathindex, m_ptzcruisepointindex,m_ptzcruisepresetpointindex ))
    {
        QMessageBox::information(this,tr("NET_DVR_PTZControl Error"),\
                tr("SDK_LASTERROR=%1").arg(NET_DVR_GetLastError()));
    }
}

/************************************************************************
 *        Function            :  on_pushButton_exit_clicked
 *        Description         :  退出云台巡航配置对话框
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void PtzCruise::on_pushButton_exit_clicked()
{
	close();
}

/************************************************************************
 *        Function            :  on_comboBox_ptzcruisepath_currentIndexChanged
 *        Description         :  修改云台巡航路径编号
 *        Input               :  int index 巡航路径编号
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void PtzCruise::on_comboBox_ptzcruisepath_currentIndexChanged(int index)
{
	m_ptzcruisepathindex =index+1;

}

/************************************************************************
 *        Function            :  on_comboBox_ptzcruisepoint_currentIndexChanged
 *        Description         :  修改云台预置点编号
 *        Input               :  int index 云台预置点编号
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void PtzCruise::on_comboBox_ptzcruisepoint_currentIndexChanged(int index)
{
	m_ptzcruisepointindex =index+1;

}

/************************************************************************
 *        Function            :  on_comboBox_ptzcruisepresetpoint_currentIndexChanged
 *        Description         :  设置云台预置点编号
 *        Input               :  int index 云台预置点编号
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void PtzCruise::on_comboBox_ptzcruisepresetpoint_currentIndexChanged(int index)
{
	m_ptzcruisepresetpointindex =index+1;

}

/************************************************************************
 *        Function            :  on_comboBox_ptzcruisetime_currentIndexChanged
 *        Description         :  修改云台巡航停留时间值
 *        Input               :  int index 停留时间值秒计
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void PtzCruise::on_comboBox_ptzcruisetime_currentIndexChanged(int index)
{
	m_ptzcruisetimeindex =index+1;

}

/************************************************************************
 *        Function            :  on_comboBox_ptzcruisespeed_currentIndexChanged
 *        Description         :  修改云台巡航速度值
 *        Input               :  int index 云台巡航速度值
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void PtzCruise::on_comboBox_ptzcruisespeed_currentIndexChanged(int index)
{
	m_ptzcruisespeedindex =index+1;
}

