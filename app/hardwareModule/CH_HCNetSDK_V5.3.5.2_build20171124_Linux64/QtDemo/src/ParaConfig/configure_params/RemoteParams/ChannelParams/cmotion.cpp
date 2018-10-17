#include "cmotion.h"
#include "publicfuc.h"
#include <QMessageBox>
#include <new>

CMotion::CMotion(QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);

	m_lUserID = -1;
	//***设置table***//
	//设置单元格个数
	ui.tableMotion->setRowCount(KIT_MOTION_HEIGHT);
	ui.tableMotion->setColumnCount(KIT_MOTION_WIDTH);
	//设置单元格内容
	int i, j;
	for(i = 0; i < KIT_MOTION_HEIGHT; i++)
	{
		for(j = 0; j < KIT_MOTION_WIDTH; j++)
		{
			ui.tableMotion->setCellWidget(i, j, &(m_checkMotionScope[i][j]));
		}
	}
	//设置单元格大小
	for(j = 0; j < KIT_MOTION_HEIGHT; j++)
	{
		ui.tableMotion->setColumnWidth(j, 30);
	}

	//加入时间编辑框
	for(i = 0; i < MAX_TIMESEGMENT_V30; i++)
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

	ui.tableChannel->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	ui.tableAlarmOut->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

	m_pstruMotion = NULL;
	m_iAlarmOutNum = 0;
	m_iArryChannel = NULL;
	m_iStartChannelNo = 0;

	//灵敏度
	QStringList list;
	list <<"0"<<"1"<<"2"<<"3"<<"4"<<"5"<<"close";
	ui.comboMotionSensitive->addItems(list);

	//精度
	list.clear();
	list<<"16*16"<<"32*32"<<"64*64";
	ui.comboPrecision->addItems(list);

	//日期
	list.clear();
	list<<"Monday"<<"Tuesday"<<"Wednesday"<<"Thursday"<<"Friday"<<"Saturday"<<"Sunday";
    ui.comboWeekDay->addItems(list);

	connect(ui.comboWeekDay, SIGNAL(activated(int)), this, SLOT(comboWeekDayChanged(int)));
	connect(ui.checkbo08, SIGNAL(clicked()), this, SLOT(checkAlarmOut()));
}

CMotion::~CMotion()
{

}

//设置控件中的值
int CMotion::setEachWidgetValue()
{
    //有注释//
    /*
	if(NULL == m_pstruMotion || NULL == m_iArryChannel)
	{
		return HPR_ERROR;
	}
	createChannelCheck(); //通道表格中根据实际通道数加入checkbox
	createAlarmOutCheck(); //在报警输出表格中加入checkbox

	//展示已有移动侦测区域
	int i, j;
	for(i = 0; i < KIT_MOTION_HEIGHT; i++)
	{
		for(j = 0; j < KIT_MOTION_WIDTH; j++)
		{
			if(m_pstruMotion->byMotionScope[i][j] == 0)
			{
				m_checkMotionScope[i][j].setChecked(FALSE);
			}
			else
			{
				m_checkMotionScope[i][j].setChecked(TRUE);
			}
		}
	}

	//移动侦测灵敏度
        if(m_pstruMotion->byMotionSensitive <= 5 //*&& m_pstruMotion->byMotionSensitive >= 0//)
	{
		ui.comboMotionSensitive->setCurrentIndex(m_pstruMotion->byMotionSensitive);
	}
	else
	{
		ui.comboMotionSensitive->setCurrentIndex(6);//关闭
	}

	//算法精度
        if(m_pstruMotion->byPrecision <= 2 )
	{
		ui.comboPrecision->setCurrentIndex(m_pstruMotion->byPrecision);
	}

	//报警处理方式
	setMotionAlarmTypeWidget();
	//布防时间
	comboWeekDayChanged(0);
	//报警触发的录像通道
	for(i = 0; i < MAX_CHANNUM_V30; i++)
	{
		if(m_pstruMotion->byRelRecordChan[i] == 1)
		{
			m_checkChannel[i].setChecked(TRUE);
		}
		else
		{
			m_checkChannel[i].setChecked(FALSE);
		}
	}

    */
	return HPR_OK;
}

