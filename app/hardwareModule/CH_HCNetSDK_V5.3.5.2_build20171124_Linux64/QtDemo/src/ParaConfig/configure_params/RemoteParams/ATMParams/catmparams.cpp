/*
 * Copyright(C) 2010,Hikvision Digital Technology Co., Ltd 
 * 
 * 文件名称：catmparams.cpp
 * 描    述：
 * 当前版本：1.0
 * 作    者：潘亚东
 * 创建日期：2010年1月12日
 * 修改记录：
 */

#include "catmparams.h"
#include "publicfuc.h"
#include "DemoPublic.h"

#include <QMessageBox>

/*******************************************************************
      Function:   CATMParams::CATMParams
     Parameter:   (IN)   QWidget *parent  父窗口指针
   Description:   构造函数
**********************************************************************/
CATMParams::CATMParams(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);

	m_lUserID = -1;
	m_pNetListen = new(std::nothrow) CATMNetParams;
	ui.widget->show();
	ui.vLayout->addWidget(m_pNetListen);
	ui.vLayout->addWidget(ui.widget);


	QStringList list;
	list<<"Sniff"<<"TCP/IP"<<"String From Serial Port"<<"ATM CMD From Serial Port";		
		
	ui.comboInputType->addItems(list);

	QStringList listATMType;
	listATMType<<"NCR"<<"DIEBOLD"<<"WINCOR-NIXDORF"<<"SIEMENS"<<"OLIVETTI";
	listATMType<<"FUJITSU"<<"HITACHI"<<"SMI"<<"IBM"<<"BULL";
	listATMType<<"怡化"<<"立德"<<"广电运通"<<"Mini-Banl"<<"广利"<<"东信";
	listATMType<<"辰通"<<"南天"<<"晓星"<<"广州御银"<<"青湖泰利特"<<"DRS918";
	listATMType<<"KALATEL"<<"NCR_2"<<"NXS";
	ui.comboATMType->addItems(listATMType);

	connect(ui.comboInputType, SIGNAL(activated(int)), this,  SLOT(comboInputTypeChanged(int)));
}

/*******************************************************************
      Function:   CATMParams::~CATMParams
     Parameter:   (IN)   无参数  
   Description:   析构函数
**********************************************************************/
CATMParams::~CATMParams()
{
	delete m_pNetListen;
}

/*******************************************************************
      Function:   CATMParams::setUserID
     Parameter:   (IN)   long lUserID  用户ID
        Return:   HPR_OK--成功，HPR_ERROR--失败。
   Description:   设置用户ID
**********************************************************************/
int CATMParams::setUserID(long lUserID)
{
	if(lUserID < 0)
	{
		return HPR_ERROR;
	}
	m_lUserID = lUserID;

	return HPR_OK;
}

/*******************************************************************
      Function:   CATMParams::comboInputTypeChanged
     Parameter:   (IN)   int iType ATM输入类型  
        Return:   无
   Description:   当ATM输入类型改变时，触发该函数，调整页面
**********************************************************************/
void CATMParams::comboInputTypeChanged(int iType)
{
	switch(iType)
	{
	case 0: //网络监听(主动)
		ui.widget->hide();
		m_pNetListen->show();
		ui.vLayout->addWidget(m_pNetListen);
		break;
	case 1: //网络协议(被动)
		ui.widget->show();
		ui.wPort->show();
		ui.wType->hide();
		m_pNetListen->hide();
		break;
	case 2: //串口直接输入(主动)
		ui.widget->show();
		ui.wPort->hide();
		ui.wType->show();
		m_pNetListen->hide();
		break;
	case 3: //串口ATM命令输入(被动)
		ui.widget->hide();
		m_pNetListen->hide();
		break;
	default: //网络监听
		break;
	}
}

