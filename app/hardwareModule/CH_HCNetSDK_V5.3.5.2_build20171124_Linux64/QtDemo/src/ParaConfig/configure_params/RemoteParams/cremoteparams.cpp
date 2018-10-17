  
#include <QMessageBox>
#include <stdio.h>
#include <new>
#include "cremoteparams.h"
#include "DemoPublic.h"
#include "publicfuc.h"
#include "qtclientdemo.h"

extern QtClientDemo* gqtclinetdemo;
CRemoteParams::CRemoteParams(QWidget *parent):
    QWidget(parent),
    m_ui(new Ui::CRemoteParamsClass)
{
    m_ui->setupUi(this);
    m_lUserID = -1;
    m_lChannel = 1;
    m_pAdvancedNet = NULL;
    m_pNfs = NULL;
    m_pZoneDst = NULL;
    m_pRtsp = NULL;
    m_pCompressAudio = NULL;
    m_iTabIndex = -1;
    m_pChannelParams = NULL;
    m_pUserParams= NULL;
    m_pATMParams = NULL;

    m_pExceptionParams = new(std::nothrow) ExceptionParams;
    m_pSerialParams = new(std::nothrow)CSerialParams;
    m_pAlarmParams = new(std::nothrow)CAlarmParamsDlg;
    m_ui->verticalLayout_ExceptionConfig->addWidget(m_pExceptionParams);
    m_ui->verticalLayoutserialparams->addWidget(m_pSerialParams);
    m_ui->verticalLayoutAlarmParams->addWidget(m_pAlarmParams);

    //click button
    connect(m_ui->btnRefresh, SIGNAL(clicked()), this,SLOT(refreshParams()));
    connect(m_ui->btnSaveParams,SIGNAL(clicked()),this, SLOT(saveParams()));
    connect(m_ui->checkboDHCP, SIGNAL(clicked()), this, SLOT(clickDHCP()));
    connect(m_ui->btnAdancedNet, SIGNAL(clicked()), this, SLOT(clickAdvancedNet()));
    connect(m_ui->btnNFS, SIGNAL(clicked()), this, SLOT(clickNFS()));
    connect(m_ui->btnZoneDst, SIGNAL(clicked()), this, SLOT(clickZoneDst()));
    connect(m_ui->btnRtsp, SIGNAL(clicked()), this, SLOT(clickRtsp()));
    connect(m_ui->btnCompressAudio, SIGNAL(clicked()), this, SLOT(clickCompressAudio()));
    //click table pages
    connect(m_ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(clickTabPage(int)));

    //Device version information
    m_ui->editSoftV->setEnabled(FALSE);
    m_ui->editDPSSoftV->setEnabled(FALSE);
    m_ui->editHardV->setEnabled(FALSE);
    m_ui->editPrePaneV->setEnabled(FALSE);

    //Base information
    m_ui->editDeviceSerialNO->setEnabled(FALSE);
    m_ui->editAlarmInNum->setEnabled(FALSE);
    m_ui->editAlarmOutNum->setEnabled(FALSE);
    m_ui->editHDNum->setEnabled(FALSE);
    m_ui->editChannelNum->setEnabled(FALSE);
    m_ui->comboDeviceType->setEditable(FALSE);

    //Net information
    m_ui->editMAC->setEnabled(FALSE);
    m_ui->editMTU->setEnabled(FALSE);

    QStringList strings;
    strings << "no" << "yes";
    m_ui->comboRecyle->addItems(strings);

    strings.clear();
    strings <<"10MBase - T"<<"10MBase-T Full Dup"<<"100MBase - TX"<<"100M Full Dup"<<"10M/100M/1000M Auto"<<"1000M Full Dup";
    m_ui->comboNetCardType->addItems(strings);

    strings.clear();
    strings<<"no"<<"yes";
    m_ui->comboMajorScale->addItems(strings);
    m_ui->comboMinorScale->addItems(strings);
}

CRemoteParams::~CRemoteParams()
{
    delete m_ui;
    //net
    if(NULL != m_pAdvancedNet)
    {
    	delete m_pAdvancedNet;
    }
    if(NULL !=  m_pNfs)
    {
    	delete m_pNfs;
    }
    if(NULL !=  m_pZoneDst)
    {
      	delete m_pZoneDst;
    }
    if(NULL !=  m_pRtsp)
    {
       	delete m_pRtsp;
     }
    if(NULL !=  m_pCompressAudio)
    {
       	delete m_pCompressAudio;
    }

    //channel
    if(NULL !=  m_pChannelParams)
     {
        delete m_pChannelParams;
     }

    //用户窗口指针
    if(NULL != m_pUserParams)
    {
    	delete m_pUserParams;
    }

    delete m_pATMParams; //交易信息
}

