#ifndef CZONEDST_H
#define CZONEDST_H

#include <QtGui/QDialog>
#include "ui_czonedst.h"

class CZoneDst : public QDialog
{
    Q_OBJECT

public:
    CZoneDst(QWidget *parent = 0);
    ~CZoneDst();

    //user ID and channel no
    int setUserIDAndChannel(long lUserID, long lChannel);

    //get zone information
    int getZoneDstInfo();
    int setZoneDsfInfo();

    //event
    void showEvent ( QShowEvent * event );

 public slots:
    void clickSave();
    void clickRefresh();
    void on_btnExit_clicked();

private:
    Ui::CZoneDstClass ui;
    long m_lUserID;
    long m_lChannel;
};

#endif // CZONEDST_H
