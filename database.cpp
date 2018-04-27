/*
 * operator here for database
*/

#include <QtSql>
#include "database.h"
tableInfo MeasureDB::table4={CREATE_SQL4,TABLE_TITLE4,TABLE_NAME4,NULL};
QPair<QStringList,QStringList> MeasureDB::deviceTables=QPair<QStringList,QStringList>(QStringList(""),QStringList(""));
MeasureDB::MeasureDB(QTableView *m_tableView,QSqlDatabase &db)/*:model(NULL)*/
{
    //#if 1
    //    addConnection(DB_PATHS);
    //    createTable(table1);
    //    createTable(table2);
    //    displayType=true;
    //#else
    //    addConnection();
    //#endif
    dataTm.clear();
    curIndex=0;
    if(db.isValid())
    {
        m_db=db;
        table4.tableView=m_tableView;
        //        table4.tableView->setModel(table4.model);
        showDataTable(table4);
    }
    //model = new subSqlModel(m_db,tableView);
}
MeasureDB::MeasureDB(QVector<QTableView *> tableView)
{
    displayType=false;
#if showALL
    if(tableView.count()>2)
    {
        table1.tableView=tableView.at(0);
        table2.tableView=tableView.at(1);
        table3.tableView=tableView.at(2);

        addConnection(DB_PATHS);
        table1.model=new subSqlModel(m_db);
        table2.model=new subSqlModel(m_db);
        table3.model=new subSqlModel(m_db);
        table4.model=new subSqlModel(m_db);

        createTable(table1);
        createTable(table2);
        createTable(table3);
        createTable(table4);

        qDebug()<<"i get the tableviews";

        showTables();
    }
    else
    {
        table1.tableView=NULL;
        table2.tableView=NULL;
        table3.tableView=NULL;
        table4.tableView=NULL;
    }
#endif
}
void MeasureDB::showTables(void)
{
#if showALL
    showDataTable(table1);
    showDataTable(table2);
    showDataTable(table3);
#endif
}
bool MeasureDB::addConnection(void)
{
    bool beConnected ;
    QSqlError err;
    m_db = QSqlDatabase::addDatabase("QMYSQL");
    qDebug()<<"database valid="<<m_db.isValid();
    m_db.setHostName("120.25.233.115");
    m_db.setDatabaseName("recievedata");
    m_db.setPort(3306);
    m_db.setUserName("root");
    m_db.setPassword("mytianjun-mysql");
    if (!m_db.open())
    {
        err = m_db.lastError();
        m_db = QSqlDatabase();
    }
    if (err.type() != QSqlError::NoError)
    {
        // QMessageBox::warning(NULL, tr("无法打开数据库"), tr("数据库操作错误: ") + err.text());
        qDebug()<<"can not open the database! "<<err;
        beConnected = false;
    }
    else
    {
        //        qDebug()<<"I can get the database\n";
        beConnected =  true;
    }
    return beConnected;
}
void MeasureDB::addConnection(QString dbName)
{
    QSqlError err;
    m_db = QSqlDatabase::addDatabase("QSQLITE", "mdb_connection");
    m_db.setDatabaseName(dbName);
    if (!m_db.open())
    {
        qDebug()<<"can not open db..........";
        err = m_db.lastError();
        m_db = QSqlDatabase();
    }
    else
    {
        qDebug()<<"db is ok";
    }
    curDataName=DB_PATHS;
}
void MeasureDB::isOpenDB(void)
{
    if(!m_db.isOpen())
        qDebug()<<"again open status="<<m_db.open();
}
void MeasureDB::isCloseDB(void)
{
    Commit();
    m_db.close();
}

