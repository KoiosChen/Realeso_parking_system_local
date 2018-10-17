/*
 * Copyright(C) 2009,Hikvision Digital Technology Co., Ltd 
 * 
 * 文件名称：cchannelparams.cpp
 * 描    述：
 * 当前版本：1.0
 * 作    者：潘亚东
 * 创建日期：2009年11月23日
 * 修改记录：
 */

#include <QMessageBox>
#include "cchannelparams.h"
#include "publicfuc.h"
#include "cdlgremoterecordschedule.h"
#include "cvilost.h"
#include "cshelter.h"
#include "cmotion.h"
#include "chidealarm.h"

/*******************************************************************
      Function:  CChannelParams::CChannelParams
     Parameter:  
           (IN)     QWidget *parent
           
   Description:   构造函数
**********************************************************************/
CChannelParams::CChannelParams(QWidget *parent) :
	QWidget(parent)
{
	ui.setupUi(this);

	m_lChannel = 0;
	m_lUserID = -1;
	m_iStartChan = 0;
	int i;
	for (i = 0; i < MAX_CHANNUM_V30; i++)
	{
		m_iArryChannel[i] = -1;
	}
	m_iSimChannelNum = 0;

    memset(&m_struRecord, 0, sizeof(NET_DVR_RECORD_V30));
    m_bSetRecord = FALSE;
    memset(&m_struVILost, 0, sizeof(NET_DVR_VILOST_V30));
    m_bVILost = FALSE;
    memset(m_struShelter, 0, MAX_SHELTERNUM*sizeof(NET_DVR_SHELTER));
    m_bShelter = FALSE;
    memset(&m_struMotion, 0, sizeof(NET_DVR_MOTION_V30));
    m_bMotion = FALSE;
    memset(&m_struHide, 0, sizeof(NET_DVR_HIDEALARM_V30));
    m_bHide = FALSE;

    ui.btnVILost->setEnabled(FALSE);

	QStringList strings;
	//compress type
	strings << "Main stream" << "Sub stream" << "Event";
	ui.comboPressType->addItems(strings);
	strings.clear();
	//pictur quality
	strings << "Best" << "Much better" << "Better" << "Normal" << "Bad" << "Worse";
	ui.comboPicQuality->addItems(strings);
	strings.clear();
	//video frame rate
	strings << "All" << "1/16" << "1/8" << "1/4" << "1/2" << "1" << "2" << "4"
			<< "6" << "8" << "10" << "12" << "16" << "20" << "15" << "18"
			<< "22";
	ui.comboVideoFrameRate->addItems(strings);
	strings.clear();
	//encode type
	strings << "hik264" << "h264" << "mpeg4";
	ui.comboVideoEncType->addItems(strings);
	strings.clear();
	//stream type
	strings << "Video" << "Video&Audio";
	ui.comboStreamType->addItems(strings);
	strings.clear();
	//video bit. 2-12
	strings << "32K" << "48k" << "64K" << "80K" << "96K" << "128K" << "160k"
			<< "192K" << "224K" << "256K" << "320K";
	//13-23
	strings << "384K" << "448K" << "512K" << "640K" << "768K" << "896K"
			<< "1024K" << "1280K" << "1536K" << "1792K" << "2048K";
	strings << "Self definition";
	ui.comboVideoBit->addItems(strings);
	strings.clear();
	//Audio Encode Type
	strings << "OggVorbis";
	ui.comboAudioEncType->addItems(strings);
	strings.clear();
	//Resolution
	strings << "DCIF" << "CIF" << "QCIF" << "4CIF" << "2CIF" << "VGA" << "UXGA"
			<< "SVGA" << "HD720p" << "XVGA" << "HD900p";
	ui.comboResolution->addItems(strings);
	strings.clear();
	//byBitrateType
	strings << "VBR" << "CBR";
	ui.comboBitrateType->addItems(strings);
	strings.clear();
	//IntervalBPFrame
	strings << "BBP" << "BP" << "P";
	ui.comboIntervalBPFrame->addItems(strings);
	strings.clear();

	//预录时间：0-不预录，1-5秒，2-10秒，3-15秒，4-20秒，5-25秒，6-30秒，7-0xffffffff(尽可能预录)
	strings<<"Never"<<"5s"<<"10s"<<"15s"<<"20s"<<"25s"<<"30s"<<"No limit";
	ui.comboPreRecordTime->addItems(strings);
	strings.clear();

	//录象延时时间，0-5秒， 1-20秒， 2-30秒， 3-1分钟， 4-2分钟， 5-5分钟， 6-10分钟
	strings<<"5s"<<"20s"<<"30s"<<"1min"<<"2min"<<"5min"<<"10min";
	ui.comboRecordDelay->addItems(strings);
	strings.clear();

	//OSD日期格式
	strings<<"year-month-day"<<"month-day-year"<<"XXXXyearXXmonthXXday"<<"XXmonthXXdayXXXXyear"<<"day-month-year"<<"XXdayXXmonthXXXXyear";
    ui.comboOSDType->addItems(strings);
    strings.clear();

    //OSD属性
    strings<<"Transparent&Twinkle"<<"Transparent&Untwinkle"<<"Untarnsparent&Twinkle"<<"Untransparent&Untwinkle";
    ui.comboOSDAttrib->addItems(strings);
    strings.clear();

    //小时制
    strings<<"24hours"<<"AM/PM";
    ui.comboHourOsdType->addItems(strings);
    strings.clear();

    //字符叠加区域
    strings<<"1"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7"<<"8";
    ui.comboStringNo->addItems(strings);
    strings.clear();

	//combox
	ui.editVideoBit->setEnabled(FALSE);
	connect(ui.comboChannelNo, SIGNAL(activated(int)), this, SLOT(comboChNoChanged(int)));
	connect(ui.comboPressType, SIGNAL(activated(int)), this, SLOT(comboCompChanged(int)));
	connect(ui.comboVideoBit,  SIGNAL(activated(int)), this, SLOT(comboVideoBitChanged (int)));
	connect(ui.comboStringNo,  SIGNAL(activated(int)), this, SLOT(comboStringNoChanged(int)));

	//button
	connect(ui.btnRemoteRecord, SIGNAL(clicked()), this, SLOT(clickBtnRemoteRecord()));
	connect(ui.btnVILost,  SIGNAL(clicked()), this, SLOT(clickBtnVILost()));
    connect(ui.btnShelter, SIGNAL(clicked()), this, SLOT(clickBtnShelter()));

	//checkbox
	connect(ui.checkboVideoRecord,  SIGNAL(clicked()), this, SLOT(clickCheckVideoRecord()));
	connect(ui.checkboShowChanName, SIGNAL(clicked()), this, SLOT(clickCheckShowChanName()));
    connect(ui.checkboVILost, SIGNAL(clicked()), this, SLOT(clickCheckVILost()));
    connect(ui.checkShelter,  SIGNAL(clicked()), this, SLOT(clickCheckShelter()));
    connect(ui.checkMotion, SIGNAL(clicked()), this, SLOT(clickCheckMotion()));
    connect(ui.checkHide,   SIGNAL(clicked()), this, SLOT(clickCheckHide()));
    connect(ui.checkShowOsd, SIGNAL(clicked()), this, SLOT(clickCheckOSD()));
    connect(ui.checkShowString, SIGNAL(clicked()), this, SLOT(clickCheckShowString()));
}