void CRemoteParams::clickTabPage(int iTabIndex)
{
       //QMessageBox::information(this, "Please check!", tr("The tab page  is \"%1\"").arg(iTabIndex));
     switch(iTabIndex)
     {
     case 0:  //net parameters.
    	 getDeviceInfo(); //Device information.
     	 getNetInfo();      //Get net iformation.
    	 break;
     case 1:  //channel parameters.
         if(NULL == m_pChannelParams)
         {
        	 //create channel table page
        	 m_pChannelParams = new(std::nothrow) CChannelParams;
        	 if(m_pChannelParams != NULL)
        	 {
        		 m_ui->vLayChannel->addWidget(m_pChannelParams);
        		 m_pChannelParams->setUserID(m_lUserID);
        	 }
        	 else //allocate memory error
        	 {
        		 QMessageBox::information(this, tr("allocate memory error"), tr("When new m_pChannelParams, error happens."));
        	 }
         }
         else  //m_pChannelParams bas been created.
         {
        	 m_pChannelParams->setUserID(m_lUserID);
         }
         break;
     case 2:  //串口参数
    	 if(m_pSerialParams != NULL)
    	 {
    		 m_pSerialParams->setUserID(m_lUserID);
    	 }
    	 break;
     case 3: //报警参数
    	 if(m_pAlarmParams != NULL)
    	 {
    		 m_pAlarmParams->setUserID(m_lUserID);
    	 }
    	 break;
     case 4: //user parameters
    	 if(NULL == m_pUserParams)
    	 {
    		 //创建用户参数窗口
    		 m_pUserParams = new(std::nothrow) CUserParams;
    		 if(m_pUserParams != NULL)
    		 {
    			 m_ui->vLayUser->addWidget(m_pUserParams);
    			 m_pUserParams->setUserID(m_lUserID);
    		 }
    		 else
    		 {
    			 QMessageBox::information(this, tr("allocate memory error"), tr("When new m_pUserParams, error happens."));
    		 }
    	 }
    	 else
    	 {
    		 m_pUserParams->setUserID(m_lUserID);
    	 }
         break;
     case 5: //异常参数
    	 if(m_pExceptionParams != NULL)
    	 {
    		 m_pExceptionParams->setUserID(m_lUserID);
    	 }
    	 break;
     case 6: //ATM参数
    	 if(NULL == m_pATMParams)
		 {
			 //创建ATM参数窗口
    		 m_pATMParams = new(std::nothrow) CATMParams;
			 if(m_pATMParams != NULL)
			 {
				 m_ui->vLayATM->addWidget(m_pATMParams);
				 m_pATMParams->setUserID(m_lUserID);
			 }
			 else
			 {
				 QMessageBox::information(this, tr("allocate memory error"), tr("When new m_pUserParams, error happens."));
			 }
		 }
		 else
		 {
			 m_pATMParams->setUserID(m_lUserID);
		 }
		 break;
     default:
    	 break;
     }
}

//add by QT automatically
void CRemoteParams::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

//Set the user ID
 int CRemoteParams::setUserID(long lUserID)
 {
     if(lUserID >= 0)
     {
         m_lUserID = lUserID;
         m_pExceptionParams->setUserID(m_lUserID);
         m_pSerialParams->setUserID(m_lUserID);
         m_pAlarmParams->setUserID(m_lUserID);
         return HPR_OK;
     }
     else
     {
         return HPR_ERROR;
     }
 }

void CRemoteParams::refreshParams()
{
    //Connect to the device(DVR).
    if(m_lUserID < 0)
    {
        return;
    }

    //Get the Params.
    m_iTabIndex = m_ui->tabWidget->currentIndex();

    switch(m_iTabIndex)
    {
        case 0://基本参数
        	getDeviceInfo(); //Device information.
        	getNetInfo();      //Get net iformation.
        	getScaleInfo();
      	    break;
        case 1://通道参数
      	    if(m_pChannelParams != NULL)
      	    {
      	    	m_pChannelParams->setUserID(m_lUserID);
      		    m_pChannelParams->getAllInfo();
      	    }
      	    break;
        case 2: //串口参数
        	if(m_pSerialParams != NULL)
        	{
        		m_pSerialParams->setUserID(m_lUserID);
        		m_pSerialParams->refreshparams();
        	}
        	break;
        case 3: //报警输入输出参数
        	if(m_pAlarmParams != NULL)
        	{
        		m_pAlarmParams->setUserID(m_lUserID);
        		m_pAlarmParams->getAlarmInfo();
        	}
        	break;
        case 4://用户参数
        	if(m_pUserParams != NULL)
        	{
        		m_pUserParams->setUserID(m_lUserID);
        		m_pUserParams->getAllInfo();
        	}
            break;
        case 6: //ATM参数
        	if(m_pATMParams != NULL)
        	{
        		m_pATMParams->setUserID(m_lUserID);
        		m_pATMParams->getATMInfo();
        	}
        	break;
        default:
       	    break;
      }
}

