/************************************************************************
 *        Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *        FileName                      :   deploy.cpp
 *        Description                   :   deploy 
 *        Modification History          :   none
 *        Version                       :   V1.0
 *        Time                          :   2009-11,12
 *        Author                        :   wanggongpu
 *        Descrp                        :   
*************************************************************************/
#include "deploy.h"
#include <QMessageBox>
#include <QString>

/************************************************************************
 *        Function            :  Deploy
 *        Description         :  instructor
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
Deploy::Deploy(QList<DeviceData> * tree, QDialog *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    m_qlistdevicetree =tree;

    
    QList<DeviceData>::iterator it;
    int i=0;
    for ( it = (*m_qlistdevicetree).begin(); it != (*m_qlistdevicetree).end(); ++it)
    {
        if ((*it).getUsrID()>=0)
        {
            items.append(new QTreeWidgetItem((QTreeWidget*)0, 
			QStringList((*it).getDeviceName())));
        }
    }
    ui.treeWidget->insertTopLevelItems(0, items);
    connect(ui.treeWidget,SIGNAL(itemClicked(QTreeWidgetItem *, 
	int)),this,SLOT(deployDevice(QTreeWidgetItem *, int)));

	m_dphandle =-1;
}

/************************************************************************
 *        Function            :  ~Deploy
 *        Description         :  none  
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
Deploy::~Deploy()
{
    items.clear();
    ui.treeWidget->clear();
}

/************************************************************************
 *        Function            :  deployDevice
 *        Description         :  deploy a device 
 *        Input               :  QTreeWidgetItem * item, int column
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void Deploy::deployDevice(QTreeWidgetItem * item, int column)
{
    QList<DeviceData>::iterator it;
    int i=0;
    for ( it = (*m_qlistdevicetree).begin(),i=0; i<256,it != (*m_qlistdevicetree).end(); 
	++it)
    {	
        if ((*it).getDeviceName()==item->text(column))
        {
       	 	//QMessageBox::information(this,tr("NET_DVR_SetupAlarmChan_V30 SUCCESS"), 
				//tr("(*it).getDeployState()=%1").arg((*it).getDeployState()));  
            
			m_useridtmp = (*it).getUsrID();
            break;
        }
		if ((*it).getUsrID()>=0)
		{
			
			i++;
		}
    }
}

/************************************************************************
 *        Function            :  on_pushButton_deployornot_clicked
 *        Description         :  deloy or not
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void Deploy::on_pushButton_deployornot_clicked()
{
	QList<DeviceData>::iterator it;
    int i=0;
    for ( it = (*m_qlistdevicetree).begin(),i=0; i<256,it != (*m_qlistdevicetree).end(); 
	++it)
    {	
		if ((*it).getUsrID()==m_useridtmp )
		{
			break;
		}
    }

    if ((*it).getDeployState()==-1)
    {	
    	m_dphandle= NET_DVR_SetupAlarmChan_V30(m_useridtmp);
        if (-1 == m_dphandle)
        {
            QMessageBox::information(this,tr("NET_DVR_SetupAlarmChan_V30 failed"), \
                    tr("SDK_Last_Error =%1 ").arg(NET_DVR_GetLastError()));
            return ;
        }
        QMessageBox::information(this,tr("NET_DVR_SetupAlarmChan_V30 SUCCESS"), 
		tr("NET_DVR_SetupAlarmChan_V30 SUCCESS"));  
		(*it).setDeployState(m_dphandle);
    }
    else
    {	
    	int i=(*it).getDeployState();
        if (!NET_DVR_CloseAlarmChan_V30(i))
        {
            QMessageBox::information(this,tr("NET_DVR_CloseAlarmChan_V30 failed"), \
                    tr("SDK_Last_Error =%1 ").arg(NET_DVR_GetLastError()));
			
			return ;
        }
        QMessageBox::information(this,tr("NET_DVR_CloseAlarmChan_V30 SUCCESS"), 
		tr("NET_DVR_CloseAlarmChan_V30 SUCCESS"));  
		(*it).setDeployState(-1);
		
        
    }
}

/************************************************************************
 *        Function            :  on_pushButton_exit_clicked
 *        Description         :  exit
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void Deploy::on_pushButton_exit_clicked()
{
    close();
}

