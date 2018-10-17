#include <QtGui/QApplication>
#include <QTextCodec>

#include "cconfigureparams.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("GB2312"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB2312"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GB2312"));
    CConfigureParams w;
    //w.setGeometry(100, 100, 832, 610);
    w.show();
    w.showLoginBtn();

    return a.exec();
}
