#include "logtable.h"
#include <QDateTime>
#include <QTableWidgetItem>
LogTable::LogTable(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);
}

LogTable::~LogTable()
{

}

void LogTable::addLogAlarmInfo(int succ, const QString &interfacename, const QString &devinfo, const QString &errorinfo)
//void LogTable::addLogAlarmInfo(int succ, QString interface, QString devinfo, QString errorinfo)
{

	QString currenttime = QDateTime::currentDateTime().toString(
				" yyyy/MM/dd hh:mm:ss");
	QTableWidgetItem *currtime = new QTableWidgetItem(currenttime);
	QTableWidgetItem *state =NULL;
	if (succ == 1)
	{
		state = new QTableWidgetItem("SUCC");
	}
	else
	{
		state = new QTableWidgetItem("FAILED");
	}
	QTableWidgetItem *func = new QTableWidgetItem(interfacename);
	QTableWidgetItem *ipinfo = new QTableWidgetItem(devinfo);
	QTableWidgetItem *error = new QTableWidgetItem(errorinfo);
	int row = ui.logtableWidget->rowCount();
	ui.logtableWidget->insertRow(row);
	ui.logtableWidget->setItem(row, 0, currtime);
	ui.logtableWidget->setItem(row, 1, state);
	ui.logtableWidget->setItem(row, 2, func);
	ui.logtableWidget->setItem(row, 3, ipinfo);
	ui.logtableWidget->setItem(row, 4, error);					
	return;
}
