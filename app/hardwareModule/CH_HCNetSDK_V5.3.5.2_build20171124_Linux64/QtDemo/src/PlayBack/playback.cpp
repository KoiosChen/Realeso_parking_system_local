/************************************************************************
 *        	Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *        	FileName                      	:   	playback.cpp
 *        	Description                   	:   	playback source file 
 *			Modification History			:      
 *  		<version >		<time>     		<author>      	<desc>
 *  		V1.0			2010-1,2		wanggongpu   	creating
************************************************************************/
#include <QMessageBox>
#include <QDateTime>
#include <QDir>
#include <QDate>
#include <QTime>
#include <QFileDialog>
#include <QTextStream>
#include <QStringList>
#include <QFile>
#include <QIODevice>
#include <QFileInfo>
#include <QtGlobal>
#include "playback.h"
#include "common.h"
#include <unistd.h>

/************************************************************************
 *	Function  		:	PlayBack
 *  Description     :  	Constructor of PlayBack class
 *  Input           :  	
 *						tree //device list tree
 *						usrid //device userid 
 *						devinfo //device info
 *						channelnum //channel number
 *						parent //parent class QWidget
 *  Output          : 	none
 *  Return          :   none
************************************************************************/
PlayBack::PlayBack( QList<DeviceData> * tree, int * usrid, 
					NET_DVR_DEVICEINFO_V30 * devinfo, int * channelnum, 
    				LogTable * table, QWidget *parent )  : QWidget( parent )
{
	//setup ui interface
    ui.setupUi(this);
	if (table != NULL)
	{
		m_pblogtable = table;
	}
	else
	{
		m_pblogtable = NULL;
	}
	//parameters Assignment
	m_qlistdevicetree = tree;
    m_pbUserID = usrid;
    m_pbCurrChannel = channelnum;
    m_pbCurrDevInfo = devinfo;
	
	//set init value
    m_pbqtimer = NULL ;
	m_pbpDvrFile = NULL;
	m_pblocalfilepath.clear();
	m_pbdstarttime.clear();
    m_pbdstoptime.clear();
	m_pbfiletype = 0;
    m_pbfileattr = 0;
    m_pbcontrollabel_1 = 0;
	m_pbcontrollabel_3 = 0;
	m_pbvolumeflag = 0 ;
	m_pbdownloading = 0 ;
	m_pbsaving = 0;
	m_pbdownloadhandle = -1;

	//play handle
    m_pbhandle = -1;

	//set page titles
    ui.tabWidget->setTabText(ui.tabWidget->indexOf(ui.tab), 
    	QApplication::translate("PlayBackClass", 
       	"remote file", 
        0, QApplication::UnicodeUTF8));
    ui.tabWidget->setTabText(ui.tabWidget->indexOf(ui.tab_2),
     	QApplication::translate("PlayBackClass", 
      	"local file", 
       	0, QApplication::UnicodeUTF8));
	ui.tabWidget->setTabText(ui.tabWidget->indexOf(ui.tab_3), 
       	QApplication::translate("PlayBackClass", 
     	"playback by time", 
       	0, QApplication::UnicodeUTF8));	
	
	//the begin page is remote page
	ui.tabWidget->setCurrentIndex(0);
	m_pbpageindex = 0;
	m_pbcurrentpageindex = 0;
	ui.vboxLayout->addWidget(&m_pbframe);
	m_pbframe.setMinimumSize(451, 370);
	ui.pushButton_playback_stop_1->setDisabled(1);
	ui.pushButton_playback_slow_1->setDisabled(1);
	ui.pushButton_playback_fast_1->setDisabled(1);
	ui.pushButton_playback_picture_1->setDisabled(1);
	ui.pushButton_playback_pos_1->setDisabled(1);
	ui.pushButton_playback_frame_1->setDisabled(1);
	ui.pushButton_playback_volume_1->setDisabled(1);
	ui.checkBox_1_carno->setDisabled(1);
	ui.lineEdit_1_cardSID->setDisabled(1);
	
	//tabPageChanged
	connect(ui.tabWidget, SIGNAL(currentChanged(int)),
		this, SLOT( tabPageChanged(int)));

	//table item been ticked emit signals and slots
    connect(ui.tableWidget_1_filelist, SIGNAL(cellClicked(int, int)), this,
           		SLOT(prepareDownload(int, int)));
    connect(ui.tableWidget_1_filelist, SIGNAL(cellDoubleClicked(int, int)), this,
                SLOT(preparePlay(int , int)));
	connect(ui.tableWidget_2_localfilist, SIGNAL(cellClicked(int, int)), this,
            	SLOT(localtablebeenclicked(int , int)));



	//set begin and end time value
	QDate startdate = QDateTime::currentDateTime().date();
	QDate stopdate = QDateTime::currentDateTime().date();
	QTime starttime(0, 0, 0);
	QTime stoptime(23, 59, 59);
	ui.dateTimeEdit_1_start->setDate(startdate);
	ui.dateTimeEdit_1_start->setTime(starttime);
	ui.dateTimeEdit_1_stop->setDate(stopdate);
	ui.dateTimeEdit_1_stop->setTime(stoptime);
	ui.dateTimeEdit_2_start->setDate(startdate);
	ui.dateTimeEdit_2_start->setTime(starttime);
	ui.dateTimeEdit_2_stop->setDate(stopdate);
	ui.dateTimeEdit_2_stop->setTime(stoptime);
	ui.dateTimeEdit_3_start->setDate(startdate);
	ui.dateTimeEdit_3_start->setTime(starttime);
	ui.dateTimeEdit_3_stop->setDate(stopdate);
	ui.dateTimeEdit_3_stop->setTime(stoptime);
}

/************************************************************************
 *	Function  		:	~PlayBack
 *  Description     :  	Destructor of PlayBack class
 *  Input           :  	none
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
PlayBack::~PlayBack()
{
	if (m_pbqtimer != NULL)
	{
		delete m_pbqtimer;
		m_pbqtimer = NULL ;
	}
	if (m_pbcurrentpageindex == 0)
	{
		on_pushButton_playback_stop_1_clicked();
	}
	else if (m_pbcurrentpageindex == 1)
	{
		on_pushButton_playback_stop_2_clicked();
	}
	else if (m_pbcurrentpageindex == 2)
	{
		on_pushButton_playback_stop_3_clicked();
	}

  	for (int i = 0; i<ui.tableWidget_1_filelist->rowCount(); i++)
    {
        for (int j = 0; j<ui.tableWidget_1_filelist->columnCount(); j++ )
        {
            delete ui.tableWidget_1_filelist->takeItem(i, j);
        }
        ui.tableWidget_1_filelist->removeRow(i);
    }
	for (int  i = 0; i<ui.tableWidget_2_localfilist->rowCount(); i++)
    {
        for (int j = 0; j<ui.tableWidget_2_localfilist->columnCount(); j++ )
        {
            delete ui.tableWidget_2_localfilist->takeItem(i, j);
        }
        ui.tableWidget_2_localfilist->removeRow(i);
    }
}

/************************************************************************
 *	Function  		:	DrawAreaInit
 *  Description     :  	init playm4 paint 
 *  Input           :  	none
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::DrawAreaInit()
{

}

/************************************************************************
 *	Function  		:	PlaybackCallBack
 *  Description     :  	call back
 *  Input           :  	
 *						lPlayHandle
 *						dwDataType
 *						pBuffer
 *						dwBufSize
 *						dwUser
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlaybackCallBack(int lPlayHandle, 
                      unsigned int dwDataType, 
                      char *pBuffer,
                      unsigned int dwBufSize,
                      unsigned int dwUser)
{
        fprintf(stderr,\
            "lPlaybackHandle[%d]: Get PlayBack StreamData! Type[%d],BufSize[%d],dwUser[%d]\n",\
            lPlayHandle, dwDataType, dwBufSize, dwUser);
}   

/************************************************************************
 *	Function  		:	tabPageChanged
 *  Description     :  	change page slots func
 *  Input           :  	
 *						index //0 remote 1 local 2 time
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::tabPageChanged( int index )
{	
	//the same page as last page
	if ( index == m_pbcurrentpageindex )
	{
		return;
	}
	//close play back
	if ( 0 == m_pbcurrentpageindex )
	{	
		on_pushButton_playback_stop_1_clicked();
	}
	else if( 1 == m_pbcurrentpageindex )
	{
		on_pushButton_playback_stop_2_clicked();
	}
	else if ( 2 == m_pbcurrentpageindex )
	{
		on_pushButton_playback_stop_3_clicked();
	}
		
	//load new page
	if ( 0 == index )
	{	
		//remote page
		ui.vboxLayout->addWidget( &m_pbframe );
	    m_pbframe.setMinimumSize(451, 370);
		m_pbcurrentpageindex = 0;
		ui.pushButton_playback_stop_1->setDisabled(1);
        ui.pushButton_playback_slow_1->setDisabled( 1 );
        ui.pushButton_playback_fast_1->setDisabled( 1 );
        ui.pushButton_playback_picture_1->setDisabled( 1 );
        ui.pushButton_playback_pos_1->setDisabled( 1 );
        ui.pushButton_playback_frame_1->setDisabled( 1 );
        ui.pushButton_playback_volume_1->setDisabled( 1 );

		ui.checkBox_1_carno->setDisabled(1);
        ui.lineEdit_1_cardSID->setDisabled(1);
	}
	else if(1 == index)
	{
		//local page
		ui.vboxLayout1->addWidget(&m_pbframe);
	    m_pbframe.setMinimumSize(445,430);
		m_pbcurrentpageindex =1;

		ui.pushButton_playback_stop_2->setDisabled(1);
        ui.pushButton_playback_slow_2->setDisabled(1);
        ui.pushButton_playback_fast_2->setDisabled(1);
        ui.pushButton_playback_picture_2->setDisabled(1);
        ui.pushButton_playback_pos_2->setDisabled(1);
        ui.pushButton_playback_frame_2->setDisabled(1);
        ui.pushButton_playback_volume_2->setDisabled(1);
	}
	else if (2 == index)
	{
		//time page
		ui.vboxLayout2->addWidget(&m_pbframe);
	    m_pbframe.setMinimumSize(590, 430);
		m_pbcurrentpageindex = 2;

	}
	m_pbframe.show();

    return;
}


/***page 0****/


