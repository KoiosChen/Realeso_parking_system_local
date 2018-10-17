/***************************************************************************************
 *      Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *      FileName        :       deviceattr.cpp
 *      Description     :       设备属性源文件
 *      Modification    :       无
 *      Version         :       V1.0.0
 *      Time            :       2009-10,11
 *      Author          :       wanggp@hikvision.com
 **************************************************************************************/
#include "deviceattr.h"

/************************************************************************
 *        Function            :  DeviceAttr
 *        Description         :  设备属性界面
 *        Input               :  parent
 *        Output              :  无
 *        Return              :  无
*************************************************************************/
DeviceAttr::DeviceAttr(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    ui.SID_lineEdit->setDisabled(1);
    ui.devType_lineEdit->setDisabled(1);
    ui.McastIP_lineEdit->setDisabled(1);
    ui.channelNum_lineEdit->setDisabled(1);
}

/************************************************************************
 *        Function            :  ~DeviceAttr
 *        Description         :  析构函数
 *        Input               :  无
 *        Output              :  无
 *        Return              :  无
*************************************************************************/
DeviceAttr::~DeviceAttr()
{

}

