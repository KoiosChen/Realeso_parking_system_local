
#include <QMessageBox>
#include "cadvancednetparams.h"
#include "DemoPublic.h"
#include "publicfuc.h"

#ifndef HPR_OK
#define HPR_OK 0
#endif

#ifndef HPR_ERROR
#define HPR_ERROR -1
#endif

CAdvancedNetParams::CAdvancedNetParams(QWidget *parent)
    : QDialog(parent), m_ui(new Ui::CAdvancedNetParamsClass)
{
	m_ui->setupUi(this);
	m_lUserID = -1;
	m_lChannel = 1;
	QStringList strings;
	strings << "Hikvision DNS" << "Dyndns"<<"PeanutHull"<<"No-IP";
	m_ui->comboDDNSType->addItems(strings);

	strings.clear();
	strings<<"2s"<<"3s"<<"4s"<<"5s";
	m_ui->comboBoEmailInterval->addItems(strings);

	//checkbox
	connect(m_ui->checkBoPPPoE, SIGNAL(clicked()), this, SLOT(clickPPPoE()));
	connect(m_ui->checkBoNTP, SIGNAL(clicked()), this, SLOT(clickNTP()));
	connect(m_ui->checkBoDDNS, SIGNAL(clicked()), this, SLOT(clickDDNS()));

	//btn
    connect(m_ui->btnSaveParams, SIGNAL(clicked()), this, SLOT(saveParams()));
    connect(m_ui->btnRefresh, SIGNAL(clicked()), this, SLOT(refreshParams()));

    //table
    //connect(m_ui->tabWEmail, SIGNAL(cellChanged(int,int)), this, SLOT(updateTable(int, int)));

    //table
    m_ui->tabWEmail->setColumnCount(2);
    m_ui->tabWEmail->setRowCount(3);

    //set the size of the table
    m_ui->tabWEmail->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

    strings.clear();
    strings<<"receiver name"<<"Email";
    m_ui->tabWEmail->setHorizontalHeaderLabels(strings);
}

CAdvancedNetParams::~CAdvancedNetParams()
{
	int i,j; // i is row;  j is  column

	//delete the table item.
	for(i = 0;  i <3; i++ )
	{
		for(j=0; j<2; j++)
		{
			delete m_ui->tabWEmail->takeItem(0,0);
		}
	    m_ui->tabWEmail->removeRow(i);
	}
	delete m_ui;
}

void CAdvancedNetParams::showEvent ( QShowEvent * event )
{
	//disable warning
    if(event == NULL)
    {
    }
	if(m_lUserID < 0)
	{
		//QMessageBox::information(this, tr("Please login firstly!"), tr("Please login firstly!."));
	    return;
	}
	getPPPoEInfo();
	getNTPInfo();
	getDDNSInfo();
}

int CAdvancedNetParams::getPPPoEInfo()
{
	NET_DVR_NETCFG_V30 netParams;
	QString qstrTemp;
	DWORD paramsLen;
	char strTemp[100];
	int iRet;

	memset(&netParams, 0, sizeof(NET_DVR_NETCFG_V30));
	iRet = NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_NETCFG_V30, m_lChannel, &netParams, sizeof(NET_DVR_NETCFG_V30), &paramsLen);
    if(iRet == 0)
    {
    	QMessageBox::information(this, tr("Please check!"), tr("Get PPPoE information unsuccessfully! error:%1").arg(iRet));
        return HPR_ERROR;
    }

	//PPPoE UserName
	memset(strTemp, 0, 100);
	sprintf(strTemp, "%s", netParams.struPPPoE.sPPPoEUser);
	m_ui->editPPPoEUserName->setText(QString(QLatin1String(strTemp)));
	//PPPoE  Passwd
	memset(strTemp, 0, 100);
	sprintf(strTemp, "%s", netParams.struPPPoE.sPPPoEPassword);
	m_ui->editPPPoEPasswd->setText(QString(QLatin1String(strTemp)));
	//PPPoE IP address
	memset(strTemp, 0, 100);
	sprintf(strTemp, "%s", netParams.struPPPoE.struPPPoEIP.sIpV4);
	m_ui->editPPPoEIP->setText(QString(QLatin1String(strTemp)));

	m_ui->editPPPoEPass2->setText(QString(QLatin1String("")));
	//whether  PPPoE is set
	if(netParams.struPPPoE.dwPPPOE == 0)
	{
		m_ui->editPPPoEUserName->setEnabled(FALSE);
		m_ui->editPPPoEPasswd->setEnabled(FALSE);
		m_ui->editPPPoEIP->setEnabled(FALSE);
		m_ui->editPPPoEPass2->setEnabled(FALSE);
		m_ui->checkBoPPPoE->setCheckState(Qt::Unchecked);
	}
	if(netParams.struPPPoE.dwPPPOE == 1)
	{
		m_ui->editPPPoEUserName->setEnabled(TRUE);
		m_ui->editPPPoEPasswd->setEnabled(TRUE);
		m_ui->editPPPoEIP->setEnabled(FALSE);
	    m_ui->editPPPoEPass2->setEnabled(TRUE);
		m_ui->checkBoPPPoE->setCheckState(Qt::Checked);
	}

	return HPR_OK;
}