/************************************************************************
 *	Function  		:	insertrow
 *  Description     :  	remote page find files and insert the table
 *  Input           :  	
 *						dvrfile //result of find file
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::insertrow(NET_DVR_FINDDATA_V30 &dvrfile)
{
    int row ;
    QTableWidgetItem *fileNameItem = new QTableWidgetItem(dvrfile.sFileName);
    fileNameItem->setFlags(Qt::ItemIsEnabled);
    QTableWidgetItem *sizeItem = new QTableWidgetItem(tr("%1").arg(
													  int(dvrfile.dwFileSize)));
    sizeItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    sizeItem->setFlags(Qt::ItemIsEnabled);
	QString month;
	month = QString::number(dvrfile.struStartTime.dwMonth);
	if (dvrfile.struStartTime.dwMonth <=9)
	{
		month.prepend("0");
	}
	QString day;
	day = QString::number(dvrfile.struStartTime.dwDay);
	if (dvrfile.struStartTime.dwDay <= 9)
	{
		day.prepend("0");
	}
	QString hour;
	hour = QString::number(dvrfile.struStartTime.dwHour);
	if (dvrfile.struStartTime.dwHour <= 9)
	{
		hour.prepend("0");
	}
	QString minute;
	minute = QString::number(dvrfile.struStartTime.dwMinute);
	if (dvrfile.struStartTime.dwMinute <= 9)
	{
		minute.prepend("0");
	}
	QString sec;
	sec = QString::number(dvrfile.struStartTime.dwSecond);
	if (dvrfile.struStartTime.dwSecond <= 9)
	{
		sec.prepend("0");
	}
    QTableWidgetItem *beginTimeItem = new QTableWidgetItem( 
        tr("%1%2%3%4%5%6").arg(int(dvrfile.struStartTime.dwYear)
        ).arg(month.toLatin1().data()).arg(day.toLatin1().data() 
        ).arg(hour.toLatin1().data()).arg(minute.toLatin1().data() 
        ).arg(sec.toLatin1().data()));
    beginTimeItem->setFlags(Qt::ItemIsEnabled);
	month =QString::number(dvrfile.struStopTime.dwMonth);
	if (dvrfile.struStopTime.dwMonth <= 9)
	{
		month.prepend("0");
	}
	day = QString::number(dvrfile.struStopTime.dwDay);
	if (dvrfile.struStopTime.dwDay <= 9)
	{
		day.prepend("0");
	}
	hour = QString::number(dvrfile.struStopTime.dwHour);
	if (dvrfile.struStopTime.dwHour <= 9)
	{
		hour.prepend("0");
	}
	minute = QString::number(dvrfile.struStopTime.dwMinute);
	if (dvrfile.struStopTime.dwMinute <= 9)
	{
		minute.prepend("0");
	}
	sec = QString::number(dvrfile.struStopTime.dwSecond);
	if (dvrfile.struStopTime.dwSecond <= 9)
	{
		sec.prepend("0");
	}
    QTableWidgetItem *endTimeItem = new QTableWidgetItem( 
        tr("%1%2%3%4%5%6").arg(int(dvrfile.struStopTime.dwYear)).arg(
        month.toLatin1().data()).arg(day.toLatin1().data() 
        ).arg(hour.toLatin1().data()).arg(minute.toLatin1().data() 
        ).arg(sec.toLatin1().data()));
    endTimeItem->setFlags(Qt::ItemIsEnabled);
    QTableWidgetItem *lockAttrItem = NULL;
    if (dvrfile.byLocked == 0)
    {
        lockAttrItem = new QTableWidgetItem("unlocked");
        lockAttrItem->setFlags(Qt::ItemIsEnabled);
    }
    else
    {
        lockAttrItem = new QTableWidgetItem("locked");
        lockAttrItem->setFlags(Qt::ItemIsEnabled);
    }
    row = ui.tableWidget_1_filelist->rowCount();
    ui.tableWidget_1_filelist->insertRow(row);
    ui.tableWidget_1_filelist->setItem(row, 0, fileNameItem);
    ui.tableWidget_1_filelist->setItem(row, 1, sizeItem);
    ui.tableWidget_1_filelist->setItem(row, 2, beginTimeItem);
    ui.tableWidget_1_filelist->setItem(row, 3, endTimeItem);
    ui.tableWidget_1_filelist->setItem(row, 4, lockAttrItem);
    return ;
}

/************************************************************************
 *	Function  		:	playbackcontrol
 *  Description     :  	control playback
 *  Input           :  	
 *						command //control command
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::playbackcontrol(int command)
{
    if (!NET_DVR_PlayBackControl(m_pbhandle, command, 0, NULL))
    {
        QMessageBox::information(this, tr("NET_DVR_PlayBackControl failed"), 
                tr("SDK_Last_Error =%1").arg(NET_DVR_GetLastError()));

    }
	else
	{

	}
    return;
}

/************************************************************************
 *	Function  		:	prepareDownload
 *  Description     :  	select a cell remote page
 *  Input           :  	
 *						row //cell row
 *						column	//cell column
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::prepareDownload(int row,int column)
{
    if (0 != column)
    {
        ui.lineEdit_1_filetotalsize->setText("");
        ui.lineEdit_1_downfilename->setText("");
        return;
    }
	QString locklabel("locked");
	if (ui.tableWidget_1_filelist->item(row,column + 4 )->text()==locklabel)
	{
		m_pblockflag =1;
        ui.pushButton_1_lock->setText( QApplication::translate("TranslateClass", 
            "unlock", 0, QApplication::UnicodeUTF8));
	}
	else
	{
		m_pblockflag =0;
        ui.pushButton_1_lock->setText( QApplication::translate("TranslateClass", 
            "lock", 0, QApplication::UnicodeUTF8));
	}
    ui.lineEdit_1_filetotalsize->setText(ui.tableWidget_1_filelist->item(row,
										column + 1 )->text());
    ui.lineEdit_1_downfilename->setText(ui.tableWidget_1_filelist->item(row,
										column )->text());
    m_pbdstarttime = ui.tableWidget_1_filelist->item(row,column+2)->text();
    m_pbdstoptime = ui.tableWidget_1_filelist->item(row,column+3)->text();
}

/************************************************************************
 *	Function  		:	preparePlay
 *  Description     :  	play remote save file
 *  Input           :  	
 *						row //cell row
 						column // cell column
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::preparePlay(int row, int column)
{
    if (0 != column)
    {
        return;
    }
    QString filename(ui.tableWidget_1_filelist->item(row,column)->text());

    NET_DVR_StopPlayBack(m_pbhandle);
    m_pbhandle = -1;

    m_pbhandle = NET_DVR_PlayBackByName(*m_pbUserID, filename.toLatin1().data(), (HWND)m_pbframe.winId());
    if (m_pbhandle < 0) 
    {
        QMessageBox::information(this, \
            tr("PlayBackByName failed"), \
            tr("SDK_Last_Error =%1").arg(NET_DVR_GetLastError()));
    }
   
    //NET_DVR_SetPlayDataCallBack(m_pbhandle, PlaybackCallBack, 9);
    m_pbcontrollabel_1 = 0;
    on_pushButton_playback_start_1_clicked();
	return;
}

/************************************************************************
 *	Function  		:	updateTime
 *  Description     :  	update Time
 *  Input           :  	
 *						none
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::updateTime()
{
	//注销报错信息是因为当从设备树上退出设备时这里会一直在更新，
	//而资源已经被释放，会不停的提示报错信息
	if (m_pbhandle >= 0)
	{
		DWORD totaltime = 0;
		if (!NET_DVR_PlayBackControl(m_pbhandle, NET_DVR_PLAYGETTIME , 0, &totaltime))
		{
	        //QMessageBox::information(this,tr("NET_DVR_PlayBackControl failed"),
	        	//tr("SDK_Last_Error =%1").arg(NET_DVR_GetLastError()));
	    	
			return;
		}
		QString qtotaltime;
		QString timevalue;
		if (totaltime/3600 <= 9)
		{
			qtotaltime.append("0");
		}
		timevalue = QString::number(totaltime/3600);
		qtotaltime.append(timevalue);
		qtotaltime.append(":");
		if (((totaltime%3600)/60) <= 9)
		{
			qtotaltime.append("0");
		}
		timevalue = QString::number((totaltime%3600)/60);
		qtotaltime.append(timevalue);
		qtotaltime.append(":");
		if ((totaltime%60) <= 9)
		{
			qtotaltime.append("0");
		}
		timevalue = QString::number(totaltime%60);
		qtotaltime.append(timevalue);
		
		unsigned int timeplayed = totaltime;
		
		qtotaltime.append("/");
		if (!NET_DVR_PlayBackControl(m_pbhandle, NET_DVR_GETTOTALTIME , 0, &totaltime))
	    {
	        //QMessageBox::information(this,tr("NET_DVR_PlayBackControl failed"), 
	        	//tr("SDK_Last_Error =%1").arg(NET_DVR_GetLastError()));
	    	return;
		}
		
		if (totaltime/3600 <= 9)
		{
			qtotaltime.append("0");
		}
		timevalue = QString::number(totaltime/3600);
		qtotaltime.append(timevalue);
		qtotaltime.append(":");
		if (((timeplayed%3600)/60) <= 9)
		{
			qtotaltime.append("0");
		}
		timevalue = QString::number((totaltime%3600)/60);
		qtotaltime.append(timevalue);
		qtotaltime.append(":");
		if ((totaltime%60) <= 9)
		{
			qtotaltime.append("0");
		}
		timevalue = QString::number(totaltime%60);
		qtotaltime.append(timevalue);
		//blank space
		qtotaltime.append("  ");
		qtotaltime.append(ui.lineEdit_1_downfilename->text());
		ui.lineEdit_1_pbprocess->setText(qtotaltime);

		if ((timeplayed*100)%(totaltime) == 0)
		{	
		    DWORD currentpos;
			if (!NET_DVR_PlayBackControl(m_pbhandle, NET_DVR_PLAYGETPOS , 0, &currentpos))
		    {
		    	//QMessageBox::information(this,tr("NET_DVR_PLAYGETPOS failed"), 
					//tr("SDK_Last_Error =%1").arg(NET_DVR_GetLastError()));
				return;
			}

			ui.horizontalSlider_1_playbackprocess->setValue(currentpos);
		}

	}

    return;
}

/************************************************************************
 *	Function  		:	on_dateTimeEdit_1_start_dateTimeChanged
 *  Description     :  	change time info
 *  Input           :  	
 *						datetime //start date time
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::on_dateTimeEdit_1_start_dateTimeChanged (const QDateTime & datetime)
{
    m_pbremotestarttime.dwYear = datetime.date().year();
    m_pbremotestarttime.dwMonth = datetime.date().month();
    m_pbremotestarttime.dwDay = datetime.date().day();
    m_pbremotestarttime.dwHour = datetime.time().hour();
    m_pbremotestarttime.dwMinute = datetime.time().minute();
    m_pbremotestarttime.dwSecond = datetime.time().second();
	return;
}

/************************************************************************
 *	Function  		:	on_dateTimeEdit_1_stop_dateTimeChanged
 *  Description     :  	change time info
 *  Input           :  	
 *						datetime //new date time value
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::on_dateTimeEdit_1_stop_dateTimeChanged ( const QDateTime & datetime )
{
    m_pbremotestoptime.dwYear = datetime.date().year();
    m_pbremotestoptime.dwMonth = datetime.date().month();
    m_pbremotestoptime.dwDay = datetime.date().day();
    m_pbremotestoptime.dwHour = datetime.time().hour();
    m_pbremotestoptime.dwMinute = datetime.time().minute();
    m_pbremotestoptime.dwSecond = datetime.time().second();
	return;
}

/************************************************************************
 *	Function  		:	on_comboBox_1_filetype_currentIndexChanged
 *  Description     :  	change file type choise
 *  Input           :  	
 *						index //file type index 
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::on_comboBox_1_filetype_currentIndexChanged( int index)
{
    if (index == 0)
    {
        m_pbfiletype = 0xff;
    }
    m_pbfiletype = index-1;
    return;
}

/************************************************************************
 *	Function  		:	on_comboBox_1_fileattr_currentIndexChanged
 *  Description     :  	file attr change 
 *  Input           :  	
 *						index //file attr index 0 is all types
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::on_comboBox_1_fileattr_currentIndexChanged ( int index)
{
    if (index == 0)
    {
        m_pbfileattr = 0xff;
    }
    m_pbfileattr = index-1;
    return;
}

/************************************************************************
 *	Function  		:	on_pushButton_1_search_clicked
 *  Description     :  	search save file on remote device
 *  Input           :  	
 *						none
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::on_pushButton_1_search_clicked()
{
    int index =0;
	//get current file type
    index = ui.comboBox_1_filetype->currentIndex();
    m_pbfiletype = index - 1;
    if (index == 0)
    {
        m_pbfiletype = 0xff;
    }

    //get current file attr
    index = ui.comboBox_1_fileattr->currentIndex();
    m_pbfileattr = index-1;
    if (index == 0)
    {
        m_pbfileattr = 0xff;
    }

    //get the begin and end date time
    m_pbremotestarttime.dwYear = ui.dateTimeEdit_1_start->date().year();
    m_pbremotestarttime.dwMonth = ui.dateTimeEdit_1_start->date().month();
    m_pbremotestarttime.dwDay = ui.dateTimeEdit_1_start->date().day();
    m_pbremotestarttime.dwHour = ui.dateTimeEdit_1_start->time().hour();
    m_pbremotestarttime.dwMinute = ui.dateTimeEdit_1_start->time().minute();
    m_pbremotestarttime.dwSecond = ui.dateTimeEdit_1_start->time().second();
    m_pbremotestoptime.dwYear = ui.dateTimeEdit_1_stop->date().year();
    m_pbremotestoptime.dwMonth = ui.dateTimeEdit_1_stop->date().month();
    m_pbremotestoptime.dwDay = ui.dateTimeEdit_1_stop->date().day();
    m_pbremotestoptime.dwHour = ui.dateTimeEdit_1_stop->time().hour();
    m_pbremotestoptime.dwMinute = ui.dateTimeEdit_1_stop->time().minute();
    m_pbremotestoptime.dwSecond = ui.dateTimeEdit_1_stop->time().second();

    bool bGetFile = true;
    int ret =0;
    int hFile =0;

    //call the hcnetsdk interface
    m_pbpDvrFile = (LPNET_DVR_FILECOND)malloc(sizeof( NET_DVR_FILECOND));
    if (m_pbpDvrFile == NULL)
    {
        QMessageBox::information(this,tr("PlayBack SearchFile"),
            tr("Malloc Error in PlayBack!"));
        return;
    }
    memset(m_pbpDvrFile,0x0,sizeof(NET_DVR_FILECOND));

    //give the variant prepare value
    memcpy(&m_pbpDvrFile->struStartTime, &m_pbremotestarttime, sizeof(NET_DVR_TIME));
    memcpy(&m_pbpDvrFile->struStopTime, &m_pbremotestoptime, sizeof(NET_DVR_TIME));
    m_pbpDvrFile->lChannel = (*m_pbCurrDevInfo).byStartChan + (*m_pbCurrChannel) -1;
	if (*m_pbCurrChannel == 0)
	{
		m_pbpDvrFile->lChannel = (*m_pbCurrDevInfo).byStartChan;
	}
    m_pbpDvrFile->dwIsLocked = m_pbfileattr;
    m_pbpDvrFile->dwFileType = m_pbfiletype;

    //clear the table items of file list
    for (int i=0; i<ui.tableWidget_1_filelist->rowCount(); i++)
    {
        for (int j=0;j<ui.tableWidget_1_filelist->columnCount();j++ )
        {
            delete ui.tableWidget_1_filelist->takeItem(i, j);
        }
        ui.tableWidget_1_filelist->removeRow(i);
    }

    ui.tableWidget_1_filelist->setRowCount(0);
    hFile = NET_DVR_FindFile_V30(*m_pbUserID, m_pbpDvrFile);
    if (hFile < 0)
    {
        QMessageBox::information(this,tr(" NET_DVR_FindFile_V30 "), 
        	tr("SDK_LASTERROR=%1").arg(NET_DVR_GetLastError()));
        

		return;
	}
	else
	{

		while (bGetFile)
	    {
	        memset(&m_pbFileData, 0, sizeof(NET_DVR_FINDDATA_V30));
	        ret = NET_DVR_FindNextFile_V30(hFile, &m_pbFileData);
	        switch (ret)
	        {
	            case NET_DVR_FILE_SUCCESS:
	                insertrow(m_pbFileData);
	                break;

	            case NET_DVR_FILE_NOFIND:
	                QMessageBox::information(this,tr(" NET_DVR_FindFile_V30"), 
	        			tr("No file find!"));
	                NET_DVR_FindClose_V30(hFile);
	                bGetFile = FALSE;
	                break;

	            case NET_DVR_ISFINDING:
					QMessageBox::information(this,tr(" NET_DVR_FindFile_V30"), 
	        			tr("Is finding now,wait a moment!"));
#if (defined(_WIN32))
#elif defined(__linux__)
	                usleep(200*1000);
#endif
	                break;

	            case NET_DVR_NOMOREFILE:
					QMessageBox::information(this,tr(" NET_DVR_FindFile_V30"), 
	        			tr("No more file find!"));
	                NET_DVR_FindClose_V30(hFile);
	                bGetFile = FALSE;
	                break;

	            case NET_DVR_FILE_EXCEPTION:
					QMessageBox::information(this,tr(" NET_DVR_FindFile_V30"), 
	        			tr("File Exception!"));                
					NET_DVR_FindClose_V30(hFile);
	                bGetFile = FALSE;
	                break;
					
	            default:
					QMessageBox::information(this,tr(" NET_DVR_FindFile_V30"), 
	        			tr("Error occured"));   
	                NET_DVR_FindClose_V30(hFile);
	                bGetFile = FALSE;
	                break;
	        }
	    }
		return ;
	}
}

/************************************************************************
 *	Function  		:	on_pushButton_1_download_clicked
 *  Description     :  	download remote save file
 *  Input           :  	
 *						none
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::on_pushButton_1_download_clicked()
{
	if (m_pbdownloading == 0)
	{
		if (0 == strcmp(ui.lineEdit_1_downfilename->text().toLatin1().data(),""))
	    {
	        QMessageBox::information(this,tr("GetFileByName Finished"), 
				tr("Please Select A File To Download"));
	        return;
	    }
	    QString filename(ui.lineEdit_1_downfilename->text());
	    //filepathname the file path and name for downloading
	    QString filepathname(DOWNLOAD_FILE_PATH);
	    QString currdir("./");
	    QDir directory(currdir);
	    directory.mkpath(filepathname);

		QList<DeviceData>::iterator it;
		QList<ChannelData>::iterator it_channel;
		for ( it = (*m_qlistdevicetree).begin(); it != (*m_qlistdevicetree).end(); ++it)
		{
		    if ((*it).getUsrID() == *m_pbUserID)
		    {
		        break;
		    }
		}
	    filepathname.append((*it).getIP());
		filepathname.append("_");

		QString channelnumtmp;
		channelnumtmp = QString::number(*m_pbCurrChannel);
		filepathname.append(channelnumtmp);
	    filepathname.append("_D");
	    filepathname.append(m_pbdstarttime.toLatin1().data());
	    filepathname.append("_");
	    filepathname.append(m_pbdstoptime.toLatin1().data());
	    filepathname.append(".mp4");

	    //call the hcnetsdk interface
	    if ((m_pbdownloadhandle = NET_DVR_GetFileByName(*m_pbUserID, 
			filename.toLatin1().data(), 
	        filepathname.toLatin1().data())) < 0)
	    {
	        QMessageBox::information(this,tr("GetFileByName failed"), 
	       		tr("SDK_Last_Error =%1").arg(NET_DVR_GetLastError()));
	        return ;
	    }
	    NET_DVR_PlayBackControl(m_pbdownloadhandle, NET_DVR_PLAYSTART, 0, NULL);
		m_pbdownloading =1;
		ui.pushButton_1_download->setText(QApplication::translate("translateClass", 
        	"stop download", 
        	0, QApplication::UnicodeUTF8));
		//have not realise download process
		DWORD processs =0;
		while(1)
		{
			NET_DVR_PlayBackControl(m_pbdownloadhandle, NET_DVR_PLAYGETPOS, 0, &processs);
#if (defined(_WIN32))
#elif defined(__linux__)
			sleep(1);
#endif
			if(processs == 100)
			{
				QMessageBox::information(this,tr("GetFileByName succ"),tr("download finish"));
				m_pbdownloading =0;
				ui.pushButton_1_download->setText(QApplication::translate("translateClass",
        			"download", 0, QApplication::UnicodeUTF8));
			
				break;
			}
				
		}
	}
    else
    {
		if (!NET_DVR_StopGetFile(m_pbdownloadhandle))
		{
	        QMessageBox::information(this,tr("GetFileByName failed"), \
	            tr("SDK_Last_Error =%1").arg(NET_DVR_GetLastError()));
	        return ;
	    }
		m_pbdownloading =0;
		ui.pushButton_1_download->setText(QApplication::translate("translateClass",
        	"download", 0, QApplication::UnicodeUTF8));
	}
    return;
}

/************************************************************************
 *	Function  		:	on_pushButton_1_lock_clicked
 *  Description     :  	lock file
 *  Input           :  	
 *						none
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::on_pushButton_1_lock_clicked()
{
    QString filename(ui.lineEdit_1_downfilename->text());
	
    //call the hcnetsdk interface
    if (m_pblockflag == 0)
    {
        if (!NET_DVR_LockFileByName(*m_pbUserID, filename.toLatin1().data()))
        {
            QMessageBox::information(this,tr("NET_DVR_LockFileByName failed"), 
                tr("SDK_Last_Error =%1").arg(NET_DVR_GetLastError()));
            return ;
        }
        m_pblockflag =1;
        ui.pushButton_1_lock->setText( QApplication::translate("TranslateClass", 
            "unlock", 0, QApplication::UnicodeUTF8));
    }
    else
    {
        if (!NET_DVR_UnlockFileByName(*m_pbUserID, filename.toLatin1().data()))
        {
            QMessageBox::information(this,tr("NET_DVR_UnlockFileByName failed"), 
                tr("SDK_Last_Error =%1").arg(NET_DVR_GetLastError()));
            return ;
        }
        m_pblockflag =0;
        ui.pushButton_1_lock->setText( QApplication::translate("TranslateClass", 
            "lock", 0, QApplication::UnicodeUTF8));
    }
}

/************************************************************************
 *	Function  		:	on_pushButton_1_downsave_clicked
 *  Description     :  	save file to certain directory
 *  Input           :  	
 *						none
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::on_pushButton_1_downsave_clicked()
{
	//弹出对话框
	QString savefilepath("");
	QString directory = QFileDialog::getOpenFileName(this,
		tr("Save the playback file"), QDir::currentPath()) ;
	if (!directory.isEmpty()) 
	{
		savefilepath.append(directory);
	}
	QMessageBox::information(this,tr("directory"), \
		tr("directory =%1").arg(directory.toLatin1().data()));
	
	//savefilepath.append("SaveFile.mp4");
 	if (!NET_DVR_PlayBackSaveData(m_pbhandle,savefilepath.toLatin1().data()))
	{
		QMessageBox::information(this,tr("NET_DVR_PlayBackSaveData failed"), \
			tr("SDK_Last_Error =%1 m_pbhandle=%2").arg(NET_DVR_GetLastError()).arg(m_pbhandle));
        return ;
	}
	m_pbsaving = 1;
}

/************************************************************************
 *	Function  		:	on_pushButton_1_downstopsave_clicked
 *  Description     :  	stop save file
 *  Input           :  	
 *						none
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::on_pushButton_1_downstopsave_clicked()
{
	if (!NET_DVR_StopPlayBackSave(m_pbhandle))
	{
		QMessageBox::information(this,tr("NET_DVR_StopPlayBackSave failed"), \
			tr("SDK_Last_Error =%1").arg(NET_DVR_GetLastError()));
        return ;
	}
	QMessageBox::information(this,tr("NET_DVR_StopPlayBackSave succ"), \
			tr("NET_DVR_StopPlayBackSave succ"));
	m_pbsaving = 0;
	return;
}

/************************************************************************
 *	Function  		:	on_pushButton_playback_start_1_clicked
 *  Description     :  	start play
 *  Input           :  	
 *						none
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::on_pushButton_playback_start_1_clicked()
{
	if (m_pbhandle == -1)
    {   
        QString filename(ui.lineEdit_1_downfilename->text());
        m_pbhandle = NET_DVR_PlayBackByName(*m_pbUserID, filename.toLatin1().data(), (HWND)m_pbframe.winId());
        if (m_pbhandle < 0 ) 
        {
            printf( "PlayBackByName failed. error[%d]\n", NET_DVR_GetLastError());
            return;
        }
    }
    if (m_pbcontrollabel_1 == 0)
    {
        //尚未播放状态
        playbackcontrol(NET_DVR_PLAYSTART);
		
        ui.pushButton_playback_stop_1->setDisabled(0);
        ui.pushButton_playback_slow_1->setDisabled(0);
        ui.pushButton_playback_fast_1->setDisabled(0);
        ui.pushButton_playback_picture_1->setDisabled(0);
        ui.pushButton_playback_pos_1->setDisabled(0);
        ui.pushButton_playback_frame_1->setDisabled(0);
        ui.pushButton_playback_volume_1->setDisabled(0);
        ui.pushButton_playback_start_1->setText(QApplication::translate(
            "PlayBackClass", "pause", 0,
            QApplication::UnicodeUTF8));
		ui.horizontalSlider_1_playbackprocess->setRange(0,100);
		ui.horizontalSlider_playback_volume_1->setRange(0,10);
		m_pbqtimer= new QTimer;   
    	m_pbqtimer->start(1000);
    	connect (m_pbqtimer, SIGNAL(timeout()), this, SLOT(updateTime()));

		QString offset;
		offset = QString::number(0);
		ui.lineEdit_1_downoffset->setText(offset);
        m_pbcontrollabel_1 = 1;
		//NET_DVR_SetPlayDataCallBack(m_pbhandle, PlaybackCallBack, 9);
    }
    else if (m_pbcontrollabel_1 == 1)
    {
        //已播放阶段
        playbackcontrol(NET_DVR_PLAYPAUSE);
        //ui.pushButton_playback_stop_1->setDisabled(1);
        ui.pushButton_playback_slow_1->setDisabled(1);
        ui.pushButton_playback_fast_1->setDisabled(1);
        ui.pushButton_playback_picture_1->setDisabled(1);
        ui.pushButton_playback_pos_1->setDisabled(1);
        ui.pushButton_playback_frame_1->setDisabled(1);
        ui.pushButton_playback_volume_1->setDisabled(1);
        ui.pushButton_playback_start_1->setText(QApplication::translate(
            "PlayBackClass", "play", 0, 
            QApplication::UnicodeUTF8));
        m_pbcontrollabel_1 = 2;
        //NET_DVR_SetPlayDataCallBack(m_pbhandle, PlaybackCallBack, 9);
    }
    else if (m_pbcontrollabel_1 == 2)
    {   
        //暂停阶段
        if (!NET_DVR_PlayBackControl(m_pbhandle, NET_DVR_PLAYRESTART, 0, NULL))
        {
            QMessageBox::information(this,tr("NET_DVR_PlayBackControl failed"), 
                tr("SDK_Last_Error =%1").arg(NET_DVR_GetLastError()));
            return ;
        }
        ui.pushButton_playback_stop_1->setDisabled(0);
        ui.pushButton_playback_slow_1->setDisabled(0);
        ui.pushButton_playback_fast_1->setDisabled(0);
        ui.pushButton_playback_picture_1->setDisabled(0);
        ui.pushButton_playback_pos_1->setDisabled(0);
        ui.pushButton_playback_frame_1->setDisabled(0);
        ui.pushButton_playback_volume_1->setDisabled(0);
        ui.pushButton_playback_start_1->setText(QApplication::translate(\
            "PlayBackClass", "pause", 0, \
            QApplication::UnicodeUTF8));
        m_pbcontrollabel_1 = 1;
        //NET_DVR_SetPlayDataCallBack(m_pbhandle, PlaybackCallBack, 9);
    }
}

/************************************************************************
 *	Function  		:	on_pushButton_playback_stop_1_clicked
 *  Description     :  	stop play
 *  Input           :  	
 *						none
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::on_pushButton_playback_stop_1_clicked()
{
	ui.horizontalSlider_1_playbackprocess->setValue(0);
	NET_DVR_StopPlayBack(m_pbhandle);
    m_pbhandle = -1;
    m_pbcontrollabel_1 = 0;
    ui.pushButton_playback_stop_1->setDisabled(1);
    ui.pushButton_playback_slow_1->setDisabled(1);
    ui.pushButton_playback_fast_1->setDisabled(1);
    ui.pushButton_playback_picture_1->setDisabled(1);
    ui.pushButton_playback_pos_1->setDisabled(1);
    ui.pushButton_playback_frame_1->setDisabled(1);
    ui.pushButton_playback_volume_1->setDisabled(1);
    ui.pushButton_playback_start_1->setText(QApplication::translate(\
        "PlayBackClass", "pause", \
        0, QApplication::UnicodeUTF8));
	if (m_pbqtimer != NULL)
	{
		delete m_pbqtimer;
		m_pbqtimer = NULL ;
	}
	return;
}

/************************************************************************
 *	Function  		:	on_pushButton_playback_fast_1_clicked
 *  Description     :  	play fast
 *  Input           :  	
 *						none
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::on_pushButton_playback_fast_1_clicked()
{
	playbackcontrol(NET_DVR_PLAYFAST);
	return;
}

/************************************************************************
 *	Function  		:	on_pushButton_playback_slow_1_clicked
 *  Description     :  	play slow
 *  Input           :  	
 *						none
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::on_pushButton_playback_slow_1_clicked()
{
	playbackcontrol(NET_DVR_PLAYSLOW);
	return;
}

/************************************************************************
 *	Function  		:	on_pushButton_playback_volume_1_clicked
 *  Description     :  	volume switch
 *  Input           :  	
 *						none
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::on_pushButton_playback_volume_1_clicked()
{
	if (m_pbvolumeflag == 0)
	{
		playbackcontrol(NET_DVR_PLAYSTARTAUDIO);
		m_pbvolumeflag = 1;
		ui.pushButton_playback_volume_1->setText(\
			QApplication::translate("translateClass", 
			"open volume", 0, 
			QApplication::UnicodeUTF8));
	}
	else
	{
		playbackcontrol(NET_DVR_PLAYSTOPAUDIO);
		m_pbvolumeflag = 0;
		ui.pushButton_playback_volume_1->setText(\
			QApplication::translate("translateClass", 
			"close volume", 0, 
			QApplication::UnicodeUTF8));
	}
	return ;
}

/************************************************************************
 *	Function  		:	on_pushButton_playback_pos_1_clicked
 *  Description     :  	play one by one frame
 *  Input           :  	
 *						none
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::on_pushButton_playback_pos_1_clicked()
{
	playbackcontrol(NET_DVR_PLAYSETPOS);
	return;
}

/************************************************************************
 *	Function  		:	on_pushButton_playback_picture_1_clicked
 *  Description     :  	capture picture
 *  Input           :  	
 *						none
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::on_pushButton_playback_picture_1_clicked()
{
	QString picturepathname(CAPTURE_PICTURE_PATH);
	QDir directory("./");
	directory.mkpath(picturepathname);
	QString time=QDateTime::currentDateTime().toString("yyyy-MM-dd");
	picturepathname.append(time.toLatin1().data());
	picturepathname.append(".bmp");
	//CaptureFile
	if (!NET_DVR_PlayBackCaptureFile(m_pbhandle, picturepathname.toLatin1().data()))
	{
		QMessageBox::information(this,tr("NET_DVR_PlayBackCaptureFile failed"), \
                tr("SDK_Last_Error =%1").arg(NET_DVR_GetLastError()));
		return ;
	}
    else{
        QMessageBox::information(this,tr("NET_DVR_PlayBackCaptureFile succ"), \
                    tr("NET_DVR_PlayBackCaptureFile success"));
    }

    char* szBuf = new char[1920*1080*8];
    DWORD bRet = 0;
    if(NET_DVR_CapturePlaybackPictureBlock_New(m_pbhandle, szBuf, 1920*1080*8, &bRet))
    {
        QMessageBox::information(this,tr("NET_DVR_CapturePictureBlock_New"),
                tr("SDK_LASTERROR=%1").arg(NET_DVR_GetLastError()));
        QFile fPic("playback.bmp");

        if (!fPic.open(QIODevice::ReadWrite | QIODevice::Text))
        {
            QMessageBox::information(this,tr("file error"),\
                tr("open device tree file error"));
        }

        fPic.write(szBuf, bRet);
        fPic.close();

    }
    else
    {
        QMessageBox::information(this,tr("NET_DVR_CapturePictureBlock_New error"),
                tr("SDK_LASTERROR=%1").arg(NET_DVR_GetLastError()));
    }

    delete[] szBuf;

}

/************************************************************************
 *	Function  		:	on_pushButton_playback_frame_1_clicked
 *  Description     :  	play one by one frame
 *  Input           :  	
 *						none
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::on_pushButton_playback_frame_1_clicked()
{
	playbackcontrol(NET_DVR_PLAYFRAME);
	return;
}

/************************************************************************
 *	Function  		:	on_horizontalSlider_1_playbackprocess_valueChanged
 *  Description     :  	process value change 
 *  Input           :  	
 *						value //play pos
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::on_horizontalSlider_1_playbackprocess_sliderReleased()
{
	int value =ui.horizontalSlider_1_playbackprocess->value();
	if (m_pbhandle >= 0)
	{
		if (!NET_DVR_PlayBackControl(m_pbhandle, NET_DVR_PLAYSETPOS, value, NULL))
	    {
	        QMessageBox::information(this,tr("NET_DVR_PlayBackControl failed"), \
	                tr("SDK_Last_Error =%1").arg(NET_DVR_GetLastError()));
	    }
		QString offset;
		offset = QString::number(((ui.lineEdit_1_filetotalsize->text().toULongLong())*value)/100);
		ui.lineEdit_1_downoffset->setText(offset);
	}
	
	return;
}

/************************************************************************
 *	Function  		:	on_horizontalSlider_playback_volume_1_valueChanged
 *  Description     :  	set volume value 
 *  Input           :  	
 *						valu //volume valuee
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::on_horizontalSlider_playback_volume_1_sliderReleased()
{
	int value =ui.horizontalSlider_playback_volume_1->value();
	if ((m_pbhandle >= 0)&&(m_pbvolumeflag == 1))
	{
		if (!NET_DVR_PlayBackControl(m_pbhandle, NET_DVR_PLAYAUDIOVOLUME, value, NULL))
	    {
	        QMessageBox::information(this,tr("NET_DVR_PlayBackControl failed"), \
	        	tr("SDK_Last_Error =%1").arg(NET_DVR_GetLastError()));
	    }
	}
	return;
}

/***************************page 1*****************************************/

