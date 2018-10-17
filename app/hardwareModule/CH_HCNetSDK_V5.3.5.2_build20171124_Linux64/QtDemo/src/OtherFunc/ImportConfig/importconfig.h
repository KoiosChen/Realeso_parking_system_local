/************************************************************************
 *        Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *        FileName                      :   importconfig.h
 *        Description                   :   import or export config 
 *        Modification History          :   none
 *        Version                       :   V1.0
 *        Time                          :   2009-11,12
 *        Author                        :   wanggongpu
 *        Descrp                        :   
*************************************************************************/
#ifndef IMPORTCONFIG_H
#define IMPORTCONFIG_H
#include <QDialog>
#include <QtGui/QWidget>
#include "ui_importconfig.h"
#include "DemoPublic.h"
#include "devicedata.h"
#include <QList>

class ImportConfig : public QDialog
{
    Q_OBJECT

public:
    ImportConfig(QList<DeviceData> * tree, QDialog *parent = 0);
    ~ImportConfig();

private slots:
    void importDevice(QTreeWidgetItem *item, int column);
    
    void on_pushButton_export_clicked();
    void on_pushButton_import_clicked();
    void on_pushButton_exit_clicked();
    void on_pushButton_browse_clicked();

private:
    Ui::ImportConfigClass ui;
    QList<DeviceData> *m_qlistdevicetree;
    int m_icuserid[256];    
    int m_icindex;
	int m_iuserid;
};

#endif // IMPORTCONFIG_H

