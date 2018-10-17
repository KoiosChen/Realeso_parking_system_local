/**  @file AddNode.cpp
 *   @note HangZhou Hikvision System Technology Co., Ltd. All Right Reserved. 
 *   @brief Add device node to the tree
 *   
 *   @author wanggongpu
 *   @modify by panyadong
 *   @date 2012-5-22
 *   
 *   @note:
 */
#include <QMessageBox>
#include <stdio.h>
#include "addnode.h"
#include "DemoPublic.h"

class DeviceData;

/**  @fn  AddNode::AddNode(QWidget *parent): QDialog(parent)
 *   @brief 
 *   @param (IN) parent  
 *   @return none.   
 */
AddNode::AddNode(QWidget *parent): QDialog(parent)
{
    ui.setupUi(this);

    connect(ui.m_pCheckBoxIPserver, SIGNAL(clicked()), this, SLOT(CheckUserIPServer()));

    m_iuserid = -1;
    memset(&m_deviceinfo, 0, sizeof(NET_DVR_DEVICEINFO_V30));
    memset(&m_ipparacfg, 0, sizeof(NET_DVR_IPPARACFG));

	ui.passwd_lineEdit->setText(QString("12345"));
	ui.usrname_lineEdit->setText(QString("admin"));
	ui.port_lineEdit->setText(QString("8000"));

    ui.m_pEditIPServerIP->setText(QString("172.9.6.91"));
    ui.m_pEditIPServerPort->setText(QString("7071"));

    ui.m_pCheckBoxIPserver->setChecked(TRUE);
}

/**  @fn  AddNode::~AddNode()
 *   @brief 
 *   @param none
 */
AddNode::~AddNode()
{
}

/**  @fn  void AddNode::on_cancel_pushButton_clicked()
 *   @brief 
 *   @param none
 *   @return none   
 */
void AddNode::on_cancel_pushButton_clicked()
{
    close();
}

/**  @fn  void AddNode::on_m_pBtnGetDeviceIP_clicked()
 *   @brief 
 *   @param none
 *   @return none   
 */
void AddNode::on_m_pBtnGetDeviceIP_clicked()
{
    //in
    char strIPServerIP[DEMO_IP_MAX_LEN] = {0};
    WORD wIPServerPort = 7071;
    char strDeviceName[NAME_LEN + 1] = {0};
    char strSearialNumber[SERIALNO_LEN + 1] = {0};
    //out
    char strDeviceIP[DEMO_IP_MAX_LEN] = {0};
    DWORD dwDevicePort = 8000;

    //Get information
    strcpy(strIPServerIP, ui.m_pEditIPServerIP->text().toLocal8Bit().data());
    wIPServerPort = ui.m_pEditIPServerPort->text().toShort();
    strcpy(strDeviceName, ui.m_pEditDeviceName->text().toLocal8Bit().data());
    strcpy(strSearialNumber, ui.m_pEditDeviceSerial->text().toLocal8Bit().data());

    if (NET_DVR_GetDVRIPByResolveSvr_EX(strIPServerIP, wIPServerPort, (BYTE *)strDeviceName, strlen(strDeviceName), \
        (BYTE *)strSearialNumber, strlen(strSearialNumber), strDeviceIP, &dwDevicePort))
    {
        //Set device ip and port
        ui.ip_lineEdit->setText(strDeviceIP);
        char strPort[10] = {0};
        sprintf(strPort, "%d", dwDevicePort);
        ui.port_lineEdit->setText(strPort);
    } 
    else
    {
        QMessageBox::information(this, tr("NET_DVR_GetDVRIPByResolveSvr_EX error"), tr("SDK_LASTERROR=%1").arg(NET_DVR_GetLastError())); 
    } 
}

/**  @fn  void AddNode::CheckUserIPServer()
 *   @brief 
 *   @param none
 *   @return none   
 */
void AddNode::CheckUserIPServer()
{
    if (ui.m_pCheckBoxIPserver->isChecked())
    {
        ui.m_pFrameIPServer->setDisabled(FALSE);
        ui.m_pBtnGetDeviceIP->setDisabled(FALSE);
    }
    else
    {
        ui.m_pFrameIPServer->setDisabled(TRUE);
         ui.m_pBtnGetDeviceIP->setDisabled(TRUE);
    }
}

