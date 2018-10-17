#include "crtspparams.h"
#include <QMessageBox>
#include "publicfuc.h"
#include "DemoPublic.h"

CRtspParams::CRtspParams(QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);

	m_lUserID = -1;

	 connect(ui.btnSave,  SIGNAL(clicked()), this, SLOT(clickSave()));
	 connect(ui.btnRefresh,  SIGNAL(clicked()), this, SLOT(clickRefresh()));
}

CRtspParams::~CRtspParams()
{

}

int CRtspParams::setUserID(long lUserID)
{
	m_lUserID = lUserID;
	return HPR_OK;
}

//get rtsp info
 int CRtspParams::getRtspInfo()
 {
	 NET_DVR_RTSPCFG  struRtspParams;
	 int iRet;

     iRet = NET_DVR_GetRtspConfig(m_lUserID, 0, &struRtspParams, sizeof(NET_DVR_RTSPCFG));
     if(iRet == 0)
     {
    	 KIT_DisplayErrorInfo("Get Rtsp information", KIT_ERRPR_DIS_TYPE);
    	 return HPR_ERROR;
     }

	 return HPR_OK;
 }

 //set rtsp info
 int CRtspParams::setRtspInfo()
 {
	 NET_DVR_RTSPCFG  struRtspParams;
	 int iRet;
	 int iPort;

	 iRet = NET_DVR_GetRtspConfig(m_lUserID, 0, &struRtspParams, sizeof(NET_DVR_RTSPCFG));
	 if(iRet == 0)
	 {
	     iRet = NET_DVR_GetLastError();
	     QMessageBox::information(this, tr("Get Rtsp fail!"), tr("Get Rtsp fail. \"%1\"").arg(iRet));
	     return HPR_ERROR;
	 }

	 KIT_editToInt(ui.editPort, &iPort);
	 struRtspParams.wPort = iPort;
	 iRet = NET_DVR_SetRtspConfig(m_lUserID, 0, &struRtspParams, sizeof(NET_DVR_RTSPCFG));
     if(iRet == 0)
	{
	    iRet = NET_DVR_GetLastError();
	    QMessageBox::information(this, tr("SetRtsp fail!"), tr("Set Rtsp fail. \"%1\"").arg(iRet));
		return HPR_ERROR;
    }

	  return HPR_OK;
 }

 void CRtspParams::showEvent ( QShowEvent * event )
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
 	else
 	{
 		getRtspInfo();
 	}
 }

void  CRtspParams::clickSave()
{
	if(m_lUserID < 0)
	{
		return;
	}
	setRtspInfo();

}

void CRtspParams::on_btnExit_clicked()
{
	close();
}

void  CRtspParams::clickRefresh()
{
	if(m_lUserID < 0)
	{
		return;
	}
	getRtspInfo();

}
