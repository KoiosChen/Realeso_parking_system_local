#include "cvilost.h"
#include "publicfuc.h"
#include <QMessageBox>

CVILost::CVILost(QDialog *parent)
    : QDialog(parent)
{
	ui.setupUi(this);

	m_lUserID = -1;
	m_pstruVILost = NULL;
	m_iAlarmOutNum = 0;

	ui.tableAlarmOut->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui.tableAlarmOut->setColumnCount(1);
    ui.tableAlarmOut->setEnabled(FALSE);

	int i;
	for(i=0; i < MAX_TIMESEGMENT_V30; i++)
	{
		ui.vLayStartHour->addWidget(&(m_editStartHour[i]));
		ui.vLayStartMin->addWidget(&(m_editStartMin[i]));
		ui.vLayEndHour->addWidget(&(m_editEndHour[i]));
		ui.vLayEndMin->addWidget(&(m_editEndMin[i]));
	}

	for(i = 0; i < MAX_ALARMOUT_V30; i++)
	{
		m_pcheckAlarmOut[i] = NULL;
	}

	QStringList strings;
	strings<<"Monday"<<"Tuesday"<<"Wednesday"<<"Thursday"<<"Friday"<<"Saturday"<<"Sunday";
	ui.comboWeekDay->addItems(strings);

	connect(ui.comboWeekDay, SIGNAL(activated(int)), this, SLOT(comboWeekDayChanged(int)));
    connect(ui.checkbo08, SIGNAL(clicked()), this, SLOT(checkAlarmOut()));
}

CVILost::~CVILost()
{

}

//调用这个对话框前必要调用的两个函数
int CVILost::setStruVILost(NET_DVR_VILOST_V30 *pstruVILost)
{
	if(NULL == pstruVILost)
	{
		return HPR_ERROR;
	}

	m_pstruVILost = pstruVILost;
	return HPR_OK;
}

//调用这个对话框前必要调用的两个函数
int CVILost::setAlarmOutNum(int iAlarmOutNum)
{
	if(iAlarmOutNum < 0)
	{
		return HPR_ERROR;
	}
	else
	{
		m_iAlarmOutNum = iAlarmOutNum;
	}

	return HPR_OK;
}

int CVILost::setUserID(long lUserID)
{
	if(lUserID < 0)
	{
		return HPR_ERROR;
	}
	else
	{
		m_lUserID = lUserID;
	}

	return HPR_OK;
}

//设置每个控件的值
int CVILost::setEachWidgetValue()
{
	if(m_pstruVILost == NULL)
	{
		return HPR_ERROR;
	}

	createAlarmOutCheck();//创建报警输出

	//布防时间
	comboWeekDayChanged(0);

	if(m_pstruVILost->strVILostHandleType.dwHandleType == 0)//无响应
	{
		return HPR_OK;
	}
	//监视器上报警
    if((m_pstruVILost->strVILostHandleType.dwHandleType & 0x01) == 0x01)
    {
    	ui.checkbo01->setChecked(TRUE);
    }
    else
    {
    	ui.checkbo01->setChecked(FALSE);
    }
    //声音报警
    if((m_pstruVILost->strVILostHandleType.dwHandleType & 0x02) == 0x02)
    {
    	ui.checkbo02->setChecked(TRUE);
    }
    else
    {
    	ui.checkbo02->setChecked(FALSE);
    }
    //上传中心
    if((m_pstruVILost->strVILostHandleType.dwHandleType & 0x04) == 0x04)
    {
    	ui.checkbo04->setChecked(TRUE);
    }
    else
    {
    	ui.checkbo04->setChecked(FALSE);
    }
    //触发报警输出
    if((m_pstruVILost->strVILostHandleType.dwHandleType & 0x08) == 0x08)
    {
    	ui.checkbo08->setChecked(TRUE);
    	ui.tableAlarmOut->setEnabled(TRUE);
    	int i;
    	for(i = 0; i < MAX_ALARMOUT_V30; i++)//报警输出
    	{
    		if(m_pcheckAlarmOut[i] != NULL)
    		{
        	    if(m_pstruVILost->strVILostHandleType.byRelAlarmOut[i] == 0)
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
    if((m_pstruVILost->strVILostHandleType.dwHandleType & 0x10) == 0x10)
    {
    	ui.checkbo10->setChecked(TRUE);
    }
    else
    {
    	ui.checkbo10->setChecked(FALSE);
    }


	return HPR_OK;
}
   //获取每个控件的值
int CVILost::getEachWidgetValue()
{
	if(m_pstruVILost == NULL)
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
					m_pstruVILost->strVILostHandleType.byRelAlarmOut[i] = 1;
				}
				else
				{
					m_pstruVILost->strVILostHandleType.byRelAlarmOut[i] = 0;
				}
			}
		}
	}
	if(ui.checkbo10->isChecked())
	{
		dwHandleType += 16;
	}
	m_pstruVILost->strVILostHandleType.dwHandleType = dwHandleType;

	//时间值
	int iTime;
	int iWeekDay;
	iWeekDay = ui.comboWeekDay->currentIndex();
	for(i = 0; i < MAX_TIMESEGMENT_V30; i++)
	{
	    //开始小时
		KIT_editToInt(&(m_editStartHour[i]), &iTime);
		 m_pstruVILost->struAlarmTime[iWeekDay][i].byStartHour = iTime;
		//开始分钟
		KIT_editToInt(&(m_editStartMin[i]), &iTime);
		 m_pstruVILost->struAlarmTime[iWeekDay][i].byStartMin = iTime;
		//结束小时
		KIT_editToInt(&(m_editEndHour[i]), &iTime);
		 m_pstruVILost->struAlarmTime[iWeekDay][i].byStopHour = iTime;
		//结束分钟
		KIT_editToInt(&(m_editEndMin[i]), &iTime);
		 m_pstruVILost->struAlarmTime[iWeekDay][i].byStopMin = iTime;
	}

	return HPR_OK;
}