void MeasureDB::exportCSVFile(QString prePaths )
{
    qDebug()<<"prePaths="<<prePaths;
    int indexBuffer=prePaths.lastIndexOf('/');

    if(indexBuffer!=-1)
    {
        qDebug()<<"love you"<<prePaths.right(prePaths.count()-indexBuffer-1);
        prePaths.chop(3);//removes 3 characters from the end of the string
    }
}
void MeasureDB::showDataTable(tableInfo &tableName,QString filter)
{
    qDebug()<<"cur filter string"<<filter;

    QSqlQueryModel *queryModel = new QSqlQueryModel(this);
    QSqlQuery query(currDatabase());

    QString strSql=QString("select * from %1 where tm like \'%2%\'").arg(tableName.tableName).arg(filter);
    if(query.exec(strSql))
    {
        qDebug()<<"string sql"<<strSql;
        //    QString strSql=QString::fromWCharArray(L"SELECT * from %1 where id>0").arg(tableName);
        //qDebug()<<"show sql"<<strSql<<bExec;
        queryModel->setQuery(query);
        for(int i=0;i<tableName.titleName.split(',').count();i++)
        {
            QString tmp=tableName.titleName.split(',').at(i);
            queryModel->setHeaderData(i,Qt::Horizontal, QString(tmp.left(tmp.length()-1)));
        }
        while (queryModel->canFetchMore())
            queryModel->fetchMore();
        tableName.tableView->setModel(queryModel);
    }
    else
        qDebug()<<"query error,string sql"<<strSql;
}
bool MeasureDB::createTable(tableInfo table)
{
    bool ok1;
    isOpenDB();

    QSqlQuery query(m_db);
    //                                                  1ID,                                2 时间,         3 里程(米), 4公里标(千米),    5站区,            6杆号,         7轨距,           8超高,            9侧面界限,            10 红线                11 结构高度          12定拉            13定高,            14坡度,      15高差,      16中高,              17中拉,              18非支拉出值,      19非支导高1,       20 水平距离,          21垂直距离,      22 500高差,             23 定位器描述,               24 巡视图片";
    QString strSql=QString("CREATE TABLE %1(%2)").arg(table.tableName).arg(table.createSQL);// ()";
    ok1=query.exec(strSql);

    if(ok1)
    {
        qDebug()<<QString("table %1 is ok.").arg(table.tableName);
    }
    else
    {
        qDebug()<<QString("table %1 is not ok.").arg(table.tableName);
    }
    //    isCloseDB();
    return ok1;
}
QByteArray MeasureDB::parseOnePackage(int index,IPOrignal &oneRecord)
{
    QByteArray tmp;
    isOpenDB();
    QSqlQuery query(m_db);
    //id,time,ip,port,bvalid,deviceinfo,packages
    QString strval = QString("select tm,ip,port,packages from %1 where id=%2").arg(table4.tableName).arg(index);

    if(query.exec(strval))
    {
        while(query.next())
        {
            oneRecord.time=query.value(0).toString();
            oneRecord.ipAddress=query.value(1).toString();
            oneRecord.ipPort=query.value(2).toString();
            tmp=query.value(3).toByteArray();//.toString();
            oneRecord.packages=QString(tmp);
        }
    }

    return tmp;
}
bool MeasureDB::modifyRecord(int id, const QString& station,
                             const QString& pid,
                             const int structFlg )
{
    isOpenDB();
    QSqlQuery query(m_db);
    QString strval = QString("UPDATE measures SET station='%1',"
                             "poleID='%2',structFlag =%3 WHERE id>=%4")
            .arg(station).arg(pid).arg(structFlg).arg(id);
    bool modifiedrecord=query.exec(strval);
    //    qDebug()<< "modify val "<<strval;
    //      qDebug()<<"exec: "<<modifiedrecord;
    //    qDebug()<<query.lastError();
    //    isCloseDB();
    return true;
}


