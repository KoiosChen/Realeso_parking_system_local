#include "cpppcfgdlg.h"
#include "ui_cpppcfgdlg.h"
#include <stdio.h>

CPPPCfgDlg::CPPPCfgDlg(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::CPPPCfgDlg)
{
    m_ui->setupUi(this);
    connect(m_ui->pushButtonsave, SIGNAL(clicked()), this, SLOT(btnSave()));
    connect(m_ui->pushButtoncancel, SIGNAL(clicked()), this, SLOT(btncancel()));

}

CPPPCfgDlg::~CPPPCfgDlg()
{
    delete m_ui;
}

void CPPPCfgDlg::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void CPPPCfgDlg::showEvent (QShowEvent * event)
{
    m_ui->comboBoxpppType->setCurrentIndex(m_pStruPPPInfo->byPPPMode);
    m_ui->comboBoxREDIALMODE->setCurrentIndex(m_pStruPPPInfo->byRedialMode);
    if(m_pStruPPPInfo->byRedial == 1)
    {
        m_ui->checkBoxRedial->setCheckState(Qt::Checked);
    }
    else
    {
        m_ui->checkBoxRedial->setCheckState(Qt::Unchecked);
    }
    if(m_pStruPPPInfo->byDataEncrypt == 1)
    {
        m_ui->checkBoxDataEncrypt->setCheckState(Qt::Checked);
    }
    else
    {
        m_ui->checkBoxDataEncrypt->setCheckState(Qt::Unchecked);
    }
    char strTemp[100];
    memset(strTemp,0, 100);
    sprintf(strTemp,"%s", m_pStruPPPInfo->sUsername);
    m_ui->lineEditpppUser->setText(QString(QLatin1String(strTemp)));

    memset(strTemp,0, 100);
    sprintf(strTemp,"%s", m_pStruPPPInfo->sPassword);
    m_ui->lineEditpppPwd->setText(QString(QLatin1String(strTemp)));
    m_ui->lineEditpppVerify->setText(QString(QLatin1String(strTemp)));

    memset(strTemp,0, 100);
    sprintf(strTemp,"%s", m_pStruPPPInfo->sTelephoneNumber);
    m_ui->lineEditPhoneNum->setText(QString(QLatin1String(strTemp)));

    memset(strTemp,0, 100);
    sprintf(strTemp,"%s", m_pStruPPPInfo->struRemoteIP.sIpV4);
    m_ui->lineEditRemoteIp->setText(QString(QLatin1String(strTemp)));

    memset(strTemp,0, 100);
    sprintf(strTemp,"%s", m_pStruPPPInfo->struLocalIP.sIpV4);
    m_ui->lineEditLocalIp->setText(QString(QLatin1String(strTemp)));

    memset(strTemp,0, 100);
    sprintf(strTemp,"%s", m_pStruPPPInfo->sLocalIPMask);
    m_ui->lineEditIpMask->setText(QString(QLatin1String(strTemp)));
}

void CPPPCfgDlg::btnSave()
{
    m_pStruPPPInfo->byPPPMode = m_ui->comboBoxpppType->currentIndex();
    m_pStruPPPInfo->byRedialMode = m_ui->comboBoxREDIALMODE->currentIndex();
    m_pStruPPPInfo->byRedial = m_ui->checkBoxRedial->isChecked();
    m_pStruPPPInfo->byDataEncrypt = m_ui->checkBoxDataEncrypt->isChecked();
    memcpy(&m_pStruPPPInfo->sUsername,  m_ui->lineEditpppUser->text().toLatin1().data(), NAME_LEN);
    memcpy(&m_pStruPPPInfo->sPassword,  m_ui->lineEditpppPwd->text().toLatin1().data(), PASSWD_LEN);
    memcpy(&m_pStruPPPInfo->sTelephoneNumber,  m_ui->lineEditPhoneNum->text().toLatin1().data(), PHONENUMBER_LEN);
    memcpy(&m_pStruPPPInfo->struRemoteIP.sIpV4,  m_ui->lineEditRemoteIp->text().toLatin1().data(), 16);
    memcpy(&m_pStruPPPInfo->struLocalIP.sIpV4,  m_ui->lineEditLocalIp->text().toLatin1().data(), 16);
    memcpy(&m_pStruPPPInfo->sLocalIPMask,  m_ui->lineEditIpMask->text().toLatin1().data(), 16);
    QDialog::done(0);
}

void CPPPCfgDlg::btncancel()
{
    QDialog::done(0);
}