/*******************************************************************
      Function:  CChannelParams::~CChannelParams
     Parameter:  
           (IN)     无参数  
   Description:  析构函数 
**********************************************************************/
CChannelParams::~CChannelParams()
{

}

/*******************************************************************
      Function:  CChannelParams::getAllInfo
     Parameter:  
           (IN)     无参数  
        Return:   HPR_OK--成功，HPR_ERROR--失败。
   Description:   获取压缩参数，录像参数，图像参数信息
**********************************************************************/
int CChannelParams::getAllInfo()
{
	if (m_lUserID < 0) {
		return HPR_ERROR;
	}
	//获取通道列表到combox中
	getChannelNum();

	//Compress Infomation.
	getCompressInfo(0);   //图像压缩参数
	getRecordInfo();      //录像参数
	getPictureInfo();     //图像参数
	getShowStringInfo();  //叠加字符参数
	return HPR_OK;
}

/*******************************************************************
      Function:  CChannelParams::setAllInfo
     Parameter:  
           (IN)     无参数  
        Return:   HPR_OK--成功，HPR_ERROR--失败。
   Description:   将控件中值取出，并设置到设备中去
**********************************************************************/
int CChannelParams::setAllInfo()
{
	if (m_lUserID < 0)
	{
		return HPR_ERROR;
	}
	setCompressInfo();
	setRecordInfo();
	setPictureInfo();
	setShowStringInfo();

	return HPR_OK;
}

/*******************************************************************
      Function:  CChannelParams::getChannelNum
     Parameter:  
           (IN)     无参数  
        Return:  HPR_OK--成功，HPR_ERROR--失败。
   Description:  获取模拟通道和IP通道的总数目
**********************************************************************/
int CChannelParams::getChannelNum() {
	NET_DVR_DEVICECFG struDeviceParams;
	DWORD paramsLen;
	int iRet;

	//***1.Get the channel number and IP channel number.***//
	iRet = NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_DEVICECFG, m_lChannel,
			&struDeviceParams, sizeof(NET_DVR_DEVICECFG), &paramsLen);
	if (iRet == 0)
	{
		iRet = NET_DVR_GetLastError();
		QMessageBox::information(this, tr("Please check!"), tr("Get device parameters error!"));
		return HPR_ERROR;
	}
	m_iStartChan = (int) (struDeviceParams.byStartChan);

	//***2.Get the channel which is enabled.***//
	NET_DVR_IPPARACFG struIPParams;
	iRet = NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_IPPARACFG, m_lChannel,
			&struIPParams, sizeof(NET_DVR_IPPARACFG), &paramsLen);
	if (iRet == 0)
	{
		//iRet = NET_DVR_GetLastError();
		//QMessageBox::information(this, tr("Please check!"), tr("Get IP channel parameters error.\"%1\"").arg(iRet));
		//return HPR_ERROR;
	}

	int i, j;
	//清理通道号信息
	for (i = 0; i < MAX_CHANNUM_V30; i++)
	{
		m_iArryChannel[i] = -1;
	}

	j = 0;
	if (iRet != 0)//if support ip channel
	{
		//channel
		for (i = 0; i < MAX_ANALOG_CHANNUM; i++)
		{
			if (struIPParams.byAnalogChanEnable[i])
			{
				m_iArryChannel[j] = i + m_iStartChan;
				j++;
			}
		}
		m_iSimChannelNum = j;
		//IP channel

		for (i = 0; i < MAX_IP_CHANNEL; i++)
		{
			if (struIPParams.struIPChanInfo[i].byIPID != 0)
			{
				m_iArryChannel[j] = i + m_iStartChan + MAX_ANALOG_CHANNUM;
				j++;
			}
		}
	}
	else//if not support ip channel
	{
		//simulate channel
		for (i = 0; (i < MAX_ANALOG_CHANNUM)
				&& (i < struDeviceParams.byChanNum); i++)
		{
			m_iArryChannel[j] = i + m_iStartChan;
			j++;
		}
		m_iSimChannelNum = j;
	}

	//***3. Add items to combox.***//
	char strTemp[100] = { 0 };
	QStringList strings;
	for (i = 0; i < MAX_CHANNUM_V30; i++)
	{
		if (m_iArryChannel[i] >= 0) {
			if (m_iArryChannel[i] < MAX_ANALOG_CHANNUM) //channel
			{
				sprintf(strTemp, "Camora%d", m_iArryChannel[i]);
				strings << strTemp;
			} else//IP channel
			{
				if (iRet != 0)//supprot IP channel
				{
					sprintf(strTemp, "IPCamora%d", (m_iArryChannel[i] - MAX_ANALOG_CHANNUM));
					strings << strTemp;
				}
			}
		}
	}

	//QMessageBox::information(this, "Please check!", tr("getChannelNum() user id is \"%1\" ").arg(m_lUserID));
	ui.comboChannelNo->clear();
	ui.comboChannelNo->addItems(strings);

	return HPR_OK;
}

/*******************************************************************
      Function:   CChannelParams::getCompressInfo
     Parameter:  
           (IN)     int iCompIndex  
        Return:   HPR_OK--成功，HPR_ERROR--失败。
   Description:   获取压缩参数
**********************************************************************/
int CChannelParams::getCompressInfo(int iCompIndex) {
	NET_DVR_COMPRESSIONCFG_V30 struCompress;
	int iRet;
	DWORD uiRetParamsLen;

	iRet = NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_COMPRESSCFG_V30,
			m_lChannel, &struCompress, sizeof(NET_DVR_COMPRESSIONCFG_V30),
			&uiRetParamsLen);
	if (iRet == 0) {
		iRet = NET_DVR_GetLastError();
		QMessageBox::information(this, tr("Please check!"), tr(
				"Get press parameters error! \"%1\"").arg(iRet));
		return HPR_ERROR;
	}

	ui.comboPressType->setCurrentIndex(iCompIndex);
	//Picture quality
	LPNET_DVR_COMPRESSION_INFO_V30 pCompressInfo;
	if (iCompIndex == 0)//video (major code stream)
	{
		pCompressInfo = &(struCompress.struNormHighRecordPara);
	}
	if (iCompIndex == 1)//net
	{
		pCompressInfo = &(struCompress.struNetPara);
	}
	if (iCompIndex == 2)//event
	{
		pCompressInfo = &(struCompress.struEventRecordPara);
	}
	setEachCompWidget(pCompressInfo);

	return HPR_OK;
}