//函数功能：向表格deviceAddress插入收到的原始数据。
//#define CREATE_SQL4      "id INTEGER PRIMARY KEY AUTOINCREMENT, time varchar(30),deviceType varchar(10),serialNumber varchar(20),ipAddress varchar(20),ipPort varchar(20),bValid bool"
//#define TABLE_TITLE4     "ID1,时间1,设备ip地址1,设备ip端口1,当前状态1,数据包1"
//#define TABLE_NAME4 "connectedClients"
bool MeasureDB::insertIPPackageRecord(IPOrignal &dval)
{
    isOpenDB();
    //    qDebug()<<"db="<<m_db.isValid()<<m_db.tables();
    QSqlQuery query(m_db);
    bool flag;

    QString sqlBuffer=QString("insert into %1 (tm,ip,port,bValid,deviceInfo,packages) values (\'%2\',\'%3\',\'%4\',%5,\'%6\',\'%7\')").arg(table4.tableName)
            .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss"))
            .arg(dval.ipAddress)
            .arg(dval.ipPort)
            .arg(1)
            .arg("f")
            .arg(dval.packages);

    flag=query.exec(sqlBuffer);
    qDebug()<<"sql="<<sqlBuffer;

    //    isCloseDB();
    return flag;
}
wireOrignalData MeasureDB::parseWirePackage(wireCmdPackage cmd)
{
    //    int id;                 //1     id
    //    QString time;           //2     时间
    //    QString deviceType;     //4     设备类型
    //    QString serialNumber;   //5     设备编号
    //    int sensorNumber;       //6     传感器编号
    //    QString voltage;        //7     设备电压
    //    QString eleCurrent;     //8     设备电流
    //    QString temperature;    //9     设备温度
    //    QString humidity;       //10     设备湿度


    //    qint8 length;   //数据长度
    //    qint8 key;      //特征码
    //    qint8 type;     //装置类型
    //    qint16 no;      //设备序号
    //    qint16 cv;      //温度或电压
    //    qint16 hi;      //湿度或电流
    //    qint8 cks;      //校验和
    //    qint16 enter;//处理多余的回车换行
    wireOrignalData dataRecord;
    //1
    dataRecord.time=QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss");
    //2
    if(cmd.type==0x06)
        dataRecord.deviceType="电缆在线检测设备";
    dataRecord.serialNumber=QString::number(cmd.no);
    //3
    if(cmd.key==0x60)
    {
        dataRecord.sensorNumber="总机";

        dataRecord.temperature=QString::number(cmd.cv);
        dataRecord.humidity=QString::number(cmd.hi);
        dataRecord.voltage="";
        dataRecord.eleCurrent="";
    }
    else
    {
        dataRecord.sensorNumber=QString("线路%1").arg(QString::number(cmd.key-0x60));

        //dataRecord.sensorNumber=cmd.key-0x60;
        dataRecord.voltage=QString::number(cmd.cv/1000.0,'g',6);
        dataRecord.eleCurrent=QString::number(cmd.hi/1000.0,'g',6);
        dataRecord.temperature="";
        dataRecord.humidity="";
    }
    //    //4
    //    dataRecord.voltage;
    //    //5
    //    dataRecord.eleCurrent;
    //    //6
    //    dataRecord.temperature;
    //    //7
    //    dataRecord.humidity;

    return dataRecord;
}
bool MeasureDB::insertRecordWireData(wireOrignalData & dval)
{
    //    isOpenDB();
    QSqlQuery query(m_db);
    QString strval = QString("INSERT INTO %1 VALUES(:id1, :id2, :id3, :id4, :id5, :id6, :id7,:id8, :id9)").arg(TABLE_NAME1);
    query.prepare(strval);

    query.bindValue(":id2",dval.time);
    query.bindValue(":id3",dval.deviceType);
    query.bindValue(":id4",dval.serialNumber);
    query.bindValue(":id5", dval.sensorNumber);
    query.bindValue(":id6", dval.voltage);
    query.bindValue(":id7", dval.eleCurrent);
    query.bindValue(":id8", dval.temperature);
    query.bindValue(":id9", dval.humidity);

    bool flag=query.exec();
    //    isCloseDB();
    //    update();
    return flag;
}
void MeasureDB::Transaction(void)
{
    if(m_db.isOpen())
        m_db.transaction();
}

void MeasureDB::Commit(void)
{
    if(m_db.isOpen())
        m_db.commit();
}