//获取界面上所有控件中的值
int CMotion::getEachWidgetValue()
{
	if(NULL == m_pstruMotion || NULL == m_iArryChannel)
	{
		return HPR_ERROR;
	}

	//获取移动侦测区域
	int i, j;
	for(i = 0; i < KIT_MOTION_HEIGHT; i++)
	{
		for(j = 0; j < KIT_MOTION_WIDTH; j++)
		{
			if(m_checkMotionScope[i][j].isChecked())
			{
				m_pstruMotion->byMotionScope[i][j] = 1;
			}
			else
			{
				m_pstruMotion->byMotionScope[i][j] = 0;
			}
		}
	}

	//获取灵敏度
	if(ui.comboMotionSensitive->currentIndex() <= 5)
	{
		m_pstruMotion->byMotionSensitive = ui.comboMotionSensitive->currentIndex();
	}
	else
	{
		m_pstruMotion->byMotionSensitive = 0xff;
	}

	//精度
    //有注释//
	//m_pstruMotion->byPrecision = ui.comboPrecision->currentIndex();

	//设置报警方式
	getMotionAlarmTypeWidget();
	//获取布防时间
	int iTime;
	int iWeekDay;
	iWeekDay = ui.comboWeekDay->currentIndex();
	for(i = 0; i < MAX_TIMESEGMENT_V30; i++)
	{
	    //开始小时
		KIT_editToInt(&(m_editStartHour[i]), &iTime);
		m_pstruMotion->struAlarmTime[iWeekDay][i].byStartHour = iTime;
		//开始分钟
		KIT_editToInt(&(m_editStartMin[i]), &iTime);
		m_pstruMotion->struAlarmTime[iWeekDay][i].byStartMin = iTime;
		//结束小时
		KIT_editToInt(&(m_editEndHour[i]), &iTime);
		m_pstruMotion->struAlarmTime[iWeekDay][i].byStopHour = iTime;
		//结束分钟
		KIT_editToInt(&(m_editEndMin[i]), &iTime);
		m_pstruMotion->struAlarmTime[iWeekDay][i].byStopMin = iTime;
	}

	//报警触发的通道
	for(i = 0; i < MAX_CHANNUM_V30; i++)
	{
		if(m_checkChannel[i].isChecked())
		{
			m_pstruMotion->byRelRecordChan[i] = 1;
		}
		else
		{
			m_pstruMotion->byRelRecordChan[i] = 0;
		}
	}

    return HPR_OK;
}

//设置报警处理方式
int  CMotion::setMotionAlarmTypeWidget()
{
	if(m_pstruMotion->struMotionHandleType.dwHandleType == 0)//无响应
	{
		return HPR_OK;
	}
	//监视器上报警
    if((m_pstruMotion->struMotionHandleType.dwHandleType & 0x01) == 0x01)
    {
    	ui.checkbo01->setChecked(TRUE);
    }
    else
    {
    	ui.checkbo01->setChecked(FALSE);
    }
    //声音报警
    if((m_pstruMotion->struMotionHandleType.dwHandleType & 0x02) == 0x02)
    {
    	ui.checkbo02->setChecked(TRUE);
    }
    else
    {
    	ui.checkbo02->setChecked(FALSE);
    }
    //上传中心
    if((m_pstruMotion->struMotionHandleType.dwHandleType & 0x04) == 0x04)
    {
    	ui.checkbo04->setChecked(TRUE);
    }
    else
    {
    	ui.checkbo04->setChecked(FALSE);
    }
    //触发报警输出
    if((m_pstruMotion->struMotionHandleType.dwHandleType & 0x08) == 0x08)
    {
    	ui.checkbo08->setChecked(TRUE);
    	ui.tableAlarmOut->setEnabled(TRUE);
    	int i;
    	for(i = 0; i < MAX_ALARMOUT_V30; i++)//报警输出
    	{
    		if(m_pcheckAlarmOut[i] != NULL)//报警输出checkbox不为空
    		{
        	    if(m_pstruMotion->struMotionHandleType.byRelAlarmOut[i] == 0)
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
    if((m_pstruMotion->struMotionHandleType.dwHandleType & 0x10) == 0x10)
    {
    	ui.checkbo10->setChecked(TRUE);
    }
    else
    {
    	ui.checkbo10->setChecked(FALSE);
    }

	return HPR_OK;
}

//获取报警处理方式
int CMotion::getMotionAlarmTypeWidget()
{
	if(NULL == m_pstruMotion)
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
					m_pstruMotion->struMotionHandleType.byRelAlarmOut[i] = 1;
				}
				else
				{
					m_pstruMotion->struMotionHandleType.byRelAlarmOut[i] = 0;
				}
			}
		}
	}
	if(ui.checkbo10->isChecked())
	{
		dwHandleType += 16;
	}
	m_pstruMotion->struMotionHandleType.dwHandleType = dwHandleType;

	return HPR_OK;
}

