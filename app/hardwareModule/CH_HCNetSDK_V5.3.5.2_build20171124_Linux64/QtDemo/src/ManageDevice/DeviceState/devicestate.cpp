/************************************************************************
 *        Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *        FileName                      :   devicestate.cpp
 *        Description                   :   devicestate show
 *        Modification History          :   none
 *        Version                       :   V1.0
 *        Time                          :   2009-11,12
 *        Author                        :   wanggongpu
 *        Descrp                        :   
*************************************************************************/
#include "devicestate.h"
#include <QMessageBox>
#include <QTableWidgetItem>

/************************************************************************
 *        Function            :  DeviceState
 *        Description         :  instructor
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
DeviceState::DeviceState(QList<DeviceData> *devicetree, int* userid, QDialog *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    m_dsuserid = userid;
    m_dsdevicetree =devicetree;
    
    QList<DeviceData>::iterator it;
    for ( it = (*m_dsdevicetree).begin(); it != (*m_dsdevicetree).end(); ++it)
    {
        if ((*it).getUsrID()==(*m_dsuserid))
        {
            ui.lineEdit_nodename->setText((*it).getDeviceName());
            ui.lineEdit_deviceIP->setText((*it).getIP());
            
            if (!NET_DVR_GetDVRWorkState(*m_dsuserid, &m_dsworkstate)) 
            {
                QMessageBox::information(this,tr("NET_DVR_GetDVRWorkState failed"), \
                    tr("SDK_Last_Error =%1").arg(NET_DVR_GetLastError()));
                return ;
            }
            printDVRState(&m_dsworkstate);
        }
    }

}

/************************************************************************
 *        Function            :  ~DeviceState
 *        Description         :  destructor
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
DeviceState::~DeviceState()
{

}

/************************************************************************
 *        Function            :  printDVRState
 *        Description         :  print the dvr state info 
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void DeviceState::printDVRState(LPNET_DVR_WORKSTATE pworkstate)
{

    QString state;
    /*0－正常；1－CPU占用率太高，超过85%；2－硬件错误，例如串口异常*/
    switch (pworkstate->dwDeviceStatic)
    {
        case 0:
            state.append("Normal");
            break;
            
        case 1:
            state.append("CPU Plus 85%");   
            break;
            
        case 2:
            state.append("Hardware Error");
            break;
        default:
            break;
    }
    ui.lineEdit_devicestate->setText(state);

    insertChannelState( pworkstate);    
    insertDiskState( pworkstate);


    return;

}

