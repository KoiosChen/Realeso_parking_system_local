/************************************************************************
 *        Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *        FileName                      :   shutdowndevice.cpp
 *        Description                   :   shutdown device
 *        Modification History          :   none
 *        Version                       :   V1.0
 *        Time                          :   2009-11,12
 *        Author                        :   wanggongpu
 *        Descrp                        :   
*************************************************************************/
#include "shutdowndevice.h"
#include <QMessageBox>


/************************************************************************
 *        Function            :  ShutdownDevice
 *        Description         :  instructor
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
ShutdownDevice::ShutdownDevice(int *userid, QDialog *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    m_sdusrid = userid;
}

/************************************************************************
 *        Function            :  ~ShutdownDevice
 *        Description         :  destructor
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
ShutdownDevice::~ShutdownDevice()
{

}

/************************************************************************
 *        Function            :  on_pushButton_sure_clicked
 *        Description         :  sure
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void ShutdownDevice::on_pushButton_sure_clicked()
{
    if (!NET_DVR_ShutDownDVR(*m_sdusrid))
    {   
        QMessageBox::information(this,tr("NET_DVR_ShutDownDVR ERROR"), \
            tr("SDK_Last_Error =%1").arg(NET_DVR_GetLastError()));  
    }
    close();
}

/************************************************************************
 *        Function            :  on_pushButton_cancel_clicked
 *        Description         :  cancel
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void ShutdownDevice::on_pushButton_cancel_clicked()
{
    close();
}