MeasureDB::~MeasureDB()
{
    if(m_db.isOpen())
        m_db.close();
    if(m_db.isOpen())
    {
        qDebug()<<"~~~~"<<m_db.connectionName();
        QSqlDatabase::removeDatabase(m_db.connectionName());
    }
}
QSqlDatabase MeasureDB::currDatabase(void)
{
    return m_db;
}
bool MeasureDB::ParaseCmd(QByteArray packages, qint16 &deviceNo)
{
    int i=0;
    int  nEndIndex;
    int nFirstIndex;
    bool flag=false;
    char m[2];
    m[0]=0x0d;
    m[1]=0x0a;
    //    packages="fe0400020d400e0002065418875c0d0afe0400020d410e000200000000640d0afe0400020d420e0002067308765c0d0afe0400020d430e000230ac0000420d0a";
    //    qDebug()<<"dd"<<packages.toHex();
    do
    {
        nEndIndex=packages.indexOf("0d0a");
        nFirstIndex=packages.indexOf("fe");
        //        qDebug()<<"index of"<<nEndIndex<<nFirstIndex<<packages.indexOf(m);
        if((nFirstIndex!=-1)&&((nEndIndex-nFirstIndex)==28))
        {
            flag=ParaseSingleCmd(packages.mid(nFirstIndex,nEndIndex-nFirstIndex+4),deviceNo);
            i++;
            //            qDebug()<<i<<flag;
        }
        packages=packages.right(packages.count()-nEndIndex-4);
    }while((nEndIndex!=-1)&&(nFirstIndex!=-1));
    dataTm.clear();
    //    qDebug()<<"package counts="<<i;
    return flag;
}