/*******************************************************************
      Function:   CChannelParams::setCompressInfo
     Parameter:  
           (IN)     无参数  
        Return:   HPR_OK--成功，HPR_ERROR--失败。
   Description:   设置压缩
**********************************************************************/
int CChannelParams::setCompressInfo()
{
	NET_DVR_COMPRESSIONCFG_V30 struCompress;
	int iRet;
	DWORD uiRetParamsLen;

	//get the compress information
	iRet = NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_COMPRESSCFG_V30,
			m_lChannel, &struCompress, sizeof(NET_DVR_COMPRESSIONCFG_V30),
			&uiRetParamsLen);
	if (iRet == 0)
	{
		iRet = NET_DVR_GetLastError();
		QMessageBox::information(this, tr("Please check!"), tr(
				"Get press parameters error! \"%1\"").arg(iRet));
		return HPR_ERROR;
	}

	//set the compress information
	int iCompIndex;
	iCompIndex = ui.comboPressType->currentIndex();

	LPNET_DVR_COMPRESSION_INFO_V30 pCompressInfo;
	if (iCompIndex == 0)//video (major code stream)
	{
		pCompressInfo = &(struCompress.struNormHighRecordPara);
	}
	if (iCompIndex == 1)//net
	{
		pCompressInfo = &(struCompress.struNetPara);
	}
	if (iCompIndex == 2)//event
	{
		pCompressInfo = &(struCompress.struEventRecordPara);
	}
	getEachCompWidget(pCompressInfo);
	iRet = NET_DVR_SetDVRConfig(m_lUserID, NET_DVR_SET_COMPRESSCFG_V30,
			m_lChannel, &struCompress, sizeof(NET_DVR_COMPRESSIONCFG_V30));
	if (iRet == 0) {
		iRet = NET_DVR_GetLastError();
		QMessageBox::information(this, tr("Please check!"), tr(
				"Set press parameters error! \"%1\"").arg(iRet));
		return HPR_ERROR;
	}

	return HPR_OK;
}

/*******************************************************************
      Function:   CChannelParams::getRecordInfo
     Parameter:  
           (IN)     无参数  
        Return:   HPR_OK--成功，HPR_ERROR--失败。
   Description:   获取录像参数
**********************************************************************/
int CChannelParams::getRecordInfo()
{
	NET_DVR_RECORD_V30 struRecord;
	int iRet;
	DWORD uiRetParamsLen;

	//get the record information
	iRet = NET_DVR_GetDVRConfig(m_lUserID,
			    NET_DVR_GET_RECORDCFG_V30,
				m_lChannel,
				&struRecord,
				sizeof(NET_DVR_RECORD_V30),
				&uiRetParamsLen);
	if (iRet == 0)
	{
		iRet = NET_DVR_GetLastError();
		QMessageBox::information(this, tr("Please check!"), tr("Get press parameters error! \"%1\"").arg(iRet));
		return HPR_ERROR;
	}

	//***set the value of each record widget***//
	//是否录像
	if(struRecord.dwRecord == 0)
	{
		ui.checkboVideoRecord->setCheckState(Qt::Unchecked);
	}
	else
	{
		ui.checkboVideoRecord->setCheckState(Qt::Checked);
	}
	//录象延时时间
	ui.comboRecordDelay->setCurrentIndex(int(struRecord.dwRecordTime));
	//预录时间
	ui.comboPreRecordTime->setCurrentIndex(int(struRecord.dwPreRecordTime ));
	//录像保存的最长时间
	KIT_intToEdit(ui.editDurationTime, int(struRecord.dwRecorderDuration));
	//是否冗余录像
	if(struRecord.byRedundancyRec  == 0)
	{
		ui.checkboRedundancy->setCheckState(Qt::Unchecked);
	}
	else
	{
		ui.checkboRedundancy->setCheckState(Qt::Checked);
	}
	//录像时复合流编码时是否记录音频数据
	if(struRecord.byAudioRec == 0)
	{
		ui.checkAudioRec->setCheckState(Qt::Unchecked);
	}
	else
	{
		ui.checkAudioRec->setCheckState(Qt::Checked);
	}

	return HPR_OK;
}

/*******************************************************************
      Function:   CChannelParams::setRecordInfo
     Parameter:  
           (IN)     无参数  
        Return:   HPR_OK--成功，HPR_ERROR--失败。
   Description:   设置录像参数
**********************************************************************/
int CChannelParams::setRecordInfo()
{
	NET_DVR_RECORD_V30 struRecord;
	int iRet;
	DWORD uiRetParamsLen;

	//1.get the record information
	iRet = NET_DVR_GetDVRConfig(m_lUserID,
				    NET_DVR_GET_RECORDCFG_V30,
					m_lChannel,
					&struRecord,
					sizeof(NET_DVR_RECORD_V30),
					&uiRetParamsLen);
	if (iRet == 0)
	{
		iRet = NET_DVR_GetLastError();
		QMessageBox::information(this, tr("Please check!"), tr("Get press parameters error! \"%1\"").arg(iRet));
		return HPR_ERROR;
	}
	//2.取控件中的值
	//(1)是否定时录像
	if(ui.checkboVideoRecord->isChecked())
	{
		if(m_bSetRecord)
		{
			memcpy(&struRecord, &m_struRecord, sizeof(NET_DVR_RECORD_V30));
		}
		struRecord.dwRecord =1;
	}
	else
	{
		struRecord.dwRecord = 0;
	}
	//(2)保存天数
	int iTemp;
	KIT_editToInt(ui.editDurationTime, &iTemp);
	struRecord.dwRecorderDuration = (unsigned int)iTemp;
	//(3)预览时间
	struRecord.dwPreRecordTime = ui.comboPreRecordTime->currentIndex();
	//(4)延迟录像
	struRecord.dwRecordTime = ui.comboRecordDelay->currentIndex();
	//(5)是否冗余录像
	if(ui.checkboRedundancy->isChecked())
	{
		struRecord.byRedundancyRec  = 1;
	}
	else
	{
		struRecord.byRedundancyRec = 0;
	}
	//(6)是否音频
	if(ui.checkAudioRec->isChecked())
	{
		struRecord.byAudioRec = 1;
	}
	else
	{
		struRecord.byAudioRec = 0;
	}

	//3.设置
	iRet = NET_DVR_SetDVRConfig(m_lUserID,
				    NET_DVR_SET_RECORDCFG_V30,
					m_lChannel,
					&struRecord,
					sizeof(NET_DVR_RECORD_V30));
	if (iRet == 0)
	{
		iRet = NET_DVR_GetLastError();
		QMessageBox::information(this, tr("Please check!"), tr("Set press parameters error! \"%1\"").arg(iRet));
		return HPR_ERROR;
	}

	return HPR_OK;
}

