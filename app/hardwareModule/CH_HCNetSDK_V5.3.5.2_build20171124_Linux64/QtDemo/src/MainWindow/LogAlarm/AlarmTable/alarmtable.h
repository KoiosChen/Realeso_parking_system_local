#ifndef ALARMTABLE_H
#define ALARMTABLE_H

#include <QtGui/QWidget>
#include "ui_alarmtable.h"
#include "DemoPublic.h"


class AlarmTable : public QWidget
{
    Q_OBJECT

public:
	friend unsigned int __stdcall MessCallBack(int lCommand, char *sDVRIP, 
		char *pBuf, unsigned int dwBufLen);
	friend void __stdcall MessCallBack_V30(int lCommand, NET_DVR_ALARMER *pAlarmer, 
		char *pAlarmInfo, unsigned int dwBufLen, void*pUser);
	friend class QtClientDemo;
    AlarmTable(QWidget *parent = 0);
    ~AlarmTable();

public:
    Ui::AlarmTableClass ui;
};

#endif // ALARMTABLE_H
