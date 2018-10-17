/*
 * Copyright(C) 2010,Hikvision Digital Technology Co., Ltd 
 * 
 * 文件名称：calarminhandletype.cpp
 * 描    述：
 * 当前版本：1.0
 * 作    者：潘亚东
 * 创建日期：2010年1月15日
 * 修改记录：
 */


#include "calarminhandletype.h"
#include "publicfuc.h"

CAlarmInHandleType::CAlarmInHandleType(QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);
	ui.tableChannel->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	ui.tableAlarmOut->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

	m_lUserID = -1;
	m_pAlarmInInfo = NULL; //外部传入，外部释放

	int i;
	//加入时间编辑框
	for(i = 0; i < MAX_TIMESEGMENT_V30; i++)
	{
		ui.vLayStartHour->addWidget(&(m_editStartHour[i]));
		ui.vLayStartMin->addWidget(&(m_editStartMin[i]));
		ui.vLayEndHour->addWidget(&(m_editEndHour[i]));
		ui.vLayEndMin->addWidget(&(m_editEndMin[i]));
	}
	QStringList list;
	list<<"Monday"<<"Tuesday"<<"Wednesday"<<"Thursday"<<"Friday"<<"Saturday"<<"Sunday";
    ui.comboWeekDay->addItems(list);

	connect(ui.comboWeekDay, SIGNAL(activated(int)), this, SLOT(comboWeekDayChanged(int)));
}

CAlarmInHandleType::~CAlarmInHandleType()
{

}

int CAlarmInHandleType::setUserID(long lUserID)
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

int CAlarmInHandleType::setAlarmInInfo(LPNET_DVR_ALARMINCFG_V30 pAlarmInInfo)
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
int CAlarmInHandleType::getAlarmInHandleInfo()
{
	if(m_pAlarmInInfo == NULL || m_lUserID < 0)
	{
		return HPR_ERROR;
	}

	//获取可用录像通道
	int iTotalChanNum;
	if(KIT_CreateCheckboxChanArr(m_lUserID, m_pCheckChannel, &iTotalChanNum)==HPR_ERROR)
	{
		return HPR_ERROR;
	}
	//将这些产生的checkbox加入到通道table中去
	//触发通道输出
    //1.清空表格
	int iRow;
	ui.tableChannel->clear();
	for(iRow = 0; iRow < ui.tableChannel->rowCount(); iRow++)
	{
		ui.tableChannel->removeRow(iRow);
	}
	//2.往表格中加入数据
    ui.tableChannel->setRowCount(iTotalChanNum);
    ui.tableChannel->setColumnCount(1);
    int i;
    iRow = 0;
    for(i = 0; i < MAX_CHANNUM_V30; i++)
    {
    	if(m_pCheckChannel[i] != NULL)
    	{
    		ui.tableChannel->setCellWidget(iRow, 0, m_pCheckChannel[i]);
    		if(m_pAlarmInInfo->byRelRecordChan[i] == 1)
    		{
    			m_pCheckChannel[i]->setChecked(TRUE);
    		}
    		else
    		{
    			m_pCheckChannel[i]->setChecked(FALSE);
    		}
    		iRow++;
    	}
    }

    //**报警输出**//
    int iTotalAlarmOutNum;
    if(KIT_CreateCheckboxAlarmOut(m_lUserID, m_pcheckAlarmOut, &iTotalAlarmOutNum) == HPR_ERROR)
    {
    	return HPR_ERROR;
    }
    //1.清空报警输出表格
	ui.tableAlarmOut->clear();
	for(iRow = 0; iRow < ui.tableAlarmOut->rowCount(); iRow++)
	{
		ui.tableAlarmOut->removeRow(iRow);
	}
	//2.往表格中加入数据
    ui.tableAlarmOut->setRowCount(iTotalAlarmOutNum);
    ui.tableAlarmOut->setColumnCount(1);
    iRow = 0;
    for(i = 0; i < MAX_ALARMOUT_V30; i++)
    {
    	if(m_pcheckAlarmOut[i] != NULL)
    	{
    		ui.tableAlarmOut->setCellWidget(iRow, 0, m_pcheckAlarmOut[i]);
    		if(m_pAlarmInInfo->struAlarmHandleType.byRelAlarmOut[i] == 1)
    		{
    			m_pcheckAlarmOut[i]->setChecked(TRUE);
    		}
    		else
    		{
    			m_pcheckAlarmOut[i]->setChecked(FALSE);
    		}
    		iRow++;
    	}
    }

    //布防时间
    comboWeekDayChanged(0);
    //报警处理方式
    setAlarmTypeWidget();

    return HPR_OK;
}