/*******************************************************************
      Function:   CChannelParams::getPictureInfo
     Parameter:  
           (IN)     无参数  
        Return:   HPR_OK--成功，HPR_ERROR--失败。
   Description:   获取图像参数
**********************************************************************/
int CChannelParams::getPictureInfo()
{
	NET_DVR_PICCFG_V30 struPictureParams;
	int iRet;
	DWORD uiRetParamsLen;

	//获取图像参数
	iRet = NET_DVR_GetDVRConfig(m_lUserID,
			    NET_DVR_GET_PICCFG_V30,
				m_lChannel,
				&struPictureParams,
				sizeof(NET_DVR_PICCFG_V30),
				&uiRetParamsLen);
	if (iRet == 0)
    {
		iRet = NET_DVR_GetLastError();
		QMessageBox::information(this, tr("Please check!"), tr("Get press parameters error! \"%1\"").arg(iRet));
		return HPR_ERROR;
	}

	//***设置控件中的值***/
        //通道名称
        ui.editChannelName->setText((char *)struPictureParams.sChanName);

	//是否显示通道名称
	if(struPictureParams.dwShowChanName == 0)
	{
		ui.checkboShowChanName->setCheckState(Qt::Unchecked);
	}
	else
	{
		ui.checkboShowChanName->setCheckState(Qt::Checked);
	}
	//显示通道名称的坐标
	unsigned short usTemp;
	usTemp = (unsigned short)(struPictureParams.wShowNameTopLeftX);
	KIT_unintToEdit(ui.editShowNameTopLeftX, usTemp);
	usTemp = (unsigned short)(struPictureParams.wShowNameTopLeftY);
	KIT_unintToEdit(ui.editShowNameTopLeftY, usTemp);

	//视频信号丢失报警
	if(struPictureParams.struVILost.byEnableHandleVILost == 0)
	{
		ui.checkboVILost->setChecked(FALSE);
        ui.btnVILost->setEnabled(FALSE);
	}
	else
	{
		ui.checkboVILost->setChecked(TRUE);
		ui.btnVILost->setEnabled(TRUE);
	}

	//是否遮挡
	if(struPictureParams.dwEnableHide == 0)
	{
		ui.checkShelter->setChecked(FALSE);
		ui.btnShelter->setEnabled(FALSE);
	}
	else
	{
		ui.checkShelter->setChecked(TRUE);
		ui.btnShelter->setEnabled(TRUE);
	}

	//是否移动侦测
	if(struPictureParams.struMotion.byEnableHandleMotion == 0)
	{
		ui.checkMotion->setChecked(FALSE);
		ui.btnMotion->setEnabled(FALSE);
	}
	else
	{
		ui.checkMotion->setChecked(TRUE);
		ui.btnMotion->setEnabled(TRUE);
	}

	//遮挡报警
	if(struPictureParams.struHideAlarm.dwEnableHideAlarm == 0)
	{
		ui.checkHide->setChecked(FALSE);
		ui.btnHide->setEnabled(FALSE);
	}
	else
	{
		ui.checkHide->setChecked(TRUE);
		ui.btnHide->setEnabled(TRUE);
	}

	//OSD显示
    if(struPictureParams.dwShowOsd == 0)
    {
    	ui.checkShowOsd->setChecked(FALSE);
    	clickCheckOSD();
    }
    else
    {
    	ui.checkShowOsd->setChecked(TRUE);
    	clickCheckOSD();
    }
    //X,Y的OSD坐标
    int iTemp;
    iTemp = (int)struPictureParams.wOSDTopLeftX;
    KIT_intToEdit(ui.editOSDTopLeftX, iTemp);
    iTemp = (int)struPictureParams.wOSDTopLeftY;
    KIT_intToEdit(ui.editOSDTopLeftY, iTemp);
    //OSD类型
    ui.comboOSDType->setCurrentIndex(struPictureParams.byOSDType);
    //是否显示星期
    if(struPictureParams.byDispWeek == 0)
    {
    	ui.checkDispWeek->setChecked(FALSE);
    }
    else
    {
    	ui.checkDispWeek->setChecked(TRUE);
    }
    //OSD属性
    ui.comboOSDAttrib->setCurrentIndex(struPictureParams.byOSDAttrib - 1);
    //小时制
    ui.comboHourOsdType->setCurrentIndex(struPictureParams.byHourOSDType);

	return HPR_OK;
}

/*******************************************************************
      Function:   CChannelParams::setPictureInfo
     Parameter:  
           (IN)     无参数  
        Return:   HPR_OK--成功，HPR_ERROR--失败。
   Description:   设置图像参数
**********************************************************************/
int CChannelParams::setPictureInfo()
{
	NET_DVR_PICCFG_V30 struPictureParams;
	int iRet;
	DWORD uiRetParamsLen;

	//1.获取图像参数
	iRet = NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_PICCFG_V30, m_lChannel,
			&struPictureParams, sizeof(NET_DVR_PICCFG_V30), &uiRetParamsLen);
	if (iRet == 0)
	{
		iRet = NET_DVR_GetLastError();
		QMessageBox::information(this, tr("Please check!"), tr(
				"Get press parameters error! \"%1\"").arg(iRet));
		return HPR_ERROR;
	}

	//2.获取控件中值
	//通道名称
	KIT_editToStr(ui.editChannelName, struPictureParams.sChanName, NAME_LEN);
        int iTemp = 0;

	//是否显示通道名称，及其显示区域
	if(ui.checkboShowChanName->isChecked())
	{
		struPictureParams.dwShowChanName = 1;
		KIT_editToInt(ui.editShowNameTopLeftX, &iTemp);
		struPictureParams.wShowNameTopLeftX = (unsigned  short)iTemp;
		KIT_editToInt(ui.editShowNameTopLeftY, &iTemp);
		struPictureParams.wShowNameTopLeftY = (unsigned short)iTemp;
	}
	else
	{
		struPictureParams.dwShowChanName = 0;
	}

	//是否设置视频信号丢失参数
	if(ui.checkboVILost->isChecked())
	{
		if(m_bVILost)
		{
			memcpy(&(struPictureParams.struVILost), &(m_struVILost), sizeof(NET_DVR_VILOST_V30));
		}
		struPictureParams.struVILost.byEnableHandleVILost = 1;
	}
	else
	{
		struPictureParams.struVILost.byEnableHandleVILost = 0;
	}

	//是否设置遮挡
	if(ui.checkShelter->isChecked())
	{
		if(m_bShelter)
		{
			memcpy(struPictureParams.struShelter, m_struShelter, MAX_SHELTERNUM*sizeof(NET_DVR_SHELTER));
		}
		struPictureParams.dwEnableHide = 1;
	}
	else
	{
		struPictureParams.dwEnableHide = 0;
	}

	//是否设置移动侦测
	if(ui.checkMotion->isChecked())
	{
		if(m_bMotion)
		{
			memcpy(&(struPictureParams.struMotion), &(m_struMotion), sizeof(NET_DVR_MOTION_V30));
		}
		struPictureParams.struMotion.byEnableHandleMotion = 1;
	}
	else
	{
		struPictureParams.struMotion.byEnableHandleMotion = 0;
	}

	//设置遮挡报警
	if(ui.checkHide->isChecked())
	{
		if(m_bHide)
		{
			memcpy(&(struPictureParams.struHideAlarm), &(m_struHide), sizeof(NET_DVR_HIDEALARM_V30));
		}
		struPictureParams.struHideAlarm.dwEnableHideAlarm = 1;
	}
	else
	{
		struPictureParams.struHideAlarm.dwEnableHideAlarm = 0;
	}

	//设置OSD
	if(ui.checkShowOsd->isChecked())
	{
		struPictureParams.dwShowOsd = 1;
		//设置OSD的X，Y坐标
		KIT_editToInt(ui.editOSDTopLeftX, &iTemp);
		struPictureParams.wOSDTopLeftX = (unsigned short)iTemp;
		KIT_editToInt(ui.editOSDTopLeftY, &iTemp);
		struPictureParams.wOSDTopLeftY = (unsigned short)iTemp;
		//OSD类型
		struPictureParams.byOSDType = ui.comboOSDType->currentIndex();
		//OSD属性
		struPictureParams.byOSDAttrib = ui.comboOSDAttrib->currentIndex() + 1;
		//小时制
		struPictureParams.byHourOSDType = ui.comboHourOsdType->currentIndex();
	}
	else
	{
		struPictureParams.dwShowOsd = 0;
	}
	//是否显示星期
	if(ui.checkDispWeek->isChecked())
	{
		struPictureParams.byDispWeek = 1;
	}
	else
	{
		struPictureParams.byDispWeek = 0;
	}

	//3.设置图像参数
	iRet = NET_DVR_SetDVRConfig(m_lUserID, NET_DVR_SET_PICCFG_V30, m_lChannel,
				&struPictureParams, sizeof(NET_DVR_PICCFG_V30));
	if (iRet == 0)
	{
		iRet = NET_DVR_GetLastError();
		QMessageBox::information(this, tr("Please check!"), tr("Get Picture parameters error! \"%1\"").arg(iRet));
		return HPR_ERROR;
	}
	return HPR_OK;
}