void CRemoteParams::saveParams()
{
    if(m_lUserID <0)
    {
        return;
    }
    m_iTabIndex = m_ui->tabWidget->currentIndex();
    switch(m_iTabIndex)
    {
    case 0:
    	  setDeviceInfo();
    	  setNetInfo();
    	  setScaleInfo();
    	  break;
    case 1:
    	if(m_pChannelParams != NULL)
    	{
    		m_pChannelParams->setAllInfo();
    	}
    	break;
    case 4:
    	if(m_pUserParams != NULL)
    	{
    		m_pUserParams->setAllInfo();
    	}
    	break;
    case 6: //ATM参数
    	if(m_pATMParams != NULL)
    	{
    		m_pATMParams->setATMInfo();
    	}
    	break;
    default:
    	break;
    }



}

//update the device version information
int CRemoteParams::getDeviceInfo()
{
	if(m_lUserID < 0)
	{
		return HPR_ERROR;
	}

    NET_DVR_DEVICECFG deviceParams ;
    memset(&deviceParams, 0, sizeof(NET_DVR_DEVICECFG));
    QString qstrTemp;
    DWORD paramsLen;
    char strTemp[100];

    int iRet;
    iRet = NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_DEVICECFG, m_lChannel, &deviceParams, sizeof(NET_DVR_DEVICECFG), &paramsLen);
    if(!iRet)
    {
    	KIT_DisplayErrorInfo("Get device information", KIT_ERRPR_DIS_TYPE);
    	return HPR_ERROR;
    }

    //***device version information***//
    //software version
    memset(strTemp,0, 100);
    //QMessageBox::information(this, "Please check!", tr("The current tab page  is \"%1\" ").arg(deviceParams.dwSoftwareVersion));
    //QMessageBox::information(this, "Please check!", tr("The current tab page  is \"%1\" ").arg(deviceParams.dwSoftwareVersion>>24));

    if ((deviceParams.dwSoftwareVersion>>24) == 0)
    {
    	sprintf(strTemp, "V%d.%d build %02d%02d%02d",\
    	                (deviceParams.dwSoftwareVersion>>16)&0xFFFF, deviceParams.dwSoftwareVersion&0xFFFF, (deviceParams.dwSoftwareBuildDate>>16)&0xFFFF, (deviceParams.dwSoftwareBuildDate>>8)&0xFF,deviceParams.dwSoftwareBuildDate&0xFF);
    }
    else
    {
    	sprintf(strTemp, "V%d.%d.%d build %02d%02d%02d",\
    			(deviceParams.dwSoftwareVersion>>24), (deviceParams.dwSoftwareVersion>>16)&0xFF, deviceParams.dwSoftwareVersion&0xFFFF, (deviceParams.dwSoftwareBuildDate>>16)&0xFFFF, (deviceParams.dwSoftwareBuildDate>>8)&0xFF,deviceParams.dwSoftwareBuildDate&0xFF);
    }
   m_ui->editSoftV->setText(QString(QLatin1String(strTemp)));

    //DPS software version
    memset(strTemp,0, 100);
    sprintf(strTemp, "V%d.%d build %02d%02d%02d", (deviceParams.dwDSPSoftwareVersion>>16)&0xFFFF, deviceParams.dwDSPSoftwareVersion&0xFFFF, (deviceParams.dwDSPSoftwareBuildDate>>16)&0xFFFF-2000, (deviceParams.dwDSPSoftwareBuildDate>>8)&0xFF,deviceParams.dwDSPSoftwareBuildDate&0xFF);
    m_ui->editDPSSoftV->setText(QString(QLatin1String(strTemp)));

    //hard version
    memset(strTemp,0, 100);
    sprintf(strTemp,"0x%x", deviceParams.dwHardwareVersion);
    m_ui->editHardV->setText(QString(QLatin1String(strTemp)));

    //previce panel version
    memset(strTemp,0, 100);
    sprintf(strTemp,"V%d", deviceParams.dwPanelVersion);
    m_ui->editPrePaneV->setText(QString(QLatin1String(strTemp)));

    //****base information****//
    //device serial NO
    memset(strTemp,0, 100);
    sprintf(strTemp,"%s", deviceParams.sSerialNumber);
    m_ui->editDeviceSerialNO->setText(QString(QLatin1String(strTemp)));
    //device name
    memset(strTemp, 0, 100);
    sprintf(strTemp,"%s", deviceParams.sDVRName);
    m_ui->editDeviceName->setText(QString(QLatin1String(strTemp)));
    //device Type
    memset(strTemp, 0, 100);
    sprintf(strTemp,"%d", deviceParams.byDVRType);
    qstrTemp = QString(QLatin1String(strTemp));
    m_ui->comboDeviceType->clear();
    m_ui->comboDeviceType->addItem(qstrTemp);
    //devic hard number
    memset(strTemp, 0, 100);
    sprintf(strTemp,"%d", deviceParams.byDiskNum);
    m_ui->editHDNum->setText(QString(QLatin1String(strTemp)));
    //Alarm in Num
    memset(strTemp, 0, 100);
    sprintf(strTemp,"%d", deviceParams.byAlarmInPortNum);
    m_ui->editAlarmInNum->setText(QString(QLatin1String(strTemp)));
    //Alarm out Num
    memset(strTemp, 0, 100);
    sprintf(strTemp,"%d", deviceParams.byAlarmOutPortNum);
    m_ui->editAlarmOutNum->setText(QString(QLatin1String(strTemp)));
    //channel num
    memset(strTemp, 0, 100);
    sprintf(strTemp,"%d", deviceParams.byChanNum);
    m_ui->editChannelNum->setText(QString(QLatin1String(strTemp)));
    //DVRID
    memset(strTemp, 0, 100);
    sprintf(strTemp,"%d", deviceParams.dwDVRID);
    m_ui->editYKQID->setText(QString(QLatin1String(strTemp)));
    //whether Recycle Record
    if(deviceParams.dwRecycleRecord == 0 || deviceParams.dwRecycleRecord == 1)
    {
        m_ui->comboRecyle->setCurrentIndex((int)(deviceParams.dwRecycleRecord));
    }

    return HPR_OK;
}

