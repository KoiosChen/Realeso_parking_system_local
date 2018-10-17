/************************************************************************
 *        Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *        FileName                      :   devicepan.h
 *        Description                   :   devicepan
 *        Modification History          :   none
 *        Version                       :   V1.0
 *        Time                          :   2009-11,12
 *        Author                        :   wanggongpu
 *        Descrp                        :   
*************************************************************************/
#ifndef DEVICEPAN_H
#define DEVICEPAN_H
#include <QDialog>
#include <QtGui/QWidget>
#include "ui_devicepan.h"
#include "DemoPublic.h"

class DevicePan : public QDialog
{
    Q_OBJECT

public:
    DevicePan(int *userid, QDialog *parent = 0);
    ~DevicePan();
    void devicePanelClickKey(int command);
private slots:

    void on_pushButton_ptzstart_clicked();
    void on_pushButton_button8_clicked();
    void on_pushButton_button5_clicked();
    void on_pushButton_button6_clicked();
    void on_pushButton_button7_clicked();
    void on_pushButton_ptzcontrol_clicked();
    void on_pushButton_panlock_clicked();
    void on_pushButton_panunlock_clicked();
    void on_pushButton_button11_clicked();
    void on_pushButton_button10_clicked();
    void on_pushButton_button12_clicked();
    void on_pushButton_button9_clicked();
    void on_pushButton_button15_clicked();
    void on_pushButton_button14_clicked();
    void on_pushButton_button16_clicked();
    void on_pushButton_button13_clicked();
    void on_pushButton_play_clicked();
    void on_pushButton_edit_clicked();
    void on_pushButton_save_clicked();
    void on_pushButton_mainmenu_clicked();
    void on_pushButton_button3_clicked();
    void on_pushButton_button2_clicked();
    void on_pushButton_button4_clicked();
    void on_pushButton_button1_clicked();
    void on_pushButton_enter_clicked();
    void on_pushButton_input_clicked();
    void on_pushButton_esc_clicked();
    void on_pushButton_multiwindow_clicked();
    void on_pushButton_down_clicked();
    void on_pushButton_intercom_clicked();
    void on_pushButton_systeminfo_clicked();
    void on_pushButton_up_clicked();
    void on_pushButton_right_clicked();
    void on_pushButton_left_clicked();
    void on_pushButton_ptzstop_clicked();
    void on_pushButton_exit_clicked();
private:
    int *m_dpuserid;
    int m_dpptzflags;
    Ui::DevicePanClass ui;
};

#endif // DEVICEPAN_H

