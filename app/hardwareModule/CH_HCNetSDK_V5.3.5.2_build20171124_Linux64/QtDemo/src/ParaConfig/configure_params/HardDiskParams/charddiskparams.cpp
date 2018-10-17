#include "charddiskparams.h"
#include "publicfuc.h"
#include <QMessageBox>

CHardDiskParams::CHardDiskParams(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);

	m_lUserID = -1;
	memset(m_struHDTableWidget, 0, sizeof(KIT_HD_WIDGET)*MAX_DISKNUM_V30);

	int i;
	for(i = 0; i < MAX_CHAN_NUM; i++ )
	{
		m_pCheckChan[i] = NULL;//释放由Qtable去完成
	}

	ui.tableHDGroup->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

	connect(ui.comboDGroup, SIGNAL(activated(int)), this, SLOT(comboHDGroupChanged(int)));
}

CHardDiskParams::~CHardDiskParams()
{

}

int CHardDiskParams::setUserID(long lUserID)
{
	if(lUserID < 0)
	{
		return HPR_ERROR;
	}
	m_lUserID = lUserID;
	return HPR_OK;
}

//获取硬盘信息, 包括设置表格信息的操作
int  CHardDiskParams::getHardDiskInfo()
{
	//从设备获取硬盘信息
	if(m_lUserID < 0)
	{
		return HPR_ERROR;
	}
	NET_DVR_HDCFG struHDParams;
	memset(&struHDParams, 0 , sizeof(NET_DVR_HDCFG));
	int iRet;
	DWORD uiReturnLen;
	iRet = NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_HDCFG, 0, &struHDParams, sizeof(NET_DVR_HDCFG), &uiReturnLen);
    if(0 == iRet)
    {
    	iRet = NET_DVR_GetLastError();
        QMessageBox::information(this, "Please check!", tr("The error is \"%1\"").arg(iRet));
        return HPR_ERROR;
    }

    //1.清空表格
    int iRow;
    ui.tableHardDisk->clear();
    for(iRow = 0; iRow < ui.tableHardDisk->rowCount(); iRow++)
    {
    	ui.tableHardDisk->removeRow(iRow);
    }

    //设置行数，列数，头
    ui.tableHardDisk->setRowCount((int)struHDParams.dwHDCount);
    ui.tableHardDisk->setColumnCount(7);

    //设置头部
    QStringList list;
    list<<"HD name"<<"Capacity"<<"Last capacity"<<"State"<<"Type"<<"Attribute"<<"Group";
    ui.tableHardDisk->setHorizontalHeaderLabels(list);

    QStringList listHdStatus; //HD状态
    listHdStatus<<"Normal"<<"Not format"<<"Error"<<"S.M.A.R.T"<<"Not match"<<"Sleep"<<"Not connect";
    QStringList listHDAttr; //HD属性
    listHDAttr<<"Default"<<"Redundancy"<<"Read";
    QStringList listHDType; //HD类型
    listHDType<<"Local"<<"ESATA"<<"NFS";
    QStringList listHdGroup; //盘组号
    listHdGroup<<"0"<<"1"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7";
    listHdGroup<<"8"<<"9"<<"10"<<"11"<<"12"<<"13"<<"14"<<"15";

    //2.往表格中写入数据
    ui.tableHardDisk->setEditTriggers(QAbstractItemView::NoEditTriggers); //表格不可编辑
    int i;
    char strTemp[100];
    for(i = 0; i < (int)struHDParams.dwHDCount; i++ )
    {
    	//硬盘号
    	memset(strTemp, 0, 100);
    	sprintf(strTemp, "%d", struHDParams.struHDInfo[i].dwHDNo);
    	m_struHDTableWidget[i].pTableItem[0] = new QTableWidgetItem(strTemp);
    	//iTableItemFlags = m_struHDTableWidget[i].pTableItem[0]->flags();
    	//m_struHDTableWidget[i].pTableItem[0]->setFlags(iTableItemFlags&0xfd);
    	ui.tableHardDisk->setItem(i, 0, m_struHDTableWidget[i].pTableItem[0]);
    	//硬盘容量
    	memset(strTemp, 0, 100);
        sprintf(strTemp, "%d", struHDParams.struHDInfo[i].dwCapacity);
        m_struHDTableWidget[i].pTableItem[1] = new QTableWidgetItem(strTemp);
        ui.tableHardDisk->setItem(i, 1, m_struHDTableWidget[i].pTableItem[1]);
        //剩余容量
        memset(strTemp, 0, 100);
        sprintf(strTemp, "%d", struHDParams.struHDInfo[i].dwFreeSpace);
        m_struHDTableWidget[i].pTableItem[2] = new QTableWidgetItem(strTemp);
        ui.tableHardDisk->setItem(i, 2, m_struHDTableWidget[i].pTableItem[2]);

        //状态
		m_struHDTableWidget[i].pTableCombo[0] = new QComboBox();
		m_struHDTableWidget[i].pTableCombo[0]->addItems(listHdStatus);
		m_struHDTableWidget[i].pTableCombo[0]->setCurrentIndex((int)struHDParams.struHDInfo[i].dwHdStatus);
		ui.tableHardDisk->setCellWidget(i, 3, m_struHDTableWidget[i].pTableCombo[0]);
		//m_struHDTableWidget[i].pTableCombo[0]->setEditable(FALSE);
		m_struHDTableWidget[i].pTableCombo[0]->setEnabled(FALSE);
		//属性
        m_struHDTableWidget[i].pTableCombo[1] = new QComboBox();
        m_struHDTableWidget[i].pTableCombo[1]->addItems(listHDAttr);
        m_struHDTableWidget[i].pTableCombo[1]->setCurrentIndex((int)struHDParams.struHDInfo[i].byHDAttr);
        ui.tableHardDisk->setCellWidget(i, 4, m_struHDTableWidget[i].pTableCombo[1]);

        //类型
		m_struHDTableWidget[i].pTableCombo[2] = new QComboBox();
		m_struHDTableWidget[i].pTableCombo[2]->addItems(listHDType);
		m_struHDTableWidget[i].pTableCombo[2]->setCurrentIndex((int)struHDParams.struHDInfo[i].byHDType);
		ui.tableHardDisk->setCellWidget(i, 5, m_struHDTableWidget[i].pTableCombo[2]);
		m_struHDTableWidget[i].pTableCombo[2]->setEnabled(FALSE);

		//盘组
		m_struHDTableWidget[i].pTableCombo[3] = new QComboBox();
		m_struHDTableWidget[i].pTableCombo[3]->addItems(listHdGroup);
		m_struHDTableWidget[i].pTableCombo[3]->setCurrentIndex((int)struHDParams.struHDInfo[i].dwHdGroup);
		ui.tableHardDisk->setCellWidget(i, 6, m_struHDTableWidget[i].pTableCombo[3]);
    }

    return HPR_ERROR;
}

