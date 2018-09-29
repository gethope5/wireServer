#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QSqlError>
#include <QDebug>

#define LOCALDEBUG 0//本地或服务器使用

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    f=false;
#if 1

    dbConnect=new QLabel(ui->statusBar);
    dbConnect->setFixedWidth(100);
    ui->statusBar->addPermanentWidget(dbConnect);
    ui->stackedWidget->setCurrentIndex(1);
    if(ui->stackedWidget->currentIndex()==1)
    {
        MeasureDB::bParse=true;
        setWindowTitle(QString::fromWCharArray(L"6C-分析程序-0625"));
    }
    else if(ui->stackedWidget->currentIndex()==0)
    {
        MeasureDB::bParse=false;
        setWindowTitle(QString::fromWCharArray(L"6C-采集程序-0625"));
    }

    connectDB();
    //    ui->lineEdit_ip->setText("127.0.0.1");
#if LOCALDEBUG
#else
    //    ui->lineEdit_ip->setText("120.25.233.115");
    ui->lineEdit_ip->setText("172.18.133.141");
#endif
    ui->lineEdit_port->setText("9090");
    count = 0;
    //startTimer(5000);
    sysDb=new MeasureDB(ui->tbIP,db) ;
    connect(sysDb,SIGNAL(updateInfo(QString)),this,SLOT(slot_showMessage(QString)));
    timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(slot_updateIndex()));

    QSettings cc("config.ini",QSettings::IniFormat);
    //        qDebug()<<"dd value"<<QTextCodec::codecForName("gb2313")->toUnicode(cc.value("dd").toByteArray());
    connect(ui->rdbCurDay,SIGNAL(clicked(bool)),this,SLOT(slot_filter(bool)));
    connect(ui->rdbCurMonth,SIGNAL(clicked(bool)),this,SLOT(slot_filter(bool)));
    connect(ui->rdbOther,SIGNAL(clicked(bool)),this,SLOT(slot_filter(bool)));
    ui->rdbCurDay->setChecked(true);
    ui->lneDate->setEnabled(false);

#if 0
    ui->lneDate->setAutoFillBackground(true);
    ui->pbtnSetDate->setAutoFillBackground(true);
    QPalette dd=ui->lneDate->palette();
    dd.setColor(QPalette::ButtonText ,Qt::blue);
    dd.setColor(QPalette::Button,Qt::red);

    ui->lneDate->setPalette(dd);
    ui->lneDate->update();
    ui->lneDate->setText("dd");
    ui->pbtnSetDate->setPalette(dd);
#else
    //    dd.setColor(Qt::red);
    ui->lneDate->setStyleSheet("color:red;");

    //进行供电段及对应的传感器初始化
    QStringList departments=sysDb->getDeparment();
    ui->lsDepartment->addItems(departments);
    if(departments.count())
        updateDeiveType(departments.at(0));
    ui->pbtnAnalysis->setToolTip(QString::fromWCharArray(L"当前处于暂停状态，单击开始分析"));
#endif
#endif
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_pushButton_startS_clicked()
{
    tcpServer = new QTcpServer(this);
    if(tcpServer->listen(QHostAddress(ui->lineEdit_ip->text()),ui->lineEdit_port->text().toInt()))
    {
        ui->statusBar->showMessage(QString::fromWCharArray(L"开启服务器成功"));
        connect(tcpServer,SIGNAL(newConnection()),this,SLOT(AcceptConnection()));
    }
    else
    {
        ui->statusBar->showMessage(QString::fromWCharArray(L"开启服务器失败。。。"));
    }
    sysDb->startUpdateTable();
    startTimer(5000);
}

void MainWindow::AcceptConnection()
{
    ++count;
    ui->label->setText(QString::fromWCharArray(L"连接数量:\t%1").arg(count));
    QTcpSocket *clientSocket = tcpServer->nextPendingConnection();
    clientSocket->setParent(this);
    connect(clientSocket,SIGNAL(readyRead()),this,SLOT(ReadData()));
    connect(clientSocket,SIGNAL(disconnected()),this,SLOT(Disconnected()));
    ui->statusBar->showMessage(QString::fromWCharArray(L"新连接成功"));
}