int CVILost::createAlarmOutCheck()
{
	if(m_lUserID < 0)
	{
		return HPR_ERROR;
	}

	int iRet;
	DWORD uiReturnLen;
	NET_DVR_IPALARMOUTCFG  struIPAlarmOut;
	iRet = NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_IPALARMOUTCFG, -1, &struIPAlarmOut, sizeof(NET_DVR_IPALARMOUTCFG), &uiReturnLen);
    if(iRet == 0)
    {
    	memset(&struIPAlarmOut, 0, sizeof(struIPAlarmOut));
		iRet = NET_DVR_GetLastError();
		//QMessageBox::information(this, tr("Please check!"), tr("Get device parameters error2! \"%1\"").arg(iRet));
		//return HPR_ERROR;
    }

	//获取IP报警输出的个数，以设置表格的行数
	int i;
	int iRow = 0;
	for(i = 0; i < MAX_IP_ALARMOUT; i++ )
	{
		if(struIPAlarmOut.struIPAlarmOutInfo[i].byIPID > 0)
		{
			iRow++;
		}
	}
	iRow += m_iAlarmOutNum;
    ui.tableAlarmOut->setRowCount(iRow);
    ui.tableAlarmOut->setColumnCount(1);
    QStringList list;
    list<<"报警输出";
    ui.tableAlarmOut->setHorizontalHeaderLabels(list);

    //清除以前表格中数据
    ui.tableAlarmOut->clear();
	for(i = 0; i < MAX_ALARMOUT_V30; i++)
	{
		m_pcheckAlarmOut[i] = NULL;
	}

    //往表格中加入模拟报警输出
	for(i = 0; i < m_iAlarmOutNum; i++)
	{
		//new 一个新的checkbox
		m_pcheckAlarmOut[i] = new(std::nothrow) QCheckBox;
		//获取报警输出名
		getAlarmOutInfo(i, m_pcheckAlarmOut[i]);
		//加入到table中去
		ui.tableAlarmOut->setCellWidget(i, 0, (m_pcheckAlarmOut[i]));
	}

	//IP报警输出
	for(i = 0; i < MAX_IP_ALARMOUT; i++)
	{
		//判断IP报警输出是否可用
		if(struIPAlarmOut.struIPAlarmOutInfo[i].byIPID > 0)
		{
			//new出来一个新的checkbox
			m_pcheckAlarmOut[i + MAX_ANALOG_ALARMOUT] = new(std::nothrow) QCheckBox;
		    //获取报警输出名称
			getAlarmOutInfo(i + MAX_ANALOG_ALARMOUT, m_pcheckAlarmOut[i + MAX_ANALOG_ALARMOUT]);
		    //加入到table中去
			ui.tableAlarmOut->setCellWidget(i + m_iAlarmOutNum, 0, (m_pcheckAlarmOut[i + MAX_ANALOG_ALARMOUT]));
		}
	}

	return HPR_OK;
}

//根据报警输出序号，获取报警输出名称
int CVILost::getAlarmOutInfo(int iAlarmOut, QCheckBox *pQCheckAlarmOut)
{
	char strTemp[100] = {0};
	if(iAlarmOut < MAX_ANALOG_ALARMOUT)
	{
		sprintf(strTemp, "AlarmOut%d", iAlarmOut + 1);
	}
	else
	{
		sprintf(strTemp, "IPAlarmOut%d", iAlarmOut - MAX_ANALOG_ALARMOUT + 1);
	}
    pQCheckAlarmOut->setText(strTemp);

	return HPR_OK;
}

void CVILost::on_btnSave_clicked()
{
	getEachWidgetValue();
}

//选中报警输出
void CVILost::checkAlarmOut()
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

void CVILost::comboWeekDayChanged(int iWeekDay)
{
	if(m_pstruVILost == NULL)
	{
		return;
	}
    int i;
    int iTemp;
	for(i = 0; i < MAX_TIMESEGMENT_V30; i++)
	{
	   	//时间
	   	iTemp = m_pstruVILost->struAlarmTime[iWeekDay][i].byStartHour;
   	    KIT_intToEdit(&(m_editStartHour[i]), iTemp);
   	    iTemp = m_pstruVILost->struAlarmTime[iWeekDay][i].byStartMin;
	   	KIT_intToEdit(&(m_editStartMin[i]), iTemp);
	   	iTemp = m_pstruVILost->struAlarmTime[iWeekDay][i].byStopHour;
	    KIT_intToEdit(&(m_editEndHour[i]), iTemp);
        iTemp = m_pstruVILost->struAlarmTime[iWeekDay][i].byStopMin;
        KIT_intToEdit(&(m_editEndMin[i]), iTemp);
	 }
}

//event
void CVILost::showEvent ( QShowEvent * event )
{
	if(event ==NULL)
	{
	}
	//获取显示参数
	setEachWidgetValue();
}
