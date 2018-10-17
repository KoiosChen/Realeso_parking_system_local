#ifndef CALARMPARAMSDLG_H
#define CALARMPARAMSDLG_H

#include <QtGui/QDialog>
#include <QCheckBox>
#include <QMessageBox>
#include "DemoPublic.h"

namespace Ui {
    class CAlarmParamsDlg;
}

class CAlarmParamsDlg : public QDialog {
    Q_OBJECT
public:

    CAlarmParamsDlg(QWidget *parent = 0);
    ~CAlarmParamsDlg();
    int setUserID(long lUserID);

    //获取报警相关参数
   int getAlarmInfo();
   //设置报警输入相关参数
   int setAlarmInInfo();
   //设置报警输出参数
   int setAlarmoutInfo();

public slots:
	//报警输入序号改变
    void changeAlarmInIndex(int index);
    //是否报警输入处理方式
    void on_checkBoxAlarmInHandle_clicked();
    //报警输入处理方式
    void on_pushButtonAlarmInHandle_clicked();
    //PTZ
    void on_pushButtonPTZ_clicked();

    //设置报警输入参数button
    void on_pushButtonAlarmInSave_clicked();
    //设置报警输出参数button
    void on_pushButtonAlarmOutSave_clicked();

    //报警输出
    void changeAlarmOutIndex(int index);
    void changeAlarmOutDateIndex(int index);

protected:
    void changeEvent(QEvent *e);
    void showEvent(QShowEvent * event);

private:
    Ui::CAlarmParamsDlg *m_ui;
    long m_lUserID;

    BOOL m_bGetAlarmInInfo;
    BOOL m_bGetAlarmOutInfo;
    int m_iAlarmInIndex;
    int m_iAlarmOutIndex;
    BOOL m_bGetAlarmIn[MAX_ALARMIN_V30];
    BOOL m_bGetAlarmOut[MAX_ALARMOUT_V30];
    NET_DVR_IPALARMINCFG m_struIPAlarmInCfg;
    NET_DVR_IPALARMOUTCFG m_struIPAlarmOutCfg;
    NET_DVR_ALARMINCFG_V30 m_struAlarmInCfg[MAX_ALARMIN_V30];
    NET_DVR_ALARMINCFG_V30 m_struCurrAlarmInCfg; //当前报警输入参数
    NET_DVR_ALARMOUTCFG_V30 m_struAlarmOutCfg[MAX_ALARMOUT_V30];
    NET_DVR_ALARMOUTCFG_V30 m_struCurrAlarmOutCfg; //当前报警输出参数
    NET_DVR_IPPARACFG m_struIpAccessCfg;

    //根据报警输入名称，获取报警输入序号
    int getAlarmInIndexByName(char * strName);
    int getAlarmOutIndexByName(char * strName);

};

#endif // CALARMPARAMSDLG_H