//change device information
int CRemoteParams::setDeviceInfo()
{
    NET_DVR_DEVICECFG deviceParams;
    DWORD paramsLen;
    //1.get params
    NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_DEVICECFG, m_lChannel, &deviceParams, sizeof(NET_DVR_DEVICECFG), &paramsLen);

    QString qstrTemp;
    QByteArray cArrTemp;

    //2. set device  params.
    //DVR name
    qstrTemp = m_ui->editDeviceName->text();
    cArrTemp = qstrTemp.toLatin1();
    memcpy(deviceParams.sDVRName, cArrTemp.data(), NAME_LEN);
    // whether recyle video
    deviceParams.dwRecycleRecord = m_ui->comboRecyle->currentIndex();
    //YKQ ID
    editToInt(m_ui->editYKQID, (int *)(&(deviceParams.dwDVRID)));

    //set params
    NET_DVR_SetDVRConfig(m_lUserID, NET_DVR_SET_DEVICECFG, m_lChannel, &deviceParams, sizeof(NET_DVR_SET_DEVICECFG));

    return HPR_OK;
}

//Get net params;
int CRemoteParams::getNetInfo()
{
    NET_DVR_NETCFG_V30 netParams;
    memset(&netParams, 0, sizeof(NET_DVR_NETCFG_V30));
    QString qstrTemp;
    DWORD paramsLen;
    char strTemp[100];

    int iRet;
    iRet = NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_NETCFG_V30, m_lChannel, &netParams, sizeof(NET_DVR_NETCFG_V30), &paramsLen);
    if(!iRet)
    {
    	KIT_DisplayErrorInfo("Get net information", KIT_ERRPR_DIS_TYPE);
    	return HPR_ERROR;
    }

    //net card type
    if(netParams.struEtherNet[0].dwNetInterface >0 && netParams.struEtherNet[0].dwNetInterface<7)
    {
        m_ui->comboNetCardType->setCurrentIndex((int)(netParams.struEtherNet[0].dwNetInterface-1));
    }

    //whether  DHCP is set.
    if(netParams.byUseDhcp == (BYTE)0xff)
    {
    	m_ui->checkboDHCP->setChecked(FALSE);
    	clickDHCP();
    }
    else
    {
    	if(netParams.byUseDhcp == (BYTE)TRUE)
    	{
    		m_ui->checkboDHCP->setChecked(TRUE);
    		clickDHCP();
    	}
    	else
    	{
      		m_ui->checkboDHCP->setChecked(FALSE);
        	clickDHCP();
    	}
    }

    //device IP
    memset(strTemp, 0, 100);
    sprintf(strTemp,"%s", netParams.struEtherNet[0].struDVRIP.sIpV4);
    m_ui->editDeviceIP->setText(QString(QLatin1String(strTemp)));

    //devic IP mask
    memset(strTemp, 0, 100);
    sprintf(strTemp, "%s", netParams.struEtherNet[0].struDVRIPMask.sIpV4);
    m_ui->editDeviceMask->setText(QString(QLatin1String(strTemp)));

    //device gate way
    memset(strTemp, 0, 100);
    sprintf(strTemp, "%s", netParams.struGatewayIpAddr.sIpV4);
    m_ui->editGateWay->setText(QString(QLatin1String(strTemp)));

    //device  DNS IP1
    memset(strTemp, 0, 100);
    sprintf(strTemp, "%s", netParams.struDnsServer1IpAddr.sIpV4);// error
    m_ui->editDNSIP1->setText(QString(QLatin1String(strTemp)));

    //device DNS IP2
    memset(strTemp, 0, 100);
    sprintf(strTemp, "%s", netParams.struDnsServer2IpAddr.sIpV4);// error
    m_ui->editDNSIP2->setText(QString(QLatin1String(strTemp)));

    //device MAC
    memset(strTemp, 0, 100);
    sprintf(strTemp,  "%02x:%02x:%02x:%02x:%02x:%02x", \
    		(unsigned char)(netParams.struEtherNet[0].byMACAddr[0]),\
    		(unsigned char)(netParams.struEtherNet[0].byMACAddr[1]),\
    		(unsigned char)(netParams.struEtherNet[0].byMACAddr[2]),\
    		(unsigned char)(netParams.struEtherNet[0].byMACAddr[3]),\
    		(unsigned char)(netParams.struEtherNet[0].byMACAddr[4]),\
    		(unsigned char)(netParams.struEtherNet[0].byMACAddr[5]));
    m_ui->editMAC->setText(QString(QLatin1String(strTemp)));

    //private DNS address
    memset(strTemp, 0, 100);
    sprintf(strTemp, "%s", netParams.byIpResolver);
    m_ui->editPrivateDNS->setText(QString(QLatin1String(strTemp)));

    //Decvice port. NET_DVR_NETCFG_V30.NET_DVR_ETHERNET_V30
    memset(strTemp, 0, 100);
    sprintf(strTemp, "%d", netParams.struEtherNet[0].wDVRPort);
    m_ui->editDevicePort->setText(QString(QLatin1String(strTemp)));

    //Http port
    memset(strTemp, 0, 100);
    sprintf(strTemp, "%d", netParams.wHttpPortNo);
    m_ui->editHttpPort->setText(QString(QLatin1String(strTemp)));

    //Multicast IP
    memset(strTemp, 0, 100);
    sprintf(strTemp, "%s", netParams.struMulticastIpAddr.sIpV4);
    m_ui->editMulitcastIP->setText(QString(QLatin1String(strTemp)));
	gqtclinetdemo->setDeviceMultiCast(m_lUserID, strTemp);

    //Alarm IP
    memset(strTemp, 0, 100);
    sprintf(strTemp, "%s", netParams.struAlarmHostIpAddr.sIpV4);
    m_ui->editAlarmIP->setText(QString(QLatin1String(strTemp)));

    //Alarm port
    memset(strTemp, 0, 100);
    sprintf(strTemp, "%d", netParams.wAlarmHostIpPort);
    m_ui->editAlarmPort->setText(QString(QLatin1String(strTemp)));

    //MTU
    memset(strTemp, 0, 100);
    sprintf(strTemp, "%d", netParams.struEtherNet[0].wMTU);
    m_ui->editMTU->setText(QString(QLatin1String(strTemp)));

    return HPR_OK;
}

