#ifndef CRTSPPARAMS_H
#define CRTSPPARAMS_H

#include <QtGui/QDialog>
#include "ui_crtspparams.h"

class CRtspParams : public QDialog
{
    Q_OBJECT

public:
    CRtspParams(QWidget *parent = 0);
    ~CRtspParams();

    int setUserID(long lUserID);

    //get rtsp info
    int getRtspInfo();
    //set rtsp info
    int setRtspInfo();

    //event
     void showEvent ( QShowEvent * event );

public slots:
   void clickSave();
   void clickRefresh();
   void on_btnExit_clicked();

private:
    Ui::CRtspParamsClass ui;
    long m_lUserID;
};

#endif // CRTSPPARAMS_H
