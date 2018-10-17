#ifndef EXCEPTIONPARAMS_H
#define EXCEPTIONPARAMS_H

#include <QtGui/QWidget>
#include <QCheckBox>
#include <QMessageBox>
#include "DemoPublic.h"

namespace Ui {
    class ExceptionParams;
}

class ExceptionParams : public QWidget {
    Q_OBJECT
public:
    int dwAlarmOutNum;
    NET_DVR_DEVICECFG m_struDeviceInfo;
    NET_DVR_EXCEPTION_V30 m_struExceptionInfo;
    ExceptionParams(QWidget *parent = 0);
    ~ExceptionParams();
    int setUserID(long lUserID);

public slots:
    void changeType(int index);
    void RefreshParams();
    void SaveParams();
    void changeState(int state);
protected:
    void changeEvent(QEvent *e);

private:
    Ui::ExceptionParams *m_ui;
    int	 m_iExceptionType;
    long m_lUserID;
    NET_DVR_IPALARMOUTCFG m_struIPAlarmOutCfg;
    QCheckBox *checkboxAlarmOut[MAX_ALARMOUT_V30];

};

#endif // EXCEPTIONPARAMS_H
