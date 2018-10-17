/************************************************************************
 *        Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *        FileName                      :   serialtransfer.cpp
 *        Description                   :   serial transfer
 *        Modification History          :   none
 *        Version                       :   V1.0
 *        Time                          :   2009-11,12
 *        Author                        :   wanggongpu
 *        Descrp                        :   
*************************************************************************/
#include "serialtransfer.h"
#include <QDateTime>
#include <QMessageBox>
#include <QString>
#include "channeldata.h"
#include "qtclientdemo.h"
extern QtClientDemo* gqtclinetdemo;

/************************************************************************
 *        Function            :  SerialTransfer
 *        Description         :  instructor
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
SerialTransfer::SerialTransfer(QList<DeviceData> *devicetree, int *userid, QDialog *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
	qtreelist = devicetree;
    m_stuserid =userid;
	m_stserialtype = 1;
	ui.comboBox_channeltype->setCurrentIndex(0);
	ui.comboBox_channelnum->setDisabled(1);

	//QString channelzero("Channel_0");
	//ui.comboBox_channelnum->addItem(channelzero);
	QList<DeviceData>::iterator it;
	QList<ChannelData>::iterator it_channel;
	for ( it = (*qtreelist).begin(); it != (*qtreelist).end(); ++it)
    {
        if ((*it).getUsrID()==(*m_stuserid))
        {
			for (it_channel = (*it).m_qlistchanneldata.begin(); \
					it_channel != (*it).m_qlistchanneldata.end();\
						++it_channel)
    		{
    			QString ipccameral("IPCameral");
        		if ((*it_channel).getChannelName().contains(ipccameral)==0)
        		{   
        			QString channelnum = QString::number((*it_channel).getChannelNum());
					channelnum.prepend("Channel_");
					ui.comboBox_channelnum->addItem(channelnum);			
        		}
			}
        }
    }
    m_sthandle =-1;
}

/************************************************************************
 *        Function            :  ~SerialTransfer
 *        Description         :  destructor
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
SerialTransfer::~SerialTransfer()
{
	if (m_sthandle != -1)
	{
		NET_DVR_SerialStop(m_sthandle);
	}
}

/************************************************************************
 *        Function            :  on_comboBox_channeltype_currentIndexChanged
 *        Description         :  channel type
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void SerialTransfer::on_comboBox_channeltype_currentIndexChanged ( int index)
{
    m_stserialtype = index+1;
	if (m_stserialtype == 1)
	{
		ui.comboBox_channelnum->setDisabled(1);
		ui.comboBox_channeltype->setCurrentIndex(0);
		m_st485channelnum = 0;
	}
	else
	{
		ui.comboBox_channelnum->setDisabled(0);
	}
}

/************************************************************************
 *        Function            :  on_comboBox_channelnum_currentIndexChanged
 *        Description         :  chanel num
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void SerialTransfer::on_comboBox_channelnum_currentIndexChanged (int index)
{
    m_st485channelnum =index+1;

}

/************************************************************************
 *        Function            :  on_comboBox_sendinterface_currentIndexChanged
 *        Description         :  interface
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void SerialTransfer::on_comboBox_sendinterface_currentIndexChanged ( int index)
{
    m_stinterfaceindex =index;
}

/************************************************************************
 *        Function            :  on_pushButton_send_clicked
 *        Description         :  send
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/  
void SerialTransfer::on_pushButton_send_clicked()
{

    m_stsenddata= ui.lineEdit->text();
    m_stinterfaceindex=ui.comboBox_sendinterface->currentIndex();
    m_stserialtype =ui.comboBox_channeltype->currentIndex()+1;
    QByteArray ba = m_stsenddata.toLatin1();
    char *pSendBuf = ba.data();
    int bufsize =strlen(pSendBuf);

    if (m_stinterfaceindex ==1)
    {  
        if (!NET_DVR_SendToSerialPort(*m_stuserid, m_stserialtype, \
            1, m_stsenddata.toLatin1().data(),bufsize))
        {
            QMessageBox::information(this,tr("NET_DVR_SerialStart ERROR"), \
                tr("SDK_Last_Error =%1").arg(NET_DVR_GetLastError()));  
            
            return;
        }
        QMessageBox::information(this, tr("NET_DVR_SerialSend success"), \
			tr("NET_DVR_SerialSend 	success"));
        return;
    }
    else if (m_stinterfaceindex ==0) 
    {   
        if (m_stserialtype== 1)
        {   
            if (!NET_DVR_SerialSend(m_sthandle, 0, m_stsenddata.toLatin1().data(), bufsize)) 
            {
				QMessageBox::information(this, tr("NET_DVR_SerialSend failed"), 
					tr("NET_DVR_SerialSend failed"));

            }  
			else
			{
				QMessageBox::information(this, tr("NET_DVR_SerialSend success"), 
							tr("NET_DVR_SerialSend success"));
			}
            
            return;
        }
        else if (m_stserialtype == 2)
        {   
            m_st485channelnum = ui.comboBox_channelnum->currentIndex();
            if  (!NET_DVR_SerialSend(m_sthandle,m_st485channelnum,\
                    m_stsenddata.toLatin1().data(), bufsize)) 
            {
				QMessageBox::information(this, tr("NET_DVR_SerialSend failed"), 
					tr("NET_DVR_SerialSend failed"));

            }  
			else
			{
				QMessageBox::information(this, tr("NET_DVR_SerialSend success"), 
							tr("NET_DVR_SerialSend success"));
			}
            return;
        }   
    }
    else if (m_stinterfaceindex ==2)
    {
        if (!NET_DVR_SendTo232Port(*m_stuserid,  m_stsenddata.toLatin1().data(), bufsize))
        {
            QMessageBox::information(this,tr("NET_DVR_SerialStart ERROR"), \
                tr("SDK_Last_Error =%1").arg(NET_DVR_GetLastError()));  
            return;
        }
        QMessageBox::information(this, tr("NET_DVR_SendTo232Port success"), 
		tr("NET_DVR_SendTo232Port success"));
        return ;

    }
    return ;
}

/************************************************************************
 *        Function            :  SerialDataCallBack
 *        Description         :  call back
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void __stdcall SerialDataCallBack( LONG lSerialHandle, 
                         char *pRecvDataBuffer,
                         DWORD dwBufSize,
                         DWORD dwUser )
{
    int i=0;
    QString currentdatetime = QDateTime::currentDateTime().toString(" yyyy-MM-dd hh:mm:ss");
    QString datastring = QString(QLatin1String(pRecvDataBuffer));

    fprintf(stderr, "No.[%d]:lSerialHandle[%d], pRecvDataBuffer[%s], dwBufSize[%d], dwUser[%d]\n",
            ++i, lSerialHandle, pRecvDataBuffer, dwBufSize, dwUser);   

	if (gqtclinetdemo->m_serial !=	NULL)
	{
		QTableWidgetItem *currnettime = new QTableWidgetItem(currentdatetime);	
		QTableWidgetItem *data = new QTableWidgetItem(datastring);	
		int row = gqtclinetdemo->m_serial->ui.tableWidget->rowCount();
		gqtclinetdemo->m_serial->ui.tableWidget->insertRow(row);
		gqtclinetdemo->m_serial->ui.tableWidget->setItem(row, 0, currnettime);
		gqtclinetdemo->m_serial->ui.tableWidget->setItem(row, 1, data);
	}

	
    return;
}

/************************************************************************
 *        Function            :  on_pushButton_startserial_clicked
 *        Description         :  start
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void SerialTransfer::on_pushButton_startserial_clicked()
{
	if (m_sthandle!=-1)
    {
    	QMessageBox::information(this,tr("NET_DVR_SerialStart succ"), \
            tr("NET_DVR_SerialStart has started already!"));  
        return;
    }
	m_sthandle = NET_DVR_SerialStart(*m_stuserid,m_stserialtype,SerialDataCallBack,0);
    if (m_sthandle==-1)
    {
        QMessageBox::information(this,tr("NET_DVR_SerialStart ERROR"), \
            tr("SDK_Last_Error =%1 *m_stuserid=%2 m_stserialtype=%3").arg(NET_DVR_GetLastError()).arg(*m_stuserid).arg(m_stserialtype));  
        return;
    }
	QMessageBox::information(this,tr("NET_DVR_SerialStart succ"), \
            tr("NET_DVR_SerialStart success"));  
}

/************************************************************************
 *        Function            :  on_pushButton_stopserial_clicked
 *        Description         :  stop
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void SerialTransfer::on_pushButton_stopserial_clicked()
{
    if (m_sthandle==-1)
    {
    	QMessageBox::information(this,tr("NET_DVR_SerialStop succ"), \
            tr("NET_DVR_SerialStop has stopped already!"));  
        return;
    }
    if (!NET_DVR_SerialStop(m_sthandle))
    {
        QMessageBox::information(this,tr("NET_DVR_SerialStop ERROR"), \
            tr("SDK_Last_Error =%1").arg(NET_DVR_GetLastError()));  
		
        return;
    }
	
    QMessageBox::information(this,tr("NET_DVR_SerialStop succ"), \
            tr("NET_DVR_SerialStop successfully"));
	m_sthandle = -1;
}

/************************************************************************
 *        Function            :  on_pushButton_exit_clicked
 *        Description         :  exit
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void SerialTransfer::on_pushButton_exit_clicked()
{
    close();
}

/************************************************************************
 *        Function            :  on_lineEdit_textChanged
 *        Description         :  none
 *        Input               :  const QString & text
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void SerialTransfer::on_lineEdit_textChanged ( const QString & text )
{
    m_stsenddata =text;
    return;
}

