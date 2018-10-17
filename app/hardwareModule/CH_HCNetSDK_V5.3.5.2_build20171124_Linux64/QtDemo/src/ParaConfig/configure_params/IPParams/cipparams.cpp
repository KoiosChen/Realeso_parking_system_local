#include "cipparams.h"
#include "publicfuc.h"
#include <QMessageBox>

CIPParams::CIPParams(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);
	m_lUserID = -1;
	m_iIPParamIndex = -1;
	m_iCurrentRow = -1;

    ui.hLayout->addWidget(ui.widgetCamora);
    ui.widgetCamora->hide();
    ui.btnSave->hide();

    //click table
    connect(ui.tableChannelInfo, SIGNAL(cellClicked(int,int)), this, SLOT(clickTabChannel(int)));
}

CIPParams::~CIPParams()
{

}

int CIPParams::setUserID(long lUserID)
{
	if(lUserID < 0)
	{
		return HPR_ERROR;
	}
	m_lUserID = lUserID;
	return HPR_OK;
}

//获取通道信息，写入表格中
int CIPParams::getIPInfo()
{
	if(m_lUserID < 0)
	{
        return HPR_ERROR;
	}

	//获取设备的IP接入信息
	NET_DVR_IPPARACFG struIPParams;
	memset(&struIPParams, 0 , sizeof(NET_DVR_IPPARACFG));
	if(getIPConInfo(&struIPParams) < 0)
	{
		return HPR_ERROR;
	}

    //先清理表格
    int iRow;
    ui.tableChannelInfo->clear();
    for(iRow = 0; iRow < ui.tableChannelInfo->rowCount(); iRow++)
    {
    	ui.tableChannelInfo->removeRow(iRow);
    }

    //获取最大数字通道和模拟通道数
    NET_DVR_DEVICECFG struDeviceInfo;
    memset(&struDeviceInfo, 0 , sizeof(NET_DVR_DEVICECFG));
    if(getDeviceInfo(&struDeviceInfo) < 0)
    {
    	return HPR_ERROR;
    }

    //设置行数，列数，头
    ui.tableChannelInfo->setRowCount(struDeviceInfo.byChanNum + struDeviceInfo.byIPChanNum);
    ui.tableChannelInfo->setColumnCount(5);

    //设置头部
    QStringList list;
    list<<"Channel No."<<"IP address"<<"Port"<<"IP channel No."<<"Whether online";
    ui.tableChannelInfo->setHorizontalHeaderLabels(list);

    //设置尺寸自适应
    ui.tableChannelInfo->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui.tableChannelInfo->setSelectionBehavior(QAbstractItemView::SelectRows);   //设置整行选中
    //ui.tableChannelInfo->setSelectionMode(QAbstractItemView::SingleSelection);  //设置当行选中

    //加入内容
    int i;
    int dwIPChanIndex;
    int dwID;
    iRow = 0;
    char strChannelName[100] = {0};
    for(i = 0; i < MAX_CHANNUM_V30; i++)
    {
    	//模拟通道
    	if(i < struDeviceInfo.byChanNum )
    	{
    		memset(strChannelName, 0, 100);
    		sprintf(strChannelName,"Camora%d", i + struDeviceInfo.byStartChan);
    		ui.tableChannelInfo->setItem(iRow, 0, new QTableWidgetItem(strChannelName));
    		ui.tableChannelInfo->setItem(iRow, 1, new QTableWidgetItem("Local"));
    		ui.tableChannelInfo->setItem(iRow, 2, new QTableWidgetItem("0"));
    		ui.tableChannelInfo->setItem(iRow, 3, new QTableWidgetItem("0"));
    		if(struIPParams.byAnalogChanEnable[i])
    		{
    		    ui.tableChannelInfo->setItem(iRow, 4, new QTableWidgetItem("Yes"));
    		}
    		else
    		{
    			ui.tableChannelInfo->setItem(iRow, 4, new QTableWidgetItem("No"));
    		}
    		iRow++;
    	}
    	//数字通道
    	if((i >= MAX_ANALOG_CHANNUM) && (i < MAX_ANALOG_CHANNUM + struDeviceInfo.byIPChanNum))
    	{
    		dwIPChanIndex = i  - MAX_ANALOG_CHANNUM;
    		memset(strChannelName, 0, 100);
    		sprintf(strChannelName,"IPCamora%d", dwIPChanIndex + struDeviceInfo.byStartChan);
    		ui.tableChannelInfo->setItem(iRow, 0, new QTableWidgetItem(strChannelName));
    		dwID = struIPParams.struIPChanInfo[dwIPChanIndex].byIPID;
            if(struIPParams.struIPChanInfo[dwIPChanIndex].byIPID != 0)
            {
            	//IP
            	ui.tableChannelInfo->setItem(iRow, 1, new QTableWidgetItem(struIPParams.struIPDevInfo[dwID-1].struIP.sIpV4));
                //端口
            	memset(strChannelName, 0, 100);
            	sprintf(strChannelName, "%d", struIPParams.struIPDevInfo[dwID-1].wDVRPort);
            	ui.tableChannelInfo->setItem(iRow, 2, new QTableWidgetItem(strChannelName));
            	//IP通道号
            	memset(strChannelName, 0, 100);
            	sprintf(strChannelName, "%d", struIPParams.struIPChanInfo[dwIPChanIndex].byChannel);
            	ui.tableChannelInfo->setItem(iRow, 3, new QTableWidgetItem(strChannelName));
            	//是否在线
            	if (struIPParams.struIPChanInfo[dwIPChanIndex].byEnable)
            	{
            		ui.tableChannelInfo->setItem(iRow, 4, new QTableWidgetItem("online"));
            	}
            	else
            	{
            		ui.tableChannelInfo->setItem(iRow, 4, new QTableWidgetItem("offline"));
            	}
            }
            else
            {
            	ui.tableChannelInfo->setItem(iRow, 1, new QTableWidgetItem("0.0.0.0"));
            	ui.tableChannelInfo->setItem(iRow, 2, new QTableWidgetItem("0"));
            	ui.tableChannelInfo->setItem(iRow, 3, new QTableWidgetItem("0"));
            	ui.tableChannelInfo->setItem(iRow, 4, new QTableWidgetItem("no"));
            }
    		iRow++;
    	}
    }

    //table不可以被编辑
    ui.tableChannelInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);
    return HPR_OK;
}
//从表格中得到数据，写入设备
int CIPParams::saveIPInfo()
{

	return HPR_OK;
}

