#include "cserialparams.h"
#include "ui_cserialparams.h"
#include "publicfuc.h"

CSerialParams::CSerialParams(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::CSerialParams)
{
    m_ui->setupUi(this);
    m_bChanged = FALSE;
    m_iCurrentIndex = 0;

    connect(m_ui->pushButton232refresh, SIGNAL(clicked()), this, SLOT(refreshparams()));
    connect(m_ui->comboBoxChanIndex, SIGNAL(currentIndexChanged(int )), this, SLOT(changeChanIndex(int )));
    connect(m_ui->pushButtonsave, SIGNAL(clicked()), this, SLOT(saveparams()));
    connect(m_ui->pushButtonPPPSETUP, SIGNAL(clicked()), this, SLOT(PppSetup()));
    connect(m_ui->comboBox232workstate, SIGNAL(currentIndexChanged(int )), this, SLOT(change232WorkState(int )));
}

CSerialParams::~CSerialParams()
{
    delete m_ui;
}

void CSerialParams::changeEvent(QEvent *e)
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

int CSerialParams::setUserID(long lUserID)
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

void CSerialParams::refreshparams()
{
    //有注释//
    /*
    m_bChanged = TRUE;
    DWORD dwReturned = -1;
    unsigned int iIPChanIndex = 0;
    memset(&m_stru232Info, 0, sizeof(NET_DVR_RS232CFG_V30));
    if(NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_RS232CFG_V30, 0, &m_stru232Info, sizeof(NET_DVR_RS232CFG_V30), &dwReturned))
    {
        m_ui->comboBox232BAUD->setCurrentIndex(m_stru232Info.struRs232.dwBaudRate);
        m_ui->comboBox232CHECKBIT->setCurrentIndex(m_stru232Info.struRs232.byParity);
        m_ui->comboBox232DATABIT->setCurrentIndex(m_stru232Info.struRs232.byDataBit);
        m_ui->comboBox232STOPBIT->setCurrentIndex(m_stru232Info.struRs232.byStopBit);
        m_ui->comboBox232FLOW->setCurrentIndex(m_stru232Info.struRs232.byFlowcontrol);
        m_ui->comboBox232workstate->setCurrentIndex(m_stru232Info.struRs232.dwWorkMode);
        if(m_stru232Info.struRs232.dwWorkMode == 0)
        {
            m_ui->pushButtonPPPSETUP->setEnabled(TRUE);
        }
        else
        {
            m_ui->pushButtonPPPSETUP->setEnabled(FALSE);
        }
    }

    int chanNum = 0;
    int startChanIndex = 0;
    int bIPRet = -1;
    int showchan = 0;
    char strTemp[10] = {0};
    int j = 0;
    NET_DVR_DEVICECFG m_struDeviceInfo;
    memset(&m_struDeviceInfo ,0 ,sizeof(NET_DVR_DEVICECFG));
    if(NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_DEVICECFG, 0, &m_struDeviceInfo, sizeof(NET_DVR_DEVICECFG), &dwReturned))
    {
        chanNum = m_struDeviceInfo.byChanNum;
        startChanIndex = m_struDeviceInfo.byStartChan;
        NET_DVR_IPPARACFG m_struIPAccessCfg = {0};
        bIPRet = NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_IPPARACFG, 0, &m_struIPAccessCfg, sizeof(NET_DVR_IPPARACFG), &dwReturned);
        m_ui->comboBoxChanIndex->clear();
        if (!bIPRet)
        {	///device no support ip access
            m_iCurrentIndex = 0;
            for (int i = 0; i < MAX_CHANNUM_V30; i++)
            {
                if (i < chanNum)
                {
                   showchan = i + startChanIndex;
                   sprintf(strTemp,"通道%d",showchan);
                   m_ui->comboBoxChanIndex->addItem(strTemp);
                   m_ui->comboBoxChanIndex->setItemData(j, i);
                   j++;

                   if(!NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_DECODERCFG_V30, showchan, &m_stru485DecoderCfg[i], sizeof(NET_DVR_DECODERCFG_V30), &dwReturned))
                   {
                       QMessageBox::information(this, tr("Error"),tr("NET_DVR_GET_DECODERCFG_V30 failed"));
                   }
                }

            }
        }
        else
        {
           for(int i = 0; i < MAX_CHANNUM_V30; i++)
           {
              if((i < chanNum) && (m_struIPAccessCfg.byAnalogChanEnable[i] == 1))
              {
                showchan = i + startChanIndex;
                sprintf(strTemp,"通道%d",showchan);
                m_ui->comboBoxChanIndex->addItem(strTemp);
                m_ui->comboBoxChanIndex->setItemData(j, i);
                j++;
                if(!NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_DECODERCFG_V30, showchan, &m_stru485DecoderCfg[i], sizeof(NET_DVR_DECODERCFG_V30), &dwReturned))
                {
                   QMessageBox::information(this, tr("Error"),tr("NET_DVR_GET_DECODERCFG_V30 failed"));
                }
              }
              else if(i>=MAX_ANALOG_CHANNUM && i<=MAX_CHANNUM_V30)
              {
                  iIPChanIndex = i - MAX_ANALOG_CHANNUM;
                  if(m_struIPAccessCfg.struIPChanInfo[iIPChanIndex].byIPID != 0)
                  {
                      showchan = i + startChanIndex;
                      sprintf(strTemp,"数字通道%d",(iIPChanIndex+1));
                      m_ui->comboBoxChanIndex->addItem(strTemp);
                      m_ui->comboBoxChanIndex->setItemData(j, i);
                      j++;
                      if(!NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_DECODERCFG_V30, showchan, &m_stru485DecoderCfg[i], sizeof(NET_DVR_DECODERCFG_V30), &dwReturned))
                      {
                         QMessageBox::information(this, tr("Error"),tr("NET_DVR_GET_DECODERCFG_V30 failed"));
                      }
                  }
              }
           }

        }
        m_ui->comboBox485BAUD->setCurrentIndex(m_stru485DecoderCfg[m_iCurrentIndex].dwBaudRate);
        m_ui->comboBox485CHECKBIT->setCurrentIndex(m_stru485DecoderCfg[m_iCurrentIndex].byParity);
        m_ui->comboBox485DATABIT->setCurrentIndex(m_stru485DecoderCfg[m_iCurrentIndex].byDataBit);
        m_ui->comboBox485FLOW->setCurrentIndex(m_stru485DecoderCfg[m_iCurrentIndex].byFlowcontrol);
        m_ui->comboBox485STOPBIT->setCurrentIndex(m_stru485DecoderCfg[m_iCurrentIndex].byStopBit);
        m_ui->comboBoxDECODETYPE->setCurrentIndex(m_stru485DecoderCfg[m_iCurrentIndex].wDecoderType);
        KIT_intToEdit(m_ui->lineEditDECODEADDRESS,(int)(m_stru485DecoderCfg[m_iCurrentIndex].wDecoderAddress));
    }

    */
}