void MainWindow::Disconnected()
{
    QTcpSocket *client = qobject_cast<QTcpSocket *>(sender());
    for(int i=0;i!= deviceAddress.size();)
    {
        if(client == deviceAddress[i].tcpSocket)
        {
            deviceAddress.erase(deviceAddress.begin()+i);

        }
        else
        {
            ++i;
        }

    }
    --count;
    ui->label->setText(QString::fromWCharArray(L"连接数量\t%1").arg(count));
    //deleteLater();
}
//函数功能：TCP数据获取槽函数
void MainWindow::ReadData()
{
    QTcpSocket * client = qobject_cast<QTcpSocket *>(sender());
    QByteArray data = client->readAll();
    ui->statusBar->showMessage(QDateTime::currentDateTime().toString("hh-mm-ss")+" "+data.toHex());

    IPOrignal dd;
    dd.ipAddress=client->peerAddress().toString();
    dd.ipPort=QString::number(client->peerPort());
    dd.packages=data.toHex();
#if sub_thread
    sysDb->curIPData=dd;
    sysDb->start();
#else
#if separte_collect
    sysDb->insertIPPackageRecord(dd);
#else
    sysDb->insertIPPackageRecord(dd);
    //    qDebug()<<"insert status="<<
    //    sysDb->showDataTable(MeasureDB::table4);

    qint16 deviceNo;
    //    qDebug()<<"data"<<data<<data.count()<<data.toHex();
    sysDb->ParaseCmd(data.toHex());
    //更新设备地址表
    if(!IsConnectedDevice(deviceNo))
    {
        qDebug()<<QString::fromWCharArray(L"新设备:")<<deviceNo;
        DeviceAddress temp;
        temp.concentratorNo = concentratorNo;
        temp.No = deviceNo;
        temp.tcpSocket = client;
        deviceAddress.push_back(temp);
    }
#endif
#endif
}
void MainWindow::connectDB()
{

#if LOCALDEBUG
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setPort(3307);
    db.setDatabaseName("test_c6");
    db.setUserName("root");
    db.setPassword("123456");
#else

    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("120.25.233.115");
    db.setPort(3306);
    db.setDatabaseName("electricdata");
    db.setUserName("root");
    db.setPassword("mytianjun-mysql");
#endif
    if( db.open())
    {
        //QMessageBox::critical(this,"错误提示",db.lastError().text());
        //return false;
        //        ui->statusBar->showMessage(QString::fromWCharArray(L"数据库打开"));
        dbConnect->setText(QString::fromWCharArray(L"数据库打开"));
        //        qDebug()<<"show all tables="<<db.tables();
        if(ui->stackedWidget->currentIndex()==1)
        {
            QTimer::singleShot(3000,this,SLOT(on_pbtnReflash_clicked()));
        }
    }
    else
    {
        //        ui->statusBar->showMessage(QString::fromWCharArray(L"连接数据库失败")+db.lastError().text());
        dbConnect->setText(QString::fromWCharArray(L"数据库失败")+db.lastError().text());
    }
}
bool MainWindow::GetSocketByNo(qint16 deviceNo, QTcpSocket * &client,qint32 &cctNo)
{
    for(int i=0; i!=deviceAddress.size();++i)
    {
        if(deviceAddress[i].No == deviceNo)
        {
            client = deviceAddress[i].tcpSocket;
            cctNo = deviceAddress[i].concentratorNo;
            return true;
        }
    }
    return false;
}
bool MainWindow::IsConnectedDevice(qint16 deviceNo)
{
    for(int i=0;i != deviceAddress.size();++i)
    {
        if(deviceAddress[i].No == deviceNo)
            return true;
    }
    return false;
}
void MainWindow::on_tbIP_clicked(const QModelIndex &index)
{
    //    qDebug()<<"111"<<index.column()<<index.row();
    IPOrignal oneRecord;
    QByteArray tmp=sysDb->parseOnePackage(index.row(),oneRecord);
    qint16 deviceNo;
    //    sysDb-> ParaseCmd(tmp);
    qDebug()<<"cur package value="<<deviceNo;
}
void MainWindow::on_pbtnAnalysis_clicked()
{
    if(f)
    {
        ui->pbtnAnalysis->setText(QString::fromWCharArray(L"2 开始分析"));
        ui->pbtnAnalysis->setToolTip(QString::fromWCharArray(L"当前处于暂停状态，单击开始分析"));
        timer->stop();
    }
    else
    {
        ui->pbtnAnalysis->setText(QString::fromWCharArray(L"2 暂停分析"));
        ui->pbtnAnalysis->setToolTip(QString::fromWCharArray(L"当前处于分析状态，单击暂停分析"));
        timer->start(30000);
        sysDb->parseAll();
    }
    f=!f;
}
//函数功能：每隔3分钟，分析一次数据
void MainWindow::slot_updateIndex(void )
{
    //    qDebug()<<"see";
    //    ui->lblCurIndex->setText(sysDb->getCurIndex());
    sysDb->parseAll();
}
void MainWindow::slot_filter(bool f)
{
    QRadioButton *rdb=(QRadioButton *)this->sender();
    if(rdb==ui->rdbCurDay)
    {
        curDate=QDateTime::currentDateTime().toString("yyyy-MM-dd");
    }
    else if(rdb==ui->rdbCurMonth)
    {
        curDate=QDateTime::currentDateTime().toString("yyyy-MM");
    }
    else if(rdb==ui->rdbOther)
    {
        setDateDlg setDateUI(this);
        setDateUI.exec();
        curDate=setDateUI.getData();
    }
    ui->lneDate->setText(curDate);
}
void MainWindow::on_pbtnSetDate_clicked()
{
    if(!curDate.isNull())
        sysDb->showDataTable(MeasureDB::table4,curDate);
}
void MainWindow::slot_showMessage(QString str)
{
    this->statusBar()->showMessage(str);
}
//函数功能：根据当前供电段，更新所属的设备类型，如电屏铠、B值、张力等
void MainWindow::on_lsDepartment_clicked(const QModelIndex &index)
{
    //    qDebug()<<"index"<<index.data().toString();
    updateDeiveType(index.data().toString());
}
//函数功能：根据
void MainWindow::updateDeiveType(QString local)
{
    ui->lsDeviceType->clear();
    ui->lsDeviceType->addItems(sysDb->getDeviceType(local));
}
void MainWindow::on_pbtnReflash_clicked()
{
    sysDb->udapteTableName();
    QTime dd;
    dd.start();
    QStringList dd1=sysDb->getDeparment();
    ui->lsDepartment->addItems(dd1);
}
void MainWindow::on_pbtnUpdateTable_clicked()
{
    sysDb->showDataTable(MeasureDB::table4);
    ui->statusBar->showMessage(QString::fromWCharArray(L"表格已刷新......"));
}