bool MeasureDB::ParaseSingleCmd(const QByteArray &singleCmd,qint16 &deviceNo)
{
    CCommand cmd;

#if 0
    QDataStream in(singleCmd);
    qint32 number;//集中器编号
    in>>number;
    in>>cmd.length;
    in>>cmd.key;
    in>>cmd.type;
    in>>cmd.no;
    in>>cmd.cv;
    in>>cmd.hi;
    in>>cmd.cks;
    in>>cmd.enter;
#else
    //    qDebug()<<"count="<<singleCmd.count();
    if(singleCmd.count()!=32)
        return false ;
    //    fe0600050a6406000a10d000005e0d0a
    //    fe060005 +0a      +64     +06     +000a     +10d0    +0000     +5e   +0d0a
    //    集中器编号4+数据长度1+特征码1+装置类型1+装置编号2+温度/电压2+湿度/电流2+校验和1+结束位

    bool ok;
    cmd.sRelayNo=singleCmd.mid(2,6);//集中器编号
    cmd.length=singleCmd.mid(8,2).toInt(&ok,16); //数据长度
    cmd.key=singleCmd.mid(10,2).toInt(&ok,16);   //特征码
    cmd.type=singleCmd.mid(12,2).toInt(&ok,16);  //装置类型
    cmd.sNo=singleCmd.mid(14,4);    //装置编号
    cmd.cv=singleCmd.mid(18,4).toInt(&ok,16);
    cmd.hi=singleCmd.mid(22,4).toInt(&ok,16);
#endif
    if(cmd.type==Wire_Type)
    {
        //        qDebug()<<"package cmd="<<singleCmd;
        //        qDebug()<<"dd"<<cmd.sRelayNo<<cmd.sLength<<cmd.sKey<<cmd.sType<<cmd.sNo<<cmd.sCv<<cmd.sHi;
        //        qDebug()<<"dd"<<cmd.sRelayNo.toInt(&ok,16)<<cmd.sLength.toInt(&ok,16)<<cmd.sKey.toInt(&ok,16)<<cmd.sType.toInt(&ok,16)<<cmd.sNo.toInt(&ok,16);

        cmd.lineNo = cmd.key-0x60;
        deviceNo = cmd.no;

        if((cmd.lineNo >= 0)&&(cmd.lineNo <=6) )
        {
            //        concentratorNo ;//= number;
            //            if(cmd.lineNo)
            //            {
            //                //            cmd.hi
            //                qDebug()<<QString::fromWCharArray(L"电流")<<cmd.hi<<QString::fromWCharArray(L"电压")<<cmd.cv;
            //            }
            //            else
            //            {
            //                qDebug()<<QString::fromWCharArray(L"湿度")<<cmd.hi<<QString::fromWCharArray(L"温度")<<cmd.cv;
            //            }
            //                        qDebug()<<"insert status="<<cmd.lineNo<<
            insertWireDb(cmd,dataTm);//插入数据库

            //更新设备状态
            UpdataStatursDB(cmd);
        }
        return true;
    }
    else if(cmd.type==B_Type)
    {
        insertBDb(cmd);
    }
    else if(cmd.type==CURRENT_Type)
    {
        insertCurrentDb(cmd);
    }
    else
    {
        qDebug()<<"not right device type";
        return false;
    }
}
bool MeasureDB::insertCurrentDb(const CCommand cmd)
{
    //  float b1;             //1
    //  float b2;
    //  float temperature;
    //  float temperatureIn;
    //  float voltage;        //0x43
    //  float voltage2;
    //  float humidity;       //0x40
    //  float humidityIn;
    //  char package;
    //    qDebug()<<"before"<<QString::number(curBValue.package,'g',16)<<"dd";

    //仪表内的湿度及温度
    if(cmd.key==0x30)
    {
        curCurrentValue.p1T1=(float)cmd.cv/100.0;
        curCurrentValue.p1H1=(float)cmd.hi/100.0;
        curCurrentValue.package|=0x01;
    }
    //外部温度及湿度
    else if(cmd.key==0x31)
    {
        curCurrentValue.p2V1=(float)cmd.cv/100.0;
        curCurrentValue.p2Current=(float)cmd.hi/100.0;
        curCurrentValue.package|=0x02;
    }
    //b1及B2值
    else if(cmd.key==0x32)
    {
        curCurrentValue.p3T2=cmd.cv;
        curCurrentValue.p3H2=cmd.hi;
        curCurrentValue.package|=0x04;
    }
    //集中器及采集器电压
    else if(cmd.key==0x34)
    {
        curCurrentValue.p4Tmp1=(float)cmd.cv/1000.0;
        curCurrentValue.p4Tmp2=(float)cmd.hi/1000.0;
        curCurrentValue.package|=0x08;
    }
    //    qDebug()<<"b device ID"<<cmd.sRelayNo+cmd.sNo<<QString::number(curBValue.package,'g',16)<<"dd";

    if((curCurrentValue.package&0x0f)==0x0f)
    {
        curCurrentValue.package=0;
        QString curDeviceId=cmd.sRelayNo+cmd.sNo;
        QString curTableName;
        QSqlQuery sqlQuery(m_db);
        bool bFlag=false;
        int devIdIndex=deviceTables.first.indexOf(curDeviceId);
        //        qDebug()<<"ok"<<deviceTables;
        if(devIdIndex!=-1)
        {
            curTableName=deviceTables.second.at(devIdIndex);
            emit updateInfo(curTableName);

            QString strSql=QString("insert into %11 (tm,deviceNo,p1t1,p1h1,p2v1,p2current,p3t2,p3h2,p4tmp1,p4tmp2,remark) "\
                                   "values (now(),\'%1\',%2,%3,%4,%5,%6,%7,%8,%9,\'%10\')")
                    .arg(curDeviceId)
                    .arg(curCurrentValue.p1T1).arg(curCurrentValue.p1H1)
                    .arg(curCurrentValue.p2V1).arg(curCurrentValue.p2Current)
                    .arg(curCurrentValue.p3T2).arg(curCurrentValue.p3H2)
                    .arg(curCurrentValue.p4Tmp1).arg(curCurrentValue.p4Tmp2).arg("")
                    .arg(curTableName);
            bFlag=sqlQuery.exec(strSql);
            qDebug()<<"insert data"<<strSql<<bFlag;
            return bFlag;
        }
    }

}
bool MeasureDB::insertBDb(const CCommand cmd)
{
    //  float b1;             //1
    //  float b2;
    //  float temperature;
    //  float temperatureIn;
    //  float voltage;        //0x43
    //  float voltage2;
    //  float humidity;       //0x40
    //  float humidityIn;
    //  char package;
    //    qDebug()<<"before"<<QString::number(curBValue.package,'g',16)<<"dd";

    //仪表内的湿度及温度
    if(cmd.key==0x40)
    {
        curBValue.humidityIn=(float)cmd.cv/100.0;
        curBValue.temperatureIn=(float)cmd.hi/100.0;
        curBValue.package|=0x01;
    }
    //外部温度及湿度
    else if(cmd.key==0x41)
    {
        curBValue.humidity=(float)cmd.cv/100.0;
        curBValue.temperature=(float)cmd.hi/100.0;
        curBValue.package|=0x02;
    }
    //b1及B2值
    else if(cmd.key==0x42)
    {
        curBValue.b1=cmd.cv;
        curBValue.b2=cmd.hi;
        curBValue.package|=0x04;
    }
    //集中器及采集器电压
    else if(cmd.key==0x43)
    {
        curBValue.voltage=(float)cmd.cv/1000.0;
        curBValue.voltage2=(float)cmd.hi/1000.0;
        curBValue.package|=0x08;
    }
    //    qDebug()<<"b device ID"<<cmd.sRelayNo+cmd.sNo<<QString::number(curBValue.package,'g',16)<<"dd";

    if((curBValue.package&0x0f)==0x0f)
    {
        curBValue.package=0;
        QString curDeviceId=cmd.sRelayNo+cmd.sNo;
        QString curTableName;
        QSqlQuery sqlQuery(m_db);
        bool bFlag=false;
        int devIdIndex=deviceTables.first.indexOf(curDeviceId);
        //        qDebug()<<"ok"<<deviceTables;
        if(devIdIndex!=-1)
        {
            curTableName=deviceTables.second.at(devIdIndex);
            emit updateInfo(curTableName);

            QString strSql=QString("insert into %11 (tm,deviceNo,b1,b2,temperature,temperatureIn,voltage,voltage2,humidity,humidityIn,remark) "\
                                   "values (now(),\'%1\',%2,%3,%4,%5,%6,%7,%8,%9,\'%10\')")
                    .arg(curDeviceId).arg(curBValue.b1).arg(curBValue.b2).arg(curBValue.temperature).arg(curBValue.temperatureIn)
                    .arg(curBValue.voltage).arg(curBValue.voltage2).arg(curBValue.humidity).arg(curBValue.humidityIn).arg("")
                    .arg(curTableName);
            bFlag=sqlQuery.exec(strSql);
            qDebug()<<"insert data"<<strSql<<bFlag;
            return bFlag;
        }
    }

}
//函数功能：将电屏铠数据插入对应的数据表格
bool MeasureDB::insertWireDb(const CCommand cmd,QString tm)
{
    QSqlQuery sqlQuery(m_db);
    static QString wendu=0;
    static QString shidu=0;
    if(cmd.lineNo)
    {
        QString strSql;
#if 0
        if(tm.isNull())
        {
            strSql=QString("insert into %7 ( tm,deviceNO,lineNo,voltage,current,temperature,humidity) values (now(),\'%1\',%2,%3,%4,%5,%6)")
                    .arg(cmd.sRelayNo+cmd.sNo).arg(cmd.lineNo).arg(QString::number(cmd.cv/1000.000,'f',3)).arg(QString::number(cmd.hi/1000.000,'f',3)).arg(wendu).arg(shidu)
                    .arg("detailinfo0514");
        }
        else
        {
            strSql=QString("insert into %8( tm,deviceNO,lineNo,voltage,current,temperature,humidity) values (\'%1\',\'%2\',%3,%4,%5,%6,%7)")
                    .arg(dataTm).arg(cmd.sRelayNo+cmd.sNo).arg(cmd.lineNo).arg(QString::number(cmd.cv/1000.000,'f',3)).arg(QString::number(cmd.hi/1000.000,'f',3)).arg(wendu).arg(shidu)
                    .arg("detailinfo0514");
        }
#else
        QString curDeviceId=cmd.sRelayNo+cmd.sNo;
        QString curTableName;
        //        deviceId,tableName
        int devIdIndex=deviceTables.first.indexOf(curDeviceId);
        qDebug()<<"curDevice iD"<<curDeviceId;
        if(devIdIndex!=-1)
        {
            curTableName=deviceTables.second.at(devIdIndex);
            emit updateInfo(curTableName);
            if(tm.isNull())
            {
                strSql=QString("insert into %7 ( tm,deviceNO,lineNo,voltage,current,temperature,humidity) values (now(),\'%1\',%2,%3,%4,%5,%6)")
                        .arg(curDeviceId).arg(cmd.lineNo).arg(QString::number(cmd.cv/1000.000,'f',3)).arg(QString::number(cmd.hi/1000.000,'f',3)).arg(wendu).arg(shidu)
                        .arg(curTableName);
            }
            else
            {
                strSql=QString("insert into %8( tm,deviceNO,lineNo,voltage,current,temperature,humidity) values (\'%1\',\'%2\',%3,%4,%5,%6,%7)")
                        .arg(dataTm).arg(curDeviceId).arg(cmd.lineNo).arg(QString::number(cmd.cv/1000.000,'f',3)).arg(QString::number(cmd.hi/1000.000,'f',3)).arg(wendu).arg(shidu)
                        .arg(curTableName);
            }
            qDebug()<<"insert data"<<strSql;
            return sqlQuery.exec(strSql);
        }
        else
            qDebug()<<"not right device id and table name"<<curDeviceId;
#endif
    }
    else
    {
        wendu=QString::number(cmd.cv/100.00,'f',2);
        shidu=QString::number(cmd.hi/100.00,'f',2);
        //        qDebug()<<"wendu="<<wendu<<"shidu"<<shidu;
        return false;
    }
}
void MeasureDB::tran(void)
{
    m_db.transaction();
}
void MeasureDB::commit(void)
{
    m_db.commit();
}

