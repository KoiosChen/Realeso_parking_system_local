/************************************************************************
 *        Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *        FileName                      :   importconfig.cpp
 *        Description                   :   import and exort config 
 *        Modification History          :   none
 *        Version                       :   V1.0
 *        Time                          :   2009-11,12
 *        Author                        :   wanggongpu
 *        Descrp                        :   
*************************************************************************/
#include "importconfig.h"
#include <QMessageBox>
#include <QString>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QDir>
/************************************************************************
 *        Function            :  ImportConfig
 *        Description         :  instructor
 *        Input               :  QList<DeviceData> * tree, QWidget *parent
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
ImportConfig::ImportConfig(QList<DeviceData> * tree, QDialog *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    m_qlistdevicetree =tree;

    QList<QTreeWidgetItem *> items;
    QList<DeviceData>::iterator it;
    int i=0;
    for ( it = (*m_qlistdevicetree).begin(),i=0; i<256,it != (*m_qlistdevicetree).end(); 
	++it,++i)
    {
        if ((*it).getUsrID()>=0)
        {
            m_icuserid[i]=(*it).getUsrID();
            items.append(new QTreeWidgetItem((QTreeWidget*)0, \
            QStringList((*it).getDeviceName())));
        }
    }
    ui.treeWidget->insertTopLevelItems(0, items);
    connect(ui.treeWidget,SIGNAL(itemClicked(QTreeWidgetItem *, 
	int)),this,SLOT(importDevice(QTreeWidgetItem *, int)));
}

/************************************************************************
 *        Function            :  ~ImportConfig
 *        Description         :  destructor
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
ImportConfig::~ImportConfig()
{

}

/************************************************************************
 *        Function            :  importDevice
 *        Description         :  select a device to import or export
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void ImportConfig::importDevice(QTreeWidgetItem *item, int column)
{
    QList<DeviceData>::iterator it;
    int i=0;
    for ( it = (*m_qlistdevicetree).begin(),i=0; i<256,it != (*m_qlistdevicetree).end(); 
	++it,++i)
    {
        if ((*it).getDeviceName()==item->text(column))
        {
            m_icindex =i;
            QString cfgname((*it).getDeviceName());
            cfgname.append("--");
            cfgname.append((*it).getIP());
            ui.lineEdit_deviceip->setText(cfgname);
            QString savecfgname("./Config/");
			QString currdir("./");
		    QDir directory(currdir);
		    directory.mkpath(savecfgname);
            savecfgname.append((*it).getDeviceName());
            savecfgname.append("-config");
            savecfgname.append(".bin");
            ui.lineEdit_configfilename->setText(savecfgname);
			m_iuserid = (*it).getUsrID();
            break;
        }
    }
}

/************************************************************************
 *        Function            :  on_pushButton_export_clicked
 *        Description         :  export
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void ImportConfig::on_pushButton_export_clicked()
{
	if (ui.lineEdit_configfilename->text().isEmpty())
	{
		QMessageBox::information(this,tr("Export file error"), \
            tr("Please select one device"));
		return;
	}
    QString exportname=ui.lineEdit_configfilename->text();
    
    DWORD * ConFileLen=NULL;
    DWORD    filelen=0;
    char *     ConfigBuf =NULL;
    ConFileLen = (DWORD *)malloc(sizeof(DWORD));
    if (NULL == ConFileLen)
    {
        printf("malloc ConfigBuf error !\n");
        return;
    }
    memset(ConFileLen,0x0,sizeof(unsigned int));
    
    if (!NET_DVR_GetConfigFile_V30(m_iuserid, NULL, 0, ConFileLen))
    {
        printf("Get config file failed!\n");
    }
    else    
    {
        printf("Get config file length success!\n");
    }
    printf(" config file len [%d]!\n",*ConFileLen);

    filelen =*ConFileLen;//文件长度
    ConfigBuf=(char*)malloc(sizeof(char)*filelen);
    if (NULL == ConfigBuf)
    {
        printf("malloc ConfigBuf error !\n");
        return;
    }
    
    memset(ConFileLen,0x0,sizeof(unsigned int));
    if (!NET_DVR_GetConfigFile_V30(m_iuserid, ConfigBuf, filelen, ConFileLen))
    {
        QMessageBox::information(this,tr("NET_DVR_GetConfigFile_V30 failed"), \
            tr("len SDK_Last_Error =%1").arg(NET_DVR_GetLastError()));
        return ;
    }

    free(ConFileLen);
    free(ConfigBuf);

    if (!NET_DVR_GetConfigFile(m_iuserid, exportname.toLatin1().data()))
    {
        QMessageBox::information(this,tr("NET_DVR_GetConfigFile failed"), \
            tr("file SDK_Last_Error =%1").arg(NET_DVR_GetLastError()));
        return ;
    }
    else    
    {
        QMessageBox::information(this,tr("NET_DVR_GetConfigFile_V30 succ"), \
            tr("NET_DVR_GetConfigFile succ"));
    }
    return; 
}

/************************************************************************
 *        Function            :  on_pushButton_import_clicked
 *        Description         :  import
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void ImportConfig::on_pushButton_import_clicked()
{
    if (!NET_DVR_SetConfigFile(m_icuserid[m_icindex],\
        ui.lineEdit_configfile->text().toLatin1().data()))
    {
        QMessageBox::information(this,tr("NET_DVR_SetConfigFile failed"), \
            tr("SDK_Last_Error =%1").arg(NET_DVR_GetLastError()));
        return ;
    }
    QMessageBox::information(this,tr("NET_DVR_SetConfigFile succ"), \
            tr("NET_DVR_SetConfigFile succ"));
    return;
}

/************************************************************************
 *        Function            :  on_pushButton_exit_clicked
 *        Description         :  exit
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void ImportConfig::on_pushButton_exit_clicked()
{
    close();
}

/************************************************************************
 *        Function            :  on_pushButton_browse_clicked
 *        Description         :  browse the dir for config file
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void ImportConfig::on_pushButton_browse_clicked()
{
     QString directory = QFileDialog::getOpenFileName(this,\
           tr("Find Config Files DIR and Filename"), QDir::currentPath());
     if (!directory.isEmpty())
     {
         ui.lineEdit_configfile->setText(directory);
     }
     return;
}

