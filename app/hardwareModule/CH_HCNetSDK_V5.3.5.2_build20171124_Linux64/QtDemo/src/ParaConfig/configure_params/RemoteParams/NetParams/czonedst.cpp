
#include <QMessageBox>
#include "czonedst.h"
#include "publicfuc.h"
#include "DemoPublic.h"

CZoneDst::CZoneDst(QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);

	m_lUserID = -1;
	m_lChannel = -1;

	QStringList strings;
	strings<<"Disable"<<"Enable";
	ui.comboEnableDST->addItems(strings);

	strings.clear();
	strings<<"30min"<<"60min"<<"90min"<<"120min";
	ui.comboSTBias->addItems(strings);

	strings.clear();
	strings<<"1"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7"<<"8"<<"9"<<"10"<<"11"<<"12";
    ui.comboMonthBegin->addItems(strings);
    ui.comboMonthEnd->addItems(strings);

    strings.clear();
    strings<<"First"<<"Second"<<"Third"<<"Fourth"<<"Last";
    ui.comboWeekNoBegin->addItems(strings);
    ui.comboWeekNoEnd->addItems(strings);

    strings.clear();
    strings<<"Sunday"<<"Monday"<<"Tuesday"<<"Wednesday"<<"Thursday"<<"Friday"<<"Saturday";
    ui.comboWeekDateBegin->addItems(strings);
    ui.comboWeekDateEnd->addItems(strings);

    strings.clear();
    strings<<"0"<<"1"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7"<<"8"<<"9"<<"10"<<"11"<<"12"<<"13"<<"14"<<"15"<<"16"<<"17"<<"18"<<"19"<<"20"<<"21"<<"22"<<"23";
    ui.comboHourBegin->addItems(strings);
    ui.comboHourEnd->addItems(strings);

    strings.clear();
    strings<<"1"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7"<<"8"<<"9"<<"10"<<"11"<<"12"<<"13"<<"14"<<"15"<<"16"<<"17"<<"18"<<"19"<<"20"<<"21"<<"22"<<"23";
    ui.comboMinuteBegin->addItems(strings);
    ui.comboMinuteEnd->addItems(strings);

    connect(ui.btnSave,  SIGNAL(clicked()), this, SLOT(clickSave()));
    connect(ui.btnRefresh,  SIGNAL(clicked()), this, SLOT(clickRefresh()));
}

CZoneDst::~CZoneDst()
{

}

int CZoneDst::getZoneDstInfo()
{
	NET_DVR_ZONEANDDST  struZoneAndDst;
	DWORD uiParamsLen;
	int iRet;

	iRet = NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_ZONEANDDST, m_lChannel, &struZoneAndDst, sizeof(NET_DVR_ZONEANDDST), &uiParamsLen);
	if(iRet == 0)
	{
		//调用错误信息函数完成
		KIT_DisplayErrorInfo("Get Zone information", KIT_ERRPR_DIS_TYPE);
		return HPR_ERROR;
	}
    ui.comboEnableDST->setCurrentIndex(int(struZoneAndDst.dwEnableDST));
    ui.comboSTBias->setCurrentIndex(((int)(struZoneAndDst.byDSTBias))/30 - 1);

    //month
    ui.comboMonthBegin->setCurrentIndex(int(struZoneAndDst.struBeginPoint.dwMonth));
    ui.comboMonthEnd->setCurrentIndex(int(struZoneAndDst.struEndPoint.dwMonth));
    //weekno
    ui.comboWeekNoBegin->setCurrentIndex(int(struZoneAndDst.struBeginPoint.dwWeekNo));
    ui.comboWeekNoEnd->setCurrentIndex(int(struZoneAndDst.struEndPoint.dwWeekNo));
    //week data
    ui.comboWeekDateBegin->setCurrentIndex(int(struZoneAndDst.struBeginPoint.dwWeekDate));
    ui.comboWeekDateEnd->setCurrentIndex(int(struZoneAndDst.struEndPoint.dwWeekDate));
    //hour
    ui.comboHourBegin->setCurrentIndex(int(struZoneAndDst.struBeginPoint.dwHour));
    ui.comboHourEnd->setCurrentIndex(int(struZoneAndDst.struEndPoint.dwHour));
    //minute
    ui.comboMinuteBegin->setCurrentIndex(int(struZoneAndDst.struBeginPoint.dwMin));
    ui.comboMinuteEnd->setCurrentIndex(int(struZoneAndDst.struEndPoint.dwMin));

	return HPR_OK;
}

