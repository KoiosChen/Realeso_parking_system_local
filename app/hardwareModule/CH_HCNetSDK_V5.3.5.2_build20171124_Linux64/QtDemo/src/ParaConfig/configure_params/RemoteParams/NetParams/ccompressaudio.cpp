#include "ccompressaudio.h"

#include <QMessageBox>
#include "publicfuc.h"
#include "DemoPublic.h"

CCompressAudio::CCompressAudio(QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);

	m_lUserID = -1;
	m_lChannel = -1;

	QStringList strings;
	strings<<"OggVorbis"<<"G711 ";
	ui.comboAudioType->addItems(strings);

	connect(ui.btnSave,  SIGNAL(clicked()), this, SLOT(clickSave()));
    connect(ui.btnRefresh,  SIGNAL(clicked()), this, SLOT(clickRefresh()));
}

CCompressAudio::~CCompressAudio()
{

}

//get CompressAudio info
int CCompressAudio::getCompressAudioInfo()
{
	NET_DVR_COMPRESSION_AUDIO  struCompressAudio;
	int iRet;
	DWORD uiRetParamsLen;

	iRet = NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_COMPRESSCFG_AUD, m_lChannel, &struCompressAudio, sizeof(NET_DVR_COMPRESSION_AUDIO), &uiRetParamsLen);
    if(iRet == 0)
    {
    	//QMessageBox::information(this, tr("Please check!"), tr("Get Audio information unsuccessfully!"));
    	KIT_DisplayErrorInfo("Get Audio information", KIT_ERRPR_DIS_TYPE);
    	return HPR_ERROR;
    }

    ui.comboAudioType->setCurrentIndex(int(struCompressAudio.byAudioEncType));

    return HPR_OK;
}

//set CompressAudio info
int CCompressAudio::setCompressAudioInfo()
{
	NET_DVR_COMPRESSION_AUDIO  struCompressAudio;
	int iRet;
	DWORD uiRetParamsLen;

	iRet = NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_COMPRESSCFG_AUD, m_lChannel, &struCompressAudio, sizeof(NET_DVR_COMPRESSION_AUDIO), &uiRetParamsLen);
	if(iRet == 0)
    {
		QMessageBox::information(this, tr("Please check!"), tr("Get Audio information unsuccessfully!"));
	    return HPR_ERROR;
    }

	struCompressAudio.byAudioEncType = ui.comboAudioType->currentIndex();

	iRet = NET_DVR_SetDVRConfig(m_lUserID, NET_DVR_SET_COMPRESSCFG_AUD, m_lChannel, &struCompressAudio, sizeof(NET_DVR_COMPRESSION_AUDIO));
	if(iRet == 0)
    {
		QMessageBox::information(this, tr("Please check!"), tr("Get Audio information unsuccessfully!"));
	    return HPR_ERROR;
    }

	return HPR_OK;
}

void  CCompressAudio::clickSave()
{
	if(m_lUserID < 0)
	{
		return;
	}
	setCompressAudioInfo();

}
void  CCompressAudio::clickRefresh()
{
	if(m_lUserID < 0)
	{
		return;
	}
	getCompressAudioInfo();

}

void CCompressAudio::on_btnExit_clicked()
{
	close();
}

void CCompressAudio::showEvent ( QShowEvent * event )
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
		getCompressAudioInfo();
	}
}

int CCompressAudio::setUserIDAndChannel(long lUserID, long lChannel)
{
	m_lUserID = lUserID;
	m_lChannel = lChannel;

	return HPR_OK;
}
