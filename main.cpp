#include "mainwindow.h"
#include <QApplication>
//#include <QTextCodec>
//qt5.8 vs2015
#include <qdebug.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug()<<"begin---------";
    //    void MainWindow::Readregedit(bool isAutoRun)
    //    {
    QSettings *reg=new QSettings("HKEY_LOCAL_MACHINE//SOFTWARE//Microsoft""//Windows//CurrentVersion//Run",QSettings::NativeFormat);

    //开机自动运行
    if (true)
    {
        reg->setValue("app",QApplication::applicationFilePath());
    }
    else
    {
        reg->setValue("app","");
    }
    //    }
    MainWindow w;
    w.show();
    return a.exec();
}
