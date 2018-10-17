
#include <QMessageBox>

#include "cnfsparams.h"
#include "publicfuc.h"

CNfsParams::CNfsParams(QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);

	memset(&m_NFSParams, 0, sizeof(NET_DVR_NFSCFG));
	m_lUserID = -1;
	m_lChannel = 1;
	m_bGetInfo = 0;

	QStringList strings;
	strings<<"1"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7"<<"8";
	ui.comboDiskNum->addItems(strings);

	connect(ui.comboDiskNum, SIGNAL(activated(int)), this, SLOT(comboChanged (int)));
	connect(ui.btnSave,  SIGNAL(clicked()), this, SLOT(clickSave()));
	connect(ui.btnRefresh,  SIGNAL(clicked()), this, SLOT(clickRefresh()));
}

CNfsParams::~CNfsParams()
{

}

void  CNfsParams::clickSave()
{
	if(m_lUserID < 0)
	{
		return;
	}
	setNFSInfo();

}
void  CNfsParams::clickRefresh()
{
	if(m_lUserID < 0)
	{
		return;
	}
	getNFSInfo();

}

void CNfsParams::on_btnExit_clicked()
{
	close();
}


void CNfsParams::comboChanged ( int index )
{
	//QMessageBox::information(this, tr("Change!"), tr("Change"));
	if(m_bGetInfo)
	{
		ui.editServerIP->setText(m_NFSParams.struNfsDiskParam[index].sNfsHostIPAddr);
                ui.editFilePath->setText((char *)m_NFSParams.struNfsDiskParam[index].sNfsDirectory);
	}
	else
	{
		QMessageBox::information(this, tr("Please check"), tr("Get params error, please click update"));
	}
}


void CNfsParams::showEvent ( QShowEvent * event )
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
		getNFSInfo();
	     // ui.comboDiskNum->setCurrentIndex(1);
	}
}
int CNfsParams::getNFSInfo()
{
	//define nfs variable
	DWORD uiReturn;
	int iRet;

	iRet = NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_NFSCFG, m_lChannel,  &m_NFSParams, sizeof(NET_DVR_NFSCFG), &uiReturn);
	if(iRet == 0)
	{
		KIT_DisplayErrorInfo("Get NFS", KIT_ERRPR_DIS_TYPE);
		//QMessageBox::information(this, tr("Please check!"), tr(".Get NFS information unsuccessfully!"));
		return HPR_ERROR;
	}
	m_bGetInfo = 1; //get information successfully.
	ui.comboDiskNum->setCurrentIndex(0);
	ui.editServerIP->setText(m_NFSParams.struNfsDiskParam[0].sNfsHostIPAddr);
    ui.editFilePath->setText((char *)m_NFSParams.struNfsDiskParam[0].sNfsDirectory);

	return HPR_OK;
}

int CNfsParams::setNFSInfo()
{
	int iComboIndex;
	int iRet;
	if(m_bGetInfo == 1)
	{
		iComboIndex = ui.comboDiskNum->currentIndex();
                KIT_editToIP(ui.editServerIP, (BYTE *)m_NFSParams.struNfsDiskParam[iComboIndex].sNfsHostIPAddr, 16);
		KIT_editToStr(ui.editFilePath, m_NFSParams.struNfsDiskParam[iComboIndex].sNfsDirectory, PATHNAME_LEN);

		iRet = NET_DVR_SetDVRConfig(m_lUserID, NET_DVR_SET_NFSCFG, m_lChannel,  &m_NFSParams, sizeof(NET_DVR_NFSCFG));
        if(iRet == 0)
        {
        	 iRet = NET_DVR_GetLastError();
        	 QMessageBox::information(this, tr("Set NFS fail!"), tr("Set NFS fail. \"%1\"").arg(iRet));
        	return HPR_ERROR;
        }
	}
	else
	{
		QMessageBox::information(this, tr("Please check"), tr("Get params error, please click update"));
	    return HPR_ERROR;
	}

	return HPR_OK;
}

int CNfsParams::setUserIDAndChannel(long lUserID, long lChannel)
{
	m_lUserID = lUserID;
	m_lChannel = lChannel;

	return HPR_OK;
}






