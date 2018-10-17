/************************************************************************
 *        Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *        FileName                      :   remoteupdate.cpp
 *        Description                   :   remote update 
 *        Modification History          :   none
 *        Version                       :   V1.0
 *        Time                          :   2009-11,12
 *        Author                        :   wanggongpu
 *        Descrp                        :   
*************************************************************************/
#include "remoteupdate.h"
#include <QString>
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QByteArray>

/************************************************************************
 *        Function            :  RemoteUpdate
 *        Description         :  instructor
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
RemoteUpdate::RemoteUpdate(int* userid,  QWidget *parent)
    : QDialog(parent)
	, m_timer (NULL)
	, m_hUpgrade(-1)
{
    ui.setupUi(this);
    m_updateuserid = userid;
	ui.horizontalScrollBar->setRange(0, 100);
	setUpdatesEnabled ( TRUE );
	update();
	repaint();
	m_updatenetworktype =0;
}

/************************************************************************
 *        Function            :  ~RemoteUpdate
 *        Description         :  destructor
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
RemoteUpdate::~RemoteUpdate()
{

}

/************************************************************************
 *        Function            :  on_comboBox_currentIndexChanged
 *        Description         :  set network environment
 *        Input               :  index 0 LAN 1 WAN
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void RemoteUpdate::on_comboBox_currentIndexChanged (int  index)
{
	m_updatenetworktype = index;
}

/************************************************************************
 *        Function            :  on_pushButton_setnetworktype_clicked
 *        Description         :  browse
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void RemoteUpdate::on_pushButton_setnetworktype_clicked()
{
	if (!NET_DVR_SetNetworkEnvironment(m_updatenetworktype))
	{
		QMessageBox::information(this, tr("setnetworktype failed"), 
			tr("errorno=%1").arg(NET_DVR_GetLastError()));
	}
	else
	{
		QMessageBox::information(this, tr("setnetworktype success"), 
			tr("setnetworktype success."));
	}
	return ;
}

/************************************************************************
 *        Function            :  on_pushButton_browse_clicked
 *        Description         :  browse
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void RemoteUpdate::on_pushButton_browse_clicked()
{
     QString directory = QFileDialog::getOpenFileName(this,\
           tr("Find Update Files DIR and Filename"), QDir::currentPath());
     if (!directory.isEmpty()) 
	 {
         ui.lineEdit->insert(directory);
     }
     return;
}


void RemoteUpdate::OnTimer()
{
    int iProcess = 0;

    iProcess = NET_DVR_GetUpgradeProgress(m_hUpgrade);
    qDebug("update progress:[%d]", iProcess);

    ui.horizontalScrollBar->setValue(iProcess);
    //ui.pushButton_setnetworktype_2->setText(tr(tmp));
    if (iProcess >= 100)
    {
        QMessageBox::information(this, tr("Update success"), tr("NET_DVR_Upgrade"));
        if (NET_DVR_CloseUpgradeHandle(m_hUpgrade))
        {
            m_hUpgrade = -1;
        }

        if (m_timer)
        {
            m_timer->stop();
            delete m_timer;
            m_timer = NULL;
        }
    }
}

/************************************************************************
 *        Function            :  on_pushButton_update_clicked
 *        Description         :  update
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void RemoteUpdate::on_pushButton_update_clicked()
{
    int nRet = 2;

    m_updatefilepath  = ui.lineEdit->text();
    m_hUpgrade = NET_DVR_Upgrade(*m_updateuserid, m_updatefilepath.toLatin1().data());
    if (m_hUpgrade < 0)
    {
        QMessageBox::information(this, tr("Update failed"), \
			tr("\"%1\" is return value of 	NET_DVR_Upgrade.").arg(m_hUpgrade));
        return;
    }

    startTimer( 0 );                            // run continuous timer
    m_timer = new QTimer( this );
    connect( m_timer, SIGNAL(timeout()), this, SLOT(OnTimer()));
    m_timer->start(1000);  //every 1 second

    return;
}

/************************************************************************
 *        Function            :  on_pushButton_cancel_clicked
 *        Description         :  cancel update 
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void RemoteUpdate::on_pushButton_cancel_clicked()
{
	if (m_hUpgrade >= 0)
	{
		int nRet = NET_DVR_CloseUpgradeHandle(m_hUpgrade);
		if (nRet == 1)
		{
			QMessageBox::information(this, tr("Update success"), tr("Update has success."));
		}
		else
		{
			QMessageBox::information(this, tr("Update failed"), \
				tr("\"%1\" is return value of 	NET_DVR_GetUpgradeState.").arg(nRet));
		}
	}
	
	if (m_timer != NULL)
	{
		m_timer->stop();
		delete m_timer;
	}
    close();
    
}

/************************************************************************
 *        Function            :  on_lineEdit_textChanged
 *        Description         :  file path 
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void RemoteUpdate::on_lineEdit_textChanged()
{
    QString path = ui.lineEdit->text();
    m_updatefilepath = path;
    return ;
}

