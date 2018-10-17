/*
 * Copyright(C) 2009,Hikvision Digital Technology Co., Ltd 
 * 
 * 文件名称：cshelter.cpp
 * 描    述：
 * 当前版本：1.0
 * 作    者：潘亚东
 * 创建日期：2009年11月24日
 * 修改记录：
 */


#include "cshelter.h"
#include "publicfuc.h"


/*******************************************************************
      Function:   CShelter::CShelter
     Parameter:  
           (IN)     QWidget *parent  
   Description:   构造函数
**********************************************************************/
CShelter::CShelter(QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);

	QStringList list;
	list<<"X"<<"Y"<<"Width"<<"Height";

	ui.tableShelter->setRowCount(MAX_SHELTERNUM);
	ui.tableShelter->setColumnCount(4);
	ui.tableShelter->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	ui.tableShelter->setHorizontalHeaderLabels(list);

	m_pShelter =NULL;
}

/*******************************************************************
      Function:   CShelter::~CShelter
     Parameter:  
           (IN)     无参数  
   Description:   析构函数
**********************************************************************/
CShelter::~CShelter()
{
}


/*******************************************************************
      Function:   CShelter::setShelter
     Parameter:  
           (IN)     NET_DVR_SHELTER *pShelter  遮挡区域指针
        Return:   HPR_OK--成功，HPR_ERROR--失败。
   Description:   设入遮挡区域地址
**********************************************************************/
int CShelter::setShelter(NET_DVR_SHELTER *pShelter)
{
	if(NULL == pShelter)
	{
		return HPR_ERROR;
	}

	m_pShelter = pShelter;
    return HPR_OK;
}


/*******************************************************************
      Function:   CShelter::setEachWidgetValue
     Parameter:  
           (IN)     无参数  
        Return:   HPR_OK--成功，HPR_ERROR--失败。
   Description:   设置对话框控件中的值
**********************************************************************/
int CShelter::setEachWidgetValue()
{
	if(NULL == m_pShelter)
	{
		return HPR_ERROR;
	}
	int i;
	char strTemp[100] = {0};
	for(i = 0; i < MAX_SHELTERNUM; i++)
	{
		//X坐标
		sprintf(strTemp, "%d", m_pShelter[i].wHideAreaTopLeftX);//整数转字符串
		m_itemShelter[i][0].setText(strTemp);                                      //加入到Item中
		ui.tableShelter->setItem(i, 0, &(m_itemShelter[i][0]));               //加入到Table中去

		//Y坐标
		memset(strTemp, 0, 100);
		sprintf(strTemp, "%d", m_pShelter[i].wHideAreaTopLeftY);
		m_itemShelter[i][1].setText(strTemp);
		ui.tableShelter->setItem(i, 1, &(m_itemShelter[i][1]));

		//宽度
		memset(strTemp, 0, 100);
		sprintf(strTemp, "%d", m_pShelter[i].wHideAreaWidth);
		m_itemShelter[i][2].setText(strTemp);
		ui.tableShelter->setItem(i, 2, &(m_itemShelter[i][2]));

		//高度
		memset(strTemp, 0, 100);
		sprintf(strTemp, "%d", m_pShelter[i].wHideAreaHeight);
		m_itemShelter[i][3].setText(strTemp);
		ui.tableShelter->setItem(i, 3, &(m_itemShelter[i][3]));
	}

	return HPR_OK;
}

/*******************************************************************
      Function:   CShelter::getEachWidgetValue
     Parameter:  
           (IN)     无参数  
        Return:   HPR_OK--成功，HPR_ERROR--失败。
   Description:   获取对话框中的控件值
**********************************************************************/
int CShelter::getEachWidgetValue()
{
	if(NULL == m_pShelter)
	{
		return HPR_ERROR;
	}

	int iTemp;
	int i;
	QString qstrTemp;
	for(i = 0; i < MAX_SHELTERNUM; i++)
	{
		qstrTemp = m_itemShelter[i][0].text();  //不这样用，直接用kit函数取地址会有waring
		KIT_qstringToInt(&(qstrTemp), &iTemp);
		m_pShelter[i].wHideAreaTopLeftX = iTemp;

		qstrTemp = m_itemShelter[i][1].text();
		KIT_qstringToInt(&(qstrTemp), &iTemp);
		m_pShelter[i].wHideAreaTopLeftY = iTemp;

		qstrTemp = m_itemShelter[i][2].text();
		KIT_qstringToInt(&(qstrTemp), &iTemp);
		m_pShelter[i].wHideAreaWidth = iTemp;

		qstrTemp = m_itemShelter[i][3].text();
		KIT_qstringToInt(&(qstrTemp), &iTemp);
		m_pShelter[i].wHideAreaHeight = iTemp;
	}

	return HPR_OK;
}

/*******************************************************************
      Function:   CShelter::on_btnSave_clicked
     Parameter:  
           (IN)     无参数  
        Return:   无
   Description:   “确定”按钮的响应函数，用于保存参数
**********************************************************************/
void CShelter::on_btnSave_clicked()
{
	getEachWidgetValue();//保存参数
    close();
}

/*******************************************************************
      Function:   CShelter::showEvent
     Parameter:  
           (IN)     QShowEvent * event  窗口显示事件
        Return:   无
   Description:   对话框显示时，将参数中的值写入控件，显示出来
**********************************************************************/
void CShelter::showEvent ( QShowEvent * event )
{
	if(event ==NULL)
	{
	}
	//获取显示参数
	setEachWidgetValue();
}
