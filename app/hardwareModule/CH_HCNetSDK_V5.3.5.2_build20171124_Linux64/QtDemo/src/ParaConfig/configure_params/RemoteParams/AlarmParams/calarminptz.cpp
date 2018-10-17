/*
 * Copyright(C) 2010,Hikvision Digital Technology Co., Ltd 
 * 
 * 文件名称：calarminptz.cpp
 * 描    述：
 * 当前版本：1.0
 * 作    者：潘亚东
 * 创建日期：2010年1月15日
 * 修改记录：
 */

#include "calarminptz.h"
#include "publicfuc.h"

CAlarmInPTZ::CAlarmInPTZ(QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);

	m_lUserID = -1;
	m_pAlarmInInfo = NULL; //外部传入，外部释放
	connect(ui.comboChannel, SIGNAL(activated(int)), this, SLOT(changeChanIndex(int)));
}

CAlarmInPTZ::~CAlarmInPTZ()
{

}


int CAlarmInPTZ::setUserID(long lUserID)
 {
     if(lUserID >= 0)
     {
         m_lUserID = lUserID;
         return HPR_OK;
     }
     else
     {
         return HPR_ERROR;
     }
 }

int CAlarmInPTZ::setAlarmInInfo(LPNET_DVR_ALARMINCFG_V30 pAlarmInInfo)
{
	if(pAlarmInInfo == NULL)
	{
		return HPR_ERROR;
	}

	//将设备参数写入界面
	m_pAlarmInInfo = pAlarmInInfo;
	return HPR_OK;
}

//设置数据到界面
int CAlarmInPTZ::getAlarmInPTZInfo()
{
	if(m_pAlarmInInfo == NULL || m_lUserID < 0)
	{
		return HPR_ERROR;
	}

	//先获取通道
	KIT_CHAN_INFO struChanInfo;
	if(KIT_GetChannelInfo(m_lUserID, &struChanInfo) == HPR_ERROR)
	{
		return HPR_ERROR;
	}
	//生成通道list表格
	int i;
	int iChanIndex;
	QStringList list;
	char strTemp[100] = {0};
	for(i = 0; i < MAX_CHANNUM_V30; i++ )
	{
		iChanIndex = struChanInfo.iArrayChanInfo[i] - struChanInfo.cStartNO;
		if(iChanIndex < 0)
		{
			continue;
		}
		if(iChanIndex >= 0 && iChanIndex < MAX_ANALOG_CHANNUM)
		{
			sprintf(strTemp, "Channel%d", struChanInfo.iArrayChanInfo[i]);
			list<<strTemp;
		}
		if( iChanIndex >= MAX_ANALOG_CHANNUM && iChanIndex < MAX_CHANNUM_V30)
		{
			sprintf(strTemp, "IPChannel%d", struChanInfo.iArrayChanInfo[i]);
			list<<strTemp;
		}
	}
	ui.comboChannel->addItems(list);

	//默认显示第一个通道的数据
	changeChanIndex(0);


	return HPR_OK;
}

//设置数据到设备上
int CAlarmInPTZ::setAlarmInPTZInfo()
{
	if(m_pAlarmInInfo == NULL || m_lUserID < 0)
	{
		return HPR_ERROR;
	}

	int iChanIndex;
	iChanIndex = ui.comboChannel->currentIndex();
	int iTemp = 0;

	//获取值到设备
	//预置点
	if(ui.rBtnPreset->isChecked())
	{
		m_pAlarmInInfo->byEnablePreset[iChanIndex] = 1;
		KIT_editToInt(ui.editPresetNo, &iTemp);
		m_pAlarmInInfo->byPresetNo[iChanIndex] = (BYTE)iTemp;
	}

	//预置点
	if(ui.rBtnCruise->isChecked())
	{
		m_pAlarmInInfo->byEnableCruise[iChanIndex] = 1;
		KIT_editToInt(ui.editCruiseNo, &iTemp);
		m_pAlarmInInfo->byCruiseNo[iChanIndex] = (BYTE)iTemp;
	}

	//预置点
	if(ui.rBtnPtzTrack->isChecked())
	{
		m_pAlarmInInfo->byEnablePtzTrack[iChanIndex] = 1;
		KIT_editToInt(ui.editPtzTrackNO, &iTemp);
		m_pAlarmInInfo->byPTZTrack[iChanIndex] = (BYTE)iTemp;
	}

	return HPR_OK;
}

void CAlarmInPTZ::changeChanIndex(int index)
{
	if(index < 0)
	{
		return ;
	}
	//是否调用预置点,
	ui.rBtnPreset->setChecked(m_pAlarmInInfo->byEnablePreset[index]);
	ui.editPresetNo->setEnabled(m_pAlarmInInfo->byEnablePreset[index]);
	KIT_intToEdit(ui.editPresetNo, (int)m_pAlarmInInfo->byPresetNo[index]);

	//是否调用预置点
	ui.rBtnCruise->setChecked(m_pAlarmInInfo->byEnableCruise[index]);
	ui.editCruiseNo->setEnabled(m_pAlarmInInfo->byEnableCruise[index]);
	KIT_intToEdit(ui.editCruiseNo, (int)m_pAlarmInInfo->byCruiseNo[index]);

	//是否调用轨迹
	ui.rBtnPtzTrack->setChecked(m_pAlarmInInfo->byEnablePtzTrack[index]);
	ui.editPtzTrackNO->setEnabled(m_pAlarmInInfo->byEnablePtzTrack[index]);
	KIT_intToEdit(ui.editPtzTrackNO, (int)m_pAlarmInInfo->byPTZTrack[index]);

}

void CAlarmInPTZ::on_rBtnPreset_clicked()
{
	ui.editPresetNo->setEnabled(TRUE);
	ui.editCruiseNo->setEnabled(FALSE);
	ui.editPtzTrackNO->setEnabled(FALSE);
}

void CAlarmInPTZ::on_rBtnCruise_clicked()
{
	ui.editPresetNo->setEnabled(FALSE);
	ui.editCruiseNo->setEnabled(TRUE);
	ui.editPtzTrackNO->setEnabled(FALSE);
}

void CAlarmInPTZ::on_rBtnPtzTrack_clicked()
{
	ui.editPresetNo->setEnabled(FALSE);
	ui.editCruiseNo->setEnabled(FALSE);
	ui.editPtzTrackNO->setEnabled(TRUE);
}

void CAlarmInPTZ::on_btnSave_clicked()
{
	setAlarmInPTZInfo();
	close();
}

void  CAlarmInPTZ::on_btnExit_clicked()
{
	close();
}

void CAlarmInPTZ::showEvent(QShowEvent * event)
{
	if(NULL == event)
	{
	}
	getAlarmInPTZInfo();
}







