/***************************************************************************************
 *      Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *      FileName        :   treemodel.cpp
 *      Description     :   树形显示模式，与treeview配合使用
 *      Modification    :   无  
 *      Version         :   V1.0.0
 *      Time            :   2009-10,11
 *      Author          :   wanggp@hikvision.com
 **************************************************************************************/
#include <QIcon>
#include <QtGui>
#include "treeitem.h"
#include "treemodel.h"
 
/************************************************************************
 *        Function            :  TreeModel
 *        Description         :  构造函数
 *        Input               :  data parent
 *        Output              :  无
 *        Return              :  无
*************************************************************************/
 TreeModel::TreeModel(const QString  &data, QObject *parent)
     : QStandardItemModel(parent)
 {
     QList<QVariant> rootData;
     rootData << "DeviceTree";
     rootItem = new TreeItem(rootData);
     setupModelData(data.split(QString("\n")), rootItem);
 }
 
/************************************************************************
 *        Function            :  ~TreeModel
 *        Description         :  析构函数
 *        Input               :  无
 *        Output              :  无
 *        Return              :  无
*************************************************************************/
 TreeModel::~TreeModel()
 {
     delete rootItem;
 }

/************************************************************************
 *        Function            :  setupModelData
 *        Description         :  构造树形列表
 *        Input               :  lines  设备和通道信息
 								 parent 设备节点
 *        Output              :  无
 *        Return              :  无
*************************************************************************/
void TreeModel::setupModelData(const QStringList &lines, TreeItem *parent)
 {
    QList<TreeItem*> parents;
    parents << parent;
    QStandardItem *parentItem = invisibleRootItem();
 
	//QStandardItem *devicetreenode = new QStandardItem(QString("HIKVISION"));
	//devicetreenode->setEditable(0);
    
	
    //add by pyd
    QStandardItem *pDeviceRootItem = new QStandardItem(QString("device tree(right click to add device)"));
    //devicetreenode->appendRow(pDeviceRootItem);
    parentItem->appendRow(pDeviceRootItem);
	pDeviceRootItem->setIcon(QIcon(":/images/tree.bmp"));
	pDeviceRootItem->setEditable(0);
 
	QList<TreeItem*> parents1;
	QString deviceBegin("<device>");
	QString deviceEnd("</device>");
	QString channelBegin("<channel>");
	QString channelEnd("</channel>");

	QStandardItem *pDeviceItem = NULL;
	QStandardItem *pChannelItem = NULL;
    for ( QStringList::const_iterator it = lines.begin(); it != lines.end(); ++it )
    {   
     //设备节点
        if ((*it)==deviceBegin)
        {
			++it;
			pDeviceItem = new QStandardItem(*it);
			pDeviceRootItem->appendRow(pDeviceItem);
			pDeviceItem->setIcon(QIcon(":/images/logout.bmp"));
			pDeviceItem->setEditable(0);
        }
 
        //通道信息
        QString chn =channelBegin;
        if ((*it)==chn)
        {
			++it;
			pChannelItem = new QStandardItem(*it);
			pDeviceItem->appendRow(pChannelItem);
			pChannelItem->setIcon(QIcon(":/images/camera.bmp"));
			pChannelItem->setEditable(0);
        }
    }
}

