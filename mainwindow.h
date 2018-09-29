﻿#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDateTime>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVector>
#include <qlabel.h>
#include "database.h"

#include "setdatedlg.h"

struct DeviceAddress
{
    qint16 No;
    qint32 concentratorNo;
    QTcpSocket * tcpSocket;

};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
    bool f;

private slots:
    void on_pushButton_startS_clicked();
    void AcceptConnection();
    void Disconnected();
    void ReadData();
    //    void on_pushButton_clicked();

    void on_tbIP_clicked(const QModelIndex &index);

    void on_pbtnAnalysis_clicked();
    void slot_updateIndex(void );
    void slot_filter(bool);    
    void on_pbtnSetDate_clicked();
    void slot_showMessage(QString);    
    void on_lsDepartment_clicked(const QModelIndex &index);
    void updateDeiveType(QString local);
    void on_pbtnReflash_clicked();

    void on_pbtnUpdateTable_clicked();

private:
    bool GetSocketByNo(qint16 deviceNo,QTcpSocket * &client,qint32 &cctNo);
    bool IsConnectedDevice(qint16 deviceNo);//判断设备是否已经连接
    void connectDB();

    //QVector<QPair<qint16,QTcpSocket*> > deviceAddress;
    QVector<DeviceAddress> deviceAddress;
    qint32 concentratorNo;
    qint16 count ;
    QSqlDatabase db;
    QTcpServer *tcpServer;
    Ui::MainWindow *ui;
    MeasureDB *sysDb;
    QLabel *dbConnect;
    QTimer *timer;

    QString curDate;
};
#endif // MAINWINDOW_H
