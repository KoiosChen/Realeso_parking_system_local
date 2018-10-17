/*
 * Copyright(C) 2010,Hikvision Digital Technology Co., Ltd 
 * 
 * 文件名称：main.cpp
 * 描    述：
 * 当前版本：1.0
 * 作    者：王公仆
 * 创建日期：2009年11月12日
 * 修改记录：
 */

#include "qtclientdemo.h"
#include <QTextCodec>
#include <QtGui>
#include <QApplication>


//主窗口
QtClientDemo* gqtclinetdemo;

/*******************************************************************
      Function:   main
   Description:   主函数入口
     Parameter:   (IN)   int argc  
                  (IN)   char *argv[]  
        Return:   0--成功，-1--失败。   
**********************************************************************/
int main(int argc, char *argv[])
{
	gqtclinetdemo = NULL;   
    QApplication a(argc, argv);
	QTextCodec::setCodecForTr(QTextCodec::codecForName("GB2312"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB2312"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GB2312"));

    //Add translation file.
    QTranslator translator;
    BOOL bSuc;
    bSuc = translator.load("..//..//..//..//translation//QtDemo_zh_CN");
    if (!bSuc)
    {
         bSuc = translator.load("..//..//translation//QtDemo_zh_CN");
    }
    if (!bSuc)
    {
        translator.load("translation/QtDemo_zh_CN");
    }
    if (!bSuc)
    {
        translator.load("./QtDemo_zh_CN");
    }
    a.installTranslator(&translator);

    //Show the main window.
    QtClientDemo w;
	w.show();
	gqtclinetdemo =&w;
    return a.exec();
}
