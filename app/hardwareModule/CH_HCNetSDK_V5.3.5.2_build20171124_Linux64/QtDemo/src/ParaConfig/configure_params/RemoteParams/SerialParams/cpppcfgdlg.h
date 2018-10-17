#ifndef CPPPCFGDLG_H
#define CPPPCFGDLG_H

#include <QtGui/QDialog>
#include <QCheckBox>
#include <QLineEdit>
#include "DemoPublic.h"

namespace Ui {
    class CPPPCfgDlg;
}

class CPPPCfgDlg : public QDialog {
    Q_OBJECT
public:
    CPPPCfgDlg(QWidget *parent = 0);
    ~CPPPCfgDlg();
    NET_DVR_PPPCFG_V30 *m_pStruPPPInfo;

protected:
    void changeEvent(QEvent *e);
    void showEvent ( QShowEvent * event);

public slots:
    void btnSave();
    void btncancel();
private:
    Ui::CPPPCfgDlg *m_ui;
};

#endif // CPPPCFGDLG_H