//Set net params;
int CRemoteParams::setNetInfo()
{
	NET_DVR_NETCFG_V30 netParams;
	QString qstrTemp;
	DWORD paramsLen;
	int iRet;
    QByteArray cArrTemp;

	//***firstly, get net params.***//
	NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_NETCFG_V30, m_lChannel, &netParams, sizeof(NET_DVR_NETCFG_V30), &paramsLen);

	//***secondly, set net params.***//
	//set net card
	netParams.struEtherNet[0].dwNetInterface = (m_ui->comboNetCardType->currentIndex() + 1);
	 //whether  DHCP is set.
	if(netParams.byUseDhcp != (BYTE)0xff)
	{
		if(m_ui->checkboDHCP->checkState() == Qt::Unchecked)
		{
			netParams.byUseDhcp = 0;
		}
		else
		{
			netParams.byUseDhcp = 1;
		}
	}

	//set device IP
    iRet = editToIP(m_ui->editDeviceIP, (BYTE *)netParams.struEtherNet[0].struDVRIP.sIpV4, 16);
	if(iRet == HPR_ERROR)
	{
		QMessageBox::information(this, tr("Net IP ERROR"), tr("Net IP ERROR, please check ip!"));
         return HPR_ERROR;
	}

	//set device mask
        iRet = editToIP(m_ui->editDeviceMask, (BYTE *)netParams.struEtherNet[0].struDVRIPMask.sIpV4, 16);
	if(iRet == HPR_ERROR)
	{
		QMessageBox::information(this, tr("Net Mask ERROR"), tr("Net Mask ERROR, please check mask!"));
		 return HPR_ERROR;
	}

	//set gate way
        iRet = editToIP(m_ui->editGateWay, (BYTE *)netParams.struGatewayIpAddr.sIpV4, 16);
	if(iRet == HPR_ERROR)
	{
		QMessageBox::information(this, tr("Net Gateway ERROR"), tr("Net Gateway ERROR, please check Gateway!"));
		return HPR_ERROR;
	}

	//set DNS IP1
	iRet = editToIP(m_ui->editDNSIP1, (BYTE *)netParams.struDnsServer1IpAddr.sIpV4, 16);
	if(iRet == HPR_ERROR)
	{
		QMessageBox::information(this, tr("Net DNS1 ERROR"), tr("Net DNS ERROR, please check DNS!"));
		return HPR_ERROR;
	}

	//set DNS IP2
	iRet = editToIP(m_ui->editDNSIP2, (BYTE *)netParams.struDnsServer2IpAddr.sIpV4, 16);
	if(iRet == HPR_ERROR)
	{
		QMessageBox::information(this, tr("Net DNS2 ERROR"), tr("Net DNS ERROR, please check DNS!"));
		return HPR_ERROR;
	}


	//private domain name
	editToStr(m_ui->editPrivateDNS, netParams.byIpResolver, MAX_DOMAIN_NAME);

	//Device port
	int iTemp;
	iTemp =  netParams.struEtherNet[0].wDVRPort;
	editToInt(m_ui->editDevicePort, &iTemp);
	netParams.struEtherNet[0].wDVRPort = iTemp;

	//HTTP port
	iTemp = netParams.wHttpPortNo;
	editToInt(m_ui->editHttpPort, &iTemp);
	netParams.wHttpPortNo = iTemp;

	//Multicast ip
	iRet = editToIP(m_ui->editMulitcastIP, (BYTE *)netParams.struMulticastIpAddr.sIpV4, 16);
	gqtclinetdemo->setDeviceMultiCast(m_lUserID, netParams.struMulticastIpAddr.sIpV4);
	if(iRet == HPR_ERROR)
	{
		QMessageBox::information(this, tr("Multicast ip ERROR"), tr("Multicast ip ERROR, please check ip!"));
	    return HPR_ERROR;
	}
	//Alarm host ip
        iRet = editToIP(m_ui->editAlarmIP, (BYTE *)netParams.struAlarmHostIpAddr.sIpV4, 16);
	if(iRet == HPR_ERROR)
	{
		QMessageBox::information(this, tr("Alarm ip ERROR"), tr("Alarm ip ERROR, please check ip!"));
	    return HPR_ERROR;
	}
	//Alarm host port
	iTemp = netParams.wAlarmHostIpPort;
    editToInt(m_ui->editAlarmPort, &iTemp);
    netParams.wAlarmHostIpPort = iTemp;

    iRet = 1;
    printf("pyd---netParams.byUseDhcp is %d. Before Set!\n", netParams.byUseDhcp);
    iRet = NET_DVR_SetDVRConfig(m_lUserID, NET_DVR_SET_NETCFG_V30, m_lChannel, &netParams, sizeof(NET_DVR_NETCFG_V30));
    if(iRet == 0)
    {
    	iRet = NET_DVR_GetLastError();
	    QMessageBox::information(this, tr("Set Net fail!"), tr("Set Net fail. \"%1\"").arg(iRet));
	    return HPR_ERROR;
    }

    return HPR_OK;
}

