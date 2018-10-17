/***************************************************************************************
 *      Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *      FileName        :       deviceattr.h
 *      Description     :       设备属性头文件
 *      Modification    :       无
 *      Version         :       V1.0.0
 *      Time            :       2009-10,11
 *      Author          :       wanggp@hikvision.com
 **************************************************************************************/
#ifndef DEVICEATTR_H
#define DEVICEATTR_H

#include <QtGui/QDialog>
#include "ui_deviceattr.h"

class DeviceAttr : public QDialog
{
    Q_OBJECT

public:
    friend class QtClientDemo;
    DeviceAttr(QWidget *parent = 0);
    ~DeviceAttr();

private:
    Ui::DeviceAttrClass ui;
};

#endif // DEVICEATTR_H

