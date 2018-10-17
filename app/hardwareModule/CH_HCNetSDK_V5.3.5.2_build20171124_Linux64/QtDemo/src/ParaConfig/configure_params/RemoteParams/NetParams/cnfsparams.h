#ifndef CNFSPARAMS_H
#define CNFSPARAMS_H

#include <QtGui/QDialog>
#include "ui_cnfsparams.h"
#include "DemoPublic.h"

class CNfsParams : public QDialog
{
    Q_OBJECT

public:
    CNfsParams(QWidget *parent = 0);
    ~CNfsParams();

    //set the uerID and channel
    int setUserIDAndChannel(long lUserID, long lChannel);

    //get nfs
    int getNFSInfo();
    int setNFSInfo();

   //event
    void showEvent ( QShowEvent * event );

 public slots:
     void comboChanged ( int index );
     void clickSave();
     void clickRefresh();

     void on_btnExit_clicked();

private:
    Ui::CNfsParamsClass ui;
    long m_lUserID;
    long m_lChannel;
    NET_DVR_NFSCFG m_NFSParams;
    int m_bGetInfo;
};

#endif // CNFSPARAMS_H