/************************************************************************
 *	Function  		:	localtablebeenclicked
 *  Description     :  	select a cell local page
 *  Input           :  	
 *						row //cell row
 *						column	//cell column
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::localtablebeenclicked(int row,int column)
{
	if (0 != column)
    {
        return;
    }
	m_pblocalfilepath = ui.tableWidget_2_localfilist->item(row,column)->text();
	return;
}

/************************************************************************
 *	Function  		:	updatelocalprocess
 *  Description     :  	update play process
 *  Input           :  	
 *						none
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::updatelocalprocess()
{
	if (m_pblocalportnum >= 0)
	{
		unsigned int totaltime;
		totaltime = PlayM4_GetPlayedTime(m_pblocalportnum);
		QString qtotaltime;
		QString timevalue;
		if (totaltime/3600 <= 9)
		{
			qtotaltime.append("0");
		}
		timevalue = QString::number(totaltime/3600);
		qtotaltime.append(timevalue);
		qtotaltime.append(":");
		if (((totaltime%3600)/60) <= 9)
		{
			qtotaltime.append("0");
		}
		timevalue = QString::number((totaltime%3600)/60);
		qtotaltime.append(timevalue);
		qtotaltime.append(":");
		if ((totaltime%60) <= 9)
		{
			qtotaltime.append("0");
		}
		timevalue = QString::number(totaltime%60);
		qtotaltime.append(timevalue);

		unsigned int timeback = totaltime;
		qtotaltime.append("/");
		totaltime =PlayM4_GetFileTime(m_pblocalportnum);
		if (totaltime/3600 <= 9)
		{
			qtotaltime.append("0");
		}
		timevalue = QString::number(totaltime/3600);
		qtotaltime.append(timevalue);
		qtotaltime.append(":");
		if (((totaltime%3600)/60) <= 9)
		{
			qtotaltime.append("0");
		}
		timevalue = QString::number((totaltime%3600)/60);
		qtotaltime.append(timevalue);
		qtotaltime.append(":");
		if ((totaltime%60) <= 9)
		{
			qtotaltime.append("0");
		}
		timevalue = QString::number(totaltime%60);
		qtotaltime.append(timevalue);
		qtotaltime.append("  ");
		qtotaltime.append(ui.lineEdit_1_downfilename->text());
		ui.lineEdit_2_pbprocess->setText(qtotaltime);

		unsigned int currentpos;
		float pos;
		pos = PlayM4_GetPlayPos(m_pblocalportnum);
		currentpos =(unsigned int)(pos*100);
		ui.horizontalSlider_playback_process_2->setValue(currentpos);
		

	}
    return;
}

/************************************************************************
 *	Function  		:	on_dateTimeEdit_2_start_dateTimeChanged
 *  Description     :  	change time info
 *  Input           :  	
 *						datetime //new date time value
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::on_dateTimeEdit_2_start_dateTimeChanged ( const QDateTime & datetime )
{
	m_pblocalstarttime = datetime.toString("yyyyMMddhhmmss");
    return;
}

/************************************************************************
 *	Function  		:	on_dateTimeEdit_2_stop_dateTimeChanged
 *  Description     :  	change time info
 *  Input           :  	
 *						datetime //new date time value
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::on_dateTimeEdit_2_stop_dateTimeChanged ( const QDateTime & datetime )
{
	m_pblocalstoptime = datetime.toString("yyyyMMddhhmmss");
    return;
}

/************************************************************************
 *	Function  		:	on_pushButton_2_search_clicked
 *  Description     :  	search file 
 *  Input           :  	
 *						none
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::on_pushButton_2_search_clicked()
{
	//clear the table items of file list
    for (int i=0; i<ui.tableWidget_2_localfilist->rowCount(); i++)
    {
        for (int j=0; j<ui.tableWidget_2_localfilist->columnCount(); j++ )
        {
            delete ui.tableWidget_2_localfilist->takeItem(i, j);
        }
        ui.tableWidget_2_localfilist->removeRow(i);
    }
	ui.tableWidget_2_localfilist->setRowCount(0);
	
	//找到指定的文件目录，搜索文件形成列表
	if ((m_pblocalalltype == 2)||(m_pblocaltype == 2))
	{
		QDir directory = QDir(DOWNLOAD_FILE_PATH).absolutePath();
		QStringList files;
		QString fileName;
     	if (fileName.isEmpty())
        {
        	fileName = "*";
     	}
     	files = directory.entryList(QStringList(fileName),
                   		QDir::Files | QDir::NoSymLinks);
		for (int i = 0; i < files.size(); ++i) 
		{
			QFile file(directory.filePath(files[i]));
			//取IP地址和通道信息进行比较
			QList<DeviceData>::iterator it;
			for ( it = (*m_qlistdevicetree).begin(); it != (*m_qlistdevicetree).end(); ++it)
			{
			    if ((*it).getUsrID() == *m_pbUserID)
			    {
			        break;
			    }
			}
			QString fileip;
			fileip.append((*it).getIP());
			if ((file.fileName().contains(fileip))&&(file.fileName().contains("_D")))
			{
				//文件自身携带的开始时间
				QString starttime;
				QString download("D");
				int i = file.fileName().lastIndexOf(download);
				starttime = file.fileName().mid(i+1,14);
				qint64 starttimenode = starttime.toLongLong();
				//文件自身携带的结束时间
				QString stoptime;
				stoptime = file.fileName().mid(i+16,14);
				qint64 stoptimenode = stoptime.toLongLong();
				
				//用户指定的时间段
				qint64 start = m_pblocalstarttime.toLongLong();
				qint64 stop = m_pblocalstoptime.toLongLong();
				
				//时间值在范围之内
				if ((start<=starttimenode)&&(stop>=stoptimenode))
				{
					//显示到table区域  文件名
					QString filepath;
					filepath =file.fileName();
					QTableWidgetItem *FilePathItem = new QTableWidgetItem(filepath);
	    			FilePathItem->setFlags(Qt::ItemIsEnabled);
					//文件大小
					QString filesize;
					QFileInfo  filecurr(file.fileName());
					filesize = QString::number(filecurr.size());
					QTableWidgetItem *FileSizeItem = new QTableWidgetItem(filesize);
	    			FileSizeItem->setFlags(Qt::ItemIsEnabled);
					//属性
					QString fileattr("Download");
					QTableWidgetItem *FileAttrItem = new QTableWidgetItem(fileattr);
	    			FileAttrItem->setFlags(Qt::ItemIsEnabled);

					//开始时间
					QTableWidgetItem *FileStartTimeItem = new QTableWidgetItem(starttime);
	    			FileStartTimeItem->setFlags(Qt::ItemIsEnabled);

					//结束时间
					QTableWidgetItem *FileStopTimeItem = new QTableWidgetItem(stoptime);
	    			FileStopTimeItem->setFlags(Qt::ItemIsEnabled);
		
					int row = ui.tableWidget_2_localfilist->rowCount();
	    			ui.tableWidget_2_localfilist->insertRow(row);
	   				ui.tableWidget_2_localfilist->setItem(row, 0, FilePathItem);
					ui.tableWidget_2_localfilist->setItem(row, 1, FileSizeItem);
					ui.tableWidget_2_localfilist->setItem(row, 2, FileAttrItem);
					ui.tableWidget_2_localfilist->setItem(row, 3, FileStartTimeItem);
					ui.tableWidget_2_localfilist->setItem(row, 4, FileStopTimeItem);
				}
			}
		}
		QMessageBox::information(this,tr("Search File SUCC"), 
      		tr("finish"));
	}
	else
	{
		QMessageBox::information(this,tr("Search File Failed"), 
      		tr("no matching file"));
	}
	return;
}

/************************************************************************
 *	Function  		:	on_pushButton_2_del_clicked
 *  Description     :  	delete selected file 
 *  Input           :  	
 *						none
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::on_pushButton_2_del_clicked()
{
	char systeminfo[200] = {0};
	sprintf(systeminfo,"rm -f %s",m_pblocalfilepath.toLatin1().data());
	system(systeminfo);
	QMessageBox::information(this,tr("delete file succ"), 
		tr("delete file succ"));
	return;
}

/************************************************************************
 *	Function  		:	on_pushButton_2_delall_clicked
 *  Description     :  	delete all files
 *  Input           :  	
 *						none
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::on_pushButton_2_delall_clicked()
{
	//system("rm -f *.mp4");
	QMessageBox::information(this,tr("delete all files succ"),
		tr("delete all files succ"));
	return;
}

/************************************************************************
 *	Function  		:	on_comboBox_2_filetype_currentIndexChanged
 *  Description     :  	change file type
 *  Input           :  	
 *						index //0 normal 1 alarm 2 download
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::on_comboBox_2_filetype_currentIndexChanged (int index)
{
	m_pblocaltype = index;
}

/************************************************************************
 *	Function  		:	on_checkBox_2_all_stateChanged
 *  Description     :  	all file type
 *  Input           :  	
 *						none
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::on_checkBox_2_all_stateChanged(int state)
{
 	// 0 no selected 2 selected
	m_pblocalalltype = state;
}

/************************************************************************
 *	Function  		:	on_pushButton_playback_start_2_clicked
 *  Description     :  	start play
 *  Input           :  	
 *						none
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::on_pushButton_playback_start_2_clicked()
{
    BOOL bSuccess;
    bSuccess = PlayM4_GetPort((LONG *)&m_pblocalportnum);

    qDebug("pyd---File name:%s", m_pblocalfilepath.toLatin1().data());
    bSuccess = PlayM4_OpenFile(m_pblocalportnum, m_pblocalfilepath.toLatin1().data());

    bSuccess = PlayM4_Play(m_pblocalportnum, (HWND)(m_pbframe.winId()));

    ui.pushButton_playback_stop_2->setDisabled(0);
    ui.pushButton_playback_slow_2->setDisabled(0);
    ui.pushButton_playback_fast_2->setDisabled(0);
    ui.pushButton_playback_picture_2->setDisabled(0);
    ui.pushButton_playback_pos_2->setDisabled(0);
    ui.pushButton_playback_frame_2->setDisabled(0);
    ui.pushButton_playback_volume_2->setDisabled(0);

	ui.horizontalSlider_playback_process_2->setRange(0,100);
	ui.horizontalSlider_playback_volume_2->setRange(0,10);
	m_pbqtimer= new QTimer;   
	m_pbqtimer->start( 1000 );
	connect(m_pbqtimer, SIGNAL(timeout()), this, SLOT(updatelocalprocess()));
	return;	
}

/************************************************************************
 *	Function  		:	on_pushButton_playback_stop_2_clicked
 *  Description     :  	stop play
 *  Input           :  	
 *						none
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::on_pushButton_playback_stop_2_clicked()
{
	PlayM4_Stop(m_pblocalportnum);
	PlayM4_CloseFile(m_pblocalportnum);
	ui.horizontalSlider_playback_process_2->setValue(0);
	ui.pushButton_playback_stop_2->setDisabled(1);
    ui.pushButton_playback_slow_2->setDisabled(1);
    ui.pushButton_playback_fast_2->setDisabled(1);
    ui.pushButton_playback_picture_2->setDisabled(1);
    ui.pushButton_playback_pos_2->setDisabled(1);
    ui.pushButton_playback_frame_2->setDisabled(1);
    ui.pushButton_playback_volume_2->setDisabled(1);

	if (m_pbqtimer != NULL)
	{
		delete m_pbqtimer;
		m_pbqtimer = NULL ;
	}
	return;
}

/************************************************************************
 *	Function  		:	on_pushButton_playback_fast_2_clicked
 *  Description     :  	play fast
 *  Input           :  	
 *						none
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::on_pushButton_playback_fast_2_clicked()
{
	PlayM4_Fast(m_pblocalportnum);
	return;
}

/************************************************************************
 *	Function  		:	on_pushButton_playback_slow_2_clicked
 *  Description     :  	slow play
 *  Input           :  	
 *						none
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::on_pushButton_playback_slow_2_clicked()
{
	PlayM4_Slow(m_pblocalportnum);
	return;
}

/************************************************************************
 *	Function  		:	on_pushButton_playback_volume_2_clicked
 *  Description     :  	volume switch
 *  Input           :  	
 *						none
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::on_pushButton_playback_volume_2_clicked()
{
	if (m_pbvolumeflag == 0)
	{
		PlayM4_PlaySound(m_pblocalportnum);
		m_pbvolumeflag = 1;
		ui.pushButton_playback_volume_2->setText(\
			QApplication::translate("translateClass", 
			"close volume", 0, 
			QApplication::UnicodeUTF8));
	}
	else
	{
		PlayM4_StopSound();
		m_pbvolumeflag = 0;
		ui.pushButton_playback_volume_2->setText(\
			QApplication::translate("translateClass", 
			"open volume", 0, 
			QApplication::UnicodeUTF8));
	}
	return ;

}

/************************************************************************
 *	Function  		:	on_pushButton_playback_pos_2_clicked
 *  Description     :  	reset play to beginning
 *  Input           :  	
 *						none
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::on_pushButton_playback_pos_2_clicked()
{
	PlayM4_SetPlayPos(m_pblocalportnum, 0);

}

/************************************************************************
 *	Function  		:	PlayM4DisplayCallBack
 *  Description     :  	capture picture
 *  Input           :  	
 *						int nPort, 
						char *pBuf, 
						long size,
						long width, 
						long height, 
						long stamp, 
						long type,
						long reserved
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/	
void __stdcall PlayM4DisplayCallBack(long nPort, char *pBuf, long size, long width, long height,
                                     long stamp, long type, long reserved)
{
	QString picturepathname(CAPTURE_PICTURE_PATH);
	QDir directory("./");
	directory.mkpath(picturepathname);
	QString time=QDateTime::currentDateTime().toString("yyyy-MM-dd__hh_mm_ss");
	picturepathname.append(time.toLatin1().data());
	picturepathname.append(".bmp");	
	//JPEG ERROR NEED FOLLOW!!!!
	PlayM4_ConvertToBmpFile(pBuf, size, width, height, type, picturepathname.toLatin1().data());
	//PlayM4_ConvertToJpegFile(pBuf, size, width, height, type, picturepathname.toLatin1().data());
}


/************************************************************************
 *	Function  		:	on_pushButton_playback_picture_2_clicked
 *  Description     :  	capture picture
 *  Input           :  	
 *						none
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::on_pushButton_playback_picture_2_clicked()
{
#if (defined(_WIN32))
    PlayM4_SetDisplayCallBack(m_pblocalportnum, (void (__stdcall *)(long,char *,long,long,long,long,long,long))PlayM4DisplayCallBack);
#elif defined(__linux__)
    PlayM4_SetDisplayCallBack(m_pblocalportnum, (void (__stdcall *)(int,char *,int,int,int,int,int,int))PlayM4DisplayCallBack);
#endif

    QMessageBox::information(this,tr("Capture Picture succ"), tr("Capture Picture succ"));
}

/************************************************************************
 *	Function  		:	on_pushButton_playback_frame_2_clicked
 *  Description     :  	play one by one frame
 *  Input           :  	
 *						none
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::on_pushButton_playback_frame_2_clicked()
{
	PlayM4_OneByOne(m_pblocalportnum);
	return;
}

/************************************************************************
 *	Function  		:	on_horizontalSlider_playback_process_2_valueChanged
 *  Description     :  	value //pos
 *  Input           :  	
 *						none
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::on_horizontalSlider_playback_process_2_sliderReleased()
{
	int value =ui.horizontalSlider_playback_process_2->value();
	PlayM4_SetPlayPos(m_pblocalportnum, ((float)value)*0.01);
	return;
}

/************************************************************************
 *	Function  		:	on_horizontalSlider_playback_volume_2_valueChanged
 *  Description     :  	set volume value
 *  Input           :  	
 *						value //volume value
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::on_horizontalSlider_playback_volume_2_sliderReleased()
{
	int value =ui.horizontalSlider_playback_volume_2->value();
	if (m_pbvolumeflag == 1)
	{
		PlayM4_SetVolume(m_pblocalportnum, value);
	}	
	return;
}

/************page 2*************/