//get NTP information
int CAdvancedNetParams::getNTPInfo()
{
	NET_DVR_NTPPARA NTPParams;
	DWORD uiParamsLen;
	int iRet;
	char strTemp[100];

	memset(&NTPParams, 0, sizeof(NET_DVR_NTPPARA));
	iRet = NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_NTPCFG, m_lChannel, &NTPParams, sizeof(NET_DVR_NTPPARA),  &uiParamsLen);
	if(iRet == 0)
	{
		KIT_DisplayErrorInfo("Get NTP", KIT_ERRPR_DIS_TYPE);
	    //QMessageBox::information(this, tr("Please check!"), tr(".Get NTP information unsuccessfully!"));
	    return HPR_ERROR;
	}

	//NTP Address
	memset(strTemp, 0, 100);
	sprintf(strTemp, "%s", NTPParams.sNTPServer);
	m_ui->editNTPAddr->setText(QString(QLatin1String(strTemp)));

	//NTP Port
	memset(strTemp, 0, 100);
    sprintf(strTemp, "%d", NTPParams.wNtpPort);
    m_ui->editNTPPort->setText(QString(QLatin1String(strTemp)));

    //interval time
    memset(strTemp, 0, 100);
    sprintf(strTemp, "%d", NTPParams.wInterval);
    m_ui->editNTFInterval->setText(QString(QLatin1String(strTemp)));

    //different time hour
    memset(strTemp, 0, 100);
    sprintf(strTemp, "%d", NTPParams.cTimeDifferenceH);
    m_ui->editTimeDifH->setText(QString(QLatin1String(strTemp)));
    //different tine mimute
    memset(strTemp, 0, 100);
    sprintf(strTemp, "%d", NTPParams.cTimeDifferenceM);
    m_ui->editTimeDifM->setText(QString(QLatin1String(strTemp)));

    if(NTPParams.byEnableNTP == 0)
    {
    	m_ui->editNTPAddr->setEnabled(FALSE);
    	m_ui->editNTPPort->setEnabled(FALSE);
    	m_ui->editNTFInterval->setEnabled(FALSE);
    	m_ui->editTimeDifH->setEnabled(FALSE);
    	m_ui->editTimeDifM->setEnabled(FALSE);
    	m_ui->checkBoNTP->setCheckState(Qt::Unchecked);
    }

    if(NTPParams.byEnableNTP == 1)
    {
      	m_ui->editNTPAddr->setEnabled(TRUE);
      	m_ui->editNTPPort->setEnabled(TRUE);
      	m_ui->editNTFInterval->setEnabled(TRUE);
      	m_ui->editTimeDifH->setEnabled(TRUE);
      	m_ui->editTimeDifM->setEnabled(TRUE);
      	m_ui->checkBoNTP->setCheckState(Qt::Checked);
    }
	return HPR_OK;
}

