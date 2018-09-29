#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>
//#include <QTextCodec>
//qt5.8 mingw4.9
#include <qdebug.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug()<<"begin---------";
//    QTextCodec:::setCodecForCStrings(QTextCodec::codecForName("GBK")); // 关键是这句


    MainWindow w;
    w.show();
    return a.exec();
}
