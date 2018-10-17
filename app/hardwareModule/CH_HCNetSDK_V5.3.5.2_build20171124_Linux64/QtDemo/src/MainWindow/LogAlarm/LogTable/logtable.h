#ifndef LOGTABLE_H
#define LOGTABLE_H

#include <QtGui/QWidget>
#include "ui_logtable.h"
#include <QString>
#include <QMessageBox>

#ifdef _WIN32
#elif defined(__linux__) || defined(__APPLE__)
#define __stdcall 
#endif

class LogTable : public QWidget
{
    Q_OBJECT

public:
    void addLogAlarmInfo(int succ, const QString &interfacename, const QString &devinfo, const QString &errorinfo);
	friend class QtClientDemo;
	friend class RealPlay;
	friend class PlayBack;
	//friend unsigned int __stdcall MessCallBack(int lCommand,  char *sDVRIP, char *pBuf, unsigned int dwBufLen);
    LogTable(QWidget *parent = 0);
    ~LogTable();
	
    //void addLogAlarmInfo(int succ, QString interface, QString devinfo, QString errorinfo);

private:
    Ui::LogTableClass ui;
};

#endif // LOGTABLE_H