/************************************************************************
 *	Function  		:	on_pushButton_3_download_clicked
 *  Description     :  	download by time 
 *  Input           :  	
 *						none
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::on_pushButton_3_download_clicked()
{
	if (m_pbdownloading == 0)
	{
	    QString filename("");
		//filepathname the file path and name for downloading
	    QString filepathname(DOWNLOAD_FILE_PATH);
	    QString currdir("./");
	    QDir directory(currdir);
	    directory.mkpath(filepathname);

		QList<DeviceData>::iterator it;
		QList<ChannelData>::iterator it_channel;
		for ( it = (*m_qlistdevicetree).begin(); it != (*m_qlistdevicetree).end(); ++it)
		{
		    if ((*it).getUsrID() == *m_pbUserID)
		    {
		        break;
		    }
		}
	    filepathname.append((*it).getIP());
		filepathname.append("_");

		QString channelnumtmp;
		channelnumtmp = QString::number(*m_pbCurrChannel);
		filepathname.append(channelnumtmp);
	    filepathname.append("_D");

		
		QString month;
		month = QString::number(m_pbplaystarttime.dwMonth);
		if (m_pbplaystarttime.dwMonth <= 9)
		{
			month.prepend("0");
		}
		QString day;
		day = QString::number(m_pbplaystarttime.dwDay);
		if (m_pbplaystarttime.dwDay <= 9)
		{
			day.prepend("0");
		}
		QString hour;
		hour = QString::number(m_pbplaystarttime.dwHour);
		if (m_pbplaystarttime.dwHour <= 9)
		{
			hour.prepend("0");
		}
		QString minute;
		minute = QString::number(m_pbplaystarttime.dwMinute);
		if (m_pbplaystarttime.dwMinute <= 9)
		{
			minute.prepend("0");
		}
		QString sec;
		sec = QString::number(m_pbplaystarttime.dwSecond);
		if (m_pbplaystarttime.dwSecond <= 9)
		{
			sec.prepend("0");
		}
		m_pbdstarttime = QString::number(m_pbplaystarttime.dwYear);
		m_pbdstarttime.append(month);
		m_pbdstarttime.append(day);
		m_pbdstarttime.append(hour);
		m_pbdstarttime.append(minute);
		m_pbdstarttime.append(sec);
		
		month = QString::number(m_pbplaystoptime.dwMonth);
		if (m_pbplaystoptime.dwMonth <= 9)
		{
			month.prepend("0");
		}
		day = QString::number(m_pbplaystoptime.dwDay);
		if (m_pbplaystoptime.dwDay <=9)
		{
			day.prepend("0");
		}
		hour = QString::number(m_pbplaystoptime.dwHour);
		if (m_pbplaystoptime.dwHour <=9)
		{
			hour.prepend("0");
		}
		minute = QString::number(m_pbplaystoptime.dwMinute);
		if (m_pbplaystoptime.dwMinute <=9)
		{
			minute.prepend("0");
		}
		sec = QString::number(m_pbplaystoptime.dwSecond);
		if (m_pbplaystoptime.dwSecond <=9)
		{
			sec.prepend("0");
		}
		m_pbdstoptime = QString::number(m_pbplaystoptime.dwYear);
		m_pbdstoptime.append(month);
		m_pbdstoptime.append(day);
		m_pbdstoptime.append(hour);
		m_pbdstoptime.append(minute);
		m_pbdstoptime.append(sec);
		
	    filepathname.append(m_pbdstarttime.toLatin1().data());
	    filepathname.append("_");
	    filepathname.append(m_pbdstoptime.toLatin1().data());
		filepathname.append("Time");
	    filepathname.append(".mp4");


		m_pbdownloadhandle = NET_DVR_GetFileByTime(*m_pbUserID,*m_pbCurrChannel, 
				&m_pbplaystarttime, &m_pbplaystoptime,filepathname.toLatin1().data());
		if (!NET_DVR_PlayBackControl(m_pbdownloadhandle, NET_DVR_PLAYSTART, 0, NULL))
	    {
	        QMessageBox::information(this,tr("NET_DVR_PlayBackControl failed"), 
	                tr("SDK_Last_Error =%1").arg(NET_DVR_GetLastError()));
	        return ;
	    }
		if (-1 == m_pbdownloadhandle)
		{
			QMessageBox::information(this,tr("NET_DVR_GetFileByTime failed"), 
				tr("SDK_Last_Error =%1").arg(NET_DVR_GetLastError()));
	        return ;
		}
		m_pbdownloading =1;
		ui.pushButton_3_download->setText(QApplication::translate("translateClass",
        	"\345\201\234\346\255\242\344\270\213\350\275\275", 
        	0, QApplication::UnicodeUTF8));
		//have not realize the downloading processing slider
	}
	else
	{
		if (!NET_DVR_StopGetFile(m_pbdownloadhandle))
		{
			QMessageBox::information(this,tr("NET_DVR_StopGetFile failed"), 
				tr("SDK_Last_Error =%1").arg(NET_DVR_GetLastError()));
	        return ;
		}
		m_pbdownloading =0;
		ui.pushButton_3_download->setText(QApplication::translate("translateClass",
        	"\344\270\213\350\275\275", 0, QApplication::UnicodeUTF8));

	}
	return;

}

/************************************************************************
 *	Function  		:	on_pushButton_3_save_clicked
 *  Description     :  	save file by time
 *  Input           :  	
 *						none
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::on_pushButton_3_save_clicked()
{
	if (m_pbsaving == 0)
	{
		//弹出对话框
		QString savefilepath("");
		QString directory = QFileDialog::getOpenFileName(this,
			tr("Save the playback file"), QDir::currentPath()) ;
		if (!directory.isEmpty()) 
		{
			savefilepath.append(directory);
		}
		QMessageBox::information(this,tr("directory"), \
				tr("directory =%1").arg(directory.toLatin1().data()));
		//savefilepath.append("SaveFileByTime.mp4");
		 
	 	if (!NET_DVR_PlayBackSaveData(m_pbhandle,savefilepath.toLatin1().data()))
		{
			QMessageBox::information(this,tr("NET_DVR_PlayBackSaveData failed"), 
				tr("SDK_Last_Error =%1 m_pbhandle=%2").arg(NET_DVR_GetLastError()
				).arg(m_pbhandle));
	        return ;
		}
		m_pbsaving = 1;
		ui.pushButton_3_save->setText(QApplication::translate("translateClass", 
			"\345\201\234\346\255\242\344\277\235\345\255\230", 0, QApplication::UnicodeUTF8));
	}
	else
	{
		if (!NET_DVR_StopPlayBackSave(m_pbhandle))
		{
			QMessageBox::information(this,tr("NET_DVR_StopPlayBackSave failed"), \
				tr("SDK_Last_Error =%1").arg(NET_DVR_GetLastError()));
	        return ;
		}
		QMessageBox::information(this,tr("NET_DVR_StopPlayBackSave succ"), \
				tr("NET_DVR_StopPlayBackSave succ"));
		m_pbsaving = 0;
		ui.pushButton_3_save->setText(QApplication::translate("translateClass", 
				"\344\277\235\345\255\230", 0, QApplication::UnicodeUTF8));
	}
	return;
}

/************************************************************************
 *	Function  		:	on_dateTimeEdit_3_start_dateTimeChanged
 *  Description     :  	change time info
 *  Input           :  	
 *						datetime //new date time value
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::on_dateTimeEdit_3_start_dateTimeChanged ( const QDateTime & datetime )
{
    m_pbplaystarttime.dwYear = datetime.date().year();
    m_pbplaystarttime.dwMonth = datetime.date().month();
    m_pbplaystarttime.dwDay = datetime.date().day();
    m_pbplaystarttime.dwHour = datetime.time().hour();
    m_pbplaystarttime.dwMinute = datetime.time().minute();
    m_pbplaystarttime.dwSecond = datetime.time().second();
	return;
}

/************************************************************************
 *	Function  		:	on_dateTimeEdit_3_stop_dateTimeChanged
 *  Description     :  	change time info
 *  Input           :  	
 *						datetime //new date time value
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::on_dateTimeEdit_3_stop_dateTimeChanged ( const QDateTime & datetime )
{	
    m_pbplaystoptime.dwYear = datetime.date().year();
    m_pbplaystoptime.dwMonth = datetime.date().month();
    m_pbplaystoptime.dwDay = datetime.date().day();
    m_pbplaystoptime.dwHour = datetime.time().hour();
    m_pbplaystoptime.dwMinute = datetime.time().minute();
    m_pbplaystoptime.dwSecond = datetime.time().second();
    return;
}

/************************************************************************
 *	Function  		:	on_pushButton_playback_start_3_clicked
 *  Description     :  	start play
 *  Input           :  	
 *						none
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::on_pushButton_playback_start_3_clicked()
{
    HWND playWnd = (HWND)(m_pbframe.winId());

    if (m_pbcontrollabel_3 == 0)
    {
        //尚未播放状态
    	m_pbplaystarttime.dwYear = ui.dateTimeEdit_3_start->date().year();
        m_pbplaystarttime.dwMonth = ui.dateTimeEdit_3_start->date().month();
        m_pbplaystarttime.dwDay = ui.dateTimeEdit_3_start->date().day();
        m_pbplaystarttime.dwHour = ui.dateTimeEdit_3_start->time().hour();
        m_pbplaystarttime.dwMinute = ui.dateTimeEdit_3_start->time().minute();
        m_pbplaystarttime.dwSecond = ui.dateTimeEdit_3_start->time().second();
        m_pbplaystoptime.dwYear = ui.dateTimeEdit_3_stop->date().year();
        m_pbplaystoptime.dwMonth = ui.dateTimeEdit_3_stop->date().month();
        m_pbplaystoptime.dwDay = ui.dateTimeEdit_3_stop->date().day();
        m_pbplaystoptime.dwHour = ui.dateTimeEdit_3_stop->time().hour();
        m_pbplaystoptime.dwMinute = ui.dateTimeEdit_3_stop->time().minute();
        m_pbplaystoptime.dwSecond = ui.dateTimeEdit_3_stop->time().second();
        if (*m_pbCurrChannel ==0)
        {
                *m_pbCurrChannel = 1;
        }

#if (defined(_WIN32)) || (defined(__APPLE__))
        m_pbhandle = NET_DVR_PlayBackByTime(*m_pbUserID,*m_pbCurrChannel, &m_pbplaystarttime, &m_pbplaystoptime, playWnd);
#elif defined(__linux__)
        m_pbhandle = NET_DVR_PlayBackByTime(*m_pbUserID,*m_pbCurrChannel, &m_pbplaystarttime, &m_pbplaystoptime, playWnd);
#endif

        if (m_pbhandle<0)
        {
            QMessageBox::information(this,tr("NET_DVR_PlayBackByTime failed"),
                tr("SDK_Last_Error =%1").arg(NET_DVR_GetLastError()));
                    return;
        }
        playbackcontrol(NET_DVR_PLAYSTART);
        m_pbcontrollabel_3 = 1;
		  
        ui.pushButton_playback_stop_3->setDisabled(0);
        ui.pushButton_playback_slow_3->setDisabled(0);
        ui.pushButton_playback_fast_3->setDisabled(0);
        ui.pushButton_playback_picture_3->setDisabled(0);
        ui.pushButton_playback_pos_3->setDisabled(0);
        ui.pushButton_playback_frame_3->setDisabled(0);
        ui.pushButton_playback_volume_3->setDisabled(0);
        
        ui.pushButton_playback_start_3->setText(QApplication::translate(
            "PlayBackClass", "pause", 0, 
            QApplication::UnicodeUTF8));
        m_pbcontrollabel_3 = 1;
        //NET_DVR_SetPlayDataCallBack(m_pbhandle, PlaybackCallBack, 9);
    }
    else if (m_pbcontrollabel_3==1)
    {
    	DWORD currentpos;
		if (!NET_DVR_PlayBackControl(m_pbhandle, NET_DVR_PLAYGETPOS , 0, &currentpos))
	    {
	    	return;
		}
		if (currentpos ==100)
		{
			on_pushButton_playback_stop_3_clicked();
			return;
		}
        //已播放阶段
        playbackcontrol(NET_DVR_PLAYPAUSE);
        
        ui.pushButton_playback_stop_3->setDisabled(1);
        ui.pushButton_playback_slow_3->setDisabled(1);
        ui.pushButton_playback_fast_3->setDisabled(1);
        ui.pushButton_playback_picture_3->setDisabled(1);
        ui.pushButton_playback_pos_3->setDisabled(1);
        ui.pushButton_playback_frame_3->setDisabled(1);
        ui.pushButton_playback_volume_3->setDisabled(1);
        ui.pushButton_playback_start_3->setText(QApplication::translate(
            "PlayBackClass", "play", 0, 
            QApplication::UnicodeUTF8));
        m_pbcontrollabel_3 = 2;
        //NET_DVR_SetPlayDataCallBack(m_pbhandle, PlaybackCallBack, 9);
    }
    else if (m_pbcontrollabel_3==2)
    {   
        //暂停阶段
        if (!NET_DVR_PlayBackControl(m_pbhandle, NET_DVR_PLAYRESTART, 0, NULL))
        {
            QMessageBox::information(this,tr("NET_DVR_PlayBackControl failed"), 
                tr("SDK_Last_Error =%1").arg(NET_DVR_GetLastError()));
            return ;
        }
        ui.pushButton_playback_stop_3->setDisabled(0);
        ui.pushButton_playback_slow_3->setDisabled(0);
        ui.pushButton_playback_fast_3->setDisabled(0);
        ui.pushButton_playback_picture_3->setDisabled(0);
        ui.pushButton_playback_pos_3->setDisabled(0);
        ui.pushButton_playback_frame_3->setDisabled(0);
        ui.pushButton_playback_volume_3->setDisabled(0);
        ui.pushButton_playback_start_3->setText(QApplication::translate(
            "PlayBackClass", "pause", 0, 
            QApplication::UnicodeUTF8));
        m_pbcontrollabel_3 = 1;
        //NET_DVR_SetPlayDataCallBack(m_pbhandle, PlaybackCallBack, 9);
    }
}

/************************************************************************
 *	Function  		:	on_pushButton_playback_stop_3_clicked
 *  Description     :  	stop play
 *  Input           :  	
 *						none
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::on_pushButton_playback_stop_3_clicked()
{
	NET_DVR_StopPlayBack(m_pbhandle);
    m_pbhandle=-1;
   
    ui.pushButton_playback_stop_3->setDisabled(1);
    ui.pushButton_playback_slow_3->setDisabled(1);
    ui.pushButton_playback_fast_3->setDisabled(1);
    ui.pushButton_playback_picture_3->setDisabled(1);
    ui.pushButton_playback_pos_3->setDisabled(1);
    ui.pushButton_playback_frame_3->setDisabled(1);
    ui.pushButton_playback_volume_3->setDisabled(1);
    ui.pushButton_playback_start_3->setText(QApplication::translate(
        "PlayBackClass", "play", 
        0, QApplication::UnicodeUTF8)); 
	m_pbcontrollabel_3=0;
    return;

}

/************************************************************************
 *	Function  		:	on_pushButton_playback_fast_3_clicked
 *  Description     :  	play fast
 *  Input           :  	
 *						none
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::on_pushButton_playback_fast_3_clicked()
{
	playbackcontrol(NET_DVR_PLAYFAST);
	return;
}

/************************************************************************
 *	Function  		:	on_pushButton_playback_slow_3_clicked
 *  Description     :  	play slow
 *  Input           :  	
 *						none
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::on_pushButton_playback_slow_3_clicked()
{
	playbackcontrol(NET_DVR_PLAYSLOW);
	return;
}

/************************************************************************
 *	Function  		:	on_pushButton_playback_pos_3_clicked
 *  Description     :  	play control reset play pos to beginning
 *  Input           :  	
 *						set pos to beginning
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::on_pushButton_playback_pos_3_clicked()
{
	if (!NET_DVR_PlayBackControl(m_pbhandle, NET_DVR_PLAYSETPOS, 0, NULL))
    {
        QMessageBox::information(this,tr("NET_DVR_PlayBackControl failed"), 
                tr("SDK_Last_Error =%1").arg(NET_DVR_GetLastError()));
        return ;
    }

}

/************************************************************************
 *	Function  		:	on_pushButton_playback_picture_3_clicked
 *  Description     :  	Capture picture
 *  Input           :  	
 *						none
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::on_pushButton_playback_picture_3_clicked()
{
	QString picturepathname(CAPTURE_PICTURE_PATH);
	QDir directory("./");
	directory.mkpath(picturepathname);
	QString time=QDateTime::currentDateTime().toString("yyyy-MM-dd");
	picturepathname.append(time.toLatin1().data());
	picturepathname.append(".bmp");

	if (!NET_DVR_PlayBackCaptureFile(m_pbhandle, picturepathname.toLatin1().data()))
	{
		QMessageBox::information(this,tr("NET_DVR_PlayBackCaptureFile failed"), 
                tr("SDK_Last_Error =%1").arg(NET_DVR_GetLastError()));
		return ;
	}
	QMessageBox::information(this,tr("NET_DVR_PlayBackCaptureFile succ"), 
                tr("NET_DVR_PlayBackCaptureFile success"));
}

/************************************************************************
 *	Function  		:	on_pushButton_playback_frame_3_clicked
 *  Description     :  	play control one by one frame
 *  Input           :  	
 *						none
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::on_pushButton_playback_frame_3_clicked()
{
	playbackcontrol(NET_DVR_PLAYFRAME);
	return;
}

/************************************************************************
 *	Function  		:	on_pushButton_playback_volume_3_clicked
 *  Description     :  	volume switch
 *  Input           :  	
 *						none
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::on_pushButton_playback_volume_3_clicked()
{
	if (m_pbvolumeflag == 0)
	{
		playbackcontrol(NET_DVR_PLAYSTARTAUDIO);
		m_pbvolumeflag = 1;
		ui.pushButton_playback_volume_3->setText(\
			QApplication::translate("translateClass", 
			"close volume", 0, 
			QApplication::UnicodeUTF8));
	}
	else
	{
		playbackcontrol(NET_DVR_PLAYSTOPAUDIO);
		m_pbvolumeflag = 0;
		ui.pushButton_playback_volume_3->setText(\
			QApplication::translate("translateClass", 
			"open volume", 0, 
			QApplication::UnicodeUTF8));
	}
	return ;
}

/************************************************************************
 *	Function  		:	on_horizontalSlider_playback_volume_3_valueChanged
 *  Description     :  	set play volume 
 *  Input           :  	
 *						value //volume value
 *  Output          :  	none
 *  Return          :  	none
************************************************************************/
void PlayBack::on_horizontalSlider_playback_volume_3_sliderReleased()
{
	int value =ui.horizontalSlider_playback_volume_3->value();
	if ((m_pbhandle >= 0)&&(m_pbvolumeflag == 1))
	{
		if (!NET_DVR_PlayBackControl(m_pbhandle, NET_DVR_PLAYAUDIOVOLUME, value, NULL))
	    {
	        QMessageBox::information(this,tr("NET_DVR_PlayBackControl failed"), \
	        	tr("SDK_Last_Error =%1").arg(NET_DVR_GetLastError()));
	    }
	}
}