void CSerialParams::changeChanIndex(int index)
{
   if(m_bChanged)
   {
       m_bChanged = FALSE;
       return;
   }
   int iSel;
   iSel = m_ui->comboBoxChanIndex->currentIndex();
   m_iCurrentIndex = m_ui->comboBoxChanIndex->itemData(iSel).toInt();
   m_ui->comboBox485BAUD->setCurrentIndex(m_stru485DecoderCfg[m_iCurrentIndex].dwBaudRate);
   m_ui->comboBox485CHECKBIT->setCurrentIndex(m_stru485DecoderCfg[m_iCurrentIndex].byParity);
   m_ui->comboBox485DATABIT->setCurrentIndex(m_stru485DecoderCfg[m_iCurrentIndex].byDataBit);
   m_ui->comboBox485FLOW->setCurrentIndex(m_stru485DecoderCfg[m_iCurrentIndex].byFlowcontrol);
   m_ui->comboBox485STOPBIT->setCurrentIndex(m_stru485DecoderCfg[m_iCurrentIndex].byStopBit);
   m_ui->comboBoxDECODETYPE->setCurrentIndex(m_stru485DecoderCfg[m_iCurrentIndex].wDecoderType);
   KIT_intToEdit(m_ui->lineEditDECODEADDRESS,(int)(m_stru485DecoderCfg[m_iCurrentIndex].wDecoderAddress));
}

