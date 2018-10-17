/************************************************************************
 *        Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *        FileName                      :   devicepan.cpp
 *        Description                   :   device pan
 *        Modification History          :   none
 *        Version                       :   V1.0
 *        Time                          :   2009-11,12
 *        Author                        :   wanggongpu
 *        Descrp                        :   
*************************************************************************/
#include "devicepan.h"
#include <QMessageBox>

/************************************************************************
 *        Function            :  DevicePan
 *        Description         :  instructor
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
DevicePan::DevicePan(int *userid, QDialog *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    m_dpuserid =userid;
}

/************************************************************************
 *        Function            :  ~DevicePan
 *        Description         :  destructor
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
DevicePan::~DevicePan()
{

}

/************************************************************************
 *        Function            :  devicePanelClickKey
 *        Description         :  none
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void DevicePan::devicePanelClickKey(int keyindex) 
{
    if (!NET_DVR_ClickKey(*m_dpuserid,keyindex))
    {
        QMessageBox::information(this,tr("NET_DVR_ClickKey Error"), 
		tr("SDK_LASTERROR=%1").arg(NET_DVR_GetLastError()));
        
    }
}

/************************************************************************
 *        Function            :  on_pushButton_panlock_clicked
 *        Description         :  panlock 
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void DevicePan::on_pushButton_panlock_clicked()
{
    if (!NET_DVR_LockPanel(*m_dpuserid))
    {
        QMessageBox::information(this,tr("NET_DVR_LockPanel Error"),\
                tr("SDK_LASTERROR=%1").arg(NET_DVR_GetLastError()));
            
    }
}

/************************************************************************
 *        Function            :  on_pushButton_panunlock_clicked
 *        Description         :  panunlock
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void DevicePan::on_pushButton_panunlock_clicked()
{
    if (!NET_DVR_UnLockPanel(*m_dpuserid))
    {
        QMessageBox::information(this,tr("NET_DVR_UnLockPanel Error"),\
                tr("SDK_LASTERROR=%1").arg(NET_DVR_GetLastError()));
            
    }
}

/************************************************************************
 *        Function            :  on_pushButton_button1_clicked
 *        Description         :   1
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void DevicePan::on_pushButton_button1_clicked()
{
    devicePanelClickKey(KEY_CODE_1);
}

/************************************************************************
 *        Function            :  on_pushButton_button2_clicked
 *        Description         :   2
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void DevicePan::on_pushButton_button2_clicked()
{
    devicePanelClickKey(KEY_CODE_2);
}

/************************************************************************
 *        Function            :  on_pushButton_button3_clicked
 *        Description         :  3
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void DevicePan::on_pushButton_button3_clicked()
{
    devicePanelClickKey(KEY_CODE_3);
}

/************************************************************************
 *        Function            :  on_pushButton_button4_clicked
 *        Description         :   4
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void DevicePan::on_pushButton_button4_clicked()
{
    devicePanelClickKey(KEY_CODE_4);
}

/************************************************************************
 *        Function            :  on_pushButton_button5_clicked
 *        Description         :   5
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void DevicePan::on_pushButton_button5_clicked()
{
    devicePanelClickKey(KEY_CODE_5);
}

/************************************************************************
 *        Function            :  on_pushButton_button6_clicked
 *        Description         :   6
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void DevicePan::on_pushButton_button6_clicked()
{
    devicePanelClickKey(KEY_CODE_6);
}

/************************************************************************
 *        Function            :  on_pushButton_button7_clicked
 *        Description         :   7
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void DevicePan::on_pushButton_button7_clicked()
{
    devicePanelClickKey(KEY_CODE_7);
}

/************************************************************************
 *        Function            :  on_pushButton_button8_clicked
 *        Description         :  8
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void DevicePan::on_pushButton_button8_clicked()
{
    devicePanelClickKey(KEY_CODE_8);
}

/************************************************************************
 *        Function            :  on_pushButton_button9_clicked
 *        Description         :  9 
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void DevicePan::on_pushButton_button9_clicked()
{
    devicePanelClickKey(KEY_CODE_9);
}

/************************************************************************
 *        Function            :  on_pushButton_button10_clicked
 *        Description         :   10
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void DevicePan::on_pushButton_button10_clicked()
{
    devicePanelClickKey(KEY_CODE_0);
}

/************************************************************************
 *        Function            :  on_pushButton_button11_clicked
 *        Description         :  11
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void DevicePan::on_pushButton_button11_clicked()
{
    devicePanelClickKey(KEY_CODE_11);
}

/************************************************************************
 *        Function            :  on_pushButton_button12_clicked
 *        Description         :   12
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void DevicePan::on_pushButton_button12_clicked()
{
    devicePanelClickKey(KEY_CODE_12);
}

/************************************************************************
 *        Function            :  on_pushButton_button13_clicked
 *        Description         :  13 
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void DevicePan::on_pushButton_button13_clicked()
{
    devicePanelClickKey(KEY_CODE_13);
}

/************************************************************************
 *        Function            :  on_pushButton_button14_clicked
 *        Description         :  14 
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void DevicePan::on_pushButton_button14_clicked()
{
    devicePanelClickKey(KEY_CODE_14);
}

/************************************************************************
 *        Function            :  on_pushButton_button15_clicked
 *        Description         :  15 
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void DevicePan::on_pushButton_button15_clicked()
{
    devicePanelClickKey(KEY_CODE_15);
}

/************************************************************************
 *        Function            :  on_pushButton_button16_clicked
 *        Description         :  16 
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void DevicePan::on_pushButton_button16_clicked()
{
    devicePanelClickKey(KEY_CODE_16);
}

/************************************************************************
 *        Function            :  on_pushButton_mainmenu_clicked
 *        Description         :  mainmenu
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void DevicePan::on_pushButton_mainmenu_clicked()
{
    devicePanelClickKey(KEY_CODE_MENU);
}

/************************************************************************
 *        Function            :  on_pushButton_edit_clicked
 *        Description         :  edit 
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void DevicePan::on_pushButton_edit_clicked()
{
    devicePanelClickKey(KEY_CODE_EDIT);
}

/************************************************************************
 *        Function            :  on_pushButton_play_clicked
 *        Description         :  play 
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void DevicePan::on_pushButton_play_clicked()
{
    devicePanelClickKey(KEY_CODE_PLAY);
}

/************************************************************************
 *        Function            :  on_pushButton_save_clicked
 *        Description         :  save 
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void DevicePan::on_pushButton_save_clicked()
{
    devicePanelClickKey(KEY_CODE_REC);
}

/************************************************************************
 *        Function            :  on_pushButton_multiwindow_clicked
 *        Description         :  multiwindow 
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void DevicePan::on_pushButton_multiwindow_clicked()
{
    devicePanelClickKey(KEY_CODE_M);
}

/************************************************************************
 *        Function            :  on_pushButton_input_clicked
 *        Description         :  input
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void DevicePan::on_pushButton_input_clicked()
{
    devicePanelClickKey(KEY_CODE_A);
}

/************************************************************************
 *        Function            :  on_pushButton_enter_clicked
 *        Description         :  enter 
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void DevicePan::on_pushButton_enter_clicked()
{
    devicePanelClickKey(KEY_CODE_ENTER);
}

/************************************************************************
 *        Function            :  on_pushButton_esc_clicked
 *        Description         :  esc 
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void DevicePan::on_pushButton_esc_clicked()
{
    devicePanelClickKey(KEY_CODE_CANCEL);
}

/************************************************************************
 *        Function            :  on_pushButton_systeminfo_clicked
 *        Description         :  systeminfo 
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void DevicePan::on_pushButton_systeminfo_clicked()
{
    devicePanelClickKey(KEY_CODE_F2);
}

/************************************************************************
 *        Function            :  on_pushButton_intercom_clicked
 *        Description         :  intercom 
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void DevicePan::on_pushButton_intercom_clicked()
{
    devicePanelClickKey(KEY_CODE_F1);
}

/************************************************************************
 *        Function            :  on_pushButton_up_clicked
 *        Description         :  up 
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void DevicePan::on_pushButton_up_clicked()
{
    devicePanelClickKey(KEY_CODE_UP);
}

/************************************************************************
 *        Function            :  on_pushButton_down_clicked
 *        Description         :  down 
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void DevicePan::on_pushButton_down_clicked()
{
    devicePanelClickKey(KEY_CODE_DOWN);
}

/************************************************************************
 *        Function            :  on_pushButton_left_clicked
 *        Description         :  left
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void DevicePan::on_pushButton_left_clicked()
{
    devicePanelClickKey(KEY_CODE_LEFT);
}

/************************************************************************
 *        Function            :  on_pushButton_right_clicked
 *        Description         :  right 
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void DevicePan::on_pushButton_right_clicked()
{
    devicePanelClickKey(KEY_CODE_RIGHT);
}

/************************************************************************
 *        Function            :  on_pushButton_ptzcontrol_clicked
 *        Description         :   
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void DevicePan::on_pushButton_ptzcontrol_clicked()
{
    
}

/************************************************************************
 *        Function            :  on_pushButton_ptzstart_clicked
 *        Description         :  ptz
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void DevicePan::on_pushButton_ptzstart_clicked()
{
    if (ui.radioButton_aperchadd->isChecked()) 
    {
        m_dpptzflags = KEY_PTZ_AP1_START;
    } 
    else if (ui.radioButton_aperturesub->isChecked()) 
    {
        m_dpptzflags = KEY_PTZ_AP2_START;
    } 
    else if (ui.radioButton_focusadd->isChecked()) 
    {
        m_dpptzflags = KEY_PTZ_FOCUS1_START;
    } 
    else if (ui.radioButton_focussub->isChecked()) 
    {
        m_dpptzflags = KEY_PTZ_FOCUS2_START;
    } 
    else if (ui.radioButton_zoomin->isChecked()) 
    {
        m_dpptzflags = KEY_PTZ_B1_START;
    } 
    else if (ui.radioButton_zoomout->isChecked()) 
    {
        m_dpptzflags = KEY_PTZ_B2_START;
    } 
    else if (ui.radioButton_up->isChecked()) 
    {
        m_dpptzflags = KEY_PTZ_UP_START;
    } 
    else if (ui.radioButton_down->isChecked()) 
    {
        m_dpptzflags = KEY_PTZ_DOWN_START;
    }
    else if (ui.radioButton_left->isChecked())
    {
        m_dpptzflags = KEY_PTZ_LEFT_START;
    } 
    else if (ui.radioButton_right->isChecked()) 
    {
        m_dpptzflags = KEY_PTZ_RIGHT_START;
    }
    devicePanelClickKey(m_dpptzflags);
}

/************************************************************************
 *        Function            :  on_pushButton_ptzstop_clicked
 *        Description         :  ptz
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void DevicePan::on_pushButton_ptzstop_clicked()
{
    if (ui.radioButton_aperchadd->isChecked()) 
    {
        m_dpptzflags = KEY_PTZ_AP1_STOP;
    } 
    else if (ui.radioButton_aperturesub->isChecked()) 
    {
        m_dpptzflags = KEY_PTZ_AP2_STOP;
    } 
    else if (ui.radioButton_focusadd->isChecked()) 
    {
        m_dpptzflags = KEY_PTZ_AP1_STOP;
    } 
    else if (ui.radioButton_focussub->isChecked()) 
    {
        m_dpptzflags = KEY_PTZ_AP2_STOP;
    } 
    else if (ui.radioButton_zoomin->isChecked()) 
    {
        m_dpptzflags = KEY_PTZ_AP1_STOP;
    } 
    else if (ui.radioButton_zoomout->isChecked()) 
    {
        m_dpptzflags = KEY_PTZ_AP2_STOP;
    } 
    else if (ui.radioButton_up->isChecked()) 
    {
        m_dpptzflags = KEY_PTZ_UP_STOP;
    } 
    else if (ui.radioButton_down->isChecked()) 
    {
        m_dpptzflags = KEY_PTZ_DOWN_STOP;
    }
    else if (ui.radioButton_left->isChecked()) 
    {
        m_dpptzflags = KEY_PTZ_LEFT_STOP;
    } 
    else if (ui.radioButton_right->isChecked()) 
    {
        m_dpptzflags = KEY_PTZ_RIGHT_STOP;
    }
    devicePanelClickKey(m_dpptzflags);
}


/************************************************************************
 *        Function            :  on_pushButton_exit_clicked
 *        Description         :  exit
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void DevicePan::on_pushButton_exit_clicked()
{
    close();
}