//设置硬盘参数
int CHardDiskParams::setHardDiskInfo()
{
	//QMessageBox::information(this, tr("Please check!"), tr("Please table widget is NULL!"));
	//从设备获取硬盘信息
	if(m_lUserID < 0)
	{
		return HPR_ERROR;
	}
	NET_DVR_HDCFG struHDParams;
	memset(&struHDParams, 0 , sizeof(NET_DVR_HDCFG));
	int iRet;
	DWORD uiReturnLen;
	iRet = NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_HDCFG, 0, &struHDParams, sizeof(NET_DVR_HDCFG), &uiReturnLen);
    if(0 == iRet)
    {
    	iRet = NET_DVR_GetLastError();
        QMessageBox::information(this, "Please check!", tr("The error is \"%1\"").arg(iRet));
        return HPR_ERROR;
    }

    //从当前的table表格中获取数据
    int i;
    for(i = 0; i < (int)struHDParams.dwHDCount; i++ )
    {
    	if(NULL == m_struHDTableWidget[i].pTableCombo[1] || NULL == m_struHDTableWidget[i].pTableCombo[3])
    	{
    		QMessageBox::information(this, tr("Please check!"), tr("Please table widget is NULL!"));
    		return HPR_ERROR;
    	}
    	struHDParams.struHDInfo[i].byHDAttr = m_struHDTableWidget[i].pTableCombo[1]->currentIndex();
    	struHDParams.struHDInfo[i].dwHdGroup = m_struHDTableWidget[i].pTableCombo[3]->currentIndex();
    }

    //设置如设备中去
	iRet = NET_DVR_SetDVRConfig(m_lUserID, NET_DVR_SET_HDCFG, 0, &struHDParams, sizeof(NET_DVR_HDCFG));
    if(0 == iRet)
    {
    	iRet = NET_DVR_GetLastError();
        QMessageBox::information(this, "Please check!", tr("The error is \"%1\"").arg(iRet));
        return HPR_ERROR;
    }

    return HPR_OK;
}