void PlayBack::OnSingleClickPlayWindow()
{
    QMessageBox::information(this,tr("NET_DVR_PlayBackControl failed"), \
        tr("SDK_Last_Error =%1").arg(NET_DVR_GetLastError()));

    QRect qRect = m_pbframe.geometry();



    qRect.setWidth(qRect.width()+100);
    qRect.setHeight(qRect.height()+100);

    m_pbframe.setGeometry(qRect);

    if (!NET_DVR_PlayBackControl(m_pbhandle, NET_DVR_CHANGEWNDRESOLUTION, 0, NULL))
    {
        QMessageBox::information(this,tr("NET_DVR_PlayBackControl failed"), \
            tr("SDK_Last_Error =%1").arg(NET_DVR_GetLastError()));
    }


}


void PlayBack::on_changeWndBtn_clicked()
{
    QRect qRect = m_pbframe.geometry();



    qRect.setWidth(qRect.width()+100);
    qRect.setHeight(qRect.height()+100);

    m_pbframe.setGeometry(qRect);

    if (!NET_DVR_PlayBackControl(m_pbhandle, NET_DVR_CHANGEWNDRESOLUTION, 0, NULL))
    {
        QMessageBox::information(this,tr("NET_DVR_PlayBackControl failed"), \
            tr("SDK_Last_Error =%1").arg(NET_DVR_GetLastError()));
    }
}
