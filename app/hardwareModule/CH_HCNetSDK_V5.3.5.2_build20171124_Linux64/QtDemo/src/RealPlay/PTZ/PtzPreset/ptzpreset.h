/************************************************************************
 *        Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *        FileName                      :   ptzpreset.h
 *        Description                   :   预览云台预置点设置相关操作
 *        Modification History          :   none
 *        Version                       :   V1.0
 *        Time                          :   2009-2010
 *        Author                        :   wanggongpu
 *        Descrp                        :   none
*************************************************************************/
#ifndef PTZPRESET_H
#define PTZPRESET_H

#include <QtGui/QDialog>
#include "ui_ptzpreset.h"
#include "DemoPublic.h"

class PtzPreset : public QDialog
{
    Q_OBJECT

public:
    PtzPreset(int realhandle,QWidget *parent = 0);
    ~PtzPreset();
	
private slots:
	void on_pushButton_addptzpresetpoint_clicked();
	void on_pushButton_deleteptzpresetpoint_clicked();
	void on_pushButton_exit_clicked();
	void on_comboBox_ptzpresetpointindex_currentIndexChanged(int index);

private:
    Ui::PtzPresetClass ui;
	int m_rpcurrentrealhandle;
	int m_presetpointindex;
};

#endif // PTZPRESET_H