//set NTP information
int CAdvancedNetParams::setNTPInfo()
{
	NET_DVR_NTPPARA NTPParams;
	DWORD uiParamsLen;
	int iRet;
    int iTemp;

	memset(&NTPParams, 0, sizeof(NET_DVR_NTPPARA));
	iRet = NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_NTPCFG, m_lChannel, &NTPParams, sizeof(NET_DVR_NTPPARA),  &uiParamsLen);
	if(iRet == 0)
	{
		KIT_DisplayErrorInfo("Get NTP", KIT_ERRPR_DIS_TYPE);
	    //QMessageBox::information(this, tr("Please check!"), tr(".Get NTP information unsuccessfully!"));
	    return HPR_ERROR;
	}

	if(m_ui->checkBoNTP->isChecked())
	{
		NTPParams.byEnableNTP = 1;
                KIT_editToStr(m_ui->editNTPAddr, (BYTE *)&(NTPParams.sNTPServer), 64);
		KIT_editToInt(m_ui->editNTFInterval, &iTemp);   //time
		NTPParams.wInterval = iTemp;
		KIT_editToInt(m_ui->editTimeDifH, &iTemp);       //hour
		NTPParams.cTimeDifferenceH = iTemp;
		KIT_editToInt(m_ui->editTimeDifM, &iTemp);      //minute
		NTPParams.cTimeDifferenceM = iTemp;
		KIT_editToInt(m_ui->editNTPPort, &iTemp);        //port
		NTPParams.wNtpPort = iTemp;
	}
	else
	{
		NTPParams.byEnableNTP = 0;
	}

	iRet = NET_DVR_SetDVRConfig(m_lUserID, NET_DVR_SET_NTPCFG, m_lChannel, &NTPParams, sizeof(NET_DVR_NTPPARA));
	if(iRet == 0)
	{
	    iRet = NET_DVR_GetLastError();
		QMessageBox::information(this, tr("Set Net fail!"), tr("Set Net fail. \"%1\"").arg(iRet));
		return HPR_ERROR;
	}

	return HPR_OK;
}

//get DDNS information
int CAdvancedNetParams::getDDNSInfo()
{
	NET_DVR_DDNSPARA_V30 DDNSParams;
	DWORD uiParamsLen;
	int iRet;
	char strTemp[100];

	memset(&DDNSParams, 0, sizeof(NET_DVR_DDNSPARA_V30));
	iRet = NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_DDNSCFG_V30, m_lChannel, &DDNSParams, sizeof(NET_DVR_DDNSPARA_V30),  &uiParamsLen);
	if(iRet == 0)
	{
		KIT_DisplayErrorInfo("Get DDNS", KIT_ERRPR_DIS_TYPE);
	    //QMessageBox::information(this, tr("Please check!"), tr(".Get NTP information unsuccessfully!"));
	    return HPR_ERROR;
	}

	//DDNS Server Type
        if( /*DDNSParams.byHostIndex >= 0 &&*/ DDNSParams.byHostIndex <= 3)
	{
	    m_ui->comboDDNSType->setCurrentIndex((int)(DDNSParams.byHostIndex));
	}

	//DDNS Server Name
	memset(strTemp, 0, 100);
	sprintf(strTemp, "%s", DDNSParams.struDDNS[int(DDNSParams.byHostIndex)].sServerName);
	m_ui->editDDNSServerName->setText(QString(QLatin1String(strTemp)));

	//DDNS Domain
	memset(strTemp, 0, 100);
	sprintf(strTemp, "%s", DDNSParams.struDDNS[int(DDNSParams.byHostIndex)].sDomainName);
	m_ui->editDDNSDomainName->setText(QString(QLatin1String(strTemp)));

	//DDNS port
	memset(strTemp, 0, 100);
	sprintf(strTemp, "%d", DDNSParams.struDDNS[int(DDNSParams.byHostIndex)].wDDNSPort);
	m_ui->editDDNSPort->setText(QString(QLatin1String(strTemp)));

	//DDNS User Name
	memset(strTemp, 0, 100);
	sprintf(strTemp, "%s", DDNSParams.struDDNS[int(DDNSParams.byHostIndex)].sUsername);
	m_ui->editDDNSUserName->setText(QString(QLatin1String(strTemp)));

	//DDNS User passwd
	memset(strTemp, 0, 100);
	sprintf(strTemp, "%s", DDNSParams.struDDNS[int(DDNSParams.byHostIndex)].sPassword);
	m_ui->editDDNSPasswd->setText(QString(QLatin1String(strTemp)));

	//whether DDNS is set
   if(DDNSParams.byEnableDDNS == 0)
   {
	   m_ui->editDDNSDomainName->setEnabled(FALSE);
	   m_ui->editDDNSServerName->setEnabled(FALSE);
	   m_ui->editDDNSPasswd->setEnabled(FALSE);
	   m_ui->editDDNSPort->setEnabled(FALSE);
	   m_ui->editDDNSUserName->setEnabled(FALSE);
	   m_ui->editDDNSpasswd2->setEnabled(FALSE);
	   m_ui->comboDDNSType->setEnabled(FALSE);
	   m_ui->checkBoDDNS->setCheckState(Qt::Unchecked);
   }
   else
   {
       if(DDNSParams.byEnableDDNS == 1)
       {
    	   m_ui->editDDNSDomainName->setEnabled(TRUE);
    	   m_ui->editDDNSServerName->setEnabled(TRUE);
    	   m_ui->editDDNSPasswd->setEnabled(TRUE);
    	   m_ui->editDDNSPort->setEnabled(TRUE);
    	   m_ui->editDDNSUserName->setEnabled(TRUE);
    	   m_ui->editDDNSpasswd2->setEnabled(TRUE);
    	   m_ui->comboDDNSType->setEnabled(TRUE);
    	   m_ui->checkBoDDNS->setCheckState(Qt::Checked);
       }
       else
       {
    	   QMessageBox::information(this, tr("Device params error!"), tr("Device params error!"));
       }
   }
	return HPR_OK;
}