//获取字符叠加参数
int CChannelParams::getShowStringInfo()
{
	if(m_lUserID < 0)
	{
		return HPR_ERROR;
	}
	comboStringNoChanged(0);

	return HPR_OK;
}

//设置字符叠加参数
int CChannelParams::setShowStringInfo()
{
	//先获取字符叠加参数
	NET_DVR_SHOWSTRING_V30 struShowString;
	int iRet;
	DWORD uiRetParamsLen;

	//get show strings information
	iRet = NET_DVR_GetDVRConfig(m_lUserID,
			    NET_DVR_GET_SHOWSTRING_V30,
				m_lChannel,
				&struShowString,
				sizeof(NET_DVR_SHOWSTRING_V30),
				&uiRetParamsLen);
	if (iRet == 0)
	{
		iRet = NET_DVR_GetLastError();
		QMessageBox::information(this, tr("Please check!"), tr("Get show strings parameters error! \"%1\"").arg(iRet));
		return HPR_ERROR;
	}

	//获取当前的字符叠加区域序号
	int iShowStringNo;
	int iTemp;
	iShowStringNo = ui.comboStringNo->currentIndex();
	if(ui.checkShowString->isChecked())
	{
		struShowString.struStringInfo[iShowStringNo].wShowString = 1;
		//x,y坐标
		KIT_editToInt(ui.editShowStringTopLeftX, &iTemp);
		struShowString.struStringInfo[iShowStringNo].wShowStringTopLeftX = iTemp;
		KIT_editToInt(ui.editShowStringTopLeftY, &iTemp);
		struShowString.struStringInfo[iShowStringNo].wShowStringTopLeftY = iTemp;
		//strings
                KIT_editToStr(ui.editString, (BYTE *)struShowString.struStringInfo[iShowStringNo].sString, KIT_SHOWSTRING_LEN);
	}
	else
	{
		struShowString.struStringInfo[iShowStringNo].wShowString = 0;
	}

	//set show strings information
	iRet = NET_DVR_SetDVRConfig(m_lUserID,
			    NET_DVR_SET_SHOWSTRING_V30,
				m_lChannel,
				&struShowString,
				sizeof(NET_DVR_SHOWSTRING_V30));
	if (iRet == 0)
	{
		iRet = NET_DVR_GetLastError();
		QMessageBox::information(this, tr("Please check!"), tr("Set show strings parameters error! \"%1\"").arg(iRet));
		return HPR_ERROR;
	}

	return HPR_OK;
}

/*******************************************************************
      Function:   CChannelParams::setEachCompWidget
     Parameter:  
           (IN)     LPNET_DVR_COMPRESSION_INFO_V30 pCompressInfo  
        Return:   HPR_OK--成功，HPR_ERROR--失败。
   Description:   将从设备获取的压缩参数值，写入控件中
**********************************************************************/
int CChannelParams::setEachCompWidget(LPNET_DVR_COMPRESSION_INFO_V30 pCompressInfo)
{
	ui.comboPicQuality->setCurrentIndex(pCompressInfo->byPicQuality);
	ui.comboVideoFrameRate->setCurrentIndex(
			int(pCompressInfo->dwVideoFrameRate));
	ui.comboVideoEncType->setCurrentIndex(pCompressInfo->byVideoEncType);

	ui.comboStreamType->setCurrentIndex(pCompressInfo->byStreamType);
	//bit rate
	unsigned int uiVideoBitrateIndex;
	uiVideoBitrateIndex = pCompressInfo->dwVideoBitrate;
	if (uiVideoBitrateIndex > 23)//define by myself.
	{
		ui.comboVideoBit->setCurrentIndex(22);
		uiVideoBitrateIndex = uiVideoBitrateIndex & 0x7fffffff;
		uiVideoBitrateIndex = uiVideoBitrateIndex/1024;
		KIT_intToEdit(ui.editVideoBit, (int)uiVideoBitrateIndex);
		ui.editVideoBit->setEnabled(TRUE);
	}
	else
	{
		ui.editVideoBit->setEnabled(FALSE);
		if (uiVideoBitrateIndex > 1)
		{
			ui.comboVideoBit->setCurrentIndex(uiVideoBitrateIndex - 2);
		}
	}
	//audio
	ui.comboAudioEncType->setCurrentIndex(pCompressInfo->byAudioEncType);

	//resolution
	int iResolution;
	iResolution = pCompressInfo->byResolution;
	if (iResolution > 4) {
		iResolution -= 11;
	}
	ui.comboResolution->setCurrentIndex(iResolution);
	//I frame interval time
	KIT_intToEdit(ui.editIntervalFrameI, int(pCompressInfo->wIntervalFrameI));

	//Bit rate type
	ui.comboBitrateType->setCurrentIndex(pCompressInfo->byBitrateType);
	//IntervalFrameI
	ui.comboIntervalBPFrame->setCurrentIndex(pCompressInfo->byIntervalBPFrame);

	return HPR_OK;
}

