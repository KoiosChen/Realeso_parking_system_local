/************************************************************************
 *        Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *        FileName                      :   exitdemo.h
 *        Description                   :   exit
 *        Modification History          :   none
 *        Version                       :   V1.0
 *        Time                          :   2009-11,12
 *        Author                        :   wanggongpu
 *        Descrp                        :   
*************************************************************************/
#ifndef EXITDEMO_H
#define EXITDEMO_H

#include <QtGui/QDialog>
#include "ui_exitdemo.h"

class ExitDemo : public QDialog
{
    Q_OBJECT

public:
    friend class QtClientDemo;
    ExitDemo(QWidget *parent = 0);
    ~ExitDemo();

private:
    Ui::ExitDemoClass ui;
};

#endif // EXITDEMO_H
