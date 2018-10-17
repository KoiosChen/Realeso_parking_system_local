/************************************************************************
 *        Copyright 2009-2011 Hikvision Digital Technology Co., Ltd.
 *        FileName                      :   common.h
 *        Description                   :   helper 
 *        Modification History          :   none
 *        Version                       :   V1.0
 *        Time                          :   2009-11,12
 *        Author                        :   wanggongpu
 *        Descrp                        :   none
*************************************************************************/
#ifndef COMMON_H
#define COMMON_H
#include <QMessageBox>

#define DEBUG 0
#ifdef DEBUG
#define DBG(a, b) \
	(QMessageBox::information(this,tr(a),tr("line=%1").arg(b)))
#else
#define DBG(a, b) 
#endif

#endif