//视频缩放
int CRemoteParams::getScaleInfo()
{
	if(m_lUserID < 0)
	{
		return HPR_ERROR;
	}

	NET_DVR_SCALECFG struScaleParams;
	int iRet;
	//获取视频缩放参数
	iRet = NET_DVR_GetScaleCFG_V30(m_lUserID, &struScaleParams);
	if (iRet == 0)
	{
		iRet = NET_DVR_GetLastError();
		QMessageBox::information(this, tr("Please check!"), tr("Get scale parameters error! \"%1\"").arg(iRet));
		return HPR_ERROR;
	}

	//写入控件中去
	m_ui->comboMajorScale->setCurrentIndex((int)struScaleParams.dwMajorScale);
	m_ui->comboMinorScale->setCurrentIndex((int)struScaleParams.dwMinorScale);


	return HPR_OK;
}
int CRemoteParams::setScaleInfo()
{
	if(m_lUserID < 0)
	{
		return HPR_ERROR;
	}

	NET_DVR_SCALECFG struScaleParams;
	int iRet;
	//获取视频缩放参数
	iRet = NET_DVR_GetScaleCFG_V30(m_lUserID, &struScaleParams);
	if (iRet == 0)
	{
		iRet = NET_DVR_GetLastError();
		QMessageBox::information(this, tr("Please check!"), tr("Get scale parameters error! \"%1\"").arg(iRet));
		return HPR_ERROR;
	}
	//从控件中获取数据
	struScaleParams.dwMajorScale = m_ui->comboMajorScale->currentIndex();
	struScaleParams.dwMinorScale = m_ui->comboMinorScale->currentIndex();
	//写入设备
	iRet = NET_DVR_SetScaleCFG_V30(m_lUserID, &struScaleParams);
	if (iRet == 0)
	{
		iRet = NET_DVR_GetLastError();
		QMessageBox::information(this, tr("Please check!"), tr("Set scale parameters error! \"%1\"").arg(iRet));
		return HPR_ERROR;
	}

	return HPR_OK;
}