//set DDNS inforamtion
 int CAdvancedNetParams::setDDNSInfo()
 {
	 NET_DVR_DDNSPARA_V30 DDNSParams;
	 DWORD uiParamsLen;
	 int iIntTemp;
	 int iRet;

	 //firstly, get the DDNS
	memset(&DDNSParams, 0, sizeof(NET_DVR_DDNSPARA_V30));
	iRet = NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_DDNSCFG_V30, m_lChannel, &DDNSParams, sizeof(NET_DVR_DDNSPARA_V30),  &uiParamsLen);
    if(iRet == 0)
	 {
    	KIT_DisplayErrorInfo("Get DDNS", KIT_ERRPR_DIS_TYPE);
	     //QMessageBox::information(this, tr("Please check!"), tr(".Get NTP information unsuccessfully!"));
	     return HPR_ERROR;
	 }

    //secondly, set the DDNS
    if(m_ui->checkBoDDNS->isChecked())
    {
    	if((m_ui->editDDNSPasswd->text()) != (m_ui->editDDNSpasswd2->text()))
    	{
    	    QMessageBox::information(this, tr("Double DDNS passwd is not equal!"), tr("Please check!"));
    	    return HPR_ERROR;
    	 }
         DDNSParams.byEnableDDNS = 1;
         DDNSParams.byHostIndex = (m_ui->comboDDNSType->currentIndex());
         //Domain Name
         KIT_editToStr(m_ui->editDDNSDomainName,  DDNSParams.struDDNS[(int)DDNSParams.byHostIndex].sDomainName, MAX_DOMAIN_NAME);
         //Server Name
         KIT_editToStr(m_ui->editDDNSServerName,  DDNSParams.struDDNS[(int)DDNSParams.byHostIndex].sServerName, MAX_DOMAIN_NAME);
         //User Name
         KIT_editToStr(m_ui->editDDNSUserName,  DDNSParams.struDDNS[(int)DDNSParams.byHostIndex].sUsername, NAME_LEN);
         //Passward
         KIT_editToStr(m_ui->editDDNSPasswd,  DDNSParams.struDDNS[(int)DDNSParams.byHostIndex].sPassword, PASSWD_LEN);

         KIT_editToInt(m_ui->editDDNSPort, &iIntTemp);
         DDNSParams.struDDNS[(int)DDNSParams.byHostIndex].wDDNSPort = iIntTemp;
    }
    else
    {
    	DDNSParams.byEnableDDNS = 0;
    }

	iRet = NET_DVR_SetDVRConfig(m_lUserID, NET_DVR_SET_DDNSCFG_V30, m_lChannel, &DDNSParams, sizeof(NET_DVR_DDNSPARA_V30));
	if(iRet == 0)
	{
	    iRet = NET_DVR_GetLastError();
		QMessageBox::information(this, tr("Set DDNS fail!"), tr("Set DDNS fail. \"%1\"").arg(iRet));
		return HPR_ERROR;
	}

	 return HPR_OK;
 }