//根据可用ID号,创建触发录像通道；
int CMotion::createChannelCheck()
{
	//先获取可用通道的数目
	int iChannelNum;
	int i;
	for(i = 0; i < MAX_CHANNUM_V30; i++)
	{
		if(m_iArryChannel[i] < 0 )
		{
			break;
		}
	}
	iChannelNum = i;

	//将channnel checkbox 加入到channel table中去
    ui.tableChannel->setRowCount(iChannelNum);
    ui.tableChannel->setColumnCount(1);
    QStringList list;
    list<<"触发通道";
    ui.tableChannel->setHorizontalHeaderLabels(list);
    int iRow = 0; //表示第几行
    char strChannelID[100] = {0};
	for(i = 0; i < MAX_CHANNUM_V30; i++)
	{
		if(m_iArryChannel[i] < 0 )
		{
			break;
		}
		else
		{
            memset(strChannelID, 0, sizeof(strChannelID));
            if(m_iArryChannel[i] < MAX_ANALOG_CHANNUM)
            {
            	sprintf(strChannelID, "Camera%d", m_iArryChannel[i]);
            }
            else
            {
            	sprintf(strChannelID, "IPCamera%d", m_iArryChannel[i]);
            }
            m_checkChannel[m_iArryChannel[i] - m_iStartChannelNo].setText(strChannelID);
			ui.tableChannel->setCellWidget(iRow, 0, &(m_checkChannel[m_iArryChannel[i] - m_iStartChannelNo]));
			iRow++;
		}
	}

	return HPR_OK;
}

int CMotion::setUserID(long lUserID)
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

//根据报警输出个数触发报警输出;
int CMotion::createAlarmOutCheck()
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
    	//不支持IP报警输出
    	memset(&struIPAlarmOut, 0, sizeof(struIPAlarmOut));
		iRet = NET_DVR_GetLastError();
		//QMessageBox::information(this, tr("Please check!"), tr("Get device parameters error! \"%1\"").arg(iRet));
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
int CMotion::getAlarmOutInfo(int iAlarmOut, QCheckBox *pQCheckAlarmOut)
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

//设置报警输出成员变量
int CMotion::setAlarmOutNum(int iAlarmOutNum)
{
	if(iAlarmOutNum < 0 || iAlarmOutNum > 64)
	{
		return HPR_ERROR;
	}
	m_iAlarmOutNum = iAlarmOutNum;
	return HPR_OK;
}

//设置通道数目
int CMotion::setChannelID(int *piChannelID)
{
	if(NULL == piChannelID)
	{
		return HPR_ERROR;
	}
	m_iArryChannel = piChannelID;
	return HPR_OK;
}

//设置移动侦测报警参数
int CMotion::setStruMotion(NET_DVR_MOTION_V30 *pstruMotion)
{
	if(NULL == pstruMotion)
	{
		return HPR_ERROR;
	}
	m_pstruMotion = pstruMotion;
	return HPR_OK;
}

//设置起始通道号
int CMotion::setStartChannelNo(int iStartChannelNo)
{
	if(iStartChannelNo < 0 || iStartChannelNo > 64)
	{
		return HPR_ERROR;
	}

	m_iStartChannelNo = iStartChannelNo;
	return HPR_OK;
}

void CMotion::comboWeekDayChanged(int iWeekDay)
{
	if(m_pstruMotion == NULL)
	{
		return;
	}
    int i;
    int iTemp;
	for(i = 0; i < MAX_TIMESEGMENT_V30; i++)
	{
	   	//时间
	   	iTemp = m_pstruMotion->struAlarmTime[iWeekDay][i].byStartHour;
   	    KIT_intToEdit(&(m_editStartHour[i]), iTemp);
   	    iTemp = m_pstruMotion->struAlarmTime[iWeekDay][i].byStartMin;
	   	KIT_intToEdit(&(m_editStartMin[i]), iTemp);
	   	iTemp = m_pstruMotion->struAlarmTime[iWeekDay][i].byStopHour;
	    KIT_intToEdit(&(m_editEndHour[i]), iTemp);
        iTemp = m_pstruMotion->struAlarmTime[iWeekDay][i].byStopMin;
        KIT_intToEdit(&(m_editEndMin[i]), iTemp);
	 }
}

//选中报警输出
void CMotion::checkAlarmOut()
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

void CMotion::on_btnSave_clicked()
{
	getEachWidgetValue();
}

//event
void CMotion::showEvent ( QShowEvent * event )
{
	if(event ==NULL)
	{
	}

	//获取显示参数
	setEachWidgetValue();
}
