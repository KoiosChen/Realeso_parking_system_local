/************************************************************************
 *        Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *        FileName                      :   timing.cpp
 *        Description                   :   timing a device
 *        Modification History          :   none
 *        Version                       :   V1.0
 *        Time                          :   2009-11,12
 *        Author                        :   wanggongpu
 *        Descrp                        :   
*************************************************************************/
#include "timing.h"
#include <QTreeWidgetItem>
#include <QDateTime>
#include <QMessageBox>

/************************************************************************
 *        Function            :  Timing
 *        Description         :  instructor
 *        Input               :  int channelnum, QList<DeviceData> * tree
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
Timing::Timing(int channelnum, QList<DeviceData> * tree, QDialog *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    m_qlistdevicetree =tree;
    m_timingchannelnum = channelnum;
    QList<QTreeWidgetItem *> items;
    QList<DeviceData>::iterator it;
    int i=0;
    for ( it = (*m_qlistdevicetree).begin(),i=0; i<256,\
		it != (*m_qlistdevicetree).end();++it,++i)
    {
        if ((*it).getUsrID()>=0)
        {
            m_timinguserid[i]=(*it).getUsrID();
            items.append(new QTreeWidgetItem((QTreeWidget*)0, \
                QStringList((*it).getDeviceName())));
        }
    }

    ui.treeWidget->insertTopLevelItems(0, items);
    connect(ui.treeWidget,SIGNAL(itemClicked(QTreeWidgetItem *, 
	int)),this,SLOT(timingDevice(QTreeWidgetItem *, int)));
    
    ui.dateTimeEdit->setDateTime(QDateTime::currentDateTime());

    
}

/************************************************************************
 *        Function            :  ~Timing
 *        Description         :  destructor
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
Timing::~Timing()
{
    ui.treeWidget->clear();
}

/************************************************************************
 *        Function            :  on_dateTimeEdit_dateTimeChanged
 *        Description         :  time
 *        Input               :  const QDateTime & datetime
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void Timing::on_dateTimeEdit_dateTimeChanged ( const QDateTime & datetime )
{
    m_timingtime.dwYear = datetime.date().year();
    m_timingtime.dwMonth = datetime.date().month();
    m_timingtime.dwDay = datetime.date().day();
    m_timingtime.dwHour = datetime.time().hour();
    m_timingtime.dwMinute = datetime.time().minute();
    m_timingtime.dwSecond = datetime.time().second();   
}

/************************************************************************
 *        Function            :  on_pushButton_timing_clicked
 *        Description         :  timing
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void Timing::on_pushButton_timing_clicked()
{
    if (!NET_DVR_SetDVRConfig(m_timinguserid[m_index],NET_DVR_SET_TIMECFG,\
		m_timingchannelnum,&m_timingtime,sizeof(NET_DVR_TIME)))
    {
        QMessageBox::information(this,tr("NET_DVR_SetDVRConfig failed"), \
                tr("SDK_Last_Error =%1").arg(NET_DVR_GetLastError()));
        return ;
    }
    QMessageBox::information(this,tr("NET_DVR_SET_TIMECFG SUCCESS"), \
                tr("SET_TIMECFG SUCCESS"));
}

/************************************************************************
 *        Function            :  on_pushButton_exit_clicked
 *        Description         :  exit
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void Timing::on_pushButton_exit_clicked()
{
    close();
}

/************************************************************************
 *        Function            :  timingDevice
 *        Description         :  timingDevice
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void Timing::timingDevice(QTreeWidgetItem *item, int column)
{
    QList<DeviceData>::iterator it;
    int i=0;
    for ( it = (*m_qlistdevicetree).begin(),i=0; i<50,\
		it != (*m_qlistdevicetree).end(); ++it,++i)
    {
        if ((*it).getDeviceName()==item->text(column))
        {
            m_index =i;
        }
    }
}