//获取硬盘组信息
int CHardDiskParams::getHDGroupInfo()
{
	//从设备获取硬盘组信息
	if(m_lUserID < 0)
	{
		return HPR_ERROR;
	}
	NET_DVR_HDGROUP_CFG struHDGroupParams;
	memset(&struHDGroupParams, 0 , sizeof(NET_DVR_HDGROUP_CFG));
	int iRet;
	DWORD uiReturnLen;
	iRet = NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_HDGROUP_CFG, 0, &struHDGroupParams, sizeof(NET_DVR_HDGROUP_CFG), &uiReturnLen);
    if(0 == iRet)
    {
    	iRet = NET_DVR_GetLastError();
        QMessageBox::information(this, "Please check!", tr("The error is \"%1\"").arg(iRet));
        return HPR_ERROR;
    }

    if(GetChanInfoToTable() == HPR_ERROR) //先获取通道信息
    {
    	return HPR_ERROR;
    }

    //设置硬盘组信息
    //硬盘号
    char strTemp[100];
    int i;
    QStringList listHDGroup;
    for(i = 0; i < ((int)struHDGroupParams.dwHDGroupCount ); i++)
    {
    	//QMessageBox::information(this, "Please check!", tr("dwHDGroupCount i is \"%1\"").arg(i));
    	memset(strTemp, 0, 100);
    	sprintf(strTemp, "%d", i + 1);
    	listHDGroup<<strTemp;
    }
    ui.comboDGroup->clear();
    ui.comboDGroup->addItems(listHDGroup);
    comboHDGroupChanged(0);//设置第1个盘组的通道信息

	 return HPR_OK;
}

//设置硬盘组参数
int CHardDiskParams::setHDGroupInfo()
{
	//从设备获取硬盘组信息
	if(m_lUserID < 0)
	{
		return HPR_ERROR;
	}
	NET_DVR_HDGROUP_CFG struHDGroupParams;
	memset(&struHDGroupParams, 0 , sizeof(NET_DVR_HDGROUP_CFG));
	int iRet;
	DWORD uiReturnLen;
	iRet = NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_HDGROUP_CFG, 0, &struHDGroupParams, sizeof(NET_DVR_HDGROUP_CFG), &uiReturnLen);
    if(0 == iRet)
    {
    	iRet = NET_DVR_GetLastError();
        QMessageBox::information(this, "Please check!", tr("The error is \"%1\"").arg(iRet));
        return HPR_ERROR;
    }

    //获取界面上填入的数据
    //当前选中序号
    int iCurrentComboIndex;
    iCurrentComboIndex = ui.comboDGroup->currentIndex();
    if(iCurrentComboIndex < 0)
    {
    	return HPR_ERROR;
    }
    int i;
    for(i=0; i < MAX_CHAN_NUM; i++)
    {
    	if(m_pCheckChan[i] != NULL)
    	{
    		struHDGroupParams.struHDGroupAttr[iCurrentComboIndex].byHDGroupChans[i]
    		                                                                     = m_pCheckChan[i]->isChecked();
    	}
    }

    //设置到设备中
	iRet = NET_DVR_SetDVRConfig(m_lUserID, NET_DVR_SET_HDGROUP_CFG, 0, &struHDGroupParams, sizeof(NET_DVR_HDGROUP_CFG ));
    if(0 == iRet)
    {
    	iRet = NET_DVR_GetLastError();
        QMessageBox::information(this, "Please check!", tr("The error is \"%1\"").arg(iRet));
        return HPR_ERROR;
    }
	 return HPR_OK;
}

//刷新硬盘参数
void CHardDiskParams::on_btnHDUpdate_clicked()
{
	getHardDiskInfo();
}

//保存参数
void CHardDiskParams::on_btnHDSave_clicked()
{
	setHardDiskInfo();
	getHardDiskInfo();//保存完，刷新下
}