//表格左键单击事件
int CIPParams::clickTabChannel(int iRow)
{
	m_iIPParamIndex = -1;

	//通过行中的camoraXX信息，获取去ID信息
	int iChannelIndex = 0;
	if(getIPChannnelIndex(iRow, &iChannelIndex) < 0)
	{
		return HPR_ERROR;
	}

	//设置修改控件中的值
	NET_DVR_IPPARACFG struIPParams;
	memset(&struIPParams, 0 , sizeof(NET_DVR_IPPARACFG));
	//获取设备中的IP接入信息
	if(getIPConInfo(&struIPParams) < 0)
	{
		return HPR_ERROR;
	}

	m_iIPParamIndex = iChannelIndex; //记录下当前的通道序号
	if(iChannelIndex < MAX_ANALOG_CHANNUM)//模拟通道
	{
		ui.widgetIPCamora->hide();
		ui.widgetCamora->show();
		if(struIPParams.byAnalogChanEnable[iChannelIndex])
		{
			ui.btnEnable->setEnabled(FALSE);
			ui.btnDisenable->setEnabled(TRUE);
		}
		else
		{
			ui.btnEnable->setEnabled(TRUE);
			ui.btnDisenable->setEnabled(FALSE);
		}
	}
	else//数字通道
	{
		int iIPChannelIndex;
		iIPChannelIndex = iChannelIndex - MAX_ANALOG_CHANNUM;
		ui.widgetIPCamora->show();
		ui.widgetCamora->hide();
		if(struIPParams.struIPChanInfo[iIPChannelIndex].byIPID > 0)//有对应的IP设备ID
		{
			int iIPID;
			iIPID = struIPParams.struIPChanInfo[iIPChannelIndex].byIPID -1;
			ui.btnEdit->setText("修改");
			ui.btnDelete->setEnabled(TRUE);
			//设置控件的值
			ui.editIP->setText(struIPParams.struIPDevInfo[iIPID].struIP.sIpV4);
                        ui.editUsername->setText((char *)struIPParams.struIPDevInfo[iIPID].sUserName);
                        ui.editPassword->setText((char *)struIPParams.struIPDevInfo[iIPID].sPassword);
			unsigned int uiPort;
			uiPort = struIPParams.struIPDevInfo[iIPID].wDVRPort;
			KIT_unintToEdit(ui.editPort, uiPort);
			unsigned int uiChannelNO;
			uiChannelNO = struIPParams.struIPChanInfo[iIPChannelIndex].byChannel;
			KIT_unintToEdit(ui.editChannelNO, uiChannelNO);
		}
		else//没有对应的IP设备ID
		{
			ui.btnEdit->setText("添加");
			ui.btnDelete->setEnabled(FALSE);
		}
	}

    m_iCurrentRow = iRow;
	//QMessageBox::information(this, "Please check!", tr("The error is \"%1\"").arg(iChannelIndex));
	return HPR_OK;
}

