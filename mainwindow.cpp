#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QSqlError>
#include <QDebug>

#define LOCALDEBUG 0//本地或服务器使用

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),bExpandUI(false),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    dbConnect=new QLabel(ui->statusBar);
    dbConnect->setFixedWidth(100);
    ui->statusBar->addPermanentWidget(dbConnect);

    //    ui->lineEdit_ip->setText("127.0.0.1");
    ui->lineEdit_ip->setText("120.25.233.115");
    ui->lineEdit_port->setText("9090");
    count = 0;
    //startTimer(5000);
    sysDb=new tbDisplayModel(TABLE_NAME4,ui->tbIP) ;
    timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(slot_updateIndex()));
    ui->pbtnAnalysis->setEnabled(false);
    ui->test_2->setEnabled(false);
    ui->test->setEnabled(false);
    qDebug()<<"3";
    QSettings cc("config.ini",QSettings::IniFormat);
    //    qDebug()<<"dd value"<<QTextCodec::codecForName("gb2313")->toUnicode(cc.value("dd").toByteArray());
    connect(ui->rdbCurDay,SIGNAL(clicked(bool)),this,SLOT(slot_filter(bool)));
    connect(ui->rdbCurMonth,SIGNAL(clicked(bool)),this,SLOT(slot_filter(bool)));
    connect(ui->rdbOther,SIGNAL(clicked(bool)),this,SLOT(slot_filter(bool)));
    connect(sysDb,SIGNAL(db_status(bool)),this,SLOT(slot_dbStatus(bool)));
    ui->rdbCurDay->setChecked(true);
    ui->lneDate->setEnabled(false);
    lblStatus=new QLabel(ui->statusBar);
    lblStatus->setFixedWidth(100);
    lblStatus->setText(QString::fromWCharArray(L"服务器未连接"));
    ui->statusBar->addPermanentWidget(lblStatus);
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
#endif
    //进行供电段及对应的传感器初始化
    QStringList departments=sysDb->getDeparment();
    ui->lsDepartment->addItems(departments);
    if(departments.count())
        updateDeiveType(departments.at(0));
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::timerEvent(QTimerEvent *)
{
    this->on_test_clicked();
    qDebug()<<"eror";
}
void MainWindow::on_pushButton_startS_clicked()
{
    tcpServer = new QTcpServer(this);
    if(tcpServer->listen(QHostAddress(ui->lineEdit_ip->text()),ui->lineEdit_port->text().toInt()))
    {
        lblStatus->setText(QString::fromWCharArray(L"开启服务器成功"));
        connect(tcpServer,SIGNAL(newConnection()),this,SLOT(AcceptConnection()));
    }
    else
    {
        lblStatus->setText(QString::fromWCharArray(L"开启服务器失败"));
    }
    //    startTimer(5000);
}