//刷新硬盘组管理参数
void CHardDiskParams::on_btnHDGroupUpdate_clicked()
{
	getHDGroupInfo();
}

//保存硬盘组管理参数
void CHardDiskParams::on_btnHDGroupSave_clicked()
{
	setHDGroupInfo();
	getHDGroupInfo();//刷新下
}

//comboxBox选择
void CHardDiskParams::comboHDGroupChanged(int iHDGroupIndex)
{
	//从设备获取硬盘组信息
	if(m_lUserID < 0)
	{
		return ;
	}
	NET_DVR_HDGROUP_CFG struHDGroupParams;
	memset(&struHDGroupParams, 0 , sizeof(NET_DVR_HDGROUP_CFG));
	int iRet;
	DWORD uiReturnLen;
	iRet = NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_HDGROUP_CFG, 0, &struHDGroupParams, sizeof(NET_DVR_HDGROUP_CFG), &uiReturnLen);
    if(0 == iRet)
    {
    	iRet = NET_DVR_GetLastError();
        QMessageBox::information(this, "Please check!", tr("The error is \"%1\"").arg(iRet));
        return ;
    }

    //设置盘组对应的通道信息
    int i;
    for(i=0; i < MAX_CHAN_NUM; i++)
    {
    	if(m_pCheckChan[i] != NULL)
    	{
    		if(struHDGroupParams.struHDGroupAttr[iHDGroupIndex].byHDGroupChans[i])
    		{
    			m_pCheckChan[i]->setChecked(TRUE);
    		}
    		else
    		{
    			m_pCheckChan[i]->setChecked(FALSE);
    		}
    	}
    }
}

void CHardDiskParams::on_checkAllChan_clicked()
{
	int bState;
	if(ui.checkAllChan->isChecked())
	{
		//ui.checkAllChan->setChecked(FALSE);
		bState = TRUE;
	}
	else
	{
		//ui.checkAllChan->setChecked(TRUE);
		bState = FALSE;
	}

    //设置其他通道状态
    int i;
    for(i=0; i < MAX_CHAN_NUM; i++)
    {
    	if(m_pCheckChan[i] != NULL)
    	{
    		m_pCheckChan[i]->setChecked(bState);
    	}
    }
}

void CHardDiskParams::showEvent ( QShowEvent * event )
{
	if(event == NULL)
	{
	}
	getHardDiskInfo();         //再获取硬盘
	getHDGroupInfo();
}

//获取通道信息
int CHardDiskParams::GetChanInfoToTable()
{
    //1.根据设备返回的信息，创建checkbox，
    //new出来的checkbox要自己释放，这里由Qtable完成
    int iTotalNum;
    if(KIT_CreateCheckboxChanArr(m_lUserID, m_pCheckChan, &iTotalNum) == HPR_ERROR)
    {
    	return HPR_ERROR;
    }

    //2.清空表格
    int iRow;
    ui.tableHDGroup->clear();
    for(iRow = 0; iRow < ui.tableHDGroup->rowCount(); iRow++)
    {
    	ui.tableHDGroup->removeRow(iRow);
    }

    //设置行数，列数: 3列
    int iRowCount; //表格的总函数
    iRowCount = (iTotalNum)/3;
    if((iTotalNum + 1)%3 > 0)
    {
    	iRowCount++;
    }
	ui.tableHDGroup->setRowCount(iRowCount);
    ui.tableHDGroup->setColumnCount(3);

    //3.将数据写入table中
    //ui.tableHDGroup->setCellWidget(0, 0, m_pCheckChanAll);
    int iColumn = 0;
    iRow = 0;
    int i;
    for(i = 0; i < MAX_CHANNUM_V30; i++)
    {
    	//找到一个已经创建的checkbox
    	if(m_pCheckChan[i] != NULL)
    	{
    		//按顺序依次加入table中
    		ui.tableHDGroup->setCellWidget(iRow, iColumn, m_pCheckChan[i]);
    		//已经到达一行的行末，则换行
    		if(iColumn == 2)
    		{
    			++iRow;
    			iColumn = 0;
    		}
    		else//下个table位置
    		{
    			++iColumn;
    		}
    	}
    }

	return HPR_OK;
}









