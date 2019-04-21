#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QSqlError>
#include <QDebug>

#define LOCALDEBUG 0//本地或服务器使用
QVector<simData> MainWindow::simDeviceInfo;         //存储了所有模拟设备的信息
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    f=false;
    dbConnect=new QLabel(ui->statusBar);
    dbConnect->setFixedWidth(120);
    ui->statusBar->addPermanentWidget(dbConnect);

    connectDB();
    //ui->lneCollectIP->setText("127.0.0.1");
    ui->lneCollectIP->setText("120.25.233.115");
    //ui->lneCollectIP->setText("172.18.133.141");
    ui->lneCollectPort->setText("9090");
    count = 0;
    //startTimer(5000);
    sysDb=new MeasureDB(ui->tbIP,db) ;
    connect(sysDb,SIGNAL(updateInfo(QString)),this,SLOT(slot_showMessage(QString)));
    parseTimer=new QTimer(this);
    connect(parseTimer,SIGNAL(timeout()),this,SLOT(slot_parseUpdate()));

    //qDebug()<<"dd value"<<QTextCodec::codecForName("gb2313")->toUnicode(cc.value("dd").toByteArray());
    connect(ui->rdbCollectDay,SIGNAL(clicked(bool)),this,SLOT(slot_filter(bool)));
    connect(ui->rdbCollectMonth,SIGNAL(clicked(bool)),this,SLOT(slot_filter(bool)));
    connect(ui->rdbCollectOther,SIGNAL(clicked(bool)),this,SLOT(slot_filter(bool)));


    ui->rdbCollectDay->setChecked(true);
    ui->lneDate->setEnabled(false);
    //    dd.setColor(Qt::red);
    ui->lneDate->setStyleSheet("color:red;");

    //进行供电段及对应的传感器初始化
    QStringList departments=sysDb->getDeparment();
    ui->lsParseDepart->addItems(departments);
    if(departments.count())
        updateDeiveType(departments.at(0));
    ui->pbtnParseUpdate->setToolTip(QString::fromWCharArray(L"当前处于暂停状态，单击开始分析..."));
    ui->lstSimDevices->setToolTip(QString::fromWCharArray(L"双击查看设备信息..."));
    ui->stackedWidget->setCurrentIndex(2);
    switch(ui->stackedWidget->currentIndex())
    {
    case (1):
    {
        MeasureDB::bParse=true;
        setWindowTitle(QString::fromWCharArray(L"6C-分析程序-1116"));
        break;
    }
    case (0):
    {
        MeasureDB::bParse=false;
        setWindowTitle(QString::fromWCharArray(L"6C-采集程序-0625"));
    }
    case (2):
    {
        setWindowTitle(QString::fromWCharArray(L"6C-模拟程序-0102"));
        simUiInitial();
        sysDb->udapteTableName();
        break;
    }
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_pbtnCollectStart_clicked()
{
    tcpServer = new QTcpServer(this);
    if(tcpServer->listen(QHostAddress(ui->lneCollectIP->text()),ui->lneCollectPort->text().toInt()))
    {
        ui->statusBar->showMessage(QString::fromWCharArray(L"开启服务器成功"));
        connect(tcpServer,SIGNAL(newConnection()),this,SLOT(AcceptConnection()));
    }
    else
    {
        ui->statusBar->showMessage(QString::fromWCharArray(L"开启服务器失败。。。"));
    }
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
    sysDb->insertIPPackageRecord(dd);
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
        dbConnect->setText(QString::fromWCharArray(L"error")+db.lastError().text());
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
void MainWindow::on_pbtnParseUpdate_clicked()
{
    if(f)
    {
        ui->pbtnParseUpdate->setText(QString::fromWCharArray(L"2 开始分析"));
        ui->pbtnParseUpdate->setToolTip(QString::fromWCharArray(L"当前处于暂停状态，单击开始分析"));
        parseTimer->stop();
    }
    else
    {
        ui->pbtnParseUpdate->setText(QString::fromWCharArray(L"2 暂停分析"));
        ui->pbtnParseUpdate->setToolTip(QString::fromWCharArray(L"当前处于分析状态，单击暂停分析"));
        parseTimer->start(30000);
    }
    f=!f;
}
//函数功能：每隔3分钟，分析一次数据
void MainWindow::slot_parseUpdate(void )
{
    sysDb->parseAll();
}
void MainWindow::slot_filter(bool f)
{
    QRadioButton *rdb=(QRadioButton *)this->sender();
    if(rdb==ui->rdbCollectDay)
    {
        curDate=QDateTime::currentDateTime().toString("yyyy-MM-dd");
    }
    else if(rdb==ui->rdbCollectMonth)
    {
        curDate=QDateTime::currentDateTime().toString("yyyy-MM");
    }
    else if(rdb==ui->rdbCollectOther)
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
void MainWindow::on_lsParseDepart_clicked(const QModelIndex &index)
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
    ui->lsParseDepart->addItems(dd1);
}
void MainWindow::on_pbtnUpdateTable_clicked()
{
    sysDb->showDataTable(MeasureDB::table4);
    ui->statusBar->showMessage(QString::fromWCharArray(L"表格已刷新......"));
}


//函数功能：模拟数据仿真部分的界面初始化
void MainWindow::simUiInitial(void)
{
    //        QTimer::singleShot(1000,this,SLOT(updateLists()));
    simTimer=new QTimer;
    connect(simTimer,                   SIGNAL(timeout()),      this,SLOT(slot_simTimer()));
    connect(ui->pbtnSimDel,             SIGNAL(clicked()),      this,SLOT(slot_simPbtn()));
    connect(ui->pbtnSimStart,           SIGNAL(clicked()),      this,SLOT(slot_simPbtn()));
    connect(ui->pbtnSimDeviceUpdate,    SIGNAL(clicked()),      this,SLOT(slot_simPbtn()));
    connect(ui->pbtnSimSavePar,         SIGNAL(clicked()),      this,SLOT(slot_simPbtn()));
    connect(ui->pbtnSimDataUpdate,      SIGNAL(clicked()),      this,SLOT(slot_simPbtn()));
    connect(ui->pbtnSetEndTime,         SIGNAL(clicked()),      this,SLOT(slot_simPbtn()));
    connect(ui->pbtnSetStartTime,       SIGNAL(clicked()),      this,SLOT(slot_simPbtn()));
    connect(ui->pbtnHistoryEndTime,     SIGNAL(clicked()),      this,SLOT(slot_simPbtn()));
    connect(ui->pbtnHistoryStartTime,   SIGNAL(clicked()),      this,SLOT(slot_simPbtn()));

    connect(ui->lneDeviceNO,            SIGNAL(textChanged(const QString )),  this,SLOT(slot_simSavePar(const QString )));
    connect(ui->lneDataNO,              SIGNAL(textChanged(const QString )),  this,SLOT(slot_simSavePar(const QString )));
    connect(ui->lneSimStart,            SIGNAL(textChanged(const QString )),  this,SLOT(slot_simSavePar(const QString )));
    connect(ui->lneSimEnd,              SIGNAL(textChanged(const QString )),  this,SLOT(slot_simSavePar(const QString )));
    connect(sysDb,                      SIGNAL(updateDevice()),             this,SLOT(slot_udpateSimDevice()));
    connect(ui->lstSimAll,              SIGNAL(doubleClicked(QModelIndex)), this,SLOT(slot_simDoubleList(QModelIndex)));
    connect(ui->lstSimDevices,          SIGNAL(doubleClicked(QModelIndex)), this,SLOT(slot_simDoubleList(QModelIndex)));
    ui->pbtnSimDeviceUpdate->setToolTip(QString::fromWCharArray(L"刷新模拟数据"));
    ui->lstSimAll->setToolTip(QString::fromWCharArray(L"双击添加设备"));
    ui->pbtnSetEndTime->setToolTip(QString::fromWCharArray(L"设置结束时间..."));
    ui->pbtnSetStartTime->setToolTip(QString::fromWCharArray(L"设置起始时间..."));
    ui->rdbSimReal->setChecked(true);
    connect(ui->rdbSimReal,SIGNAL(clicked()),this,SLOT(slot_SimSetType()));
    connect(ui->rdbSimHistory,SIGNAL(clicked()),this,SLOT(slot_SimSetType()));
    ui->rdbSimReal->click();
}
void MainWindow::slot_simSavePar(const QString tmp )
{
    //qDebug()<<"sim par is changed..."<<tmp;
    ui->statusBar->showMessage(QString::fromWCharArray(L"参数已变化..."));
}
//函数功能：保存模拟参数
void MainWindow::simSavePar(void)
{
    for(int i=0;i<simDeviceInfo.count();i++)
    {
        if(simDeviceInfo.at(i).deviceNo.contains(ui->lneDeviceNO->text()))
        {
            if((!ui->lneDataNO->text().isEmpty())&&
                    (!ui->lneSimStart->text().isEmpty())&&
                    (!ui->lneSimEnd->text().isEmpty()))
            {
                simData tmp=simDeviceInfo.at(i);
                tmp.dataNo=ui->lneDataNO->text();
                tmp.dataStartTm=ui->lneSimStart->text();
                tmp.dataEndTm=ui->lneSimEnd->text();
                tmp.dataTable=sysDb->getDeviceTable(tmp.dataNo);
                tmp.deviceTable=sysDb->getDeviceTable(tmp.deviceNo );

                if(tmp.dataTable.isEmpty()||tmp.deviceTable.isEmpty())
                    continue;
                simDeviceInfo.replace(i,tmp);
                qDebug()<<"cur save par"<<simDeviceInfo.at(i).deviceNo<<simDeviceInfo.at(i).dataNo
                       <<simDeviceInfo.at(i).dataTable<<simDeviceInfo.at(i).dataStartTm<<simDeviceInfo.at(i).dataEndTm;
                ui->statusBar->showMessage(QString::fromWCharArray(L"参数已保存"),3000);
            }
            else
            {
                ui->statusBar->showMessage(QString::fromWCharArray(L"参数错误,请重新配置"),3000);
            }
        }
    }
}
//函数功能：用于模拟数据的实时更新
void MainWindow::slot_simTimer(void)
{
    simData tmpDatas;
    // sysDb->transaction();
    //qDebug()<<"i will insert sim data"<<simTimer->interval();
    //对所有的模拟设备进行刷新
    qDebug()<<"sim count"<<simDeviceInfo.count()<<simTimer->interval();
    for(int i=0;i<simDeviceInfo.count();i++)
    {
        tmpDatas=simDeviceInfo.at(i);
        QString tm=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        updateSimData(tmpDatas,tm);
        simDeviceInfo.replace(i,tmpDatas);
    }
    //sysDb->rollback();
}
//函数功能：更新模拟历史数据(即给一个时间段和时间间隔，根据模拟数据进行数据更新)
void MainWindow::updateSimHistory(void)
{
    for(int i=0;i<simDeviceInfo.count();i++)
    {
        simData tmpDatas;
        tmpDatas=simDeviceInfo.at(i);
        //找到需要进行历史更新的设备
        if(tmpDatas.deviceNo.contains(ui->lneDeviceNO->text()))
        {
            for(int idx=0;idx<simHistoryTms.count();idx++)
            {

                QString tm=simHistoryTms.at(idx);
                qDebug()<<"sim history"<<idx<<tm;
                updateSimData(tmpDatas,tm);
            }
            break;
        }
        simDeviceInfo.replace(i,tmpDatas);
    }
}
//函数功能：设置数据仿真的格式：实时仿真或历史数据插入仿真
void MainWindow::slot_SimSetType(void)
{
    if(ui->rdbSimReal->isChecked())
    {
        ui->stkSetType->setCurrentWidget(ui->pgSimReal);
    }
    else
    {
        ui->stkSetType->setCurrentWidget(ui->pgSimHistory);
    }
}
//函数功能：将当前所有的设备信息进行显示
void MainWindow::slot_udpateSimDevice(void)
{
    ui->lstSimAll->clear();
    for(int i=0;i<MeasureDB::deviceInfo.deviceNo.count();i++)
    {
        //                      编号+表格名称+供电段
        ui->lstSimAll->addItem(MeasureDB::deviceInfo.deviceNo.at(i)+","+MeasureDB::deviceInfo.tableName.at(i)+","+MeasureDB::deviceInfo.departments.at(i));
    }
}
//函数功能：两个作用，1是将当前器件信息添加确认为模拟器件，2是展示模拟器件的信息
void MainWindow::slot_simDoubleList(const QModelIndex &index)
{
    QListWidget *lst=(QListWidget *)this->sender();
    if(lst==ui->lstSimAll)
    {
        //qDebug()<<"cur value"<<index.data().toString();
        QString curDeviceId;
        if(index.data().toString().split(",").count()>1)
            curDeviceId=index.data().toString().split(",").at(0);
        addSimDevice(curDeviceId);
    }
    else if(lst==ui->lstSimDevices)
    {
        for(int i=0;i<simDeviceInfo.count();i++)
        {
            if(simDeviceInfo.at(i).deviceNo.contains(index.data().toString()))
            {
                ui->lneDeviceNO->setText(simDeviceInfo.at(i).deviceNo);
                ui->lneDataNO->setText(simDeviceInfo.at(i).dataNo);
                ui->lneSimStart->setText(simDeviceInfo.at(i).dataStartTm);
                ui->lneSimEnd->setText(simDeviceInfo.at(i).dataEndTm);
                ui->lneSimDataCounts->setText(QString::number(simDeviceInfo.at(i).dataCount));
                ui->lneLastUpdate->setText(simDeviceInfo.at(i).recentTm);
            }
        }
    }
}
void MainWindow::slot_simPbtn(void)
{
    QPushButton *pbtn=(QPushButton *)this->sender();
    setDateDlg setDateUI(this);
    if(pbtn==ui->pbtnSimDeviceUpdate)
    {
        slot_udpateSimDevice();
    }
    else if(pbtn==ui->pbtnSimDel)
    {
        //    qDebug()<<"before devices"<<simDevices;
        qDebug()<<"del "<<ui->lstSimDevices->currentItem()->text();
        if(simDeviceInfo.count())
        {
            for(int i=0;i<simDeviceInfo.count();i++)
            {
                if(simDeviceInfo.at(i).deviceNo.contains(ui->lstSimDevices->currentItem()->text()))
                {
                    simDeviceInfo.remove(i);
                }
            }
            ui->lstSimDevices->takeItem(ui->lstSimDevices->currentIndex().row());
        }
    }
    else if(pbtn==ui->pbtnSimDataUpdate)
    {
        ui->statusBar->showMessage(QString::fromWCharArray(L"数据更新中..."));
        upateSimData();
        ui->statusBar->showMessage(QString::fromWCharArray(L"数据更新完成"),3000);
    }
    else if(pbtn==ui->pbtnSimSavePar)
    {
        simSavePar();
    }
    else if(pbtn==ui->pbtnSimStart)
    {
        //历史时间段仿真
        if(ui->stkSetType->currentWidget()==ui->pgSimHistory)
        {
            if((!ui->lneSimEnd_History->text().trimmed().isEmpty())&&
                    (!ui->lneSimStart_History->text().trimmed().isEmpty())&&
                    (!ui->lneSimTmInterval_history->text().trimmed().isEmpty())&&
                    (!ui->lneDataNO->text().trimmed().isEmpty())&&
                    (!ui->lneDeviceNO->text().trimmed().isEmpty()))
            {
                if(simTimer->isActive())
                {
                    simTimer->stop();
                }
                ui->statusBar->showMessage(QString::fromWCharArray(L"仿真中..."));
                //生成模拟的时间序列
                updateSimTime(ui->lneSimStart_History->text(),ui->lneSimEnd_History->text(),ui->lneSimTmInterval_history->text(),simHistoryTms);
                //数据刷新
                updateSimHistory();
                ui->statusBar->showMessage(QString::fromWCharArray(L"仿真已停止..."));
            }
            else
            {
                ui->statusBar->showMessage(QString::fromWCharArray(L"历史仿真设置错误..."));
                return ;
            }
        }
        //实时仿真
        else
        {
            int tm=ui->lneSimTmInterval_real->text().toInt()*60000;
            if(tm>0)
            {
                if(simTimer->isActive())
                {
                    simTimer->stop();
                    ui->pbtnSimStart->setText(QString::fromWCharArray(L"开始仿真"));
                    ui->statusBar->showMessage(QString::fromWCharArray(L"仿真已停止..."));
                }
                else
                {
                    simTimer->start(tm);
                    ui->pbtnSimStart->setText(QString::fromWCharArray(L"停止仿真"));
                    ui->statusBar->showMessage(QString::fromWCharArray(L"仿真中..."));
                }
            }
            else
            {
                ui->statusBar->showMessage(QString::fromWCharArray(L"仿真设置错误..."));
            }
        }
    }
    //设置数据源起止时间
    else if(pbtn==ui->pbtnSetEndTime)
    {
        setDateUI.exec();
        ui->lneSimEnd->setText(setDateUI.getData());
    }
    //设置数据源起止时间
    else if(pbtn==ui->pbtnSetStartTime)
    {
        setDateUI.exec();
        ui->lneSimStart->setText(setDateUI.getData());
    }
    //设置插入历史记录的起止时间
    else if(pbtn==ui->pbtnHistoryEndTime)
    {
        setDateUI.exec();
        ui->lneSimEnd_History->setText(setDateUI.getData());
    }
    //设置插入历史记录的起止时间
    else if(pbtn==ui->pbtnHistoryStartTime)
    {
        setDateUI.exec();
        ui->lneSimStart_History->setText(setDateUI.getData());
    }
}
//函数功能：根据起止时间及时间间隔，产生即将模拟的历史数据的时间序列
void  MainWindow::updateSimTime(QString start,QString end,QString interval,QStringList & tmList)
{
    int tmInterval=interval.trimmed().toInt();
    QDateTime startTm=QDateTime::fromString(start,"yyyy-MM-dd");
    QDateTime endTm=QDateTime::fromString(end,"yyyy-MM-dd");
    QDateTime tmpDT;
    for(int i=0;tmpDT<endTm;i++)
    {
        tmpDT=startTm.addSecs(tmInterval*60*i);
        tmList<<tmpDT.toString("yyyy-MM-dd hh:mm:ss");
    }
}
simData MainWindow::updateSimPar(QString devcieId,QString dataId)
{
    simData tmpSimDevice;
    tmpSimDevice.deviceNo=devcieId;
    tmpSimDevice.dataNo=dataId;
    tmpSimDevice.deviceTable=sysDb->getDeviceTable(devcieId );
    tmpSimDevice.dataTable=sysDb->getDeviceTable(dataId );
    tmpSimDevice.eType=sysDb->getType(devcieId);
    return tmpSimDevice;
}
void MainWindow::addSimDevice(QString curDeviceId)
{
    //QVector<QPair<QString,QString>> simDevices;
    bool flag=true;
#if 1
    //0418修复了以前在模拟设备之间切换参数的时候，参数未保存的bug
    for(int i=0;i<simDeviceInfo.count();i++)
    {
        if(simDeviceInfo.at(i).dataNo.contains(curDeviceId))
        {
            flag=false;
            break;
        }
    }
    if(flag)
    {
        simData tmpSimDevice=updateSimPar(curDeviceId,curDeviceId);

        if(tmpSimDevice.dataTable.isEmpty()||tmpSimDevice.deviceTable.isEmpty())
            return;
        ui->lstSimDevices->addItem(curDeviceId);
        simDeviceInfo<<tmpSimDevice;
    }
    else
    {
        qDebug()<<"already device";
    }

    for(int i=0;i<simDeviceInfo.count();i++)
        qDebug()<<"cur devcie"<<i<<simDeviceInfo.at(i).dataNo;
#else
    simDeviceInfo.clear();
    //避免重复添加设备
    for(int i=0;i<ui->lstSimDevices->count();i++)
    {
        //        qDebug()<<i<<ui->lstSimDevices->item(i)->text()<<(tmp);
        QString tmp=ui->lstSimDevices->item(i)->text();

        simData tmpSimDevice=updateSimPar(curDeviceId,curDeviceId);

        if(tmpSimDevice.dataTable.isEmpty()||tmpSimDevice.deviceTable.isEmpty())
            continue;
        simDeviceInfo<<tmpSimDevice;
        if(tmp.contains(curDeviceId))
            flag=false;
    }
    //添加设备至模拟设备显示列表
    if(flag)
    {
        simData tmpSimDevice=updateSimPar(curDeviceId,curDeviceId);
        if(tmpSimDevice.dataTable.isEmpty()||tmpSimDevice.deviceTable.isEmpty())
            return;
        simDeviceInfo<<tmpSimDevice;
        ui->lstSimDevices->addItem(simDeviceInfo.at(simDeviceInfo.count()-1).deviceNo);
    }
    else
    {
        qDebug()<<"already device";
    }
#endif
}
//函数功能:更新当前数据源ID所对应条件的模拟数据源
void MainWindow::upateSimData(void)
{
    simData tmpDatas;
    sysDb->transaction();
    for(int i=0;i<simDeviceInfo.count();i++)
    {
        tmpDatas=simDeviceInfo.at(i);
        qDebug()<<"original type"<<tmpDatas.eType;
        if(tmpDatas.deviceNo.contains(ui->lstSimDevices->currentIndex().data().toString()))
        {
            sysDb->getDataRecords(tmpDatas);
            ui->lneSimDataCounts->setText(QString::number(tmpDatas.dataCount));
            ui->lneLastUpdate->setText(tmpDatas.recentTm);
            simDeviceInfo.replace(i,tmpDatas);
            break;
        }
    }
    sysDb->commit();
}
//函数功能：更新一次模拟设备的数据，电屏铠设备有6个传感器一次更新6调数据
void MainWindow::updateSimData(simData &tmpDatas,QString tm)
{
    eDeviceType type=tmpDatas.eType;
    switch(type)
    {
    case CURRENT:
    {
        qDebug()<<"insert current"<<tmpDatas.curIdx<<tmpDatas.dataCount<<tm;
        if(tmpDatas.curIdx<tmpDatas.simCurrentValues.count())
        {
            sysDb->insertCurrentRec(tmpDatas.deviceTable,
                                    tmpDatas.deviceNo,
                                    tmpDatas.simCurrentValues.at(tmpDatas.curIdx),
                                    tm);
            tmpDatas.curIdx++;
        }
        else
        {
            qDebug()<<"current device overed"<<tmpDatas.simCurrentValues.count()<<tmpDatas.dataCount;
            tmpDatas.curIdx=0;
        }
        break;
    }
    case WIRE:
    {
        //电屏铠需要一次更新六组数据
        int idx;
        qDebug()<<"insert wire"<<tmpDatas.curIdx<<tmpDatas.dataCount<<tm;
        for(idx=tmpDatas.curIdx;idx<(tmpDatas.curIdx+6);idx++)
        {
            if(idx<tmpDatas.simWireValues.count())
            {
                wire_Data wire=tmpDatas.simWireValues.at(idx);
                wire.deviceNo=tmpDatas.deviceNo;
                wire.tm=tm;
                qDebug()<<"insert wire"<<tmpDatas.curIdx<<wire.deviceNo<<wire.lineNo<<
                sysDb->insertWireRec(wire,tmpDatas.deviceTable);
            }
            else
            {
                qDebug()<<"wire device overed"<<tmpDatas.simWireValues.count()<<tmpDatas.dataCount;
                idx=0;
                break;
            }
        }
        tmpDatas.curIdx=idx;
        break;
    }
    case BRANCH:
    {
        if(tmpDatas.curIdx<tmpDatas.simBranchValues.count())
        {

            qDebug()<<"i got sim branch data"<<tmpDatas.dataCount<<tm<<
                      sysDb->insertBranchRec(tmpDatas.deviceTable,
                                             tmpDatas.deviceNo,
                                             tmpDatas.simBranchValues.at(tmpDatas.curIdx),
                                             tm);;
            tmpDatas.curIdx++;
        }
        else
        {
            qDebug()<<"branch device overed"<<tmpDatas.simCurrentValues.count()<<tmpDatas.dataCount;
            tmpDatas.curIdx=0;
        }
        break;
    }
    }
}