/*******************************************************************
      Function:   CChannelParams::getEachCompWidget
     Parameter:  
           (IN)     LPNET_DVR_COMPRESSION_INFO_V30 pCompressInfo  
        Return:   HPR_OK--成功，HPR_ERROR--失败。
   Description:   获取控件中值，写入将要传入设备的结构体中
**********************************************************************/
int CChannelParams::getEachCompWidget(LPNET_DVR_COMPRESSION_INFO_V30 pCompressInfo) 
{
	pCompressInfo->byPicQuality = ui.comboPicQuality->currentIndex();
	pCompressInfo->dwVideoFrameRate = ui.comboVideoFrameRate->currentIndex();
	pCompressInfo->byVideoEncType = ui.comboVideoEncType->currentIndex();

	pCompressInfo->byStreamType = ui.comboStreamType->currentIndex();
	//bit rate
	int iVideoBitrate;
	if ((ui.comboVideoBit->currentIndex()) == 22)//define by myself
	{
		KIT_editToInt(ui.editVideoBit, &iVideoBitrate);
		if(iVideoBitrate <32)
		{
			iVideoBitrate = 32;
		}
		if(iVideoBitrate > 8192)
		{
			iVideoBitrate = 8192;
		}
		pCompressInfo->dwVideoBitrate = (unsigned int)iVideoBitrate*1024 + 0x80000000;
	} else {
		pCompressInfo->dwVideoBitrate = (ui.comboVideoBit->currentIndex() + 2);
	}
	//audio
	pCompressInfo->byAudioEncType = ui.comboAudioEncType->currentIndex();

	//resolution
	int iResolution;
	iResolution = ui.comboResolution->currentIndex();
	if (iResolution > 4)
	{
		pCompressInfo->byResolution = iResolution + 11;
	}
	else
	{
		pCompressInfo->byResolution = iResolution;
	}
	//I frame interval time
	int iIntervalFrameI;
	KIT_editToInt(ui.editIntervalFrameI, &iIntervalFrameI);
	pCompressInfo->wIntervalFrameI = iIntervalFrameI;

	//Bit rate type
	pCompressInfo->byBitrateType = ui.comboBitrateType->currentIndex();
	//IntervalFrameI
	pCompressInfo->byIntervalBPFrame = ui.comboIntervalBPFrame->currentIndex();

	return HPR_ERROR;
}

/*******************************************************************
      Function:   CChannelParams::comboChNoChanged
     Parameter:  
           (IN)     int iChannelIndex  
        Return:   HPR_OK--成功，HPR_ERROR--失败。
   Description:   通道ID被改变了
**********************************************************************/
void CChannelParams::comboChNoChanged(int iChannelIndex)
{
	m_lChannel = m_iArryChannel[iChannelIndex];
	//getAllInfo();

	//获取所有通道参数信息，会把序号默认为0，下面重设为当前选定值
	getCompressInfo(0);   //图像压缩参数
	getRecordInfo();      //录像参数
	getPictureInfo();     //图像参数
	getShowStringInfo();  //叠加字符参数

}

/*******************************************************************
      Function:   CChannelParams::comboCompChanged
     Parameter:  
           (IN)     int iCompIndex  
        Return:   HPR_OK--成功，HPR_ERROR--失败。
   Description:   压缩参数的索引值改变：事件，网络，..
**********************************************************************/
void CChannelParams::comboCompChanged(int iCompIndex)
{
	getCompressInfo(iCompIndex);
}

/*******************************************************************
      Function:   CChannelParams::comboVideoBitChanged
     Parameter:  
           (IN)     int iVideoBitIndex  
        Return:   HPR_OK--成功，HPR_ERROR--失败。
   Description:   录像比特率被改变
**********************************************************************/
void CChannelParams::comboVideoBitChanged(int iVideoBitIndex)
{
	if (22 == iVideoBitIndex)
	{
		ui.editVideoBit->setEnabled(TRUE);
	}
	else
	{
		ui.editVideoBit->setEnabled(FALSE);
	}
}

//字符叠加区域号改变
void CChannelParams::comboStringNoChanged(int iStringNo)
{
	NET_DVR_SHOWSTRING_V30 struShowString;
	int iRet;
	DWORD uiRetParamsLen;

	//get show strings information
	iRet = NET_DVR_GetDVRConfig(m_lUserID,
			    NET_DVR_GET_SHOWSTRING_V30,
				m_lChannel,
				&struShowString,
				sizeof(NET_DVR_SHOWSTRING_V30),
				&uiRetParamsLen);
	if (iRet == 0)
	{
		iRet = NET_DVR_GetLastError();
		QMessageBox::information(this, tr("Please check!"), tr("Get show strings parameters error! \"%1\"").arg(iRet));
		return ;
	}

	//是否显示叠加区域
	if(struShowString.struStringInfo[iStringNo].wShowString == 0)
	{
		ui.checkShowString->setChecked(FALSE);
		ui.editShowStringTopLeftX->setEnabled(FALSE);
		ui.editShowStringTopLeftY->setEnabled(FALSE);
		ui.editString->setEnabled(FALSE);
	}
	else
	{
		ui.checkShowString->setChecked(TRUE);
		ui.editShowStringTopLeftX->setEnabled(TRUE);
		ui.editShowStringTopLeftY->setEnabled(TRUE);
		ui.editString->setEnabled(TRUE);
	}

	//x,y坐标
	int iTemp;
	iTemp = (int)(struShowString.struStringInfo[iStringNo].wShowStringTopLeftX);
	KIT_intToEdit(ui.editShowStringTopLeftX, iTemp);
	iTemp = (int)(struShowString.struStringInfo[iStringNo].wShowStringTopLeftY);
	KIT_intToEdit(ui.editShowStringTopLeftY, iTemp);
	//字符串
	ui.editString->setText(struShowString.struStringInfo[iStringNo].sString);
}

/*******************************************************************
      Function:   CChannelParams::clickBtnRemoteRecord
     Parameter:  
           (IN)     无参数  
        Return:   HPR_OK--成功，HPR_ERROR--失败。
   Description:   调出设置时间计划的界面
**********************************************************************/
void CChannelParams::clickBtnRemoteRecord()
{
	NET_DVR_RECORD_V30 struRecord;
	int iRet;
	DWORD uiRetParamsLen;

	//get the record information
	iRet = NET_DVR_GetDVRConfig(m_lUserID,
			    NET_DVR_GET_RECORDCFG_V30,
				m_lChannel,
				&struRecord,
				sizeof(NET_DVR_RECORD_V30),
				&uiRetParamsLen);
	if (iRet == 0)
	{
		iRet = NET_DVR_GetLastError();
		QMessageBox::information(this, tr("Please check!"), tr("Get press parameters error! \"%1\"").arg(iRet));
		return ;
	}

	//Create the dialog which is used to set the schedule.
	CDlgRemoteRecordSchedule dlgVedioRecord;
	dlgVedioRecord.SetStruRecord(&struRecord);
	dlgVedioRecord.exec();//以模式对话框方式显示
	memcpy(&m_struRecord, &struRecord, sizeof(NET_DVR_RECORD_V30));
	m_bSetRecord = TRUE;
}