void MeasureDB::UpdataStatursDB(CCommand command)
{
    QSqlQuery queryStaturs;
    if(command.lineNo == 0)
    {
        //更新温度湿度
        queryStaturs.prepare(QString::fromWCharArray(L"UPDATE devieceStaturs SET 温度= ? ,湿度 = ?,最近更新时间 = now() WHERE 设备编号 = ?"));
        queryStaturs.bindValue(0,command.cv);
        queryStaturs.bindValue(1,command.hi);
        queryStaturs.bindValue(2,command.no);
    }
    else
    {
        //更新电压电流
        switch (command.lineNo)
        {
        case 1:
            queryStaturs.prepare(QString::fromWCharArray(L"UPDATE devieceStaturs SET 线路1= ?,电源剩余= ?  WHERE 设备编号 = ?"));
            break;
        case 2:
            queryStaturs.prepare(QString::fromWCharArray(L"UPDATE devieceStaturs SET 线路2= ?,电源剩余= ?  WHERE 设备编号 = ?"));
            break;
        case 3:
            queryStaturs.prepare(QString::fromWCharArray(L"UPDATE devieceStaturs SET 线路3= ?,电源剩余= ?  WHERE 设备编号 = ?"));
            break;
        case 4:
            queryStaturs.prepare(QString::fromWCharArray(L"UPDATE devieceStaturs SET 线路4= ?,电源剩余= ?  WHERE 设备编号 = ?"));
            break;
        case 5:
            queryStaturs.prepare(QString::fromWCharArray(L"UPDATE devieceStaturs SET 线路5= ?,电源剩余= ?  WHERE 设备编号 = ?"));
            break;
        case 6:
            queryStaturs.prepare(QString::fromWCharArray(L"UPDATE devieceStaturs SET 线路6= ?,电源剩余= ?  WHERE 设备编号 = ?"));
            break;
        default:
            break;
        }
        queryStaturs.bindValue(0,command.hi);
        queryStaturs.bindValue(1,(command.cv - 3500)/10);
        queryStaturs.bindValue(2,command.no);
    }
    if(!queryStaturs.exec())
    {

        //        ui->statusBar->showMessage(queryStaturs.lastError().text());
        QString tmp=queryStaturs.lastError().text();
        //        emit updateInfo(tmp);

    }
    else
    {
        qDebug()<<QString::fromWCharArray(L"更新数据库成功");
    }
}
void MeasureDB::parseAll(void)
{
    //    m_db.transaction();
    QSqlQuery query(m_db);
    if(query.exec(QString("select *from %1").arg(table4.tableName)))
    {
        while(query.next())
        {
            qint16 d;
            IPOrignal cc;
            qDebug()<<"index"<<curIndex;
            QByteArray mm=parseOnePackage(curIndex++,cc);

            dataTm=cc.time;
            if(ParaseCmd( mm ,d))
            {
                qDebug()<<"time"<<cc.time;
            }
        }
    }
}
QString MeasureDB::getCurIndex(void)
{
    return QString::number(curIndex);
}
QStringList MeasureDB::getALLDeviceNo(void)
{
    QSqlQuery query(m_db);
    QString strSql=QString("Select distinct deviceNo  from %1").arg("detailinfo0514");
    QStringList cc;
    if(query.exec(strSql))
    {
        while(query.next())
        {
            cc<<query.value(0).toString();
        }
    }
    return cc;
}
bool MeasureDB::insertDeviceStatus(void)
{
    "ID，deviceNo，department，position，installTm，addTm，line1，line2，line3，line4，line6，remainCell，temperature，humidity，recentTmUpdate";
    QSqlQuery query(m_db);

    QString strSql="select deviceNO from deviecestaturs0514";
    QStringList cc;
    QStringList cc1=getALLDeviceNo();
    if(query.exec(strSql))
    {
        while(query.next())
            cc<<query.value(0).toString();
    }
    qDebug()<<"cc"<<cc<<cc1;
    for(int i=0;i<cc1.count();i++)
    {
        if(cc.contains(cc1.at(i)))
        {
            qDebug()<<"i get it";
        }
        else
        {
            strSql=QString("insert into deviecestaturs0514 (deviceNO) values ('%1')").arg(cc1.at(i));
            qDebug()<<"insert device no"<<strSql<<query.exec(strSql);
            //             return true;
        }
    }
    return true;
}
QString MeasureDB::getTableName(QString deviceNo)
{
    QSqlQuery query(m_db);

    QString strSql=QString("select tableName from %2 where deviceNO=\'%1\'").arg(deviceNo).arg(MYSQL_deviceTable_name);
    qDebug()<<"get device table Name"<<strSql;
    if(query.exec(strSql))
    {
        while(query.next())
        {
            return query.value(0).toString();
        }
    }
    return "";
}
void MeasureDB::udapteTableName(void)
{
    QSqlQuery query(m_db);
    QString strSql=QString("select deviceNO,tableName,department,deviceType from %1 ").arg(MYSQL_deviceTable_name);
    qDebug()<<"update all device tables,sql="<<strSql;
    int i=0;
    if(query.exec(strSql))
    {
        deviceTables.first.clear();
        deviceTables.second.clear();
        while(query.next())
        {
            deviceTables.first<<query.value(0).toString();
            deviceTables.second<<query.value(1).toString();
            if(i<deviceTables.first.count())
            {
                qDebug()<<"device table"<<i<<deviceTables.first.at(i)<<deviceTables.second.at(i)<<query.value(2).toString()<<query.value(3).toString();
                i++;
            }
        }
    }
}
//函数功能：根据供电段的名称查询该供电段所属的传感器类型
QStringList MeasureDB::getDeviceType(QString department)
{
    QSqlQuery query(m_db);
    QString strSql=QString("select distinct deviceType from %1 where department= \'%2\'").arg(MYSQL_deviceTable_name).arg(department);
    //    qDebug()<<"found tablename ,sql="<<strSql;
    if(query.exec(strSql))
    {
        QStringList deviceTypes;
        while(query.next())
        {
            deviceTypes<<query.value(0).toString();
        }
        return deviceTypes;
    }
    return QStringList("");
}
QStringList MeasureDB::getDeparment(void)
{
    QSqlQuery query(m_db);
    QString strSql=QString("select DISTINCT department from %1 ").arg(MYSQL_deviceTable_name);
    //    qDebug()<<"found tablename ,sql="<<strSql;
    if(query.exec(strSql))
    {
        QStringList departments;
        while(query.next())
        {
            departments<<query.value(0).toString();
        }
        return departments;
    }
    return QStringList("");
}