//单击IPC区域的"编辑"按钮
void CIPParams::on_btnEdit_clicked()
{
	//获取当前焦点信息
	if(m_iIPParamIndex  >= MAX_ANALOG_CHANNUM)
	{
		int iIPChannelIndex;
		int iTemp;
		NET_DVR_IPPARACFG struIPParams; //= {0};
		memset(&struIPParams, 0, sizeof(NET_DVR_IPPARACFG));
		//获取设备中的IP接入信息
		if(getIPConInfo(&struIPParams) < 0)
		{
			return;
		}

		iIPChannelIndex = m_iIPParamIndex - MAX_ANALOG_CHANNUM;
		int iIPID = 0;
		if(struIPParams.struIPChanInfo[iIPChannelIndex].byIPID <= 0)//新添加设备
		{
			//找一个可用IP设备ID号
			int i;
			for(i = 0; i< MAX_IP_DEVICE; i++)
			{
				if (!struIPParams.struIPDevInfo[i].dwEnable)
				{
					iIPID = i+1;//find the first empty node
					break;
				}
			}

			//修改IP通道信息
			KIT_editToInt(ui.editChannelNO, &iTemp);
			struIPParams.struIPChanInfo[iIPChannelIndex].byChannel = (BYTE)iTemp;
			struIPParams.struIPChanInfo[iIPChannelIndex].byIPID = (BYTE)iIPID;

			//修改iIPID对应的设备
                        KIT_editToIP(ui.editIP, (BYTE *)(struIPParams.struIPDevInfo[iIPID-1].struIP.sIpV4), IP_LEN);
			struIPParams.struIPDevInfo[iIPID-1].dwEnable = TRUE;
			KIT_editToInt(ui.editPort, &iTemp);
			struIPParams.struIPDevInfo[iIPID-1].wDVRPort = iTemp;//port
			KIT_editToStr(ui.editUsername, struIPParams.struIPDevInfo[iIPID-1].sUserName, NAME_LEN);
			KIT_editToStr(ui.editPassword, struIPParams.struIPDevInfo[iIPID-1].sPassword, PASSWD_LEN);
		}
		else //修改IP通道信息
		{
			iIPID =  struIPParams.struIPChanInfo[iIPChannelIndex].byIPID;
			KIT_editToInt(ui.editChannelNO, &iTemp);
			struIPParams.struIPChanInfo[iIPChannelIndex].byChannel = (BYTE)iTemp;

			//修改iIPID对应的设备
                        KIT_editToIP(ui.editIP, (BYTE *)(struIPParams.struIPDevInfo[iIPID-1].struIP.sIpV4), IP_LEN);
			KIT_editToInt(ui.editPort, &iTemp);
			struIPParams.struIPDevInfo[iIPID-1].wDVRPort = iTemp;//port
			KIT_editToStr(ui.editUsername, struIPParams.struIPDevInfo[iIPID-1].sUserName, NAME_LEN);
			KIT_editToStr(ui.editPassword, struIPParams.struIPDevInfo[iIPID-1].sPassword, PASSWD_LEN);
		}

		//往设备发送参数设置信息, 保存到设备
		int iRet = NET_DVR_SetDVRConfig(m_lUserID, NET_DVR_SET_IPPARACFG, -1, &struIPParams, sizeof(NET_DVR_IPPARACFG));
	    if(0 == iRet)
	    {
		    iRet = NET_DVR_GetLastError();
		    QMessageBox::information(this, tr("Please check!"), tr("Set User parameters error! \"%1\"").arg(iRet));
		    return ;
	    }
	}//IP通道参数修改或添加结束

	if(m_iIPParamIndex < 0)
	{
		QMessageBox::information(this, tr("Please check!"), tr("Please choose a row of table!"));
	}

	getIPInfo(); //刷新Table
	if(m_iCurrentRow >= 0)
	{
		ui.tableChannelInfo->setCurrentCell(m_iCurrentRow, 0);
		clickTabChannel(m_iCurrentRow);
	}
}

