/************************************************************************
 *        Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *        FileName                      :   formatdisk.h
 *        Description                   :   format disk
 *        Modification History          :   none
 *        Version                       :   V1.0
 *        Time                          :   2009-11,12
 *        Author                        :   wanggongpu
 *        Descrp                        :   
*************************************************************************/
#ifndef FORMATDISK_H
#define FORMATDISK_H

#include <QtGui/QDialog>
#include "ui_formatdisk.h"
#include "DemoPublic.h"

class FormatDisk : public QDialog
{
    Q_OBJECT

public:
    FormatDisk(int *userid, QWidget *parent = 0);
    ~FormatDisk();
private slots:
    void on_comboBox_currentIndexChanged ( int index);
    void on_pushButton_format_clicked();
    void on_pushButton_exit_clicked();
private:
    Ui::FormatDiskClass ui;
    
    int * m_fduserid;
    NET_DVR_HDCFG m_fdhdcfg;
    int m_fdindex;
    LONG m_disknum[MAX_DISKNUM_V30];
    int m_indexnum;
};

#endif // FORMATDISK_H

