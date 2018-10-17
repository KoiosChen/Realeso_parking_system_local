/************************************************************************
 *        Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *        FileName                      :   exitdemo.cpp
 *        Description                   :   exit dialogue 
 *        Modification History          :   none
 *        Version                       :   V1.0
 *        Time                          :   2009-11,12
 *        Author                        :   wanggongpu
 *        Descrp                        :   responding implemnet in mainwindow
*************************************************************************/
#include "exitdemo.h"

/************************************************************************
 *        Function            :  ExitDemo
 *        Description         :  instructor 
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
ExitDemo::ExitDemo(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
}

/************************************************************************
 *        Function            :  ~ExitDemo
 *        Description         :  destructor
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
ExitDemo::~ExitDemo()
{

}
