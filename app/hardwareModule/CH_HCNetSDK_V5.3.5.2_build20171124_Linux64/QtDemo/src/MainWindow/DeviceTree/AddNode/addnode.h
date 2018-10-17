/**  @file addnode.h
 *   @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved. 
 *   @brief 
 *   
 *   @author wanggongpu
 *   @modify by panyadong
 *   @date 2012-5-22
 *   
 *   @note:
 */
#ifndef ADDNODE_H
#define ADDNODE_H

#include <QtGui/QDialog>
#include "ui_addnode.h"

#include "DemoPublic.h"
#include "devicedata.h"

class AddNode : public QDialog
{
    Q_OBJECT

public:
    friend class QtClientDemo;

    AddNode(QWidget *parent = 0);
    ~AddNode();

private slots:
    void on_cancel_pushButton_clicked();
    void on_m_pBtnGetDeviceIP_clicked();
    void CheckUserIPServer();
        
private:
    Ui::AddNodeClass ui;
    int m_iuserid;
    NET_DVR_DEVICEINFO_V30 m_deviceinfo;
    NET_DVR_IPPARACFG m_ipparacfg;
};

#endif // ADDNODE_H

