
#include <QMessageBox>
#include "cdlgremoterecordschedule.h"
#include "publicfuc.h"


CDlgRemoteRecordSchedule::CDlgRemoteRecordSchedule(QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);

	//录象类型：0-定时录像，1-移动侦测录像，2-报警录像，3-移动侦测或报警录像，4-移动侦测和报警录像，5-命令触发录像，6-智能报警录像
	QStringList strings;
	strings<<"Schedule"<<"Motion detect"<<"Alarm"<<"Motion or alarm"<<"Motion and alarm"<<"Command trigger"<<"VCA alarm";
	int i;
	for(i=0; i < 8; i++)
	{
		ui.vLayStartHour->addWidget(&(m_editStartHour[i]));
		ui.vLayStartMin->addWidget(&(m_editStartMin[i]));
		ui.vLayEndHour->addWidget(&(m_editEndHour[i]));
		ui.vLayEndMin->addWidget(&(m_editEndMin[i]));
		ui.vLayRecordType->addWidget(&(m_comboRecordType[i]));
		m_comboRecordType[i].addItems(strings);
	}
	ui.comboDayRecordType->addItems(strings);
	strings.clear();
	strings<<"Monday"<<"Tuesday"<<"Wednesday"<<"Thursday"<<"Friday"<<"Saturday"<<"Sunday";
	ui.comboWeekDay->addItems(strings);
	strings<<"Whole week";
	ui.comboCopyRecordDay->addItems(strings);

	m_pstruRecord = NULL;

	connect(ui.comboWeekDay, SIGNAL(activated(int)), this, SLOT(comboWeekDayChanged (int)));
    connect(ui.checkRecordAllDay, SIGNAL(clicked()), this, SLOT(clickRecordAllDay()));
}

CDlgRemoteRecordSchedule::~CDlgRemoteRecordSchedule()
{

}

void CDlgRemoteRecordSchedule::on_btnCopyDay_clicked()
{
	if(m_pstruRecord == NULL)
	{
	    QMessageBox::information(this, tr("Please check!"), tr("the m_pstruRecord is null!"));
	    return;
	}
	int iCopyDay;;
	int i;
	iCopyDay = ui.comboCopyRecordDay->currentIndex();

	if(iCopyDay < 7)//只复制某一天
	{
		copyDaySetting(iCopyDay);
	}
	else //复制到所有天
	{
		for(i = 0; i < MAX_DAYS; i++)
		{
			copyDaySetting(i);
		}
	}
}

//保存按钮被点击
void CDlgRemoteRecordSchedule::on_btnSave_clicked()
{
	if(m_pstruRecord != NULL)
	{
		int iWeekDay;
		int i;
		int iTime;
		iWeekDay = ui.comboWeekDay->currentIndex();
		if(ui.checkRecordAllDay->isChecked())//全天录像
		{
			m_pstruRecord->struRecAllDay[iWeekDay].wAllDayRecord = 1;
			m_pstruRecord->struRecAllDay[iWeekDay].byRecordType = ( BYTE)(ui.comboDayRecordType->currentIndex());
		}
		else//时间段录像
		{
			for(i = 0; i < MAX_TIMESEGMENT; i++)
			{
				//类型
				m_pstruRecord->struRecordSched[iWeekDay][i].byRecordType = m_comboRecordType[i].currentIndex();
                //开始小时
				KIT_editToInt(&(m_editStartHour[i]), &iTime);
				m_pstruRecord->struRecordSched[iWeekDay][i].struRecordTime.byStartHour = iTime;
				//开始分钟
				KIT_editToInt(&(m_editStartMin[i]), &iTime);
				m_pstruRecord->struRecordSched[iWeekDay][i].struRecordTime.byStartMin = iTime;
				//结束小时
				KIT_editToInt(&(m_editEndHour[i]), &iTime);
				m_pstruRecord->struRecordSched[iWeekDay][i].struRecordTime.byStopHour = iTime;
				//结束分钟
				KIT_editToInt(&(m_editEndMin[i]), &iTime);
				m_pstruRecord->struRecordSched[iWeekDay][i].struRecordTime.byStopMin = iTime;
			}
		}
	}
	else
	{
	    QMessageBox::information(this, tr("Please check!"), tr("the m_pstruRecord is null!"));
	}
}

int CDlgRemoteRecordSchedule::comboWeekDayChanged(int iWeekDay)
{
	setEachWidget(iWeekDay);
	return HPR_OK;
}