int CZoneDst::setZoneDsfInfo()
{
	NET_DVR_ZONEANDDST  struZoneAndDst;
	DWORD uiParamsLen;
	int iRet;

	iRet = NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_ZONEANDDST, m_lChannel, &struZoneAndDst, sizeof(NET_DVR_ZONEANDDST), &uiParamsLen);
    if(iRet == 0)
	{
		QMessageBox::information(this, tr("Please check!"), tr(".Get Zone information unsuccessfully!"));
		return HPR_ERROR;
	}
    struZoneAndDst.dwEnableDST = ui.comboEnableDST->currentIndex();
    struZoneAndDst.byDSTBias = 30*(ui.comboSTBias->currentIndex() + 1);

    //month
    struZoneAndDst.struBeginPoint.dwMonth = ui.comboMonthBegin->currentIndex();
    struZoneAndDst.struEndPoint.dwMonth = ui.comboMonthEnd->currentIndex();

    //weekno
    struZoneAndDst.struBeginPoint.dwWeekNo = ui.comboWeekNoBegin->currentIndex();
    struZoneAndDst.struEndPoint.dwWeekNo = ui.comboWeekNoEnd->currentIndex();

    //week data
    struZoneAndDst.struBeginPoint.dwWeekDate = ui.comboWeekDateBegin->currentIndex();
    struZoneAndDst.struEndPoint.dwWeekDate = ui.comboWeekDateEnd->currentIndex();

    //hour
    struZoneAndDst.struBeginPoint.dwHour =  ui.comboHourBegin->currentIndex();
    struZoneAndDst.struEndPoint.dwHour = ui.comboHourEnd->currentIndex();

    //minute
    struZoneAndDst.struBeginPoint.dwMin = ui.comboMinuteBegin->currentIndex();
    struZoneAndDst.struEndPoint.dwMin = ui.comboMinuteEnd->currentIndex();

    iRet = NET_DVR_SetDVRConfig(m_lUserID, NET_DVR_SET_ZONEANDDST, m_lChannel,  &struZoneAndDst, sizeof(NET_DVR_ZONEANDDST));
    if(iRet == 0)
    {
       	 iRet = NET_DVR_GetLastError();
         QMessageBox::information(this, tr("Set Zone fail!"), tr("Set Zone fail. \"%1\"").arg(iRet));
         return HPR_ERROR;
     }

	return HPR_OK;
}

void  CZoneDst::clickSave()
{
	if(m_lUserID < 0)
	{
		return;
	}
	setZoneDsfInfo();

}
void  CZoneDst::clickRefresh()
{
	if(m_lUserID < 0)
	{
		return;
	}
	getZoneDstInfo();

}


void CZoneDst::on_btnExit_clicked()
{
	close();
}


void CZoneDst::showEvent ( QShowEvent * event )
{
	//disable warning
    if(event == NULL)
    {
    }
	if(m_lUserID < 0)
	{
		//QMessageBox::information(this, tr("Please login firstly!"), tr("Please login firstly!."));
	    return;
	}
	else
	{
		getZoneDstInfo();
	}
}



int CZoneDst::setUserIDAndChannel(long lUserID, long lChannel)
{
	m_lUserID = lUserID;
	m_lChannel = lChannel;

	return HPR_OK;
}