void CRemoteParams::clickDHCP()
{
    if(m_ui->checkboDHCP->isChecked())
    {
        m_ui->editDeviceIP->setEnabled(FALSE);
        m_ui->editDeviceMask->setEnabled(FALSE);
        m_ui->editGateWay->setEnabled(FALSE);
        m_ui->editDNSIP1->setEnabled(FALSE);
        m_ui->editDNSIP2->setEnabled(FALSE);
    }
    else
    {
        m_ui->editDeviceIP->setEnabled(TRUE);
        m_ui->editDeviceMask->setEnabled(TRUE);
        m_ui->editGateWay->setEnabled(TRUE);
        m_ui->editDNSIP1->setEnabled(TRUE);
        m_ui->editDNSIP2->setEnabled(TRUE);
    }
}

void CRemoteParams::clickAdvancedNet()
{
	if(m_pAdvancedNet == NULL)
	{
	     m_pAdvancedNet = new(std::nothrow) CAdvancedNetParams;
	     if(m_pAdvancedNet == NULL)
	     {
	    	 QMessageBox::information(this, tr("Create advanced net form failly!"), tr("error"));
	         return;
	     }
	     else
	     {
	    	 m_pAdvancedNet->setUserIDAndChannel(m_lUserID, m_lChannel);
	    	 m_pAdvancedNet->show();
	     }
	}
	else
	{
		m_pAdvancedNet->setUserIDAndChannel(m_lUserID, m_lChannel);
		m_pAdvancedNet->show();
	}
}

//show the NFS dialog
void CRemoteParams::clickNFS()
{
	if(m_pNfs == NULL)
	{
		m_pNfs = new(std::nothrow) CNfsParams;
		if(m_pNfs == NULL)
		{
			 QMessageBox::information(this, tr("New error!"), tr("new CNfsParams error!"));
		     return;
		}
		else
		{
			m_pNfs->setUserIDAndChannel(m_lUserID, m_lChannel);
			m_pNfs->show();
		}
	}
	else
	{
		m_pNfs->setUserIDAndChannel(m_lUserID, m_lChannel);
		m_pNfs->show();
	}
}

// zone and dfs
void CRemoteParams::clickZoneDst()
{
	//m_pZoneDst
	if(m_pZoneDst == NULL)
	{
		m_pZoneDst = new(std::nothrow) CZoneDst;
		if(m_pZoneDst == NULL)
		{
			 QMessageBox::information(this, tr("New error!"), tr("new CNfsParams error!"));
		     return;
		}
		else
		{
			m_pZoneDst->setUserIDAndChannel(m_lUserID, m_lChannel);
			m_pZoneDst->show();
		}
	}
	else
	{
		m_pZoneDst->setUserIDAndChannel(m_lUserID, m_lChannel);
		m_pZoneDst->show();
	}
}

