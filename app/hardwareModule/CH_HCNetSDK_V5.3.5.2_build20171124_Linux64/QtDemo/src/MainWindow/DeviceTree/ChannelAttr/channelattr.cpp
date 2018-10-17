/***************************************************************************************
 *      Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *      FileName        :       channelattr.cpp
 *      Description     :       通道属性源文件
 *      Modification    :       无
 *      Version         :       V1.0.0
 *      Time            :       2009-10,11
 *      Author          :       wanggp@hikvision.com
 **************************************************************************************/
#include "channelattr.h"

/************************************************************************
 *        Function            :  ChannelAttr
 *        Description         :  构造函数
 *        Input               :  parent
 *        Output              :  建立界面
 *        Return              :  无
*************************************************************************/
ChannelAttr::ChannelAttr(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    ui.channNum_lineEdit->setDisabled(1);
    ui.devAddr_lineEdit->setDisabled(1);
    ui.devName_lineEdit->setDisabled(1);
}

/************************************************************************
 *        Function            :  ~ChannelAttr
 *        Description         :  析构函数
 *        Input               :  无
 *        Output              :  关闭界面
 *        Return              :  无
*************************************************************************/
ChannelAttr::~ChannelAttr()
{

}

