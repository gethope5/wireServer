#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>
#include <QSplashScreen>

//#include <QTextCodec>
//qt5.8 mingw4.9
#include <qdebug.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug()<<"begin---------";
//    QTextCodec:::setCodecForCStrings(QTextCodec::codecForName("GBK")); // 关键是这句
    QSplashScreen *pSplash = new QSplashScreen;
//    pSplash->setPixmap(QPixmap(":/images/splash.jpg"));
    pSplash->show();
  //  pSplash->showMessage(QApplication::translate("pSplash", "\345\212\240\350\275\275\344\270\255,\350\257\267\347\250\215\345\220\216", Q_NULLPTR),Qt::AlignCenter,Qt::blue);

    pSplash->show();
          a.processEvents();
    MainWindow w;
    w.show();
    pSplash->finish(&w);
    delete pSplash;
    return a.exec();
}
