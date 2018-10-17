/************************************************************************
 *        Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *        FileName                      :   resetdevice.cpp
 *        Description                   :   reset device
 *        Modification History          :   none
 *        Version                       :   V1.0
 *        Time                          :   2009-11,12
 *        Author                        :   wanggongpu
 *        Descrp                        :   
*************************************************************************/
#include "resetdevice.h"
#include <QMessageBox>


/************************************************************************
 *        Function            :  ResetDevice
 *        Description         :  instructor
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
ResetDevice::ResetDevice(int* userid, QDialog *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    m_rsusrid = userid;
}

/************************************************************************
 *        Function            :  ~ResetDevice
 *        Description         :  destructor
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
ResetDevice::~ResetDevice()
{

}

/************************************************************************
 *        Function            :  on_pushButton_sure_clicked
 *        Description         :  sure
 *        Input               :  none
 *        Output              :  none
 *        Return              :  none
*************************************************************************/
void ResetDevice::on_pushButton_sure_clicked()
{
    if (!NET_DVR_RestoreConfig(*m_rsusrid))
    {   
        QMessageBox::information(this,tr("NET_DVR_RestoreConfig ERROR"), \
            tr("SDK_Last_Error =%1").arg(NET_DVR_GetLastError()));  
        return;
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
void ResetDevice::on_pushButton_cancel_clicked()
{
    close();
}