/*******************************************************************
      Function:   CChannelParams::clickBtnVILost
     Parameter:
           (IN)     无参数
        Return:   无
   Description:   视频信号丢失报警参数
**********************************************************************/
void CChannelParams::clickBtnVILost()
{
	if(m_lUserID < 0)
	{
		QMessageBox::information(this, tr("Please check!"), tr("User ID error! "));
		return;
	}

	int iRet;
	DWORD uiRetParamsLen;

	//获取图像参数中的信号丢失报警信息
	NET_DVR_PICCFG_V30 struPictureParams;
	iRet = NET_DVR_GetDVRConfig(m_lUserID,
			    NET_DVR_GET_PICCFG_V30,
				m_lChannel,
				&struPictureParams,
				sizeof(NET_DVR_PICCFG_V30),
				&uiRetParamsLen);
	if (iRet == 0)
	{
		iRet = NET_DVR_GetLastError();
		QMessageBox::information(this, tr("Please check!"), tr("Get picture parameters error! \"%1\"").arg(iRet));
		return ;
	}

	//获取设备参数中的最大报警输出数
	NET_DVR_DEVICECFG struDeviceParams;
	iRet = NET_DVR_GetDVRConfig(m_lUserID,
			    NET_DVR_GET_DEVICECFG,
			    0,
				&struDeviceParams,
				sizeof(NET_DVR_DEVICECFG),
				&uiRetParamsLen);
	if (iRet == 0)
	{
		iRet = NET_DVR_GetLastError();
		QMessageBox::information(this, tr("Please check!"), tr("Get device parameters error1! \"%1\"").arg(iRet));
		return ;
	}

	memcpy(&(m_struVILost), &(struPictureParams.struVILost), sizeof(NET_DVR_VILOST_V30));
	//Create the dialog which is used to set the schedule.
	CVILost dlgVILost;
	dlgVILost.setStruVILost(&(m_struVILost)); //信号丢失报警参数
	dlgVILost.setAlarmOutNum(struDeviceParams.byAlarmOutPortNum);
	dlgVILost.setUserID(m_lUserID);
	dlgVILost.exec();//以模式对话框方式显示
	m_bVILost = TRUE;
}

//遮挡区域
void CChannelParams::clickBtnShelter()
{
	if(m_lUserID < 0)
	{
		QMessageBox::information(this, tr("Please check!"), tr("User ID error! "));
		return;
	}

	int iRet;
	DWORD uiRetParamsLen;

	//获取图像参数种的遮挡信息
	NET_DVR_PICCFG_V30 struPictureParams;
	iRet = NET_DVR_GetDVRConfig(m_lUserID,
			    NET_DVR_GET_PICCFG_V30,
				m_lChannel,
				&struPictureParams,
				sizeof(NET_DVR_PICCFG_V30),
				&uiRetParamsLen);
	if (iRet == 0)
	{
		iRet = NET_DVR_GetLastError();
		QMessageBox::information(this, tr("Please check!"), tr("Get picture parameters error! \"%1\"").arg(iRet));
		return ;
	}

	//拷贝遮挡信息到类成员中
	memcpy(m_struShelter, struPictureParams.struShelter, MAX_SHELTERNUM*sizeof(NET_DVR_SHELTER));

	//将类成员设置到对话框中
	CShelter dlgShelter;
	dlgShelter.setShelter(m_struShelter);//设置遮挡区域的地址
	dlgShelter.exec();

	m_bShelter = TRUE;
}

void CChannelParams::on_btnMotion_clicked()
{
	if(m_lUserID < 0)
	{
		QMessageBox::information(this, tr("Please check!"), tr("User ID error! "));
		return;
	}

	int iRet;
	DWORD uiRetParamsLen;

	//获取图像参数中的移动侦测信息
	NET_DVR_PICCFG_V30 struPictureParams;
	iRet = NET_DVR_GetDVRConfig(m_lUserID,
			    NET_DVR_GET_PICCFG_V30,
				m_lChannel,
				&struPictureParams,
				sizeof(NET_DVR_PICCFG_V30),
				&uiRetParamsLen);
	if (iRet == 0)
	{
		iRet = NET_DVR_GetLastError();
		QMessageBox::information(this, tr("Please check!"), tr("Get picture parameters error! \"%1\"").arg(iRet));
		return ;
	}
	//拷贝移动侦测参数到类结构体
	memcpy(&m_struMotion, &(struPictureParams.struMotion), sizeof(NET_DVR_MOTION_V30));

	//获取设备参数中的最大报警输出数
	NET_DVR_DEVICECFG struDeviceParams;
	iRet = NET_DVR_GetDVRConfig(m_lUserID,
			    NET_DVR_GET_DEVICECFG,
			    0,
				&struDeviceParams,
				sizeof(NET_DVR_DEVICECFG),
				&uiRetParamsLen);
	if (iRet == 0)
	{
		iRet = NET_DVR_GetLastError();
		QMessageBox::information(this, tr("Please check!"), tr("Get device parameters error! \"%1\"").arg(iRet));
		return ;
	}

	CMotion dlgMotion;
	//设置移动侦测参数
	dlgMotion.setStruMotion(&m_struMotion);
	//设置用户ID, 获取数字通道
	dlgMotion.setUserID(m_lUserID);
	//设置模拟报警输出数目
	dlgMotion.setAlarmOutNum(struDeviceParams.byAlarmOutPortNum);
	//设置可触发的录像通道数
	dlgMotion.setChannelID(m_iArryChannel);
	//设置起始通道号
	dlgMotion.setStartChannelNo(m_iStartChan);
	dlgMotion.exec();
	m_bMotion = TRUE;
}

