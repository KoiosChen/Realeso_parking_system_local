/***************************************************************************************
 *      Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *      FileName        :       treeitem.h
 *      Description     :       树型显示节点头文件
 *      Modification    :       无
 *      Version         :       V1.0.0
 *      Time            :       2009-10,11
 *      Author          :       wanggp@hikvision.com
 **************************************************************************************/


#include <QStringList>
#include <QMessageBox>
#include "treeitem.h"

/************************************************************************
*        Function            :  TreeItem
*        Description         :  构造函数
*        Input               :  data,parent
*        Output              :  无
*        Return              :  无
*************************************************************************/
TreeItem::TreeItem(const QList<QVariant> &data, TreeItem *parent)
{
    parentItem = parent;
    itemData = data;
}

/************************************************************************
*        Function            :  ~TreeItem
*        Description         :  析构函数
*        Input               :  无
*        Output              :  无
*        Return              :  无
*************************************************************************/
TreeItem::~TreeItem()
{
    qDeleteAll(childItems);
}

/************************************************************************
*        Function            :  appendChild
*        Description         :  添加节点
*        Input               :  item
*        Output              :  无
*        Return              :  无
*************************************************************************/
void TreeItem::appendChild(TreeItem *item)
{
    childItems.append(item);
}

/************************************************************************
*        Function            :  child
*        Description         :  取孩子结点
*        Input               :  row
*        Output              :  无
*        Return              :  childItems
*************************************************************************/
TreeItem *TreeItem::child(int row)
{
    return childItems.value(row);
}

/************************************************************************
*        Function            :  childCount
*        Description         :  子节点计数
*        Input               :  无
*        Output              :  无
*        Return              :  无
*************************************************************************/
int TreeItem::childCount() const
{
    return childItems.count();
}

/************************************************************************
*        Function            :  columnCount
*        Description         :  列数计数
*        Input               :  无
*        Output              :  无
*        Return              :  子节点的总个数
*************************************************************************/
int TreeItem::columnCount() const
{
    return itemData.count();
}

/************************************************************************
*        Function            :  data
*        Description         :  某一列数据
*        Input               :  column
*        Output              :  无
*        Return              :  itemData.value
*************************************************************************/ 
QVariant TreeItem::data(int column) const
{
	QString data =itemData.value(column).toString();
	//QMessageBox::information(this,tr("TreeItem::data"),tr("data=%1").arg(data.toLatin1().data()));
	//printf("%s\n",data.toLatin1().data());
    return itemData.value(column);
}

bool TreeItem::setData(const QVariant &value)
{
	//printf("1232434534555=%s\n",value.toString().toLatin1().data());
  	//itemData.clear();
	//itemData.append(value);
	//printf("1232434534555=%s\n",value.toString().toLatin1().data());
		
	 	return true;
     
    // return false;


}

/************************************************************************
*        Function            :  parent
*        Description         :  取其父节点
*        Input               :  无
*        Output              :  无
*        Return              :  parentItem
*************************************************************************/
TreeItem *TreeItem::parent()
{
    return parentItem;
}

/************************************************************************
*        Function            :  row
*        Description         :  统计行数
*        Input               :  无
*        Output              :  无
*        Return              :  0 
*************************************************************************/
int TreeItem::row() const
{
    if (parentItem)
    {
        return parentItem->childItems.indexOf(const_cast<TreeItem*>(this));
    }
    return 0;
}

