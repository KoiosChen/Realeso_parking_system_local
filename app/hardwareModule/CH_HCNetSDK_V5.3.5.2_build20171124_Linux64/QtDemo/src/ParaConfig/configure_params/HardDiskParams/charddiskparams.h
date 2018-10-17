#ifndef CHARDDISKPARAMS_H
#define CHARDDISKPARAMS_H

#include <QtGui/QWidget>
#include <QtGui/QCheckBox>
#include "ui_charddiskparams.h"
#include "DemoPublic.h"

#define MAX_CHAN_NUM 64

//往HD table中写入的控件
typedef struct
{
	//0:盘号; 1:容量; 2:剩余空间;
	QTableWidgetItem *pTableItem[3];
	//0:状态; 1:属性; 2:类型; 3:盘组
	QComboBox *pTableCombo[4];
}KIT_HD_WIDGET;

class CHardDiskParams : public QWidget
{
    Q_OBJECT

public:
    CHardDiskParams(QWidget *parent = 0);
    ~CHardDiskParams();

    //set userID
    int setUserID(long lUserID);

    //获取硬盘信息
    int getHardDiskInfo();
    //设置硬盘参数
    int setHardDiskInfo();

    //获取硬盘组信息
    int getHDGroupInfo();
    //设置硬盘组参数
    int setHDGroupInfo();

public slots:
    //刷新硬盘管理参数
    void on_btnHDUpdate_clicked();
    //保存硬盘管理参数
    void on_btnHDSave_clicked();

    //刷新硬盘组管理参数
    void on_btnHDGroupUpdate_clicked();
    //保存硬盘组管理参数
    void on_btnHDGroupSave_clicked();

    //comboxBox选择
    void comboHDGroupChanged(int iHDGroupIndex);
    //点击选择所有通道
    void on_checkAllChan_clicked();

private:
    Ui::CHardDiskParamsClass ui;
    long m_lUserID;
    KIT_HD_WIDGET m_struHDTableWidget[MAX_DISKNUM_V30];
    //QCheckBox *m_pCheckChanAll; //HDGroup table中所有的checkbo
    QCheckBox *m_pCheckChan[MAX_CHAN_NUM];

    //窗口显示时触发
    void showEvent ( QShowEvent * event );
    //获取table中的通道信息
    int GetChanInfoToTable();
    //设置table中值
};

#endif // CHARDDISKPARAMS_H