void CSerialParams::saveparams()
{
    /*
    m_stru232Info.struRs232.dwBaudRate = m_ui->comboBox232BAUD->currentIndex();
    m_stru232Info.struRs232.byParity = m_ui->comboBox232CHECKBIT->currentIndex();
    m_stru232Info.struRs232.byDataBit = m_ui->comboBox232DATABIT->currentIndex();
    m_stru232Info.struRs232.byStopBit = m_ui->comboBox232STOPBIT->currentIndex();
    m_stru232Info.struRs232.byFlowcontrol = m_ui->comboBox232FLOW->currentIndex();
    m_stru232Info.struRs232.dwWorkMode = m_ui->comboBox232workstate->currentIndex();

    if(!NET_DVR_SetDVRConfig(m_lUserID, NET_DVR_SET_RS232CFG_V30, 0, &m_stru232Info, sizeof(NET_DVR_RS232CFG_V30)))
    {
        QMessageBox::information(this, tr("Error"),tr("NET_DVR_SET_RS232CFG_V30 failed"));
        return;
    }

    int iSel;
    iSel = m_ui->comboBoxChanIndex->currentIndex();
    m_iCurrentIndex = m_ui->comboBoxChanIndex->itemData(iSel).toInt();
    m_stru485DecoderCfg[m_iCurrentIndex].dwBaudRate = m_ui->comboBox485BAUD->currentIndex();
    m_stru485DecoderCfg[m_iCurrentIndex].byParity = m_ui->comboBox485CHECKBIT->currentIndex();
    m_stru485DecoderCfg[m_iCurrentIndex].byDataBit = m_ui->comboBox485DATABIT->currentIndex();
    m_stru485DecoderCfg[m_iCurrentIndex].byFlowcontrol = m_ui->comboBox485FLOW->currentIndex();
    m_stru485DecoderCfg[m_iCurrentIndex].byStopBit = m_ui->comboBox485STOPBIT->currentIndex();
    m_stru485DecoderCfg[m_iCurrentIndex].wDecoderType = m_ui->comboBoxDECODETYPE->currentIndex();
    KIT_editToInt(m_ui->lineEditDECODEADDRESS, (int *)(&(m_stru485DecoderCfg[m_iCurrentIndex].wDecoderAddress)));
    if(!NET_DVR_SetDVRConfig(m_lUserID, NET_DVR_SET_DECODERCFG_V30, (m_iCurrentIndex + 1), &(m_stru485DecoderCfg[m_iCurrentIndex]), sizeof(NET_DVR_DECODERCFG_V30)))
    {
        QMessageBox::information(this, tr("Error"),tr("NET_DVR_SET_DECODERCFG_V30 failed"));
        return;
    }
    QMessageBox::information(this, tr("Succ"),tr("Save parameters succ"));

    */
}

void CSerialParams::PppSetup()
{
    m_PppParams = new(std::nothrow) CPPPCfgDlg;
    m_PppParams->m_pStruPPPInfo = &(m_stru232Info.struPPPConfig);
    m_PppParams->show();
}

void CSerialParams::change232WorkState(int index)
{
    if(index == 0)
    {
        m_ui->pushButtonPPPSETUP->setEnabled(TRUE);
    }
    else
    {
        m_ui->pushButtonPPPSETUP->setEnabled(FALSE);
    }
}

void CSerialParams::showEvent ( QShowEvent * event )
{
	refreshparams();
}




