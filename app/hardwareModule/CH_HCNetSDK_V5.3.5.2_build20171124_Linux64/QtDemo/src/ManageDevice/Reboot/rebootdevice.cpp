/************************************************************************
 *        Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *        FileName                      :   rebootdevice.cpp
 *        Description                   :   reboot 
 *        Modification History          :   none
 *        Version                       :   V1.0
 *        Time                          :   2009-11,12
 *        Author                        :   wanggongpu
 *        Descrp                        :   
*************************************************************************/
#include "rebootdevice.h"
#include <QMessageBox>

/************************************************************************
 *        Function            :  RebootDevice
 *        Description         :  instructor
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
RebootDevice::RebootDevice(int *userid, QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    m_rbusrid = userid;
}

/************************************************************************
 *        Function            :  ~RebootDevice
 *        Description         :  destructor
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
RebootDevice::~RebootDevice()
{

}

/************************************************************************
 *        Function            :  on_pushButton_sure_clicked
 *        Description         :  reboot
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void RebootDevice::on_pushButton_sure_clicked()
{
    if (!NET_DVR_RebootDVR(*m_rbusrid))
    {   
        QMessageBox::information(this,tr("NET_DVR_RebootDVR ERROR"),\
            tr("SDK_Last_Error =%1").arg(NET_DVR_GetLastError()));  
        return;
    }
    close();
}

/************************************************************************
 *        Function            :  on_pushButton_cancel_clicked
 *        Description         :  cancel reboot
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void RebootDevice::on_pushButton_cancel_clicked()
{
    close();
}