//refresh params
void CAdvancedNetParams::refreshParams()
{
	if(m_lUserID < 0)
	{
		QMessageBox::information(this, tr("Please check."), tr("User ID is wrong!"));
		return;
	}
	getNTPInfo();
	getPPPoEInfo();
	getDDNSInfo();
	getEmailInfo();
	//delete m_ui->tabWEmail->takeItem(0,0);
}

void CAdvancedNetParams::on_btnExit_clicked()
{
	close();
}

//save params
void CAdvancedNetParams::saveParams()
{
	if(m_lUserID < 0)
	{
		QMessageBox::information(this, tr("Please check."), tr("User ID is wrong!"));
		return;
	}
	//set PPPoE
	setPPPoEInfo();
	//set NTP
	setNTPInfo();
	//set DDNS
	setDDNSInfo();
	//set Email
	setEmailInfo();

}

int CAdvancedNetParams::setPPPoEInfo()
{
	NET_DVR_NETCFG_V30 netParams;
	QString qstrTemp;
	DWORD paramsLen;
	int iRet;

	//get the params
	memset(&netParams, 0, sizeof(NET_DVR_NETCFG_V30));
    iRet = NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_NETCFG_V30, m_lChannel, &netParams, sizeof(NET_DVR_NETCFG_V30), &paramsLen);

    //set the parms
    if(m_ui->checkBoPPPoE->isChecked())
    {
    	if((m_ui->editPPPoEPasswd->text()) != (m_ui->editPPPoEPass2->text()))
    	{
    		QMessageBox::information(this, tr("Double is net equal!"), tr("Please check!"));
            return HPR_ERROR;
    	}
    	netParams.struPPPoE.dwPPPOE = 1;
    	//user name
        KIT_editToStr(m_ui->editPPPoEUserName, (BYTE *)(&(netParams.struPPPoE.sPPPoEUser)), NAME_LEN);
    	//pass word
        KIT_editToStr(m_ui->editPPPoEPasswd, (BYTE *)&(netParams.struPPPoE.sPPPoEPassword), PASSWD_LEN);
    	//PPPoE IP Addess
    	//KIT_editToIP(m_ui->editPPPoEIP, (char *)&(netParams.struPPPoE.struPPPoEIP.sIpV4), 16);
    }
    else
    {
    	netParams.struPPPoE.dwPPPOE = 0;
    }

    iRet = NET_DVR_SetDVRConfig(m_lUserID, NET_DVR_SET_NETCFG_V30, m_lChannel, &netParams, sizeof(NET_DVR_NETCFG_V30));
    if(iRet == 0)
    {
    	iRet = NET_DVR_GetLastError();
	    QMessageBox::information(this, tr("Set Net fail!"), tr("Set Net fail. \"%1\"").arg(iRet));
	    return HPR_ERROR;
    }

	return HPR_OK;
}