int CDlgRemoteRecordSchedule::clickRecordAllDay()
{
	int i;
	if(ui.checkRecordAllDay->isChecked())
	{
		ui.comboDayRecordType->setEnabled(TRUE);
		for(i = 0; i <MAX_TIMESEGMENT_V30; i++)
		{
			m_comboRecordType[i].setEnabled(FALSE);
			m_editStartHour[i].setEnabled(FALSE);
			m_editStartMin[i].setEnabled(FALSE);
			m_editEndHour[i].setEnabled(FALSE);
			m_editEndMin[i].setEnabled(FALSE);
		}
	}
	else
	{
		ui.comboDayRecordType->setEnabled(FALSE);
		for(i = 0; i <MAX_TIMESEGMENT_V30; i++)
		{
			m_comboRecordType[i].setEnabled(TRUE);
			m_editStartHour[i].setEnabled(TRUE);
			m_editStartMin[i].setEnabled(TRUE);
			m_editEndHour[i].setEnabled(TRUE);
			m_editEndMin[i].setEnabled(TRUE);
		}
	}

	return HPR_OK;
}


//拷贝当前设置到某一天，内部接口默认m_pstruRecord不为空
int CDlgRemoteRecordSchedule::copyDaySetting(int iWeekNo)
{
	int i;
	int iTime;

	if(ui.checkRecordAllDay->isChecked())
	{
		m_pstruRecord->struRecAllDay[iWeekNo].wAllDayRecord = 1;
		m_pstruRecord->struRecAllDay[iWeekNo].byRecordType = ( BYTE)(ui.comboDayRecordType->currentIndex());
	}
	else
	{
		for (i = 0; i < MAX_TIMESEGMENT_V30; i++)
		{
			//类型
			m_pstruRecord->struRecordSched[iWeekNo][i].byRecordType = m_comboRecordType[i].currentIndex();
			//开始小时
			KIT_editToInt(&(m_editStartHour[i]), &iTime);
			m_pstruRecord->struRecordSched[iWeekNo][i].struRecordTime.byStartHour = iTime;
			//开始分钟
			KIT_editToInt(&(m_editStartMin[i]), &iTime);
			m_pstruRecord->struRecordSched[iWeekNo][i].struRecordTime.byStartMin
					= iTime;
			//结束小时
			KIT_editToInt(&(m_editEndHour[i]), &iTime);
			m_pstruRecord->struRecordSched[iWeekNo][i].struRecordTime.byStopHour
					= iTime;
			//结束分钟
			KIT_editToInt(&(m_editEndMin[i]), &iTime);
			m_pstruRecord->struRecordSched[iWeekNo][i].struRecordTime.byStopMin = iTime;
		}
	}

     return HPR_ERROR;
}

int CDlgRemoteRecordSchedule::SetStruRecord(NET_DVR_RECORD_V30 *pstruRecord)
{
	m_pstruRecord = pstruRecord;
	return HPR_ERROR;
}

//event
void CDlgRemoteRecordSchedule::showEvent ( QShowEvent * event )
{
	if(event ==NULL)
	{
	}
	setEachWidget(0);
}

int  CDlgRemoteRecordSchedule::setEachWidget(int iWeekDay)
{
	if(m_pstruRecord == NULL)
	{
	    QMessageBox::information(this, tr("Please check!"), tr("the m_pstruRecord is null!"));
	    return HPR_ERROR;
	}
	int iTemp;
	iTemp = m_pstruRecord->struRecAllDay[iWeekDay].byRecordType;
    ui.comboDayRecordType->setCurrentIndex(int(iTemp));
    iTemp = m_pstruRecord->struRecAllDay[iWeekDay].wAllDayRecord;
    if(iTemp == 0)//是否全天录像
    {
    	ui.checkRecordAllDay->setCheckState(Qt::Unchecked);
    }
    else
    {
	   	ui.checkRecordAllDay->setCheckState(Qt::Checked);
	 }
     int i;
	 for(i = 0; i < MAX_TIMESEGMENT_V30; i++)
	 {
	   	//类型
	   	iTemp = m_pstruRecord->struRecordSched[iWeekDay][i].byRecordType;
	   	m_comboRecordType[i].setCurrentIndex(iTemp);
	   	//时间
    	iTemp = m_pstruRecord->struRecordSched[iWeekDay][i].struRecordTime.byStartHour;
    	KIT_intToEdit(&(m_editStartHour[i]), iTemp);
    	iTemp = m_pstruRecord->struRecordSched[iWeekDay][i].struRecordTime.byStartMin;
	   	KIT_intToEdit(&(m_editStartMin[i]), iTemp);
	   	iTemp = m_pstruRecord->struRecordSched[iWeekDay][i].struRecordTime.byStopHour;
	    KIT_intToEdit(&(m_editEndHour[i]), iTemp);
        iTemp = m_pstruRecord->struRecordSched[iWeekDay][i].struRecordTime.byStopMin;
        KIT_intToEdit(&(m_editEndMin[i]), iTemp);
	 }

	return HPR_OK;
}


