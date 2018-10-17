/*
 * Copyright(C) 2010,Hikvision Digital Technology Co., Ltd 
 * 
 * 文件名称：catmparams.h
 * 描    述：
 * 当前版本：1.0
 * 作    者：潘亚东
 * 创建日期：2010年1月11日
 * 修改记录：
 */


#ifndef CATMPARAMS_H
#define CATMPARAMS_H

#include <QtGui/QWidget>
#include "ui_catmparams.h"
#include "catmnetparams.h"

class CATMParams : public QWidget
{
    Q_OBJECT

public:
    CATMParams(QWidget *parent = 0);
    ~CATMParams();

    int getATMInfo();
    int setATMInfo();

    int setUserID(long lUserID);

 public slots:
	 void comboInputTypeChanged(int iType);


private:
    long m_lUserID;
    CATMNetParams *m_pNetListen;
    Ui::CATMParamsClass ui;


    //event
    void showEvent ( QShowEvent * event );

    //判断系统是否支持ATM机， 如果不支持，界面不可用
    int isATMDevice();
};

#endif // CATMPARAMS_H