//遮挡报警
void CChannelParams::on_btnHide_clicked()
{
	if(m_lUserID < 0)
	{
		QMessageBox::information(this, tr("Please check!"), tr("User ID error! "));
		return;
	}

	int iRet;
	DWORD uiRetParamsLen;

	//获取图像参数中的移动侦测信息
	NET_DVR_PICCFG_V30 struPictureParams;
	iRet = NET_DVR_GetDVRConfig(m_lUserID,
			    NET_DVR_GET_PICCFG_V30,
				m_lChannel,
				&struPictureParams,
				sizeof(NET_DVR_PICCFG_V30),
				&uiRetParamsLen);
	if (iRet == 0)
	{
		iRet = NET_DVR_GetLastError();
		QMessageBox::information(this, tr("Please check!"), tr("Get picture parameters error! \"%1\"").arg(iRet));
		return ;
	}
	//拷贝移动侦测参数到类结构体
	memcpy(&m_struHide, &(struPictureParams.struHideAlarm), sizeof(NET_DVR_HIDEALARM_V30));

	//获取设备参数中的最大报警输出数
	NET_DVR_DEVICECFG struDeviceParams;
	iRet = NET_DVR_GetDVRConfig(m_lUserID,
			    NET_DVR_GET_DEVICECFG,
			    0,
				&struDeviceParams,
				sizeof(NET_DVR_DEVICECFG),
				&uiRetParamsLen);
	if (iRet == 0)
	{
		iRet = NET_DVR_GetLastError();
		QMessageBox::information(this, tr("Please check!"), tr("Get device parameters error! \"%1\"").arg(iRet));
		return ;
	}

	CHideAlarm dlgHideAlarm;
	dlgHideAlarm.setStruHideAlarm(&m_struHide);
	dlgHideAlarm.setAlarmOutNum(struDeviceParams.byAlarmOutPortNum);
	dlgHideAlarm.setUserID(m_lUserID);
	dlgHideAlarm.exec();
	m_bHide = TRUE;
}

/*******************************************************************
      Function:   CChannelParams::clickCheckVideoRecord
     Parameter:  
           (IN)     无参数  
        Return:  无
   Description:   是否定时录像
**********************************************************************/
void CChannelParams::clickCheckVideoRecord()
{
	if(ui.checkboVideoRecord->isChecked())
	{
		ui.btnRemoteRecord->setEnabled(true);
	}
	else
	{
		m_bSetRecord = FALSE;
		ui.btnRemoteRecord->setEnabled(false);
	}
}

/*******************************************************************
      Function:   CChannelParams::clickCheckShowChanName
     Parameter:
           (IN)     无参数
        Return:   HPR_OK--成功，HPR_ERROR--失败。
   Description:   //(click check)是否在屏幕上显示通道名称
**********************************************************************/
 void CChannelParams::clickCheckShowChanName()
 {
	 if(ui.checkboShowChanName->isChecked())
	 {
		 ui.editShowNameTopLeftX->setEnabled(TRUE);
		 ui.editShowNameTopLeftY->setEnabled(TRUE);
	 }
	 else
	 {
		 ui.editShowNameTopLeftX->setEnabled(FALSE);
		 ui.editShowNameTopLeftY->setEnabled(FALSE);
	 }
 }

 /*******************************************************************
       Function:   CChannelParams::clickCheckVILost
      Parameter:
            (IN)     无
         Return:   无
    Description:   是否设置视频信号丢失报警参数
 **********************************************************************/
 void CChannelParams::clickCheckVILost()
 {
	 if(ui.checkboVILost->isChecked())
	 {
		 ui.btnVILost->setEnabled(TRUE);
	 }
	 else
	 {
		 ui.btnVILost->setEnabled(FALSE);
		 m_bVILost = FALSE;
	 }
 }


 void CChannelParams::clickCheckShelter()
 {
	 //QMessageBox::information(this, tr("Please check!"), tr("sss"));
	 if(ui.checkShelter->isChecked())
	 {
		 ui.btnShelter->setEnabled(TRUE);
	 }
	 else
	 {
		 ui.btnShelter->setEnabled(FALSE);
		 m_bShelter = FALSE;
	 }
 }

 //(click check)是否设置移动侦测
 void CChannelParams::clickCheckMotion()
 {
	 if(ui.checkMotion->isChecked())
	 {
		 ui.btnMotion->setEnabled(TRUE);
	 }
	 else
	 {
		 ui.btnMotion->setEnabled(FALSE);
		 m_bMotion = FALSE;
	 }

 }
 //(click check)是否设置遮挡
 void CChannelParams::clickCheckHide()
 {
	 if(ui.checkHide->isChecked())
	 {
		 ui.btnHide->setEnabled(TRUE);
	 }
	 else
	 {
		 ui.btnHide->setEnabled(FALSE);
		 m_bHide = FALSE;
	 }
 }

 //OSD显示
 void CChannelParams::clickCheckOSD()
 {
	 if(ui.checkShowOsd->isChecked())
	 {
		 ui.editOSDTopLeftX->setEnabled(TRUE);
		 ui.editOSDTopLeftY->setEnabled(TRUE);
		 ui.checkDispWeek->setEnabled(TRUE);
		 ui.comboOSDAttrib->setEnabled(TRUE);
		 ui.comboOSDType->setEnabled(TRUE);
	 }
	 else
	 {
		 ui.editOSDTopLeftX->setEnabled(FALSE);
		 ui.editOSDTopLeftY->setEnabled(FALSE);
		 ui.checkDispWeek->setEnabled(FALSE);
		 ui.comboOSDAttrib->setEnabled(FALSE);
		 ui.comboOSDType->setEnabled(FALSE);
	 }
 }

 //是否显示字符叠加
 void CChannelParams::clickCheckShowString()
 {
		//是否显示叠加区域
		if(ui.checkShowString->isChecked())
		{
			ui.editShowStringTopLeftX->setEnabled(TRUE);
			ui.editShowStringTopLeftY->setEnabled(TRUE);
			ui.editString->setEnabled(TRUE);
		}
		else
		{
			ui.editShowStringTopLeftX->setEnabled(FALSE);
			ui.editShowStringTopLeftY->setEnabled(FALSE);
			ui.editString->setEnabled(FALSE);
		}
 }

/*******************************************************************
      Function:   CChannelParams::setUserID
     Parameter:  
           (IN)     long lUserID  
        Return:   HPR_OK--成功，HPR_ERROR--失败。
   Description:   设置用户ID
**********************************************************************/
int CChannelParams::setUserID(long lUserID)
{
	m_lUserID = lUserID;
	return HPR_ERROR;
}

/*******************************************************************
      Function:   CChannelParams::showEvent
     Parameter:  
           (IN)     QShowEvent * event  
        Return:   HPR_OK--成功，HPR_ERROR--失败。
   Description:   在该窗口显示时，刷新页面的参数
**********************************************************************/
void CChannelParams::showEvent(QShowEvent * event)
{
	//disable warning
	if (event == NULL)
	{
	}
	if (m_lUserID < 0)
	{
		QMessageBox::information(this, tr("Please login firstly!"), tr("User ID is not correct!."));
		return;
	}
	else
	{
		//QMessageBox::information(this, "Please check!", tr("showEvent() user id is \"%1\" ").arg(m_lUserID));
		getChannelNum(); //get channel list
		m_lChannel = m_iArryChannel[ui.comboChannelNo->currentIndex()];
		getAllInfo(); // update all data of widget
	}
}
