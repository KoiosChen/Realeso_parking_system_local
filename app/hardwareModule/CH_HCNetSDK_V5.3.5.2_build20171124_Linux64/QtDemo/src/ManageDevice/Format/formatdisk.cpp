/************************************************************************
 *        Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *        FileName                      :   formatdisk.cpp
 *        Description                   :   format disk 
 *        Modification History          :   none
 *        Version                       :   V1.0
 *        Time                          :   2009-11,12
 *        Author                        :   wanggongpu
 *        Descrp                        :   
*************************************************************************/
#include "formatdisk.h"
#include <QString>
#include <QMessageBox>
#include "common.h"
#include <unistd.h>
/************************************************************************
 *        Function            :  FormatDisk
 *        Description         :  instructor
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
FormatDisk::FormatDisk(int *userid, QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    m_fduserid =userid;
    DWORD returnbytes =0;
    
    if (!NET_DVR_GetDVRConfig(*m_fduserid, NET_DVR_GET_HDCFG,\
        0xFFFFFFFF, &m_fdhdcfg, sizeof(NET_DVR_HDCFG),&returnbytes))
    {
        QMessageBox::information(this,tr("NET_DVR_GetDVRConfig failed"), \
            tr("SDK_Last_Error =%1").arg(NET_DVR_GetLastError()));
        return ;
    }
    QString all("All Disks");
    ui.comboBox->addItem(all);
	
    m_disknum[0]=0xff;
    int j=0;
    for (int i=0;i<m_fdhdcfg.dwHDCount;i++,j++)
    {   
        m_disknum[++j]=m_fdhdcfg.struHDInfo[i].dwHDNo;
        ui.comboBox->addItem(QString::number(m_fdhdcfg.struHDInfo[i].dwHDNo));
    }
    ui.horizontalSlider->setRange(0, 100);
	setUpdatesEnabled ( TRUE );
	update();
	repaint();
}

/************************************************************************
 *        Function            :  ~FormatDisk 
 *        Description         :  destructor 
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
FormatDisk::~FormatDisk()
{
    for (int i=0;i<=m_indexnum;i++)
    {
        ui.comboBox->removeItem (i);
    }
     
}

/************************************************************************
 *        Function            :  
 *        Description         :   
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void FormatDisk::on_comboBox_currentIndexChanged ( int index)
{
    m_fdindex =index;
}

/************************************************************************
 *        Function            :  on_pushButton_format_clicked
 *        Description         :  format 
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void FormatDisk::on_pushButton_format_clicked()
{
    int format=0;
	if (m_fdindex!=0)
	{
		m_disknum[m_fdindex] = m_fdindex;
	}

    format = NET_DVR_FormatDisk(*m_fduserid, m_disknum[m_fdindex]);
    if (format < 0)
    {
        QMessageBox::information(this,tr("NET_DVR_FormatDisk failed"), \
            tr("SDK_Last_Error =%1").arg(NET_DVR_GetLastError()));
        return ;
    }

    LONG pos = 0;
    LONG pstatic = 0;
    while (pstatic != 1)
    {  
#if (defined(_WIN32))
#elif defined(__linux__)
        sleep(1);
#endif
		
        if (!NET_DVR_GetFormatProgress(format,&m_disknum[m_fdindex],&pos, &pstatic))
        {
            QMessageBox::information(this,tr("NET_DVR_GetFormatProgress failed"), \
                tr("SDK_Last_Error =%1").arg(NET_DVR_GetLastError()));
           //return ;
        }

        ui.horizontalSlider->setValue(pos++);
        if (pstatic !=0)
        {
        	ui.horizontalSlider->setValue(100);
            break;
        }
    }
	NET_DVR_CloseFormatHandle(format);
    QMessageBox::information(this,tr("NET_DVR_GetFormatProgress"), \
            tr("FORMAT SUCCESS OVER format=%1 ").arg(format));

}

/************************************************************************
 *        Function            :  on_pushButton_exit_clicked
 *        Description         :  exit
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void FormatDisk::on_pushButton_exit_clicked()
{
    close();    
}

