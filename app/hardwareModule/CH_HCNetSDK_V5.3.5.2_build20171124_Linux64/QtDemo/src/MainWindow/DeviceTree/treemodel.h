/***************************************************************************************
 *      Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *      FileName        :   treemodel.h
 *      Description     :   树形显示模式，与treeview配合使用
 *      Modification    :   无  
 *      Version         :   V1.0.0
 *      Time            :   2009-10,11
 *      Author          :   wanggp@hikvision.com
 **************************************************************************************/

#ifndef TREEMODEL_H_
#define TREEMODEL_H_

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QString>
#include <QObject>
#include <QStringList>
#include <QStandardItemModel>



class TreeItem;

class TreeModel : public QStandardItemModel
{
    Q_OBJECT
		
public:
	
	void setupModelData(const QStringList &lines, TreeItem *parent);
    TreeModel(const QString &data, QObject *parent = 0);
    ~TreeModel();

private:
  
    TreeItem *rootItem;
};

#endif /* TREEMODEL_H_ */

