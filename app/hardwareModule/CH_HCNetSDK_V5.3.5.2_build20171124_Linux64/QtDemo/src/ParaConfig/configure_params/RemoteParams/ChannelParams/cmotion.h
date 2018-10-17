#ifndef CMOTION_H
#define CMOTION_H

#include <QtGui/QDialog>
#include <QtGui/QCheckBox>
#include <QtGui/QPushButton>
#include <QtGui/QLineEdit>
#include "ui_cmotion.h"
#include "DemoPublic.h"

#define KIT_MOTION_WIDTH 22  //移动侦测有22*18个小宏块
#define KIT_MOTION_HEIGHT 18

class CMotion : public QDialog
{
    Q_OBJECT

public:
    CMotion(QWidget *parent = 0);
    ~CMotion();

    int setAlarmOutNum(int iAlarmOutNum);
    //设置可用通道ID
    int setChannelID(int *piChannelID);
    //设置移动侦测报警参数
    int setStruMotion(NET_DVR_MOTION_V30 *pstruMotion);
    //设置起始通道号
    int setStartChannelNo(int iStartChannelNo);
    //设置用户ID
    int setUserID(long lUserID);

    //event
    void showEvent ( QShowEvent * event );

public slots:
     //星期被改变
     void comboWeekDayChanged(int iWeekDay);
     //是否触发报警输出
     void checkAlarmOut();
     //保存参数
     void on_btnSave_clicked();

private:
    Ui::CMotionClass ui;
    long m_lUserID;   //登陆时获取的ID值
    //移动侦测区域
    QCheckBox m_checkMotionScope[KIT_MOTION_HEIGHT][KIT_MOTION_WIDTH];
    QCheckBox *m_pcheckAlarmOut[MAX_ALARMOUT_V30]; //触发的报警输出
    //触发的录像通道, 数组编号代表：通道ID - 起始通道号
    QCheckBox m_checkChannel[MAX_CHANNUM_V30];
    QLineEdit m_editStartHour[MAX_TIMESEGMENT_V30]; //布防开始时间(小时)
    QLineEdit m_editStartMin[MAX_TIMESEGMENT_V30];   //布防开始时间(分钟)
    QLineEdit m_editEndHour[MAX_TIMESEGMENT_V30];  //布防结束时间(小时)
    QLineEdit m_editEndMin[MAX_TIMESEGMENT_V30];    //布防结束时间(分钟)

    //移动侦测参数指针
    NET_DVR_MOTION_V30 *m_pstruMotion;
    int *m_iArryChannel;                                       //可用通道ID数组
    int m_iAlarmOutNum;                                      //报警输出个数
    int m_iStartChannelNo;                                    //起始通道号

    //根据可用ID号,创建触发录像通道；
    int createChannelCheck();
    //创建报警输出check
    int createAlarmOutCheck();
    int getAlarmOutInfo(int iAlarmOut, QCheckBox *pQCheckAlarmOut);

    int setEachWidgetValue(); //设置界面上所有控件中的值
    int getEachWidgetValue(); //获取界面上所有控件中的值

    int setMotionAlarmTypeWidget();//设置报警处理方式
    int getMotionAlarmTypeWidget();//获取报警处理方式
};

#endif // CMOTION_H
