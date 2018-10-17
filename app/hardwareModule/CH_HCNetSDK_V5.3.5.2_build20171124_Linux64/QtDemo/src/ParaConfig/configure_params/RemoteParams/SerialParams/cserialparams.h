#ifndef CSERIALPARAMS_H
#define CSERIALPARAMS_H

#include <QtGui/QWidget>
#include <QCheckBox>
#include <QMessageBox>
#include <QLineEdit>
#include "DemoPublic.h"
#include "cpppcfgdlg.h"

#ifndef HPR_OK
#define HPR_OK 0
#endif

#ifndef HPR_ERROR
#define HPR_ERROR 1
#endif

#ifndef INT_LEN
#define INT_LEN 10  //the length of the string of 32 bit int
#endif

namespace Ui {
    class CSerialParams;
}

class CSerialParams : public QWidget {
    Q_OBJECT
public:
    int m_iCurrentIndex;
    BOOL m_bChanged;
    NET_DVR_RS232CFG_V30 m_stru232Info;
    NET_DVR_DECODERCFG_V30 m_stru485DecoderCfg[MAX_CHANNUM_V30];
    CSerialParams(QWidget *parent = 0);
    ~CSerialParams();
    int setUserID(long lUserID);

    //event
    void showEvent ( QShowEvent * event );

protected:
    void changeEvent(QEvent *e);
public slots:
    void refreshparams();
    void changeChanIndex(int index);
    void change232WorkState(int index);
    void saveparams();
    void PppSetup();

private:
    Ui::CSerialParams *m_ui;
    long m_lUserID;
    CPPPCfgDlg *m_PppParams;
};

#endif // CSERIALPARAMS_H