//删除IP通道配置信息
void CIPParams::on_btnDelete_clicked()
{
	//获取当前Table中的哪一行被选中，m_iIPParamIndex表示被选中那一行的通道编号(start no.到最大通道号)
	if(m_iIPParamIndex  >= MAX_ANALOG_CHANNUM)
	{
		int iIPChannelIndex;
		NET_DVR_IPPARACFG struIPParams; //= {0};
		memset(&struIPParams, 0, sizeof(NET_DVR_IPPARACFG));
		//获取设备中的IP接入信息
		if(getIPConInfo(&struIPParams) < 0)
		{
			return;
		}
		iIPChannelIndex = m_iIPParamIndex - MAX_ANALOG_CHANNUM;//IP参数数组序号

		int iIPID = 0;
		//获取IP设备数组序号
		iIPID = struIPParams.struIPChanInfo[iIPChannelIndex].byIPID;
		if (iIPID <= 0) //无效IP设备序号，删除失败，直接返回
		{
			QMessageBox::information(this, tr("Please check!"), tr("Please ip device ID is error!"));
			return;
		}

		//判断一下是否有IP通道在设用该设备
		int i;
		bool bDeleteDev = TRUE;
		for (i = 0; i < MAX_IP_CHANNEL; i++)
		{
			if (iIPChannelIndex != i)
			{
				if (iIPID == struIPParams.struIPChanInfo[i].byIPID)
				{
					//已有IP通道使用该设备,
					//QMessageBox::information(this, tr("Please check!"), tr("Some IP channel has used this Device!"));
					bDeleteDev = FALSE;
					break;
				}
			}
		}

		//清空ip设备的信息/clear current ip device
		if (bDeleteDev)
		{
			memset(&(struIPParams.struIPDevInfo[iIPID-1]), 0, sizeof(NET_DVR_IPDEVINFO));
		}

		//清空当前ip通道的信息/clear current ip chan
		memset(&(struIPParams.struIPChanInfo[iIPChannelIndex]), 0, sizeof(NET_DVR_IPCHANINFO));
		//往设备发送参数设置信息, 保存到设备
		int iRet = NET_DVR_SetDVRConfig(m_lUserID, NET_DVR_SET_IPPARACFG, -1, &struIPParams, sizeof(NET_DVR_IPPARACFG));
	    if(0 == iRet)
	    {
		    iRet = NET_DVR_GetLastError();
		    QMessageBox::information(this, tr("Please check!"), tr("Set User parameters error! \"%1\"").arg(iRet));
		    return ;
	    }
	}
	if(m_iIPParamIndex < 0)
	{
		QMessageBox::information(this, tr("Please check!"), tr("Please choose a row of table!"));
	}

	getIPInfo(); //刷新Table
	if(m_iCurrentRow >= 0)
	{
		ui.tableChannelInfo->setCurrentCell(m_iCurrentRow, 0);
		clickTabChannel(m_iCurrentRow);
	}
}

//启用模拟通道
void CIPParams::on_btnEnable_clicked()
{
	if(m_iIPParamIndex < 0)
	{
		QMessageBox::information(this, tr("Please check!"), tr("Please choose a row of table!"));
		return;
	}

	if(m_iIPParamIndex < MAX_ANALOG_CHANNUM)//模拟通道
	{
		NET_DVR_IPPARACFG struIPParams; //= {0};
		memset(&struIPParams, 0, sizeof(NET_DVR_IPPARACFG));
		//获取设备中的IP接入信息
		if(getIPConInfo(&struIPParams) < 0)
		{
			return;
		}

		struIPParams.byAnalogChanEnable[m_iIPParamIndex] = TRUE;//模拟通道参数数组序号
		//往设备发送参数设置信息, 保存到设备
		int iRet = NET_DVR_SetDVRConfig(m_lUserID, NET_DVR_SET_IPPARACFG, -1, &struIPParams, sizeof(NET_DVR_IPPARACFG));
	    if(!iRet)
	    {
		    iRet = NET_DVR_GetLastError();
		    QMessageBox::information(this, tr("Please check!"), tr("Set User parameters error! \"%1\"").arg(iRet));
		    return ;
	    }

    	ui.btnEnable->setEnabled(FALSE);
    	ui.btnDisenable->setEnabled(TRUE);
	    if(getIPInfo() == HPR_OK) //刷新Table
	    {
	    }
	}
}

