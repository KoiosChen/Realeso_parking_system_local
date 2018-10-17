#include "exceptionparams.h"
#include "ui_exceptionparams.h"
#include <stdio.h>

ExceptionParams::ExceptionParams(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::ExceptionParams)
{
    m_ui->setupUi(this);

     for(int i = 0; i < MAX_ALARMOUT_V30; i++ ) //NULL
     {
           checkboxAlarmOut[i] = NULL;
     }

    connect(m_ui->btnrefresh, SIGNAL(clicked()), this, SLOT(RefreshParams()));
    connect(m_ui->btnsave, SIGNAL(clicked()), this, SLOT(SaveParams()));
    connect(m_ui->comboBoxExceptionType, SIGNAL(currentIndexChanged(int )), this, SLOT(changeType(int )));
    connect(m_ui->checkBoxInvokeAlarmOut, SIGNAL(stateChanged(int )), this, SLOT(changeState(int )));

    QStringList strings;
    strings <<"HDD full"<<"HDD error"<<"Network error"<<"IP conflict"<<"Illegal access"<<"Input/Output video standard not match"<<"Video signal abnormality";
    m_ui->comboBoxExceptionType->addItems(strings);
    m_ui->comboBoxExceptionType->setCurrentIndex(0);
}

ExceptionParams::~ExceptionParams()
{
    delete m_ui;
}

void ExceptionParams::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void ExceptionParams::RefreshParams()
{
    DWORD dwReturned;
    int i;
    memset(&m_struIPAlarmOutCfg, 0, sizeof(m_struIPAlarmOutCfg));
    memset(&m_struDeviceInfo,0,sizeof(m_struDeviceInfo));
    NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_DEVICECFG, 0, &m_struDeviceInfo, sizeof(m_struDeviceInfo), &dwReturned);
    dwAlarmOutNum = m_struDeviceInfo.byAlarmOutPortNum;
    memset(&m_struExceptionInfo,0,sizeof(m_struExceptionInfo));

    NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_IPALARMOUTCFG, 0, &m_struIPAlarmOutCfg, sizeof(NET_DVR_IPALARMOUTCFG), &dwReturned);
   if(!NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_EXCEPTIONCFG_V30, 0, &m_struExceptionInfo, sizeof(NET_DVR_EXCEPTION_V30), &dwReturned))
    {
       //QMessageBox::information(this, tr("Error"),tr("NET_DVR_GET_EXCEPTIONCFG_V30 failed"));
       return;
    }
   else
   {
       m_iExceptionType = m_ui->comboBoxExceptionType->currentIndex();
       if(m_struExceptionInfo.struExceptionHandleType[m_iExceptionType].dwHandleType &0x01 == 1)
       {
           m_ui->checkBoxMontiorAlarm->setCheckState(Qt::Checked);
       }
       else
       {
            m_ui->checkBoxMontiorAlarm->setCheckState(Qt::Unchecked);
       }
       if((m_struExceptionInfo.struExceptionHandleType[m_iExceptionType].dwHandleType >> 1) &0x01 == 1)
       {
           m_ui->checkBoxSoundAlarm->setCheckState(Qt::Checked);
       }
       else
       {
           m_ui->checkBoxSoundAlarm->setCheckState(Qt::Unchecked);
       }
       if((m_struExceptionInfo.struExceptionHandleType[m_iExceptionType].dwHandleType >> 2) &0x01 == 1)
       {
           m_ui->checkBoxCenterAlarm->setCheckState(Qt::Checked);
       }
       else
       {
           m_ui->checkBoxCenterAlarm->setCheckState(Qt::Unchecked);
       }
       if((m_struExceptionInfo.struExceptionHandleType[m_iExceptionType].dwHandleType >> 3) &0x01 == 1)
       {
           m_ui->checkBoxInvokeAlarmOut->setCheckState(Qt::Checked);
           char strTemp[100];
            for(i = 0; i < MAX_ALARMOUT_V30; i++ ) //NULL
            {
                if(checkboxAlarmOut[i] != NULL)
                {
                    delete checkboxAlarmOut[i];
                }
                checkboxAlarmOut[i] = NULL;

                if (i<(int)dwAlarmOutNum || (i>=MAX_ANALOG_ALARMIN && m_struIPAlarmOutCfg.struIPAlarmOutInfo[i-MAX_ANALOG_ALARMOUT].byIPID > 0))
                {
                     if (i<(int)dwAlarmOutNum)
                     {
                        sprintf(strTemp,"AlarmOut%d",(i+1));
                     }
                     else if (i>=MAX_ANALOG_ALARMOUT)
                     {
                        sprintf(strTemp,"IPAlarmOut%d",(i+1-MAX_ANALOG_ALARMOUT));
                     }
                     checkboxAlarmOut[i] = new QCheckBox(strTemp);
                     m_ui->verticalLayoutAlarmOut->addWidget( checkboxAlarmOut[i]);
                     if(m_struExceptionInfo.struExceptionHandleType[m_iExceptionType].byRelAlarmOut[i] == 1)
                     {
                        checkboxAlarmOut[i]->setCheckState(Qt::Checked);
                     }
                }
            }
       }
       else
       {
           m_ui->checkBoxInvokeAlarmOut->setCheckState(Qt::Unchecked);
       }
    }
}
int ExceptionParams::setUserID(long lUserID)
 {
     if(lUserID >= 0)
     {
         m_lUserID = lUserID;
         return 1;
     }
     else
     {
         return 0;
     }
 }

