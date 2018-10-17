/*
 * Copyright(C) 2010,Hikvision Digital Technology Co., Ltd 
 * 
 * 文件名称：catmnetparams.h
 * 描    述：
 * 当前版本：1.0
 * 作    者：潘亚东
 * 创建日期：2010年1月11日
 * 修改记录：
 */

#ifndef CATMNETPARAMS_H
#define CATMNETPARAMS_H

#include <QtGui/QWidget>
#include "ui_catmnetparams.h"
#include "DemoPublic.h"

//using namespace Ui;

class CATMNetParams : public QWidget
{
    Q_OBJECT

public:
    CATMNetParams(QWidget *parent = 0);
    ~CATMNetParams();

    //将设备中的数据写入控件中
    int getATMInfo(LPNET_DVR_FRAMEFORMAT pstruATMInfo);
    //将控件中的数据写入设备中
    int setATMInfo(LPNET_DVR_FRAMEFORMAT pstruATMInfo);

public slots:
	void comboCodeTypeChanged(int iType);

private:
    Ui::CATMNetParamsClass ui;
    int m_bCode; //是否有代码
    NET_DVR_FRAMETYPECODE    m_frameTypeCode[10]; //ATM 代码信息
};

#endif // CATMNETPARAMS_H
