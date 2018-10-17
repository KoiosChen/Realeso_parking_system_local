#ifndef CADVANCEDNETPARAMS_H
#define CADVANCEDNETPARAMS_H

#include <QtGui/QDialog>
#include<QStandardItemModel>
#include "ui_cadvancednetparams.h"

namespace Ui
{
    class CAdvancedNetParams;
}

class CAdvancedNetParams : public QDialog
{
    Q_OBJECT

public:
    CAdvancedNetParams(QWidget *parent = 0);
    ~CAdvancedNetParams();
    void showEvent ( QShowEvent * event );

    //set the uerID and channel
    int setUserIDAndChannel(long lUserID, long lChannel);

    //PPoE params.
    int getPPPoEInfo();
    int setPPPoEInfo();

    //NTP
    int getNTPInfo();
    int setNTPInfo();

     //DDNS
    int getDDNSInfo();
    int setDDNSInfo();

    //Email
    int getEmailInfo();
    int setEmailInfo();

 public slots:
     void clickPPPoE();
     void clickNTP();
     void clickDDNS();
     void saveParams();
     void refreshParams();
     void on_btnExit_clicked();

     //void updateTable(int row, int column );

private:
    Ui::CAdvancedNetParamsClass *m_ui;
    long m_lUserID;
    long m_lChannel;
};

#endif // CADVANCEDNETPARAMS_H
