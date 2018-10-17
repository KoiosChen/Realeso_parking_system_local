/*
 * Copyright(C) 2009,Hikvision Digital Technology Co., Ltd 
 * 
 * 文件名称：cshelter.h
 * 描    述：
 * 当前版本：1.0
 * 作    者：潘亚东
 * 创建日期：2009年11月24日
 * 修改记录：
 */


#ifndef CSHELTER_H
#define CSHELTER_H

#include <QtGui/QDialog>
#include "ui_cshelter.h"
#include <QtGui/QLineEdit>
#include <QtGui/QCheckBox>
#include "DemoPublic.h"

class CShelter : public QDialog
{
    Q_OBJECT

public:
    CShelter(QWidget *parent = 0);
    ~CShelter();

    //遮挡区域
    int setShelter(NET_DVR_SHELTER *pShelter);

    //保存设置

    //event
    void showEvent ( QShowEvent * event );

public slots:
    //保存设置
    void on_btnSave_clicked();

private:
    Ui::CShelterClass ui;
    NET_DVR_SHELTER *m_pShelter;
    QTableWidgetItem m_itemShelter[MAX_SHELTERNUM][4];//遮挡区域

    //设置表示遮挡区域的table中的值
    int setEachWidgetValue();
    //获取表示遮挡区域的table中的值
    int getEachWidgetValue();
};

#endif // CSHELTER_H
