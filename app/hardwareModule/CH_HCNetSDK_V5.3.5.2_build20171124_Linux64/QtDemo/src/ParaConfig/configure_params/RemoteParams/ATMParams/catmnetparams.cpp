/*
 * Copyright(C) 2010,Hikvision Digital Technology Co., Ltd 
 * 
 * 文件名称：catmnetparams.cpp
 * 描    述：
 * 当前版本：1.0
 * 作    者：潘亚东
 * 创建日期：2010年1月12日
 * 修改记录：
 */


#include "catmnetparams.h"
#include "publicfuc.h"


#include <QMessageBox>

/*******************************************************************
      Function:   CATMNetParams::CATMNetParams
     Parameter:   (IN)   QWidget *parent 父窗口指针  
   Description:   构造函数
**********************************************************************/
CATMNetParams::CATMNetParams(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);

	//ATM类型
	QStringList listATMType;
	listATMType<<"NCR"<<"DIEBOLD"<<"WINCOR-NIXDORF"<<"SIEMENS"<<"OLIVETTI";
	listATMType<<"FUJITSU"<<"HITACHI"<<"SMI"<<"IBM"<<"BULL";
	listATMType<<"怡化"<<"立德"<<"广电运通"<<"Mini-Banl"<<"广利"<<"东信";
	listATMType<<"辰通"<<"南天"<<"晓星"<<"广州御银"<<"青湖泰利特"<<"DRS918";
	listATMType<<"KALATEL"<<"NCR_2"<<"NXS";
	ui.comboATMType->addItems(listATMType);

	//查询类型
	QStringList listCodeType;
	listCodeType<<"Query"<<"Fetch"<<"Disposit"<<"Modify Passwd"<<"Transfer";		
		
	ui.comboCodeType->addItems(listCodeType);

	m_bCode = FALSE;
	memset(m_frameTypeCode, 0, sizeof(NET_DVR_FRAMETYPECODE)*10);

	connect(ui.comboCodeType, SIGNAL(activated(int)), this, SLOT(comboCodeTypeChanged(int)));
}

/*******************************************************************
      Function:   CATMNetParams::~CATMNetParams
     Parameter:   (IN)   无参数  
   Description:   析构函数
**********************************************************************/
CATMNetParams::~CATMNetParams()
{

}

/*******************************************************************
      Function:   CATMNetParams::getATMInfo
     Parameter:   (IN)   LPNET_DVR_FRAMEFORMAT pstruATMInfo  ATM参数指针
        Return:   HPR_OK--成功，HPR_ERROR--失败。
   Description:   将外部获取的参数设置到界面中
**********************************************************************/
int CATMNetParams::getATMInfo(LPNET_DVR_FRAMEFORMAT pstruATMInfo)
{
	if(NULL == pstruATMInfo)
	{
		m_bCode = FALSE;
		return HPR_ERROR;
	}

	//IP地址
	ui.editIP->setText(pstruATMInfo->sATMIP);
	//ATM类型
	ui.comboATMType->setCurrentIndex((int)pstruATMInfo->dwATMType);

	//报文标志位：起始位置
	KIT_intToEdit(ui.editSignBeginPos, (int)pstruATMInfo->dwFrameSignBeginPos);
	//报文标志位：长度
	KIT_intToEdit(ui.editSignLength, (int)pstruATMInfo->dwFrameSignLength);
	//报文标志位：内容
        ui.editSignContent->setText((char *)pstruATMInfo->byFrameSignContent);

	//卡号长度信息: 起始位置
	KIT_intToEdit(ui.editCardLenBeginPos, (int)pstruATMInfo->dwCardLengthInfoBeginPos);
	//卡号长度信息: 长度
	KIT_intToEdit(ui.editCardLenLen, (int)pstruATMInfo->dwCardLengthInfoLength);

	//卡号信息: 起始位置
	KIT_intToEdit(ui.editCardPos, (int)pstruATMInfo->dwCardNumberInfoBeginPos);
	//卡号信息: 长度
	KIT_intToEdit(ui.editCardLen, (int)pstruATMInfo->dwCardNumberInfoLength);

	//交易信息: 起始位置
	KIT_intToEdit(ui.editBusinessPos, (int)pstruATMInfo->dwBusinessTypeBeginPos);
	//交易信息: 起始位置
	KIT_intToEdit(ui.editBusinessLen, (int)pstruATMInfo->dwBusinessTypeLength);

	//代码
	m_bCode = TRUE;
	memcpy(m_frameTypeCode, pstruATMInfo->frameTypeCode, sizeof(NET_DVR_FRAMETYPECODE)*10);
	ui.comboCodeType->setCurrentIndex(0);
        ui.editCode->setText( (char *)pstruATMInfo->frameTypeCode[0].code);
	return HPR_OK;
}

