/************************************************************************
 *        Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *        FileName                      :   deploy.h
 *        Description                   :   deploy
 *        Modification History          :   none
 *        Version                       :   V1.0
 *        Time                          :   2009-11,12
 *        Author                        :   wanggongpu
 *        Descrp                        :   
*************************************************************************/
#ifndef DEPLOY_H
#define DEPLOY_H

#include <QtGui/QWidget>
#include "ui_deploy.h"
#include <QList>
#include "DemoPublic.h"
#include "devicedata.h"
#include <QDialog>
class Deploy : public QDialog
{
    Q_OBJECT

public:
    Deploy(QList<DeviceData> * tree, QDialog *parent = 0);
    ~Deploy();

private slots:
    void deployDevice(QTreeWidgetItem * item, int column);
    void on_pushButton_deployornot_clicked();
    void on_pushButton_exit_clicked();

private:
    Ui::DeployClass ui;
    QList<QTreeWidgetItem *> items;
    QList<DeviceData> *m_qlistdevicetree;

	int m_dphandle;
	//保存USERID为区分设备使用
	int m_useridtmp;
};

#endif // DEPLOY_H