//get the email information
int  CAdvancedNetParams::getEmailInfo()
{
	NET_DVR_EMAILCFG_V30 EmailParams;
	DWORD uiParamsLen;
	int iRet;
	char strTemp[100];

	memset(&EmailParams, 0, sizeof(NET_DVR_EMAILCFG_V30));
	iRet = NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_EMAILCFG_V30, m_lChannel, &EmailParams, sizeof(NET_DVR_EMAILCFG_V30),  &uiParamsLen);
	if(iRet == 0)
	{
	    QMessageBox::information(this, tr("Please check!"), tr(".Get Email information unsuccessfully!"));
	    return HPR_ERROR;
	}

	//user name
	memset(strTemp, 0, 100);
	sprintf(strTemp, "%s", EmailParams.sAccount);
	m_ui->editEmailAccount->setText(QString(QLatin1String(strTemp)));

	//Email pass word
	memset(strTemp, 0, 100);
	sprintf(strTemp, "%s", EmailParams.sPassword);
	m_ui->editEmailPasswd->setText(QString(QLatin1String(strTemp)));

	//Email sender name
	memset(strTemp, 0, 100);
	sprintf(strTemp, "%s", EmailParams.struSender.sName);
	m_ui->editEmailSenderName->setText(QString(QLatin1String(strTemp)));

	//Email sender address
	memset(strTemp, 0, 100);
	sprintf(strTemp, "%s", EmailParams.struSender.sAddress);
	m_ui->editEmailSenderAddr->setText(QString(QLatin1String(strTemp)));

	//smtp
	memset(strTemp, 0, 100);
	sprintf(strTemp, "%s", EmailParams.sSmtpServer);
	m_ui->editEmailSMTP->setText(QString(QLatin1String(strTemp)));

	//pop3
	memset(strTemp, 0, 100);
	sprintf(strTemp, "%s", EmailParams.sPop3Server);
	m_ui->editEmailPop3->setText(QString(QLatin1String(strTemp)));

	//interval time
        if(/*EmailParams.byMailInterval >= 0 && */ EmailParams.byMailInterval <= 3)
	{
		m_ui->comboBoEmailInterval->setCurrentIndex((int)(EmailParams.byMailInterval));
	}

	//whether need to verify
	if(EmailParams.bySmtpServerVerify == 0)
	{
		m_ui->checkBoEmailsmtp->setCheckState(Qt::Unchecked);
	}
	else
	{
		m_ui->checkBoEmailsmtp->setCheckState(Qt::Checked);
	}

	if(EmailParams.byAttachment == 0)
	{
		m_ui->checkBoEmailAttach->setCheckState(Qt::Unchecked);
	}
	else
	{
		m_ui->checkBoEmailAttach->setCheckState(Qt::Checked);
	}

	//delete the table
	int i,j;
	for(i = 0;  i <3; i++ )
	{
		for(j=0; j<2; j++)
		{
			delete m_ui->tabWEmail->takeItem(0,0);
		}
	    //m_ui->tabWEmail->removeRow(i);
	}

	//write data to the table widget
	for(i = 0;  i <3; i++ )
	{
                m_ui->tabWEmail->setItem(i, 0, new QTableWidgetItem((char *)EmailParams.struReceiver[i].sName));
                m_ui->tabWEmail->setItem(i, 1, new QTableWidgetItem((char *)EmailParams.struReceiver[i].sAddress));
	}

	//repait the table
	m_ui->tabWEmail->hide();
	m_ui->tabWEmail->show();

    return HPR_OK;
}

//set the email information
int  CAdvancedNetParams::setEmailInfo()
{
	NET_DVR_EMAILCFG_V30 EmailParams;
	DWORD uiParamsLen;
	int iRet;

    memset(&EmailParams, 0, sizeof(NET_DVR_EMAILCFG_V30));
	iRet = NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_EMAILCFG_V30, m_lChannel, &EmailParams, sizeof(NET_DVR_EMAILCFG_V30),  &uiParamsLen);
	if(iRet == 0)
	{
	    QMessageBox::information(this, tr("Please check!"), tr(".Get Email information unsuccessfully!"));
	    return HPR_ERROR;
	}
	if(m_ui->editEmailPasswd->text() != m_ui->editEmailPasswd2->text())
	{
		QMessageBox::information(this, tr("Double Email passwd is net equal!"), tr("Please check!"));
		return HPR_ERROR;
	}
	KIT_editToStr(m_ui->editEmailAccount, EmailParams.sAccount, NAME_LEN);
	KIT_editToStr(m_ui->editEmailPasswd, EmailParams.sPassword, MAX_EMAIL_PWD_LEN);
    KIT_editToStr(m_ui->editEmailSenderName, EmailParams.struSender.sName, NAME_LEN);
    KIT_editToStr(m_ui->editEmailSenderAddr, EmailParams.struSender.sAddress, MAX_EMAIL_ADDR_LEN);

    KIT_editToStr(m_ui->editEmailSMTP, EmailParams.sSmtpServer, MAX_EMAIL_ADDR_LEN);
    KIT_editToStr(m_ui->editEmailPop3, EmailParams.sPop3Server, MAX_EMAIL_ADDR_LEN);

    if(m_ui->checkBoEmailAttach->isChecked())
    {
    	EmailParams.byAttachment = 1;
    }
    else
    {
    	EmailParams.byAttachment = 0;
    }

    if(m_ui->checkBoEmailsmtp->isChecked())
    {
    	EmailParams.bySmtpServerVerify =1;
    }
    else
    {
    	EmailParams.bySmtpServerVerify = 0;
    }

    //interval time
    EmailParams.byMailInterval = m_ui->comboBoEmailInterval->currentIndex();

    //receiver
    QTableWidgetItem *pItemTemp;
    int i;
    for(i = 0;  i <3; i++ )
    {
    	pItemTemp = m_ui->tabWEmail->item(i, 0);
    	if(pItemTemp !=NULL)
    	{
            //KIT_qstringToStr(&(pItemTemp->text()), EmailParams.struReceiver[i].sName, NAME_LEN);
    	}
    	else
    	{
    		memset(EmailParams.struReceiver[i].sName, 0, NAME_LEN);
    	}

    	pItemTemp = m_ui->tabWEmail->item(i, 1);
        if(pItemTemp !=NULL)
    	{
            //KIT_qstringToStr(&(pItemTemp->text()), EmailParams.struReceiver[i].sAddress, MAX_EMAIL_ADDR_LEN);
    	}
    	else
    	{
    	    memset(EmailParams.struReceiver[i].sAddress, 0, MAX_EMAIL_ADDR_LEN);
    	}
   }

    iRet = NET_DVR_SetDVRConfig(m_lUserID, NET_DVR_SET_EMAILCFG_V30, m_lChannel, &EmailParams, sizeof(NET_DVR_EMAILCFG_V30));
    if(iRet == 0)
    {
       	iRet = NET_DVR_GetLastError();
   	    QMessageBox::information(this, tr("Set Email fail!"), tr("Set Email fail. \"%1\"").arg(iRet));
   	    return HPR_ERROR;
    }

    return HPR_OK;
}