/************************************************************************
 *        Function            :  insertChannelState
 *        Description         :  insert item to table 
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void DeviceState::insertChannelState( LPNET_DVR_WORKSTATE pworkstate)
{   
    int i=0;
    int j=0;
    int row=0;
    for (i=0; i<MAX_CHANNUM; i++) 
    {
        QTableWidgetItem *DiskNum = new QTableWidgetItem(tr("Cameral%1").arg(int(i+1)));
        DiskNum->setFlags(Qt::ItemIsEnabled);
        QTableWidgetItem *RecordStatic ;
        if (int(pworkstate->struChanStatic[i].byRecordStatic)==0)
        {
            RecordStatic = new QTableWidgetItem(tr("%1").arg("Not record"));
        }
        else
        {
            RecordStatic = new QTableWidgetItem(tr("%1").arg("Record"));
        }
        RecordStatic->setFlags(Qt::ItemIsEnabled);
        QTableWidgetItem *SignalStatic ;
        if (pworkstate->struChanStatic[i].bySignalStatic==0)
        {
            SignalStatic= new QTableWidgetItem(tr("%1").arg("Record"));
        }
        else 
        {
            SignalStatic= new QTableWidgetItem(tr("%1").arg("信号丢失"));    
        }
        SignalStatic->setFlags(Qt::ItemIsEnabled);
        
        QTableWidgetItem *HardwareStatic;
        if (pworkstate->struChanStatic[i].byHardwareStatic==0)
        {
            HardwareStatic= new QTableWidgetItem(tr("%1").arg("正常"));
        }
        else
        {
            HardwareStatic= new QTableWidgetItem(tr("%1").arg("异常"));
        }
        HardwareStatic->setFlags(Qt::ItemIsEnabled);

        QTableWidgetItem *BitRate = new QTableWidgetItem(tr("%1 "\
			).arg(int(pworkstate->struChanStatic[i].dwBitRate)));
        BitRate->setFlags(Qt::ItemIsEnabled);
        
        QTableWidgetItem *LinkNum = new QTableWidgetItem(tr("%1 "\
			).arg(int(pworkstate->struChanStatic[i].dwLinkNum)));
        LinkNum->setFlags(Qt::ItemIsEnabled);

        row = ui.channeltableWidget->rowCount();
        ui.channeltableWidget->insertRow(row);

        ui.channeltableWidget->setItem(row, 0, DiskNum);
        ui.channeltableWidget->setItem(row, 1, RecordStatic);
        ui.channeltableWidget->setItem(row, 2, SignalStatic);
        ui.channeltableWidget->setItem(row, 3, HardwareStatic);
        ui.channeltableWidget->setItem(row, 4, LinkNum);
        ui.channeltableWidget->setItem(row, 5, BitRate);

        j+=pworkstate->struChanStatic[i].dwLinkNum;

    }
    QString string = QString::number(j);
    ui.lineEdit_totalconnum->setText(string);
    return ;

}

/************************************************************************
 *        Function            :  insertDiskState
 *        Description         :  add item to table 
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void DeviceState::insertDiskState( LPNET_DVR_WORKSTATE pworkstate)
{
    int row ;
    int i;
    
    for (i=0; i<MAX_DISKNUM; i++)//硬盘状态
    {
        QTableWidgetItem *hisknum = new QTableWidgetItem(tr("DISK%1").arg(int(i+1)));
        hisknum->setFlags(Qt::ItemIsEnabled);

        QTableWidgetItem *disksize = new 
        QTableWidgetItem(tr("%1").arg(int(pworkstate->struHardDiskStatic[i].dwVolume)));
        disksize->setFlags(Qt::ItemIsEnabled);

        QTableWidgetItem *freesize = new 
        QTableWidgetItem(tr("%1").arg(int(pworkstate->struHardDiskStatic[i].dwFreeSpace)));
        freesize->setFlags(Qt::ItemIsEnabled);

		QString diskstate;
		if ((pworkstate->struHardDiskStatic[i].dwVolume!=0)
			&&(pworkstate->struHardDiskStatic[i].dwHardDiskStatic == 0))
		{
			diskstate.append("活动");
		}
		else
		{
			diskstate =QString::number(int(pworkstate->struHardDiskStatic[i].dwFreeSpace));
		}
        QTableWidgetItem *diskstatic = new QTableWidgetItem(diskstate);
        diskstatic->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        diskstatic->setFlags(Qt::ItemIsEnabled);

        row = ui.disktableWidget->rowCount();
        ui.disktableWidget->insertRow(row);

        ui.disktableWidget->setItem(row, 0, hisknum);
        ui.disktableWidget->setItem(row, 1, disksize);
        ui.disktableWidget->setItem(row, 2, freesize);
        ui.disktableWidget->setItem(row, 3, diskstatic);
    }
    return ;
}

/************************************************************************
 *        Function            :  on_pushButton_refresh_clicked
 *        Description         :  refresh the dialogue 
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void DeviceState::on_pushButton_refresh_clicked()
{   
    //待完善
    for (int i=0; i<ui.disktableWidget->rowCount(); i++)
    {
        for (int j=0;j<ui.disktableWidget->columnCount();j++ )
        {
            delete ui.disktableWidget->takeItem(i, j);
        }
        ui.disktableWidget->removeRow(i);
    }

    ui.disktableWidget->setRowCount(0);

	for (int i=0; i<ui.channeltableWidget->rowCount(); i++)
    {
        for (int j=0;j<ui.channeltableWidget->columnCount();j++ )
        {
            delete ui.channeltableWidget->takeItem(i, j);
        }
        ui.channeltableWidget->removeRow(i);
    }

    ui.channeltableWidget->setRowCount(0);

	if (!NET_DVR_GetDVRWorkState(*m_dsuserid, &m_dsworkstate)) 
    {
        QMessageBox::information(this,tr("NET_DVR_GetDVRWorkState failed"), \
            tr("SDK_Last_Error =%1").arg(NET_DVR_GetLastError()));
        return ;
    }	
    printDVRState(&m_dsworkstate);
    return;
}

/************************************************************************
 *        Function            :  on_pushButton_exit_clicked
 *        Description         :  exit the dialogue 
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void DeviceState::on_pushButton_exit_clicked()
{
    close();
    return;
}