void CRemoteParams::clickRtsp()
{
	if(m_pRtsp == NULL)
	{
		    m_pRtsp = new(std::nothrow) CRtspParams;
			if(m_pRtsp == NULL)
			{
				 QMessageBox::information(this, tr("New error!"), tr("new CRtspParams error!"));
			     return;
			}
			else
			{
				m_pRtsp->setUserID(m_lUserID);
				m_pRtsp->show();
			}
		}
		else
		{
			m_pRtsp->setUserID(m_lUserID);
			m_pRtsp->show();
		}
}

void CRemoteParams::clickCompressAudio()
{
	if(m_pCompressAudio == NULL)
	{
		m_pCompressAudio = new(std::nothrow) CCompressAudio;
		if(m_pCompressAudio == NULL)
		{
			 QMessageBox::information(this, tr("New error!"), tr("new CRtspParams error!"));
		     return;
		}
		else
	   {
			m_pCompressAudio->setUserIDAndChannel(m_lUserID, m_lChannel);
			m_pCompressAudio->show();
		}
	}
	else
	{
		m_pCompressAudio->setUserIDAndChannel(m_lUserID, m_lChannel);
		m_pCompressAudio->show();
	}
}

//convert the QLineEdit to  int
int CRemoteParams::editToInt(const QLineEdit *pEdit, int *pNum)
{
    QString qstrTemp;
    QByteArray cArrTemp;
    const char *pTemp;
    qstrTemp = pEdit->text();
    cArrTemp = qstrTemp.toLatin1();
    pTemp = cArrTemp.data();
    *pNum = atoi(pTemp);

    return HPR_OK;
}

//convert QLineEdit to  char*
int CRemoteParams::editToStr(const QLineEdit *pEdit, BYTE  *pStr, int iLen)
{
	if( pEdit == NULL || pStr == NULL)
	{
		return HPR_ERROR;
	}

	QString qstrTemp;
	QByteArray cArrTemp;

	//first, convert  QLineEdit  to QByteArray
	qstrTemp = pEdit->text();
	cArrTemp = qstrTemp.toLatin1();
	//second, convert QByteArray to char*.
	memcpy(pStr,  cArrTemp.data(), iLen);

	return HPR_OK;
}

//convert edit to ip
int CRemoteParams::editToIP(const QLineEdit *pEditIP, BYTE * pIP, int iLen)
{
	if( pEditIP == NULL || pIP == NULL)
	{
		return HPR_ERROR;
	}

	int iRet;
	char strIP[20];
	memset(strIP, 0, 20);
	//1.get ip str
        iRet = editToStr(pEditIP, (BYTE *)strIP, 20);
	if(iRet == HPR_ERROR)
	{
		return HPR_ERROR;
	}
    //2.check ip
	iRet = checkIP(strIP);
	if(iRet == HPR_ERROR)
	{
		return HPR_ERROR;
	}
	else
	{
		memcpy(pIP, strIP, iLen);
	}

	return HPR_OK;
}

//convert the int to QLineEdit
int CRemoteParams::intToEdit(QLineEdit *pEdit, int iNum)
{
    char strTemp[INT_LEN];
    memset(strTemp, 0, INT_LEN);
    //firstly, convert int to char *
    sprintf(strTemp, "%d", iNum);
    //secondly, convert char * to QLineEdit
    pEdit->setText(QString(QLatin1String(strTemp)));
    return  HPR_OK;
}

//event
void CRemoteParams::showEvent ( QShowEvent * event )
{
	if(event ==NULL)
	{
	}
	//获取显示参数
	refreshParams();
	clickTabPage(4);
}

//check ip
int CRemoteParams::checkIP(char * ip)
{
	if(ip == NULL)
	{
		return HPR_ERROR; //-1
	}
	if(strlen(ip) == 0)
	{
		return HPR_OK;
	}

	char str[20];
	memset(str, 0, 20);
	strcpy(str,ip);
	char a;
	int dot=0;
	int a3, a2, a1, a0, i;
	i = 0;
	a3 = a2 = a1 = a0 = -1;

	while ((a=str[i++]))
	{
		if ((a==' ')||(a=='.')||((a>='0')&&(a<='9')))
		{
			if (a=='.')
			{
				dot++;
			}
		}
		else
		{
			return HPR_ERROR;
		}
	}
	if (dot!=3)
	{
		return HPR_ERROR;
	}
	else
	{
		sscanf(str,"%d.%d.%d.%d",&a3,&a2,&a1,&a0);
		if ((a0>255)||(a1>255)||(a2>255)||(a3>255))
		{
			return HPR_ERROR;
		}
		if ((a0 < 0) || (a1 < 0) || (a2 < 0) || (a3 < 0))
		{
		    return HPR_ERROR;
		}
	}

    return HPR_OK;
}


