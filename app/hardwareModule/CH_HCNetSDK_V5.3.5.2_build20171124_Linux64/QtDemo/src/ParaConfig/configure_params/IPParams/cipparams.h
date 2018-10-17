#ifndef CIPPARAMS_H
#define CIPPARAMS_H

#include <QtGui/QWidget>
#include <QtGui/QPushButton>
#include "ui_cipparams.h"
#include "DemoPublic.h"

class CIPParams : public QWidget
{
    Q_OBJECT

public:
    CIPParams(QWidget *parent = 0);
    ~CIPParams();

    //set userID
    int setUserID(long lUserID);

    //获取通道信息，写入表格中
    int getIPInfo();
    //从表格中得到数据，写入设备
    int saveIPInfo();

public slots:

    //(click table)响应表格单击事件
    int clickTabChannel(int iRow);
    //IP通道参数设置
    void on_btnEdit_clicked();
    //删除IP通道参数
    void on_btnDelete_clicked();
    //启用模拟通道
    void on_btnEnable_clicked();
    //禁用模拟通道
    void on_btnDisenable_clicked();

private:
    Ui::CIPParamsClass ui;
    long m_lUserID;
    int m_iIPParamIndex;
    int m_iCurrentRow;

    //获取设备信息
    int getDeviceInfo(LPNET_DVR_DEVICECFG pDeviceInfo);
    //获取IP接入信息
    int getIPConInfo(LPNET_DVR_IPPARACFG pIPConInfo);
    //获取用于IP接入参数的索引值
    int getIPChannnelIndex(int iRow, int *piChannelIndex);
    //图形显示时出发
    void showEvent ( QShowEvent * event );
};

#endif // CIPPARAMS_H