void ExceptionParams::changeType(int index)
{
      RefreshParams();
}

void ExceptionParams::SaveParams()
{
    m_iExceptionType = m_ui->comboBoxExceptionType->currentIndex();

    m_struExceptionInfo.struExceptionHandleType[m_iExceptionType].dwHandleType = 0;
    m_struExceptionInfo.struExceptionHandleType[m_iExceptionType].dwHandleType |= (m_ui->checkBoxMontiorAlarm->isChecked())<<0;
    m_struExceptionInfo.struExceptionHandleType[m_iExceptionType].dwHandleType |= (m_ui->checkBoxSoundAlarm->isChecked())<<1;
    m_struExceptionInfo.struExceptionHandleType[m_iExceptionType].dwHandleType |= (m_ui->checkBoxCenterAlarm->isChecked())<<2;
    m_struExceptionInfo.struExceptionHandleType[m_iExceptionType].dwHandleType |= (m_ui->checkBoxInvokeAlarmOut->isChecked())<<3;

    for(int i=0; i < MAX_ALARMOUT_V30; i++)
    {
        if (i<(int)dwAlarmOutNum || (i>=MAX_ANALOG_ALARMIN && m_struIPAlarmOutCfg.struIPAlarmOutInfo[i-MAX_ANALOG_ALARMOUT].byIPID > 0))
        {
            m_struExceptionInfo.struExceptionHandleType[m_iExceptionType].byRelAlarmOut[i] = checkboxAlarmOut[i]->isChecked();
        }
    }

    if(NET_DVR_SetDVRConfig(m_lUserID, NET_DVR_SET_EXCEPTIONCFG_V30, 0, &m_struExceptionInfo, sizeof(NET_DVR_EXCEPTION_V30)))
    {
        QMessageBox::information(this, tr("Succ"),tr("NET_DVR_SET_EXCEPTIONCFG_V30 succ"));
    }
}

void ExceptionParams::changeState(int state)
{
    if(state == 0)
    {
        for(int i=0; i < MAX_ALARMOUT_V30; i++)
        {
            if(checkboxAlarmOut[i] != NULL)
            {
                checkboxAlarmOut[i]->setEnabled(FALSE);
            }
        }
    }
    else if(state == 2)
    {
       char strTemp[100];
       for(int i = 0; i < MAX_ALARMOUT_V30; i++ ) //NULL
       {
           if(checkboxAlarmOut[i] != NULL)
           {
               delete checkboxAlarmOut[i];
           }
           checkboxAlarmOut[i] = NULL;

           if (i<(int)dwAlarmOutNum || (i>=MAX_ANALOG_ALARMIN && m_struIPAlarmOutCfg.struIPAlarmOutInfo[i-MAX_ANALOG_ALARMOUT].byIPID > 0))
           {
                if (i<(int)dwAlarmOutNum)
                {
                    sprintf(strTemp,"AlarmOut%d",(i+1));
                }
                else if (i>=MAX_ANALOG_ALARMOUT)
                {
                    sprintf(strTemp,"IPAlarmOut%d",(i+1-MAX_ANALOG_ALARMOUT));
                }
                checkboxAlarmOut[i] = new QCheckBox(strTemp);
                m_ui->verticalLayoutAlarmOut->addWidget( checkboxAlarmOut[i]);
                if(m_struExceptionInfo.struExceptionHandleType[m_iExceptionType].byRelAlarmOut[i] == 1)
                {
                    checkboxAlarmOut[i]->setCheckState(Qt::Checked);
                }
           }
       }
    }
}