void CAdvancedNetParams::clickPPPoE()
{
	if(m_ui->checkBoPPPoE->isChecked())
	{
		 m_ui->editPPPoEUserName->setEnabled(TRUE);
	     m_ui->editPPPoEPasswd->setEnabled(TRUE);
	     m_ui->editPPPoEIP->setEnabled(FALSE);
	     m_ui->editPPPoEPass2->setEnabled(TRUE);

	 }
	 else
	 {
		 m_ui->editPPPoEUserName->setEnabled(FALSE);
		 m_ui->editPPPoEPasswd->setEnabled(FALSE);
		 m_ui->editPPPoEIP->setEnabled(FALSE);
		 m_ui->editPPPoEPass2->setEnabled(FALSE);
	  }
}

void CAdvancedNetParams::clickNTP()
{
	if(m_ui->checkBoNTP->isChecked())
	{
		m_ui->editNTPAddr->setEnabled(TRUE);
		m_ui->editNTPPort->setEnabled(TRUE);
		m_ui->editNTFInterval->setEnabled(TRUE);
		m_ui->editTimeDifH->setEnabled(TRUE);
		m_ui->editTimeDifM->setEnabled(TRUE);
	}
	else
	{
		m_ui->editNTPAddr->setEnabled(FALSE);
		m_ui->editNTPPort->setEnabled(FALSE);
		m_ui->editNTFInterval->setEnabled(FALSE);
		m_ui->editTimeDifH->setEnabled(FALSE);
		m_ui->editTimeDifM->setEnabled(FALSE);
	}
}

//whether choose DDNS
void CAdvancedNetParams::clickDDNS()
{
	if(m_ui->checkBoDDNS->isChecked())
	{
		m_ui->editDDNSDomainName->setEnabled(TRUE);
		m_ui->editDDNSServerName->setEnabled(TRUE);
		m_ui->editDDNSPasswd->setEnabled(TRUE);
		m_ui->editDDNSPort->setEnabled(TRUE);
		m_ui->editDDNSUserName->setEnabled(TRUE);
		m_ui->editDDNSpasswd2->setEnabled(TRUE);
		m_ui->comboDDNSType->setEnabled(TRUE);

	}
	else
	{
		m_ui->editDDNSDomainName->setEnabled(FALSE);
		m_ui->editDDNSServerName->setEnabled(FALSE);
		m_ui->editDDNSPasswd->setEnabled(FALSE);
		m_ui->editDDNSPort->setEnabled(FALSE);
		m_ui->editDDNSUserName->setEnabled(FALSE);
		m_ui->editDDNSpasswd2->setEnabled(FALSE);
	    m_ui->comboDDNSType->setEnabled(FALSE);
	}

}

int CAdvancedNetParams::setUserIDAndChannel(long lUserID, long lChannel)
{
	m_lUserID = lUserID;
	m_lChannel = lChannel;

	return HPR_OK;
}