/*******************************************************************
      Function:   CATMParams::getATMInfo
     Parameter:   (IN)   无参数  
        Return:   HPR_OK--成功，HPR_ERROR--失败。
   Description:   获取ATM参数，并刷新界面
**********************************************************************/
int CATMParams::getATMInfo()
{
	if(m_lUserID < 0 )
	{
		return HPR_ERROR;
	}
	isATMDevice();

	int iRet;
	NET_DVR_FRAMEFORMAT struATMParams;
	iRet = NET_DVR_ClientGetframeformat(m_lUserID, &struATMParams );
    if(0 == iRet)
    {
    	iRet = NET_DVR_GetLastError();
        QMessageBox::information(this, "Please check!", tr("The error is \"%1\"").arg(iRet));
        return HPR_ERROR;
    }

    ui.comboInputType->setCurrentIndex((int)struATMParams.dwInputMode);
    //设置界面的值
    switch(struATMParams.dwInputMode)
    {
    case 0: //网路侦听
    	//ATM地址
    	m_pNetListen->getATMInfo(&struATMParams);
    	break;
    case 1: //网络协议
    	//获取端口号
    	unsigned short  sPort;
    	if(NET_DVR_GetATMPortCFG(m_lUserID, &sPort))
    	{
    		KIT_intToEdit(ui.editPort, (int)sPort);
    	}
    	break;
    case 2: //串口监听
    	//ATM类型
    	ui.comboATMType->setCurrentIndex((int)struATMParams.dwATMType);
    	break;
    case 3: //
    	break;
    default:
    	break;
    }

	return HPR_OK;
}

/*******************************************************************
      Function:   CATMParams::setATMInfo
     Parameter:   (IN)   无参数  
        Return:   HPR_OK--成功，HPR_ERROR--失败。
   Description:   设置界面参数，但是不会刷新界面
**********************************************************************/
int CATMParams::setATMInfo()
{
	if(m_lUserID < 0 )
	{
		return HPR_ERROR;
	}

	int iRet;
	NET_DVR_FRAMEFORMAT struATMParams;
	iRet = NET_DVR_ClientGetframeformat(m_lUserID, &struATMParams );
    if(0 == iRet)
    {
    	iRet = NET_DVR_GetLastError();
        QMessageBox::information(this, "Please check!", tr("The error is \"%1\"").arg(iRet));
        return HPR_ERROR;
    }

    int iInputType;
    iInputType = ui.comboInputType->currentIndex();
    struATMParams.dwInputMode = (unsigned int)iInputType;
    //获取界面的值
	switch(iInputType)
	{
	case 0: //网路侦听
		//ATM地址
		m_pNetListen->setATMInfo(&struATMParams);
		break;
	case 1: //网络协议
		//获取端口号
		int  iPort;
		KIT_editToInt(ui.editPort, &iPort);
		if(!NET_DVR_SetATMPortCFG(m_lUserID, (unsigned short)iPort))
		{
			return HPR_ERROR;
		}
		break;
	case 2: //串口监听
		//ATM类型
		struATMParams.dwATMType = (unsigned int)ui.comboATMType->currentIndex();
		break;
	case 3: //
		break;
	default:
		break;
	}

	//设置到设备中去
	iRet = NET_DVR_ClientSetframeformat(m_lUserID, &struATMParams );
    if(!iRet)
    {
    	iRet = NET_DVR_GetLastError();
        QMessageBox::information(this, "Please check!", tr("The error is \"%1\"").arg(iRet));
        return HPR_ERROR;
    }

    //重启后方可生效
    QMessageBox::information(this, "Information", "Please restart device to enable the setting!");
	return HPR_OK;
}

int CATMParams::isATMDevice()
{
	if(m_lUserID < 0 )
	{
		return HPR_ERROR;
	}

	int iRet;
	DWORD paramsLen;
	NET_DVR_DEVICECFG deviceParams ;
	memset(&deviceParams, 0, sizeof(NET_DVR_DEVICECFG));
	iRet =     NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_DEVICECFG, -1, &deviceParams, sizeof(NET_DVR_DEVICECFG), &paramsLen);
    if(!iRet)
    {
    	KIT_DisplayErrorInfo("Get device information", KIT_ERRPR_DIS_TYPE);
    	return HPR_ERROR;
    }

    int iDeviceType;
    iDeviceType = deviceParams.byDVRType;
    if(iDeviceType != ATMDVR && iDeviceType != ATMDVR_S)
    {
    	setEnabled(FALSE);
    	return FALSE;
    }
    else
    {
    	setEnabled(TRUE);
    	return TRUE;
    }
}

/*******************************************************************
      Function:   CATMParams::showEvent
     Parameter:   (IN)   QShowEvent * event  窗口显示事件
        Return:   HPR_OK--成功，HPR_ERROR--失败。
   Description:   当窗口显示时，自动从设备获取数据，刷新界面
**********************************************************************/
void CATMParams::showEvent ( QShowEvent * event )
{
	if(NULL == event)
	{
	}


	comboInputTypeChanged(0); //刷下界面
	getATMInfo(); //更新窗口数据
}