/*******************************************************************
      Function:   CATMNetParams::setATMInfo
     Parameter:   (IN)   LPNET_DVR_FRAMEFORMAT pstruATMInfo ATM参数指针 
        Return:   HPR_OK--成功，HPR_ERROR--失败。
   Description:   将界面上的数据写入AMT参数指针中
**********************************************************************/
int CATMNetParams::setATMInfo(LPNET_DVR_FRAMEFORMAT pstruATMInfo)
{
	if(NULL == pstruATMInfo)
	{
		return HPR_ERROR;
	}

	//IP地址
        KIT_editToIP(ui.editIP, (BYTE *)pstruATMInfo->sATMIP, 16);
	//设置ATM类型
	pstruATMInfo->dwATMType = ui.comboATMType->currentIndex();

	//报文标志位：起始位置
	KIT_editToInt(ui.editSignBeginPos, (int *)&(pstruATMInfo->dwFrameSignBeginPos));
	//报文标志位：长度
	KIT_editToInt(ui.editSignLength, (int *)&(pstruATMInfo->dwFrameSignLength));
	//报文标志位：内容
	KIT_editToStr(ui.editSignContent, pstruATMInfo->byFrameSignContent, 12);

	//卡号长度信息: 起始位置
	KIT_editToInt(ui.editCardLenBeginPos, (int *)&(pstruATMInfo->dwCardLengthInfoBeginPos));
	//卡号长度信息: 长度
	KIT_editToInt(ui.editCardLenLen, (int *)&(pstruATMInfo->dwCardLengthInfoLength));

	//卡号信息: 起始位置
	KIT_editToInt(ui.editCardPos, (int *)&(pstruATMInfo->dwCardNumberInfoBeginPos));
	//卡号信息: 长度
	KIT_editToInt(ui.editCardLen, (int *)&(pstruATMInfo->dwCardNumberInfoLength));

	//交易信息: 起始位置
	KIT_editToInt(ui.editBusinessPos, (int *)&(pstruATMInfo->dwBusinessTypeBeginPos));
	//交易信息: 起始位置
	KIT_editToInt(ui.editBusinessLen, (int *)&(pstruATMInfo->dwBusinessTypeLength));

	//代码
	int iCodeType;
	iCodeType = ui.comboCodeType->currentIndex();
	KIT_editToStr(ui.editCode, pstruATMInfo->frameTypeCode[iCodeType].code, 12);

	return HPR_OK;
}

/*******************************************************************
      Function:   CATMNetParams::comboCodeTypeChanged
     Parameter:   (IN)   int iType  ATM参数的code字段的序号
        Return:   HPR_OK--成功，HPR_ERROR--失败。
   Description:   ATM参数的code字段的序号改变时，在edit控件中写入相应的值
**********************************************************************/
void CATMNetParams::comboCodeTypeChanged(int iType)
{
	//QMessageBox::information(this, "Please check!", "The error is");
	if(m_bCode)
	{
                ui.editCode->setText((char *)m_frameTypeCode[iType].code);
	}
}





