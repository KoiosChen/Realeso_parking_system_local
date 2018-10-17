/************************************************************************
 *        Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *        FileName                      :   logsearch.cpp 
 *        Description                   :   log search
 *        Modification History          :   none
 *        Version                       :   V1.0
 *        Time                          :   2009-11,12
 *        Author                        :   wanggongpu
 *        Descrp                        :   
*************************************************************************/
#include "logsearch.h"
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QString>
#include "common.h"

/************************************************************************
 *        Function            :  LogSearch
 *        Description         :  instructor  
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
LogSearch::LogSearch(QList<DeviceData> *devicetree, int* userid, QDialog *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    m_lsuserid = userid;
    m_lsdevicetree =devicetree;
    
    QList<DeviceData>::iterator it;
    for ( it = (*m_lsdevicetree).begin(); it != (*m_lsdevicetree).end(); ++it)
    {
        if ((*it).getUsrID()==(*m_lsuserid))
        {
            ui.lineEdit_nodename->setText((*it).getDeviceName());
            ui.lineEdit_deviceIP->setText((*it).getIP());	
			ui.lineEdit_nodename->setDisabled(1);
			ui.lineEdit_deviceIP->setDisabled(1);
            break;
        }   
    }

	QDate startdate = QDateTime::currentDateTime().date();
	QDate stopdate = QDateTime::currentDateTime().date();
	QTime starttime(0, 0, 0);
	QTime stoptime(23, 59, 59);
	ui.dateTimeEdit_start->setDate(startdate);
	ui.dateTimeEdit_start->setTime(starttime);
	ui.dateTimeEdit_stop->setDate(stopdate);
	ui.dateTimeEdit_stop->setTime(stoptime);


	ui.comboBox_logtype->setCurrentIndex(2);
	m_lsmode = 2;
	ui.comboBox_maintype->setCurrentIndex(0);
	m_lsmaintype = 0;

}

/************************************************************************
 *        Function            :  ~LogSearch
 *        Description         :  destructor 
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
LogSearch::~LogSearch()
{

}

/************************************************************************
 *        Function            :  on_comboBox_logtype_currentIndexChanged
 *        Description         :  change log type 
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void LogSearch::on_comboBox_logtype_currentIndexChanged (int index)
{
    m_lsmode = index;
}

/************************************************************************
 *        Function            :  on_comboBox_maintype_currentIndexChanged
 *        Description         :  change the main type
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void LogSearch::on_comboBox_maintype_currentIndexChanged ( int index)
{
    m_lsmaintype = index;
    
}

/************************************************************************
 *        Function            :  on_comboBox_subtype_currentIndexChanged
 *        Description         :  change the sub type   
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void LogSearch::on_comboBox_subtype_currentIndexChanged (int index)
{
	if (index ==0)
	{
		m_lssubtype =0x1;
	}
	else if ((index >0)&&(index <=8))
	{
		m_lssubtype =index;
	}
	else if ((index >=10)&&(index <=20))
	{
		m_lssubtype = 0x20 - 9 + index;
	}
	else if((index >=22)&&(index <=25))
	{
		m_lssubtype = 0x40 - 21 + index;
	}
	else if((index >=26)&&(index <=41))
	{
		m_lssubtype = 0x50 - 26 + index;
	}
	else if((index >=42)&&(index <=54))
	{
		m_lssubtype = 0x60 - 42 + index;
	}
	else if((index >=55)&&(index <=86))
	{
		m_lssubtype = 0x70 - 55 + index;
	}
	else if((index >=88)&&(index <=95))
	{
		m_lssubtype = 0xa1 - 88 + index;
	}
	else
	{
		m_lssubtype =0x1;
	}
    
	
}

/************************************************************************
 *        Function            :  on_dateTimeEdit_start_dateTimeChanged
 *        Description         :  change the date time info  
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void LogSearch::on_dateTimeEdit_start_dateTimeChanged ( const QDateTime & datetime )
{
    m_lsstarttime.dwYear = datetime.date().year();
    m_lsstarttime.dwMonth = datetime.date().month();
    m_lsstarttime.dwDay = datetime.date().day();
    m_lsstarttime.dwHour = datetime.time().hour();
    m_lsstarttime.dwMinute = datetime.time().minute();
    m_lsstarttime.dwSecond = datetime.time().second();

    return;
}

/************************************************************************
 *        Function            :  on_dateTimeEdit_stop_dateTimeChanged
 *        Description         :  change the date time info  
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void LogSearch::on_dateTimeEdit_stop_dateTimeChanged ( const QDateTime & datetime )
{
    m_lsstoptime.dwYear = datetime.date().year();
    m_lsstoptime.dwMonth = datetime.date().month();
    m_lsstoptime.dwDay = datetime.date().day();
    m_lsstoptime.dwHour = datetime.time().hour();
    m_lsstoptime.dwMinute = datetime.time().minute();
    m_lsstoptime.dwSecond = datetime.time().second();

    return;
}

/************************************************************************
 *        Function            :  on_pushButton_searchlog_clicked
 *        Description         :  sure search
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void LogSearch::on_pushButton_searchlog_clicked()
{
    int logsearchhandle=0;

	for (int i=0; i<ui.logtableWidget->rowCount(); i++)
    {
        for (int j=0;j<ui.logtableWidget->columnCount();j++ )
        {
            delete ui.logtableWidget->takeItem(i, j);
        }
        ui.logtableWidget->removeRow(i);
    }
    ui.logtableWidget->setRowCount(0);

	int smart =0;
	if (ui.checkBox_smart->checkState()==2)
	{
		smart =1;
	}
    logsearchhandle = NET_DVR_FindDVRLog_V30(*m_lsuserid, m_lsmode,
        m_lsmaintype,m_lssubtype,&m_lsstarttime,&m_lsstoptime,smart);
    
    if (logsearchhandle == -1)
    {
        QMessageBox::information(this,tr("NET_DVR_FindDVRLog_V30 failed"), \
            tr("SDK_Last_Error =%1").arg(NET_DVR_GetLastError()));
        return ;
    }


    int nextreturn=0;
    NET_DVR_LOG_V30* LogData =NULL;
	LogData = (NET_DVR_LOG_V30* )malloc(sizeof(NET_DVR_LOG_V30));
	memset(LogData,0x0,sizeof(NET_DVR_LOG_V30));
    while (nextreturn !=-1)
    {
        nextreturn = NET_DVR_FindNextLog_V30(logsearchhandle, LogData);
        if (nextreturn < 0)
        {
            QMessageBox::information(this,tr("NET_DVR_FindDVRLog_V30 failed"), \
                tr("SDK_Last_Error =%1").arg(NET_DVR_GetLastError()));
            return ;
        }
        else
        {

            switch (nextreturn)
            {
                case NET_DVR_FILE_SUCCESS:
					
                    printTable(LogData);
                    break;
                case NET_DVR_FILE_NOFIND:
                    QMessageBox::information(this,tr("NET_DVR_FindDVRLog_V30"), 
                    tr("NET_DVR_FILE_NOFIND"));
					NET_DVR_FindLogClose_V30(logsearchhandle);
					free(LogData);
                    return;
                case NET_DVR_ISFINDING:
                    break;
                case NET_DVR_NOMOREFILE :
                    QMessageBox::information(this,tr("NET_DVR_FindDVRLog_V30"), 
                    tr("NET_DVR_NOMOREFILE"));
					NET_DVR_FindLogClose_V30(logsearchhandle);
					free(LogData);
                    return; 
                case NET_DVR_FILE_EXCEPTION :
                    QMessageBox::information(this,tr("NET_DVR_FindDVRLog_V30"), 
                    tr("NET_DVR_FILE_EXCEPTION"));
					//NET_DVR_FindLogClose_V30(logsearchhandle);
					free(LogData);
                    return; 
            }
        }
		
    }
	free(LogData);
	NET_DVR_FindLogClose_V30(logsearchhandle);
}

/************************************************************************
 *        Function            :  printTable
 *        Description         :  show the item of search result in table 
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void LogSearch::printTable(NET_DVR_LOG_V30* LogData)
{
    int row=0;
    row = ui.logtableWidget->rowCount();
    QString datetimeinfo;
    datetimeinfo.append(QString::number(LogData->strLogTime.dwYear));
    datetimeinfo.append("-");
    datetimeinfo.append(QString::number(LogData->strLogTime.dwMonth));
    datetimeinfo.append("-");
    datetimeinfo.append(QString::number(LogData->strLogTime.dwDay));
    datetimeinfo.append(" ");
    datetimeinfo.append(QString::number(LogData->strLogTime.dwHour));
    datetimeinfo.append(":");
    datetimeinfo.append(QString::number(LogData->strLogTime.dwMinute));
    datetimeinfo.append(":");
    datetimeinfo.append(QString::number(LogData->strLogTime.dwSecond));
    QTableWidgetItem *datetime = new QTableWidgetItem(tr("%1").arg(datetimeinfo));
    datetime->setFlags(Qt::ItemIsEnabled);

	QString majortype;
	switch(int(LogData->dwMajorType))
	{
		/*
		MAJOR_ALARM 0x1 报警 
		MAJOR_EXCEPTION 0x2 异常 
		MAJOR_OPERATION 0x3 操作 
		MAJOR_INFORMATION 0x4 日志附加信息 
		*/		
		case 1:
			majortype.append("MAJOR_ALARM");
			break;
		case 2:
			majortype.append("MAJOR_EXCEPTION");
			break;
		case 3:
			majortype.append("MAJOR_OPERATION");
			break;
		case 4:
			majortype.append("MAJOR_INFORMATION");
			break;
		default:
			majortype.append("UNKOWN_TYPE");
			break;

	}
	

    QTableWidgetItem *maintype = new QTableWidgetItem(majortype);
    maintype->setFlags(Qt::ItemIsEnabled);

    QTableWidgetItem *subtype = new QTableWidgetItem(tr("%1 ").arg(int(LogData->dwMinorType)));
    subtype->setFlags(Qt::ItemIsEnabled);

    QTableWidgetItem *remotehost = new 
    QTableWidgetItem(tr("%1").arg(LogData->struRemoteHostAddr.sIpV4));
    remotehost->setFlags(Qt::ItemIsEnabled);

	QTableWidgetItem *description;
    char *info=NULL;
    info= new char[LOG_INFO_LEN];
	if (LogData->dwInfoLen != 0)
	{
		memcpy(info,&LogData->sInfo,sizeof(char)*(LogData->dwInfoLen)); 
		description = new QTableWidgetItem(tr("%1").arg(info));
			
	}
    else
    {
		description = new QTableWidgetItem("");
	}
   
    description->setFlags(Qt::ItemIsEnabled);
    description->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
   

    ui.logtableWidget->insertRow(row);
    ui.logtableWidget->setItem(row, 0, datetime);
    ui.logtableWidget->setItem(row, 1, maintype);
    ui.logtableWidget->setItem(row, 2, subtype);
    ui.logtableWidget->setItem(row, 3, remotehost);
    ui.logtableWidget->setItem(row, 4, description);
	delete []info;
}

/************************************************************************
 *        Function            :  on_pushButton_exit_clicked
 *        Description         :  exit
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void LogSearch::on_pushButton_exit_clicked()
{
    close();
}
    