//从界面获取到数据写入设备
int CAlarmInHandleType::setAlarmInHandleInfo()
{
	if(m_pAlarmInInfo == NULL)
	{
		return HPR_ERROR;
	}

	//设置录像通道
    int i;
    for(i = 0; i < MAX_CHANNUM_V30; i++)
    {
    	if(m_pCheckChannel[i] != NULL)
    	{
    		if(m_pCheckChannel[i]->isChecked())
    		{
    			m_pAlarmInInfo->byRelRecordChan[i] = 1;
    		}
    		else
    		{
    			m_pAlarmInInfo->byRelRecordChan[i] =0;
    		}
    	}
    }

    //设置报警输出
    for(i = 0; i < MAX_ALARMOUT_V30; i++)
    {
    	if(m_pcheckAlarmOut[i] != NULL)
    	{
    		if(m_pcheckAlarmOut[i]->isChecked())
    		{
    			m_pAlarmInInfo->struAlarmHandleType.byRelAlarmOut[i] = 1;
    		}
    		else
    		{
    			m_pAlarmInInfo->struAlarmHandleType.byRelAlarmOut[i] = 0;
    		}
    	}
    }

    //时间
	//获取布防时间
	int iTime;
	int iWeekDay;
	iWeekDay = ui.comboWeekDay->currentIndex();
	for(i = 0; i < MAX_TIMESEGMENT_V30; i++)
	{
	    //开始小时
		KIT_editToInt(&(m_editStartHour[i]), &iTime);
		m_pAlarmInInfo->struAlarmTime[iWeekDay][i].byStartHour = iTime;
		//开始分钟
		KIT_editToInt(&(m_editStartMin[i]), &iTime);
		m_pAlarmInInfo->struAlarmTime[iWeekDay][i].byStartMin = iTime;
		//结束小时
		KIT_editToInt(&(m_editEndHour[i]), &iTime);
		m_pAlarmInInfo->struAlarmTime[iWeekDay][i].byStopHour = iTime;
		//结束分钟
		KIT_editToInt(&(m_editEndMin[i]), &iTime);
		m_pAlarmInInfo->struAlarmTime[iWeekDay][i].byStopMin = iTime;
	}

	//报警方式
	getAlarmTypeWidget();
	return HPR_OK;
}

//获取报警处理方式
int CAlarmInHandleType::getAlarmTypeWidget()
{
	if(NULL == m_pAlarmInInfo)
	{
		return HPR_ERROR;
	}

	//处理方式参数
	int dwHandleType;
	int i;
	dwHandleType = 0;
	if(ui.checkbo01->isChecked())
	{
		dwHandleType += 1;
	}
	if(ui.checkbo02->isChecked())
	{
		dwHandleType += 2;
	}
	if(ui.checkbo04->isChecked())
	{
		dwHandleType += 4;
	}
	if(ui.checkbo08->isChecked())
	{
		dwHandleType += 8;
		for(i = 0; i < MAX_ALARMOUT_V30; i++)
		{
			if(m_pcheckAlarmOut[i] != NULL)
			{
				if(m_pcheckAlarmOut[i]->isChecked())
				{
					m_pAlarmInInfo->struAlarmHandleType.byRelAlarmOut[i] = 1;
				}
				else
				{
					m_pAlarmInInfo->struAlarmHandleType.byRelAlarmOut[i] = 0;
				}
			}
		}
	}
	if(ui.checkbo10->isChecked())
	{
		dwHandleType += 16;
	}
	m_pAlarmInInfo->struAlarmHandleType.dwHandleType = dwHandleType;

	return HPR_OK;
}