void MainWindow::AcceptConnection()
{
    ++count;
    ui->label->setText(QString::fromWCharArray(L"count\t%1").arg(count));
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
void MainWindow::ReadData()
{
    QTcpSocket * client = qobject_cast<QTcpSocket *>(sender());
    QByteArray data = client->readAll();
    ui->statusBar->showMessage(data.toHex());

#if 0
    QFile file("originalData.txt");


    if(!file.open(QIODevice::ReadWrite|QIODevice::Text | QIODevice::Append))
        ui->statusBar->showMessage(QString::fromWCharArray(L"打开文件失败"));
    else
    {
        //保持原始数据
        QTextStream outfile(&file);
        outfile<<QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss")<<endl;
        outfile<<data.toHex()<<endl;
        file.close();
    }
#else
    IPOrignal dd;
    dd.ipAddress=client->peerAddress().toString();
    dd.ipPort=QString::number(client->peerPort());
    dd.packages=data.toHex();
    sysDb->insertIPPackageRecord(dd);
    //    qDebug()<<"insert status="<<
    sysDb->showtable(TABLE_NAME4);
#endif
    qint16 deviceNo;
    qDebug()<<"data"<<data<<data.count()<<data.toHex();
    sysDb->ParaseCmd(data.toHex(),deviceNo);

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
}
void MainWindow::UpdataAddress(qint16 no,QString ip,qint16 port)
{
    //    QSqlQuery queryAddress;
    //    queryAddress.prepare("SELECT *FROM devieceAddress WHERE no = ?");
    //    queryAddress.bindValue(0,no);
    //    if(!queryAddress.exec())
    //    {
    //        qDebug()<<queryAddress.lastError();
    //        return;
    //    }
    //    if(queryAddress.size() == 0)
    //    {
    //        //插入数据
    //        queryAddress.prepare("INSERT INTO devieceAddress VALUES(?,?,?)");
    //        queryAddress.bindValue(0,ip);
    //        queryAddress.bindValue(1,port);
    //        queryAddress.bindValue(2,no);

    //        qDebug()<<"port:"<<port;
    //        if(!queryAddress.exec())
    //        {
    //            qDebug()<<queryAddress.lastError();
    //        }
    //    }
    //    else
    //    {
    //        //更新数据
    //        queryAddress.prepare("UPDATE devieceAddress SET ip = ?,port = ? WHERE no = ?");
    //        queryAddress.bindValue(0,ip);
    //        queryAddress.bindValue(1,port);
    //        queryAddress.bindValue(2,no);
    //        if(!queryAddress.exec())
    //        {
    //            qDebug()<<queryAddress.lastError();
    //        }
    //    }
}

//void MainWindow::on_pushButton_clicked()
//{
//    QSqlQuery queryCmd;
//    QByteArray cmd;
//    if(!queryCmd.exec("SELECT *FROM devieceCmd"))
//    {
//        qDebug()<<queryCmd.lastError();
//    }
//    qint16 no;
//    while(queryCmd.next())
//    {
//        no = queryCmd.value(0).toInt();
//        cmd = queryCmd.value(1).toByteArray();
//        qDebug()<<no;
//        qDebug()<<cmd;
//    }
//}

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



void MainWindow::on_test_clicked()
{
    QSqlQuery query;
    qint16 no;
    QByteArray cmd;
    query.exec("SELECT *FROM deviececmd");
    while(query.next())
    {
        no = query.value(0).toInt();

        cmd = query.value(1).toByteArray();

        QByteArray orgCmd = cmd;//保护原始的命令以便发送成功后删除
        QTcpSocket *client = NULL;
        qint32 cctNo;//集中器编号
        if(GetSocketByNo(no,client,cctNo))
        {
            if(client->isValid())
            {
                QByteArray bytes;
                QDataStream out(&bytes,QIODevice::ReadWrite);
                out<<qint32(cctNo);

                cmd.push_front(bytes);

                //计算校验和
                qint8 checksum = 0;
                for(int i =0;i!=12;++i)
                {

                    checksum += qint8(cmd.at(i+1));
                }
                qDebug()<<QString::fromWCharArray(L"校验和:")<<checksum;

                //写入校验和以及开始字符EF
                cmd[0] = qint8(0xEF);
                cmd[13] = checksum;
                client->write(cmd);


                ui->statusBar->showMessage(QString::fromWCharArray(L"已发送命令:")+cmd.toHex());

                query.prepare(QString::fromWCharArray(L"DELETE FROM deviececmd WHERE 设备编号 = ? AND 命令 = ?"));
                query.bindValue(0,no);
                query.bindValue(1,orgCmd);
                if(!query.exec())
                {
                    qDebug()<<QString::fromWCharArray(L"删除命令失败");
                }
                break;//中断发送等待,以防止速度过快对方无法接收
            }
        }
    }
}
void MainWindow::on_pbtnUIExpand_clicked()
{
    if(bExpandUI)
    {
        ui->groupBox->setVisible(true);
        //        ui->groupBox_2->setVisible(true);
    }
    else
    {
        ui->groupBox->setVisible(false);
        //        ui->groupBox_2->setVisible(false);
    }
    bExpandUI=!bExpandUI;
}

void MainWindow::on_tbIP_clicked(const QModelIndex &index)
{
    //    qDebug()<<"111"<<index.column()<<index.row();
    IPOrignal dd;
    QByteArray tmp=sysDb->parseOnePackage(index.row(),dd);
    qint16 deviceNo;
    sysDb-> ParaseCmd(tmp,deviceNo);
    qDebug()<<"cur package value="<<deviceNo;
}

void MainWindow::on_pbtnAnalysis_clicked()
{
    timer->start(1000);
    sysDb->parseAll();
}
void MainWindow::slot_updateIndex(void )
{
    qDebug()<<"see";
    ui->lblCurIndex->setText(sysDb->getCurIndex());
}
QString GBK2UTF8(const QString &inStr)
{
    QTextCodec *gbk = QTextCodec::codecForName("GB18030");
    QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");

    QString g2u = gbk->toUnicode(gbk->fromUnicode(inStr));              // gbk  convert utf8
    return g2u;
}
inline QString UTF82GBK(const QString &inStr)
{
    QTextCodec *gbk = QTextCodec::codecForName("GB18030");
    QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");

    QString utf2gbk = gbk->toUnicode(inStr.toLocal8Bit());
    return utf2gbk;
}
void MainWindow::on_test_2_clicked()
{
    qDebug()<<"insert record"<<sysDb->insertDeviceStatus();
    QSettings cc("config.ini",QSettings::IniFormat);
    //    cc.value(dd).toBitArray();

    qDebug()<<"dd value"<</*QString::fromUtf8*/(cc.value("dd","d").toString())<<cc.value("cc","d").toString();
    //    cc.setValue("cc","擦擦擦");
    //    QFile file("utf8.txt");
    //    if(file.open(QFile::ReadWrite))
    //    {
    //      qDebug()<<"dlj"<<QString::fromUtf8(file.readAll());
    //      QTextStream cc1(&file);
    //      qDebug()<<"text value"<<ui->pushButton_startS->text();
    //      wchar_t d[100];

    //      ui->pushButton_startS->text().toWCharArray(d);
    //      cc1<<GBK2UTF8(ui->pushButton_startS->text());
    //      QString::toUtf8();
    //      qDebug()<<"dlj"<<QString::fromUtf8(file.readAll());
    //      file.close();
    //}
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
        sysDb->showtable(TABLE_NAME4);
//    (tbDisplayModel::table4,curDate);
}
void MainWindow::slot_dbStatus(bool flag)
{
    if(flag)
    {
        dbConnect->setText(QString::fromWCharArray(L"MYSQL已打开"));
    }
    else
    {
        dbConnect->setText(QString::fromWCharArray(L"MYSQL未打开"));
    }
}

void MainWindow::on_pushButton_clicked()
{
    QTime dd;
    dd.start();
    qDebug()<<"all device"<<sysDb->getTableName("0600040002");
    QStringList dd1=sysDb->getDeparment();
    ui->lsDepartment->addItems(dd1);

    for(int i=0;i<dd1.count();i++)
    {
        qDebug()<<"department "<<dd1.at(i)<<sysDb->getDeviceType(dd1.at(i));
    }
}

void MainWindow::on_lsDepartment_clicked(const QModelIndex &index)
{
//    qDebug()<<"index"<<index.data().toString();
    updateDeiveType(index.data().toString());
}
void MainWindow::updateDeiveType(QString local)
{
    ui->lsDeviceType->clear();
    ui->lsDeviceType->addItems(sysDb->getDeviceType(local));
}
