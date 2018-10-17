/*
 * Copyright(C) 2010,Hikvision Digital Technology Co., Ltd 
 * 
 * 文件名称：cconfigureparams.cpp
 * 描    述：
 * 当前版本：1.0
 * 作    者：潘亚东
 * 创建日期：2010年2月1日
 * 修改记录：
 */

#include "cconfigureparams.h"
#include "ui_cconfigureparams.h"
#include <QPushButton>
#include <new>
#include "DemoPublic.h"
#include <stdio.h>

/*******************************************************************
      Function:   CConfigureParams::CConfigureParams
   Description:   构造函数
     Parameter:   (IN)   QWidget *parent 父窗口 
        Return:   0--成功，-1--失败。   
**********************************************************************/
CConfigureParams::CConfigureParams(QWidget *parent): QWidget(parent), ui(new Ui::CConfigureParams)
{
    ui->setupUi(this);
    m_lUserID = -1;

    m_pRemoteparams = NULL; 
    m_pIPParams = NULL;
    m_pCurrentWidget = NULL;
    m_pHardDiskParams = NULL;

   connect(ui->btnRemoteParams, SIGNAL(clicked()), this, SLOT(loadRemoteParams()));
   connect(ui->btnLogin, SIGNAL(clicked()), this, SLOT(GetDVRUserID()));
   connect(ui->btnLogout, SIGNAL(clicked()), qApp, SLOT(quit()));

   ui->btnLogin->hide();
   ui->btnLogout->hide();
   ui->btnLocalParams->hide();
}

/*******************************************************************
      Function:   CConfigureParams::~CConfigureParams
   Description:   析构函数
     Parameter:   (IN)   无参数  
        Return:   无
**********************************************************************/
CConfigureParams::~CConfigureParams()
{
    if(m_pRemoteparams != NULL)
    {
        delete m_pRemoteparams;
    }
    if(m_pIPParams != NULL)
    {
    	delete m_pIPParams;
    }
    delete m_pHardDiskParams;   //删除硬盘配置

    delete ui;
}

/*******************************************************************
      Function:   CConfigureParams::loadRemoteParams
   Description:   load the remot params form!
     Parameter:   (IN)   无参数  
        Return:   0--成功，-1--失败。   
**********************************************************************/
void CConfigureParams::loadRemoteParams()
{
	if(NULL == m_pRemoteparams)
	{
		m_pRemoteparams = new(std::nothrow) CRemoteParams;
		if(NULL == m_pRemoteparams)
		{
	    	m_MessageBox.setText("addWidget error!");
	    	m_MessageBox.exec();
	    	return ;
		}
	}

	if(m_pCurrentWidget != NULL) //隐藏当前窗口
	{
		m_pCurrentWidget->hide();
	}
	m_pCurrentWidget = m_pRemoteparams;
	m_pRemoteparams->setUserID(m_lUserID);
	ui->vLayParamForm->addWidget(m_pCurrentWidget);
	m_pCurrentWidget->show();
}

/*******************************************************************
      Function:   CConfigureParams::on_btnIPParams_clicked
   Description:   ip接入配置
     Parameter:   (IN)   无参数  
        Return:   0--成功，-1--失败。   
**********************************************************************/
void CConfigureParams::on_btnIPParams_clicked()
{
	if(NULL == m_pIPParams)
	{
		m_pIPParams = new(std::nothrow) CIPParams;
		if(NULL == m_pIPParams)
		{
	    	m_MessageBox.setText("addWidget error!");
	    	m_MessageBox.exec();
	    	return ;
		}
	}
	//隐藏当前窗口
	if(m_pCurrentWidget != NULL)
	{
		m_pCurrentWidget->hide();
	}
	m_pIPParams->setUserID(m_lUserID);
	m_pCurrentWidget = m_pIPParams;
	ui->vLayParamForm->addWidget(m_pCurrentWidget);
    m_pCurrentWidget->show();
}

/*******************************************************************
      Function:   CConfigureParams::on_btnHardDiskParams_clicked
   Description:   硬盘配置
     Parameter:   (IN)   无参数  
        Return:   0--成功，-1--失败。   
**********************************************************************/
void CConfigureParams::on_btnHardDiskParams_clicked()
{
	if(NULL == m_pHardDiskParams)
	{
		m_pHardDiskParams = new(std::nothrow) CHardDiskParams;
		if(NULL == m_pHardDiskParams)
		{
	    	m_MessageBox.setText("addWidget error!");
	    	m_MessageBox.exec();
	    	return ;
		}
	}
	//隐藏当前窗口
	if(m_pCurrentWidget != NULL)
	{
		m_pCurrentWidget->hide();
	}
	m_pHardDiskParams->setUserID(m_lUserID);
	m_pCurrentWidget = m_pHardDiskParams;
	ui->vLayParamForm->addWidget(m_pCurrentWidget);
    m_pCurrentWidget->show();
}

/*******************************************************************
      Function:   CConfigureParams::GetDVRUserID
   Description:   Get the user ID.
     Parameter:   (IN)   无参数  
        Return:   无   
**********************************************************************/
void CConfigureParams::GetDVRUserID()
{
    //1.m_lUserID =lUerID;

    //2.login the divice
    char *m_serverIP = "172.7.39.111";
    NET_DVR_DEVICEINFO m_devInfo;
    int iRet;
    int iError;
    iRet = NET_DVR_Init();
    iError = NET_DVR_GetLastError();

    m_lUserID = NET_DVR_Login(m_serverIP, 8000, "admin", "091103", &m_devInfo);
     if(m_lUserID < 0)
     {
         iError = NET_DVR_GetLastError();
        printf( "Login DVR failed ... LastError[%d]\n", NET_DVR_GetLastError());
        QMessageBox::information(this, tr("Login failed"), tr("\"%1\" is return value of net_dvr_login.").arg(m_lUserID));
        NET_DVR_Cleanup();
        return ;
     }
     else
     {
         ui->btnLogin->setEnabled(FALSE);
     }

}

/*******************************************************************
      Function:   CConfigureParams::SetDVRUserID
   Description:   Set User ID to this class.
     Parameter:   (IN)   long lUserID  
        Return:   none   
**********************************************************************/
void CConfigureParams::SetDVRUserID(long lUserID)
{
	if(lUserID >= 0)
	{
		m_lUserID = lUserID;
	}
}

/*******************************************************************
      Function:   CConfigureParams::showEvent
   Description:   show event
     Parameter:   (IN)   QShowEvent * event  
        Return:   bone   
**********************************************************************/
void CConfigureParams::showEvent ( QShowEvent * event )
{
	if(m_lUserID < 0)
	{
		return;
	}

	//QMessageBox::information(this, tr("Login failed"), tr(" is return value of net_dvr_login."));
	if(NULL != m_pRemoteparams)
	{
		m_pRemoteparams->setUserID(m_lUserID);
	}

	if(NULL != m_pIPParams)
	{
		m_pIPParams->setUserID(m_lUserID);
	}

	if(NULL != m_pHardDiskParams)
	{
		m_pHardDiskParams->setUserID(m_lUserID);
	}
}

/*******************************************************************
      Function:   CConfigureParams::showLoginBtn
   Description:   Show buttons.
     Parameter:   (IN)   无参数  
        Return:   none   
**********************************************************************/
void CConfigureParams::showLoginBtn()
{
	ui->btnLogin->show();
	ui->btnLogout->show();
}