//禁用模拟通道
void CIPParams::on_btnDisenable_clicked()
{
	if(m_iIPParamIndex < 0)
	{
		QMessageBox::information(this, tr("Please check!"), tr("Please choose a row of table!"));
		return;
	}

	if(m_iIPParamIndex < MAX_ANALOG_CHANNUM)//模拟通道
	{
		NET_DVR_IPPARACFG struIPParams; //= {0};
		memset(&struIPParams, 0, sizeof(NET_DVR_IPPARACFG));
		//获取设备中的IP接入信息
		if(getIPConInfo(&struIPParams) < 0)
		{
			return;
		}

		struIPParams.byAnalogChanEnable[m_iIPParamIndex] = FALSE;//模拟通道参数数组序号
		//往设备发送参数设置信息, 保存到设备
		int iRet = NET_DVR_SetDVRConfig(m_lUserID, NET_DVR_SET_IPPARACFG, -1, &struIPParams, sizeof(NET_DVR_IPPARACFG));
	    if(!iRet)
	    {
		    iRet = NET_DVR_GetLastError();
		    QMessageBox::information(this, tr("Please check!"), tr("Set User parameters error! \"%1\"").arg(iRet));
		    return ;
	    }

		ui.btnEnable->setEnabled(TRUE);
	    ui.btnDisenable->setEnabled(FALSE);
	    getIPInfo(); //刷新Table
	}
}

//获取设备信息: 起始通道号，便于在table中显示用
int CIPParams::getDeviceInfo(LPNET_DVR_DEVICECFG pDeviceInfo)
{
	if(m_lUserID < 0 && pDeviceInfo ==NULL)
	{
		return HPR_ERROR;
	}

	int iRet;
	DWORD uiReturnLen;
	iRet = NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_DEVICECFG, 0, pDeviceInfo, sizeof(NET_DVR_DEVICECFG), &uiReturnLen);
    if(0 == iRet)
    {
    	iRet = NET_DVR_GetLastError();
        QMessageBox::information(this, "Please check!", tr("The error is \"%1\"").arg(iRet));
        return HPR_ERROR;
    }

	return HPR_OK;
}

//获取IP接入信息
int CIPParams::getIPConInfo(LPNET_DVR_IPPARACFG pIPConInfo)
{
	if(m_lUserID < 0 && pIPConInfo ==NULL)
	{
		return HPR_ERROR;
	}

	int iRet;
	DWORD uiReturnLen;
	iRet = NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_IPPARACFG, 0, pIPConInfo, sizeof(NET_DVR_IPPARACFG), &uiReturnLen);
    if(0 == iRet)
    {
    	iRet = NET_DVR_GetLastError();
        QMessageBox::information(this, "Please check!", tr("The error is \"%1\"").arg(iRet));
        return HPR_ERROR;
    }

    return HPR_OK;
}

//根据行号，获取通道序号
int CIPParams::getIPChannnelIndex(int iRow, int *piChannelIndex)
{
	if(NULL == piChannelIndex)
	{
		return HPR_ERROR;
	}

	//先获取表格中的元素
	QTableWidgetItem *pChannelName = NULL;
	pChannelName = ui.tableChannelInfo->item(iRow, 0);

	//取得元素中的text
	if(NULL ==pChannelName)
	{
		return HPR_ERROR;
	}
	char strChannelName[20] = {0};
	QString qstrChannelName;
	qstrChannelName = pChannelName->text();
        KIT_qstringToStr(&qstrChannelName, (BYTE *)strChannelName, 20);

	//获取设备信息
	NET_DVR_DEVICECFG struDeviceInfo;
	memset(&struDeviceInfo, 0, sizeof(NET_DVR_DEVICECFG));
	if(getDeviceInfo(&struDeviceInfo) < 0)
	{
		return HPR_ERROR;
	}

	//处理
	int iIndex = 0;
	if('C' == strChannelName[0])//模拟通道名称
	{
		iIndex = atoi(&(strChannelName[6]));
	}
	if('I' == strChannelName[0])//IP通道
	{
		iIndex =  atoi(&(strChannelName[8]));
		iIndex += MAX_ANALOG_CHANNUM;
	}

	iIndex -= struDeviceInfo.byStartChan;
	*piChannelIndex = iIndex;

	return HPR_OK;
}

void CIPParams::showEvent ( QShowEvent * event )
{
	if(event == NULL)
	{
	}
	getIPInfo();
}