//设置报警处理方式
int CAlarmInHandleType::setAlarmTypeWidget()
{
	if(m_pAlarmInInfo == NULL)
	{
		return HPR_ERROR;
	}

	//监视器上报警
    if((m_pAlarmInInfo->struAlarmHandleType.dwHandleType & 0x01) == 0x01)
    {
    	ui.checkbo01->setChecked(TRUE);
    }
    else
    {
    	ui.checkbo01->setChecked(FALSE);
    }
    //声音报警
    if((m_pAlarmInInfo->struAlarmHandleType.dwHandleType & 0x02) == 0x02)
    {
    	ui.checkbo02->setChecked(TRUE);
    }
    else
    {
    	ui.checkbo02->setChecked(FALSE);
    }
    //上传中心
    if((m_pAlarmInInfo->struAlarmHandleType.dwHandleType & 0x04) == 0x04)
    {
    	ui.checkbo04->setChecked(TRUE);
    }
    else
    {
    	ui.checkbo04->setChecked(FALSE);
    }
    //触发报警输出
    if((m_pAlarmInInfo->struAlarmHandleType.dwHandleType & 0x08) == 0x08)
    {
    	ui.checkbo08->setChecked(TRUE);
    	ui.tableAlarmOut->setEnabled(TRUE);
    	int i;
    	for(i = 0; i < MAX_ALARMOUT_V30; i++)//报警输出
    	{
    		if(m_pcheckAlarmOut[i] != NULL)//报警输出checkbox不为空
    		{
        	    if(m_pAlarmInInfo->struAlarmHandleType.byRelAlarmOut[i] == 0)
        	    {
        	    	m_pcheckAlarmOut[i]->setChecked(FALSE);
        	    }
        	    else
        	    {
        	    	m_pcheckAlarmOut[i]->setChecked(TRUE);
        	    }
    		}
    	}
    }
    else
    {
    	ui.checkbo08->setChecked(FALSE);
    	ui.tableAlarmOut->setEnabled(FALSE);
    }
    //JPEG抓图，并上传中心
    if((m_pAlarmInInfo->struAlarmHandleType.dwHandleType & 0x10) == 0x10)
    {
    	ui.checkbo10->setChecked(TRUE);
    }
    else
    {
    	ui.checkbo10->setChecked(FALSE);
    }

	return HPR_OK;

}

void CAlarmInHandleType::comboWeekDayChanged(int iWeekDay)
{
	if(m_pAlarmInInfo == NULL)
	{
		return;
	}
    int i;
    int iTemp;
	for(i = 0; i < MAX_TIMESEGMENT_V30; i++)
	{
	   	//时间
	   	iTemp = m_pAlarmInInfo->struAlarmTime[iWeekDay][i].byStartHour;
   	    KIT_intToEdit(&(m_editStartHour[i]), iTemp);
   	    iTemp = m_pAlarmInInfo->struAlarmTime[iWeekDay][i].byStartMin;
	   	KIT_intToEdit(&(m_editStartMin[i]), iTemp);
	   	iTemp = m_pAlarmInInfo->struAlarmTime[iWeekDay][i].byStopHour;
	    KIT_intToEdit(&(m_editEndHour[i]), iTemp);
        iTemp = m_pAlarmInInfo->struAlarmTime[iWeekDay][i].byStopMin;
        KIT_intToEdit(&(m_editEndMin[i]), iTemp);
	 }
}

void CAlarmInHandleType::on_btnSave_clicked()
{
	setAlarmInHandleInfo();
	//quit();
	close();
}


void CAlarmInHandleType::on_checkbo08_clicked()
{
	if(ui.checkbo08->isChecked())
	{
		ui.tableAlarmOut->setEnabled(TRUE);
	}
	else
	{
		ui.tableAlarmOut->setEnabled(FALSE);
	}
}

void CAlarmInHandleType::showEvent(QShowEvent * event)
{
	if(NULL == event)
	{
	}
	getAlarmInHandleInfo();
}







