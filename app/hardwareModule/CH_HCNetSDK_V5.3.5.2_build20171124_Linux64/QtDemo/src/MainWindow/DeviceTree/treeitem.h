/***************************************************************************************
 *      Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *      FileName        :       treeitem.h
 *      Description     :       树型显示节点头文件
 *      Modification    :       无
 *      Version         :       V1.0.0
 *      Time            :       2009-10,11
 *      Author          :       wanggp@hikvision.com
 **************************************************************************************/

#ifndef TREEITEM_H_
#define TREEITEM_H_

#include <QList>
#include <QVariant>

class TreeItem
{
public:
    TreeItem(const QList<QVariant> &data, TreeItem *parent = 0);
    ~TreeItem();

    void appendChild(TreeItem *child);
    TreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
	bool setData(const QVariant &value);
    int row() const;
    TreeItem *parent();

private:
    //每个节点都可作为父节点下拉下许多子节点出来
    QList<TreeItem*> childItems;
    //节点数据，包含第一级设备树和第二级设备还有第三极通道相关信息
    //QVariant可以转化为任意结构体数据内容
    QList<QVariant> itemData;
    TreeItem *parentItem;
};
#endif /* TREEITEM_H_ */

