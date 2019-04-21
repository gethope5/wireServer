/*
 * operator here for database
*/

#include <QtSql>
#include "database.h"
tableInfo MeasureDB::table4={CREATE_SQL4,TABLE_TITLE4,TABLE_NAME4,NULL};
bool MeasureDB::bParse=true;
deviceInfos MeasureDB::deviceInfo;
#if sub_thread
MeasureDB::MeasureDB(QTableView *m_tableView,QSqlDatabase &db,QObject *parent)/*:model(NULL)*/
#else
MeasureDB::MeasureDB(QTableView *m_tableView,QSqlDatabase &db)/*:model(NULL)*/
#endif
{
    //#if 1
    //    addConnection(DB_PATHS);
    //    createTable(table1);
    //    createTable(table2);
    //    displayType=true;
    //#else
    //    addConnection();
    //#endif
    wireCount=0;
    angleCount=0;
    currentCount=0;
    curIndex=0;
    if(db.isValid())
    {
        m_db=db;
        table4.tableView=m_tableView;
        //        table4.tableView->setModel(table4.model);
        if(!bParse)
            showDataTable(table4);
    }
    timer=new QTimer;
    connect(timer,SIGNAL(timeout()),this,SLOT(slot_showTable()));
    //model = new subSqlModel(m_db,tableView);
    speicalCurrentDevice<<jiaozuoCurrent_Device1;
    speicalCurrentDevice<<jiaozuoCurrent_Device2;
    
    
    //    QByteArray mm(
    //    "fe0616020a600616020d8317d8070d0a"
    //    "fe0616020a610616020443038b5e0d0a"
    //    "fe0616020a620616020443037b4f0d0a"
    //    "fe0616020a630616020443037b500d0a"
    //    "fe0616020a6406160204430383590d0a"
    //    "fe0616020a6506160204430000d40d0a"
    //    "fe0616020a6606160204430000d50d0a"
    //    "fe0616020a600616020d8317d8070d0a"
    //    "fe0616020a61061602044303683b0d0a"
    //    "fe0616020a62061602044303592d0d0a"
    //    "fe0616020a63061602044303592e0d0a"
    //    "fe0616020a6406160204430361370d0a"
    //    "fe0616020a6506160204430000d40d0a"
    //    "fe0616020a6606160204430000d50d0a");
    //    ParaseCmd(mm);

}

void MeasureDB::run(void)
{
    transaction();
#if 0
    insertIPPackageRecord(curIPData);
    //    qDebug()<<"insert status="<<
    //    showDataTable(MeasureDB::table4);//按照一定的频率刷新表格,不需要每次收到数据后就刷新表格,节约系统资源
    //    qDebug()<<"data"<<data<<data.count()<<data.toHex();
    ParaseCmd(curIPData.values);
#else
    while(1)
    {
        
    }
#endif
    commit();
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
#define ZHONGXINYIYUAN 0
bool MeasureDB::addConnection(void)
{
    bool beConnected ;
    QSqlError err;
    m_db = QSqlDatabase::addDatabase("QMYSQL");
    qDebug()<<"database valid="<<m_db.isValid();
#if ZHONGXINYIYUAN
    m_db.setHostName("127.0.0.1");
    m_db.setDatabaseName("test_c6");
#else
    m_db.setHostName("120.25.233.115");
    m_db.setDatabaseName("recievedata");
#endif
    m_db.setPort(3306);
    m_db.setUserName("root");
#if ZHONGXINYIYUAN
    m_db.setPassword("123456");
#else
    m_db.setPassword("mytianjun-mysql");
#endif
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
    commit();
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
        //        qDebug()<<"string sql"<<strSql;
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
            oneRecord.packages=(tmp);
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
    
    QString sqlBuffer=QString("insert into %1 (tm,ip,port,bValid,deviceInfo,packages) values (\'%2\',\'%3\',\'%4\',%5,\'%6\',\'%7\')")
            .arg(table4.tableName)//table name
            .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss"))  //date time
            .arg(dval.ipAddress)    //ip
            .arg(dval.ipPort)       //ip port
            .arg(1)                 //
            .arg(dval.devcieInfo)   //deviceInfo(longtext)
            .arg(QString(dval.packages));    //
    
    flag=query.exec(sqlBuffer);
    //    qDebug()<<"cur original data,sql="<<dval.packages;
    
    //    isCloseDB();
    return flag;
}

void MeasureDB::transaction(void)
{
    if(m_db.isOpen())
        m_db.transaction();
}

void MeasureDB::commit(void)
{
    if(m_db.isOpen())
        m_db.commit();
}
void MeasureDB::rollback(void)
{
    if(m_db.isOpen())
        m_db.rollback();
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
//函数功能：根据收到的原始数据(packages)进行分析，获得对应的传感器数据
bool MeasureDB::ParaseCmd(originalPackage &package)
{
    int i=0;
    int  nEndIndex;
    int nFirstIndex;
    bool flag=false;
    char m[2];
    m[0]=0x0d;
    m[1]=0x0a;
    //    packages="fe0400020d400e0002065418875c0d0afe0400020d410e000200000000640d0afe0400020d420e0002067308765c0d0afe0400020d430e000230ac0000420d0a";
    //    if(!package.values.contains("fe060016"))
    //        return false;
    //    package.values="fe0000060a400e0001117e07bdac0d0afe0000060a670637aa02590e27ee0d0afe0000060a410e0001117e07bdad0d0afe0000060a670637aa02590e27ee0d0afe0000060a420e000102fa01d9310d0afe0000060a670637aa02590e27ee0d0afe0000060a430e000131a30000300d0afe0000060a670637aa02590e27ee0d0a";
    //    qDebug()<<"dd"<<package.values;
    
    originalPackage tmpPac=package;
    do
    {
        nEndIndex=package.values.indexOf("0d0a");
        nFirstIndex=package.values.indexOf("fe");
        //qDebug()<<"index of"<<nEndIndex<<nFirstIndex<<values.indexOf(m);
        if((nFirstIndex!=-1)&&((nEndIndex-nFirstIndex)==28))
        {
            tmpPac.values=package.values.mid(nFirstIndex,nEndIndex-nFirstIndex+4);
            flag=parseOneCmd(tmpPac);
            //            qDebug()<<i++<<flag<<tmpPac.values;
        }
        package.values=package.values.right(package.values.count()-nEndIndex-4);
    }while((nEndIndex!=-1)&&(nFirstIndex!=-1));
    dataTm.clear();
    //    qDebug()<<"package counts="<<i;
    return flag;
}
bool MeasureDB::parseOneCmd(const originalPackage &singleCmd )
{
    CCommand cmd;
    cmd.packages=singleCmd.values;
    
    //    QDataStream in(singleCmd);
    //    qint32 number;//集中器编号
    //    in>>number;
    //    in>>cmd.length;
    //    in>>cmd.key;
    //    in>>cmd.type;
    //    in>>cmd.no;
    //    in>>cmd.cv;
    //    in>>cmd.hi;
    //    in>>cmd.cks;
    //    in>>cmd.enter;
    
    //    qDebug()<<"count="<<singleCmd.count();
    if(singleCmd.values.count()!=32)
        return false ;
    //    fe0600050a6406000a10d000005e0d0a
    //    fe060005 +0a      +64     +06     +000a     +10d0    +0000     +5e   +0d0a
    //    集中器编号4+数据长度1+特征码1+装置类型1+装置编号2+温度/电压2+湿度/电流2+校验和1+结束位
    
    bool ok;
    bool flag=false;
    cmd.sRelayNo=singleCmd.values.mid(2,6).toUpper();//集中器编号
    cmd.length=singleCmd.values.mid(8,2).toInt(&ok,16); //数据长度
    cmd.key=singleCmd.values.mid(10,2).toInt(&ok,16);   //特征码
    cmd.sType=singleCmd.values.mid(12,2).toUpper();//.toInt(&ok,16);  //装置类型
    cmd.sNo=singleCmd.values.mid(14,4).toUpper();//.toInt(&ok,16);    //装置编号
    cmd.cv=singleCmd.values.mid(18,4).toInt(&ok,16);
    cmd.hi=singleCmd.values.mid(22,4).toInt(&ok,16);
    //    qDebug()<<"package="<<singleCmd.values<<cmd.sRelayNo<<cmd.key<<cmd.sNo<<cmd.sType;
    cmd.deviceId=cmd.sRelayNo+cmd.sNo+cmd.sType;
    switch(getType(cmd.deviceId))
    {
#if 1
    case WIRE:
        cmd.lineNo = cmd.key-0x60;
        
        if((cmd.lineNo >= 0)&&(cmd.lineNo <=6) )
        {
            updateWireDb(cmd,dataTm,singleCmd);//插入数据库
            //更新设备状态
            //UpdataStatursDB(cmd);
        }
        //        qDebug()<<"i got wire "<<cmd.packages<<cmd.deviceId;
        break;
    case BVALUE:
        flag=insertBDb(cmd,singleCmd);
        //        qDebug()<<"i got b value"<<flag;
        break;
    case CURRENT:
        //        qDebug()<<"current type"<<cmd.values;
        flag=updateCurrentDb(cmd,singleCmd);
        break;
    case ANGLE:
        flag=insertAngleDb(cmd,singleCmd);
        break;
    case FORCE:
        
        flag=insertForceDb(cmd,singleCmd);
        //        qDebug()<<"i got force"<<flag;
        break;
    case AIR:
        flag=insertAirDb(cmd,singleCmd);
        //        qDebug()<<"i got air"<<flag;
        break;
#endif
    case BRANCH:
    {
        //        qDebug()<<"i got branch";
        flag=updateBranchDb(cmd,singleCmd);
        break;
    }
    default:
        //        qDebug()<<"not right device type"<<cmd.deviceId<<cmd.packages;
        flag=false;
    }
    return flag;
}
QString MeasureDB::getCurTable(QString curDeviceID)
{
    QString tmp="";
    int devIdIndex=deviceInfo.deviceNo.indexOf(curDeviceID);
    if(devIdIndex!=-1)
    {
        tmp=deviceInfo.tableName.at(devIdIndex);
    }
    else
    {
        tmp="";
    }
    return tmp;
}
//函数功能：根据设备编码搜索remark字段或直接通过remark字段获取当前设备的类型，并返回eDeviceType 对象
eDeviceType MeasureDB::getType(QString tmp)
{
    QString curRemark="";
    if(tmp.isNull())
        return eNULL;
    //判断是否为设备编号
    if(tmp.left(1).contains(QRegExp ("[0123456789]")))
    {
        int index=deviceInfo.deviceNo.indexOf(tmp);
        if(index!=-1)
        {
            curRemark=deviceInfo.sTypes.at(index);
        }
    }
    else
        curRemark=tmp;

    eDeviceType  type;
    if(curRemark.contains("wire",Qt::CaseInsensitive))
    {
        type=WIRE;
    }
    else if(curRemark.contains("BValue",Qt::CaseInsensitive))
    {
        type=BVALUE;
    }
    else if(curRemark.contains("angle",Qt::CaseInsensitive))
    {
        type=ANGLE;
    }
    else if(curRemark.contains("current",Qt::CaseInsensitive))
    {
        type=CURRENT;
    }
    else if(curRemark.contains("force",Qt::CaseInsensitive))//forceChangfang forceChangfang
    {
        type=FORCE;
    }
    else if(curRemark.contains("air",Qt::CaseInsensitive))
    {
        type=AIR;
    }
    else if(curRemark.contains("vibration",Qt::CaseInsensitive))
    {
        type=VIBRATION;
    }
    else if(curRemark.contains("branch",Qt::CaseInsensitive))
    {
        type=BRANCH;
    }
    return type;
}
//函数功能：将电屏铠数据插入对应的数据表格
bool MeasureDB::updateWireDb(const CCommand cmd,QString tm,originalPackage pac)
{
    QSqlQuery sqlQuery(currDatabase());
    static QString wendu=0;
    static QString shidu=0;
    if(cmd.lineNo)
    {
        QString strSql;
        QString curDeviceId=cmd.deviceId;
        QString curTableName=getCurTable(curDeviceId);
        if(curTableName.isEmpty())
        {
            qDebug()<<"not right wire devcie id"<<curDeviceId;
            return false;
        }
#if NO_SIGNAL
        emit updateInfo(curTableName);
#endif
        if(tm.isNull())
        {
            strSql=QString("insert into %7 ( tm,deviceNO,lineNo,voltage,current,temperature,humidity) values (now(),\'%1\',%2,%3,%4,%5,%6)")
                    .arg(curDeviceId).arg(cmd.lineNo).arg(QString::number(cmd.cv/1000.000,'f',3)).arg(QString::number(cmd.hi/1000.000,'f',3)).arg(wendu).arg(shidu)
                    .arg(curTableName);
        }
        else
        {
            strSql=QString("insert into %8( tm,deviceNO,lineNo,voltage,current,temperature,humidity) values (\'%1\',\'%2\',%3,%4,%5,%6,%7)")
                    .arg(pac.tm).arg(curDeviceId).arg(cmd.lineNo).arg(QString::number(cmd.cv/1000.000,'f',3)).arg(QString::number(cmd.hi/1000.000,'f',3)).arg(wendu).arg(shidu)
                    .arg(curTableName);
        }
        bool b=sqlQuery.exec(strSql);
        if(b)
        {
            //            qDebug()<<"insert wire data count"<<wireCount++;//strSql;
            modifyIds<<pac.id;
        }
        else
        {
            qDebug()<<"insert wire error"<<strSql;
        }
        return b;
    }
    else
    {
        wendu=QString::number(cmd.cv/100.00,'f',2);
        shidu=QString::number(cmd.hi/100.00,'f',2);
        //        qDebug()<<"wendu="<<wendu<<"shidu"<<shidu;
        return false;
    }
}
bool MeasureDB::updateCurrentDb(const CCommand cmd,originalPackage pac)
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
    //    qDebug()<<"current package"<<cmd.values;

    //fe0300070a30030006 0d67 1246 0f 0d0a-->t1=0d67=3431           h1=1246=4678
    //fe0300070a31030006 110c 000403  0d0a-->v1=110c=4364=4.364V    current1=0403=1027=1.027mA
    //fe0300070a32030006 0d85 11e7 cf 0d0a-->t2=0d85=3461           h2=11e7=4583
    //fe0300070a33030006 110c 000433  0d0a-->v=110c=4364=4.364V     current2=000433=1075uA=1.075mA
    //fe0300074a5a5130960b9b0d79f10d0a 0300070006
    //tm  '2018-06-24T11:52:42'
    //deviceNo  0300070006
    //t1 34.31
    //h1 179.35
    //v1 4.364
    //current 1.027
    //t2  3461
    //h2 -6193
    //tmp1 4.364
    //tmp2 1.075
    //insert into currentdetectjiaozuo (tm,deviceNo,t1,h1,v1,current,t2,h2,tmp1,tmp2,remark)
    //values ('2018-06-24T11:52:42','0300070006',34.31,179.35,4.364,1.027,3461,-6193,4.364,1.075,'')
    bool ok;
    bool bFlag=false;
    //仪表内的湿度及温度
    if(cmd.key==0x30)
    {
        curCurrentValue.p1T1=(float)cmd.cv/100.0;//?
        curCurrentValue.p1H1=(float)cmd.hi/100.0;//?
        curCurrentValue.package|=0x01;
    }
    //外部温度及湿度
    else if(cmd.key==0x31)
    {
        curCurrentValue.p2V1=(float)cmd.cv/1000.0;
        curCurrentValue.p2Current=cmd.packages.mid(22,6).toInt(&ok,16)/1000.0;

        curCurrentValue.package|=0x02;
    }
    //b1及B2值
    else if(cmd.key==0x32)
    {
        curCurrentValue.p3T2=cmd.cv/100.0;
        curCurrentValue.p3H2=cmd.hi/100.0;
        curCurrentValue.package|=0x04;
    }
    //集中器及采集器电压
    else if(cmd.key==0x33)
    {
        curCurrentValue.p4Tmp1=(float)cmd.cv/1000.0;
        curCurrentValue.p4Tmp2=cmd.packages.mid(22,6).toInt(&ok,16)/1000.0;
        curCurrentValue.package|=0x08;
    }
    char packgeChar=0x00;
    if(speicalCurrentDevice.contains(cmd.deviceId))
    {
        //        qDebug()<<"-----------------------------------------------jiaozuo  special current device ";
        packgeChar=0x0f;
    }
    else
    {
        //        qDebug()<<"-----------------------------------------------gerenal  current device ";
        packgeChar=0x07;
    }
    if((curCurrentValue.package&packgeChar)==packgeChar)
    {
        curCurrentValue.package=0;
        QString curDeviceId=cmd.deviceId;
        QString curTableName= getCurTable(curDeviceId);

        if(curTableName.isEmpty())
        {
            qDebug()<<"not right current devcie id"<<curDeviceId;
            return false;
        }
#if NO_SIGNAL
        emit updateInfo(curTableName);
#endif
#if 1
        bFlag=insertCurrentRec(curTableName,curDeviceId,curCurrentValue,pac.tm);
#else
        QSqlQuery sqlQuery(m_db);
        //            `id` bigint(20) NOT NULL AUTO_INCREMENT,
        //            `tm` datetime DEFAULT NULL,
        //            `deviceNo` text,
        //            `current` float DEFAULT NULL COMMENT '泄露电流',
        //            `t1` float DEFAULT NULL COMMENT '温度1',
        //            `h1` float DEFAULT NULL COMMENT '湿度1',
        //            `v1` float DEFAULT NULL COMMENT '电压',
        //            `t2` float DEFAULT NULL COMMENT '温度2',
        //            `h2` float DEFAULT NULL COMMENT '湿度2',
        //            `tmp1` float DEFAULT NULL COMMENT '湿度',
        //            `tmp2` float DEFAULT NULL COMMENT '仪表内湿度',
        //            `remark` text COMMENT '备注',
        QString strSql=QString("insert into %11 (tm,deviceNo,t1,h1,v1,current,t2,h2,tmp1,tmp2,remark) "\
                               "values (\'%12\',\'%1\',%2,%3,%4,%5,%6,%7,%8,%9,\'%10\')")
                .arg(curDeviceId)
                .arg(curCurrentValue.p1T1).arg(curCurrentValue.p1H1)
                .arg(curCurrentValue.p2V1).arg(curCurrentValue.p2Current)
                .arg(curCurrentValue.p3T2).arg(curCurrentValue.p3H2)
                .arg(curCurrentValue.p4Tmp1).arg(curCurrentValue.p4Tmp2).arg("")
                .arg(curTableName)
                .arg(pac.tm);
        bFlag=sqlQuery.exec(strSql);
#endif
        if(bFlag)
        {
            //            qDebug()<<"insert data current count"<<currentCount++<<strSql;
            modifyIds<<pac.id;
        }
        return bFlag;
    }
}
//函数功能：进行新乡分支电流参数处理
bool MeasureDB::updateBranchDb(const CCommand cmd ,originalPackage pac)
{

    switch (cmd.key)
    {
    case 0x61:
        curBranch.package|=0x01;
        curBranch.aV=cmd.hi/10.0;    //
        break;
    case 0x62:
        curBranch.package|=0x02;
        curBranch.aC=cmd.hi/10.0;
        break;
    case 0x63:
        curBranch.package|=0x04;
        curBranch.bV=cmd.hi/10.0;
        break;
    case 0x64:
        curBranch.package|=0x08;
        curBranch.bC=cmd.hi/10.0;
        break;
#if 0
    case 0x65:
        curBranch.package|=0x10;
        break;
    case 0x66:
        curBranch.package|=0x20;
        break;
#endif
    }
    //    qDebug()<<"cur branch"<<pac.values<<curBranch.package;
    if((curBranch.package&0x0f)==0x0f)
    {
        curBranch.package=0x00;

        curBValue.package=0;
        QString curDeviceId=cmd.deviceId;
        QString curTableName=getCurTable(curDeviceId);
        //        if(curTableName.isEmpty())
        //        {
        //            qDebug()<<"not right branch devcie id"<<curDeviceId;
        //            return false;
        //        }
#if 1
        if(insertBranchRec(curTableName,curDeviceId,curBranch,pac.tm))
        {
            modifyIds<<pac.id;
            return true;
        }
#else
        //20190419
        QSqlQuery query(currDatabase());
        QString strSql=QString("insert into %1 (tm,deviceNo,AVoltage,ACurrent,BVoltage,BCurrent,CVoltage,CCurrent,temperature1,humidity1,tmp1,tmp2,remark)"
                               " values(\'%2\',\'%3\',%4,%5,%6,%7,%8,%9,%10,%11,\'%12\',\'%13\',\'%14\')")
                .arg(curTableName)      //1
                .arg(pac.tm)            //2
                .arg(curDeviceId)       //3
                .arg(curBranch.aV)      //4
                .arg(curBranch.aC)      //5
                .arg(curBranch.bV)      //6
                .arg(curBranch.bC)      //7
                .arg(curBranch.cV)      //8
                .arg(curBranch.cC)      //9
                .arg(curBranch.t)       //10
                .arg(curBranch.h)       //11
                .arg(curBranch.tmp1)    //12
                .arg(curBranch.tmp2)    //13
                .arg(curBranch.remark); //14
        bool ok=query.exec(strSql);
        if(ok)
        {
            modifyIds<<pac.id;
        }
        else
            qDebug()<<"update branch  data"<<ok<<strSql<<query.lastError().text();
        return ok;
#endif
    }
    return false;
}

bool MeasureDB::insertAirDb(const CCommand cmd,originalPackage pac)
{
    bool bFlag=false;
    //仪表内的湿度及温度
    switch (cmd.key)
    {
    case 0x70:
    {
        curAirValue.temperature=(float)cmd.cv/100.0;//?
        curAirValue.humidity=(float)cmd.hi/100.0;//?
        curAirValue.package|=0x01;
        break;
    }
        //外部温度及湿度
    case 0x71:
    {
        curAirValue.voltage=(float)cmd.cv/1000.0;//v
        curAirValue.temperature1=(float)cmd.hi/100.0;//?
        curAirValue.package|=0x02;
        break;
    }
    case 0x72:
    {
        curAirValue.humidity1=(float)cmd.cv/100.0;
        curAirValue.airPressure=cmd.hi;
        curAirValue.package|=0x04;
        break;
    }
    case 0x73:
    {
        curAirValue.windSpeed=(float)cmd.cv*100.0;
        curAirValue.windDir=cmd.hi;
        
        curAirValue.package|=0x08;
        break;
    }
    case 0x74:
    {
        curAirValue.rainFall=(float)cmd.cv;
        curAirValue.package|=0x10;
        break;
    }
    }
    //    qDebug("air value,package,%x",curAirValue.package);
    //    qDebug()<<int(curAirValue.package)<<cmd.key;
    if((curAirValue.package&0x1f)==0x1f)
    {
        curAirValue.package=0;
        QString curDeviceId=cmd.deviceId;
        QString curTableName= getCurTable(curDeviceId);
        QSqlQuery sqlQuery(m_db);
        
        if(curTableName.isEmpty())
        {
            qDebug()<<"not right air devcie id"<<curDeviceId;
            return false;
        }
        //`id` bigint(20) NOT NULL AUTO_INCREMENT,
        //`tm` datetime NOT NULL,
        //`deviceNo` text,
        //`temperature` float(8,0) DEFAULT NULL COMMENT '温度',1
        //`humidity` float DEFAULT NULL COMMENT '湿度',2
        //`temperature1` float DEFAULT NULL COMMENT '环境温度',3
        //`humidity1` float DEFAULT NULL COMMENT '环境湿度',4
        //`voltage1` float DEFAULT NULL COMMENT '仪表内电压',5
        //`airPressure` float DEFAULT NULL COMMENT '气压',6
        //`windSpeed` text COMMENT '风速',7
        //`windDirection` text COMMENT '风向',8
        //`rainFall` float DEFAULT NULL COMMENT '雨量',9
        //`tmp1` text COMMENT '备注1',10
        //`remark` text COMMENT '备注',11
        //
        //-- fe0000060a700700010779157f960d0a
        //-- fe0000060a71070001340307692a0d0a
        //-- fe0000060a720700011dc6275deb0d0a
        //-- fe0000060a730700010087007a860d0a
        //-- fe0000060a7407000100000000860d0a
        //                                  1    2  3       4           5           6           7           8       9        10         11              12      13  14
        QString strSql=QString("insert into %1 (tm,deviceNo,temperature,humidity,temperature1,humidity1,voltage1,airPressure,windSpeed,windDirection,rainFall,tmp1,remark) "\
                               "values (\'%2\',\'%3\',%4,%5,%6,%7,%8,%9,%10,%11,%12,\'%13\',\'%14\')")
                .arg(curTableName)
                .arg(pac.tm)
                .arg(curDeviceId)
                .arg(curAirValue.temperature).arg(curAirValue.humidity)
                .arg(curAirValue.temperature1).arg(curAirValue.humidity1)
                .arg(curAirValue.voltage).arg(curAirValue.airPressure)
                .arg(curAirValue.windSpeed).arg(curAirValue.windDir)
                .arg(curAirValue.rainFall).arg(curAirValue.tmp1).arg(curAirValue.remark);
        bFlag=sqlQuery.exec(strSql);
        if(bFlag)
        {
            //            //            qDebug()<<"insert data current count"<<currentCount++<<strSql;
            modifyIds<<pac.id;
        }
        else
            qDebug()<<"insert data air,error,"<<strSql<<bFlag;
        return bFlag;
    }
    
}
bool MeasureDB::insertForceDb(const CCommand cmd,originalPackage pac)
{
    bool bFlag=false;
    //仪表内的湿度及温度
    switch (cmd.key)
    {
    case 0x10:
    {
        curForceValue.temperature=(float)cmd.cv/100.0;//?
        curForceValue.humidity=(float)cmd.hi/100.0;//?
        curForceValue.package|=0x01;
        //        qDebug()<<"force key1";
        break;
    }
        //外部温度及湿度
    case 0x11:
    {
        curForceValue.voltage=(float)cmd.cv/1000.0;
        curForceValue.force=cmd.hi*2.37;
        
        curForceValue.package|=0x02;
        //        qDebug()<<"force key2";
        break;
    }
    }
    //    qDebug()<<"force value,package"<<int(curForceValue.package)<<cmd.packages<<cmd.key;
    if((curForceValue.package&0x03)==0x03)
    {
        curForceValue.package=0;
        QString curDeviceId=cmd.deviceId;
        QString curTableName= getCurTable(curDeviceId);
        QSqlQuery sqlQuery(m_db);
        
        if(curTableName.isEmpty())
        {
            qDebug()<<"not right force devcie id"<<curDeviceId;
            return false;
        }
        //`id` bigint(20) NOT NULL AUTO_INCREMENT,
        //`tm` datetime NOT NULL,
        //`deviceNo` text,
        //`force` float(8,0) DEFAULT NULL COMMENT '张力值',1
        //`temperature` float DEFAULT NULL COMMENT '温度',2
        //`humidity` float DEFAULT NULL COMMENT '湿度',3
        //`temperature1` float DEFAULT NULL COMMENT '仪表内温度',4
        //`humidity1` float DEFAULT NULL COMMENT '仪表内湿度',5
        //`voltage1` float DEFAULT NULL COMMENT '集中器电压',6
        //`tmp1` text COMMENT '备注1',7
        //`tmp2` text COMMENT '备注2',8
        //`remark` text COMMENT '备注3',9
        //PRIMARY KEY (`id`,`tm`)
        
        //    -- fe0600160a10010001099f167d570d0a
        //    -- fe0600160a110100011d390896110d0a
        //                                  1    2  3       4       5           6           7           8       9       10   11     12
        QString strSql=QString("insert into %1 (tm,deviceNo,forcevalue,temperature,humidity,temperature1,humidity1,voltage1,tmp1,tmp2,remark) "\
                               "values (\'%2\',\'%3\',%4,%5,%6,%7,%8,%9,\'%10\',\'%11\',\'%12\')")
                .arg(curTableName)
                .arg(pac.tm)
                .arg(curDeviceId)
                .arg(curForceValue.force).arg(curForceValue.temperature)
                .arg(curForceValue.humidity).arg(curForceValue.temperature1)
                .arg(curForceValue.humidity1).arg(curForceValue.voltage)
                .arg(curForceValue.tmp1).arg(curForceValue.tmp2).arg(curForceValue.remark);
        bFlag=sqlQuery.exec(strSql);
        if(bFlag)
        {
            //            //            qDebug()<<"insert data current count"<<currentCount++<<strSql;
            modifyIds<<pac.id;
        }
        else
            qDebug()<<"insert data force,error,"<<strSql<<bFlag;
        return bFlag;
    }
    
}
bool MeasureDB::insertAngleDb(const CCommand cmd,originalPackage pac)
{
    //fe0500140a50050002 0796 0f36 430d0a
    //fe0500140a51050002 331d 005b 0d0d0a
    //fe0500140a52050002 331d 0074 270d0a
    //fe0500140a53052357fadb0d30070d0a
    //湿度及温度
    if(cmd.key==0x50)
    {
        curAngleValue.temperature=(float)cmd.cv/100.0;
        curAngleValue.humidity=(float)cmd.hi/100.0;
        curAngleValue.package|=0x01;
    }
    //电压及X倾角
    else if(cmd.key==0x51)
    {
        curAngleValue.voltage1=(float)cmd.cv/1000.0;
        curAngleValue.xAngle=(float)cmd.hi/100.0;
        curAngleValue.package|=0x02;
    }
    //电压及X倾角
    else if(cmd.key==0x52)
    {
        curAngleValue.voltage2=cmd.cv/1000.0;
        curAngleValue.yAngle=cmd.hi/100.0;
        curAngleValue.package|=0x04;
    }

    if((curAngleValue.package&0x0f)==0x07)
    {
        curAngleValue.package=0;
        QString curDeviceId=cmd.deviceId;
        QString curTableName= getCurTable(curDeviceId);
        QSqlQuery sqlQuery(m_db);
        bool bFlag=false;
        if(curTableName.isEmpty())
        {
            qDebug()<<"not right angle devcie id"<<curDeviceId;
            return false;
        }

#if NO_SIGNAL
        emit updateInfo(curTableName);
#endif
        //            REATE TABLE `anglexining` (
        //              `id` bigint(20) NOT NULL AUTO_INCREMENT,
        //              `tm` datetime DEFAULT NULL,
        //              `deviceNo` text,
        //              `xAngle` float(8,0) DEFAULT NULL COMMENT 'X倾角',
        //              `yAngle` float DEFAULT NULL COMMENT 'Y倾角',
        //              `temperature` float DEFAULT NULL COMMENT '温度',
        //              `humidity` float DEFAULT NULL COMMENT '湿度',
        //              `voltage1` float DEFAULT NULL COMMENT '电压1',
        //              `voltage2` float DEFAULT NULL COMMENT '电压2',
        //              `remark` text COMMENT '注释',
        //              PRIMARY KEY (`id`)
        QString strSql=QString("insert into %9 (tm,deviceNo,xAngle,yAngle,temperature,humidity,voltage1,voltage2,remark) "\
                               "values (\'%10\',\'%1\',round(%2,2),round(%3,2),%4,%5,%6,%7,\'%8\')")
                .arg(curDeviceId)
                .arg(curAngleValue.xAngle).arg(curAngleValue.yAngle)
                .arg(curAngleValue.temperature).arg(curAngleValue.humidity)
                .arg(curAngleValue.voltage1).arg(curAngleValue.voltage2).arg("")
                .arg(curTableName)
                .arg(pac.tm);
        bFlag=sqlQuery.exec(strSql);
        if(bFlag)
        {
            qDebug()<<"insert angle count"<<angleCount++;
        }
        else
            qDebug()<<"insert data angle,error"<<strSql<<bFlag;
        return bFlag;

    }
}

bool MeasureDB::insertBDb(const CCommand cmd,originalPackage pac)
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
        if(curBValue.b2<200||curBValue.b1<200)
        {
            curBValue.package=0x00;
            qDebug()<<"error b value"<<curBValue.b1<<curBValue.b2<<"serial no"<<getCurTable(cmd.deviceId);
            return false;
        }
        curBValue.package|=0x04;
    }
    //集中器及采集器电压
    else if(cmd.key==0x43)
    {
        curBValue.voltage=(float)cmd.cv/1000.0;
        curBValue.voltage2=(float)cmd.hi/1000.0;
        curBValue.package|=0x08;
    }
    //    qDebug()<<"b device ID"<<curBValue.package;

    if((curBValue.package&0x0f)==0x0f)
    {
        curBValue.package=0;
        QString curDeviceId=cmd.deviceId;
        QString curTableName=getCurTable(curDeviceId);
        QSqlQuery sqlQuery(m_db);
        bool bFlag=false;
        if(curTableName.isEmpty())
        {
            qDebug()<<"not right B devcie id"<<curDeviceId;
            return false;
        }
#if NO_SIGNAL
        emit updateInfo(curTableName);
#endif
        QString strSql=QString("insert into %11 (tm,deviceNo,b1,b2,temperature,temperatureIn,voltage,voltage2,humidity,humidityIn,remark) "\
                               "values (\'%12\',\'%1\',%2,%3,%4,%5,%6,%7,%8,%9,\'%10\')")
                .arg(curDeviceId).arg(curBValue.b1).arg(curBValue.b2).arg(curBValue.temperature).arg(curBValue.temperatureIn)
                .arg(curBValue.voltage).arg(curBValue.voltage2).arg(curBValue.humidity).arg(curBValue.humidityIn).arg("")
                .arg(curTableName)
                .arg(pac.tm);
        bFlag=sqlQuery.exec(strSql);
        if(bFlag)
        {
            modifyIds<<pac.id;
        }
        else
            qDebug()<<"error b insert data"<<strSql<<bFlag;
        return bFlag;
    }
}

bool MeasureDB::insertWireRec(wire_Data & dval,QString table)
{
    //    isOpenDB();
    QSqlQuery query(m_db);
    QString strSql;
#if 1
    strSql= QString("INSERT INTO %1 VALUES(:id1, :id2, :id3, :id4, :id5, :id6, :id7,:id8)").arg(table);
    query.prepare(strSql);
    query.bindValue(":id2",dval.tm);
    query.bindValue(":id3",dval.deviceNo);
    query.bindValue(":id4",dval.lineNo);
    query.bindValue(":id5", dval.voltage);
    query.bindValue(":id6", dval.current);
    query.bindValue(":id7", dval.temperature);
    query.bindValue(":id8", dval.humidity);
#else
    strSql= QString("INSERT INTO %1 (tm,deviceNo,lineNo,voltage,current,temperature,humidity) "
                    "VALUES(\'%2\',\'%3\',%4,%5,%6,%7,%8)")
            .arg(table)
            .arg(dval.tm)
            .arg(dval.deviceNo)
            .arg(dval.lineNo)
            .arg(dval.voltage)
            .arg(dval.current)
            .arg(dval.temperature)
            .arg(dval.humidity);
#endif
    bool flag=query.exec();
    //isCloseDB();
    //update();
    if(!flag)
        qDebug()<<"insert wire record"<<flag<<strSql;
    return flag;
}
//函数功能：插入一条分支电流的数据
bool MeasureDB::insertBranchRec(QString table,QString deviceNo,branch_Data curRecord,QString tm)
{
    QSqlQuery query(currDatabase());
    QString strSql=QString("insert into %1 (tm,deviceNo,AVoltage,ACurrent,BVoltage,BCurrent,CVoltage,CCurrent,temperature1,humidity1,tmp1,tmp2,remark)"
                           " values(\'%2\',\'%3\',%4,%5,%6,%7,%8,%9,%10,%11,\'%12\',\'%13\',\'%14\')")
            .arg(table)      //1
            .arg(tm)            //2
            .arg(deviceNo)       //3
            .arg(curRecord.aV)      //4
            .arg(curRecord.aC)      //5
            .arg(curRecord.bV)      //6
            .arg(curRecord.bC)      //7
            .arg(curRecord.cV)      //8
            .arg(curRecord.cC)      //9
            .arg(curRecord.t)       //10
            .arg(curRecord.h)       //11
            .arg(curRecord.tmp1)    //12
            .arg(curRecord.tmp2)    //13
            .arg(curRecord.remark); //14
    bool ok=query.exec(strSql);
    if(!ok)
    {
        qDebug()<<"error update branch  data"<<ok<<strSql<<query.lastError().text();
    }
    return ok;
}
bool MeasureDB::insertCurrentRec(QString table,QString deviceNo,current_Data curCurrentValue,QString tm)
{
    bool bFlag;

    QSqlQuery sqlQuery(m_db);
    //            `id` bigint(20) NOT NULL AUTO_INCREMENT,
    //            `tm` datetime DEFAULT NULL,
    //            `deviceNo` text,
    //            `current` float DEFAULT NULL COMMENT '泄露电流',
    //            `t1` float DEFAULT NULL COMMENT '温度1',
    //            `h1` float DEFAULT NULL COMMENT '湿度1',
    //            `v1` float DEFAULT NULL COMMENT '电压',
    //            `t2` float DEFAULT NULL COMMENT '温度2',
    //            `h2` float DEFAULT NULL COMMENT '湿度2',
    //            `tmp1` float DEFAULT NULL COMMENT '湿度',
    //            `tmp2` float DEFAULT NULL COMMENT '仪表内湿度',
    //            `remark` text COMMENT '备注',
    QString strSql=QString("insert into %11 (tm,deviceNo,t1,h1,v1,current,t2,h2,tmp1,tmp2,remark) "\
                           "values (\'%12\',\'%1\',%2,%3,%4,%5,%6,%7,%8,%9,\'%10\')")
            .arg(deviceNo)
            .arg(curCurrentValue.p1T1).arg(curCurrentValue.p1H1)
            .arg(curCurrentValue.p2V1).arg(curCurrentValue.p2Current)
            .arg(curCurrentValue.p3T2).arg(curCurrentValue.p3H2)
            .arg(curCurrentValue.p4Tmp1).arg(curCurrentValue.p4Tmp2).arg("")
            .arg(table)
            .arg(tm);
    bFlag=sqlQuery.exec(strSql);
    if(!bFlag)
        qDebug()<<"insert data current,error,"<<strSql<<bFlag;
    return bFlag;
}


void MeasureDB::parseAll(void)
{
    //    m_db.transaction();
    QSqlQuery query(m_db);
#if 0
    
#else
    //    qDebug()<<"begin parse";
    //    QString strSql=QString("select *from %1 where id=826696"/*packages like 'fe030007%'"*/).arg(TABLE_NAME4);
    //    QString strSql=QString("select *from %1 where packages like 'fe030007%'" ).arg(TABLE_NAME4);
    //    QString strSql=QString("select *from %1 where packages like 'fe030003%'" ).arg(TABLE_NAME4);
    //    QString strSql=QString("select *from %1 where packages like 'fe030002%'" ).arg(TABLE_NAME4);
    QString strSql=QString("select *from %1 where bValid=1" ).arg(TABLE_NAME4);
    //    QString strSql("select * from deviceaddress where packages like 'fe000006%' ORDER BY tm");
    
    QVector<long> nIds;
    
    //fe0600160a600600060b2d0ceda70d0a
    //fe0600160a61060006117f0000070d0a
    //fe0600160a62060006117f0000080d0a
    //fe0600160a63060006117f0000090d0a
    //fe0600160a64060006117f00000a0d0a
    //fe0600160a65060006117f00000b0d0a
    //fe0600160a66060006117f00000c0d0a
    //fe0600164a5a512e5603f31abb600d0a

    //fe0600160a100100010adf0ec4d70d0a
    //fe0600160a110100011c8e0042090d0a
    QTime time;
    int i=0;
    transaction();
    if(query.exec(strSql))
    {
        time.start();
        qDebug()<<"begin parse"<<strSql;
        
        while(query.next())
        {
            IPOrignal cc;
            int nValid=query.value("bValid").toInt();
            package.id=query.value("id").toLongLong();
            package.values=query.value("packages").toByteArray();//parseOnePackage(curIndex++,cc);
            package.tm=query.value("tm").toString();
            //            if(!package.values.contains("fe060016"))
            //                    break;
            //                        qDebug()<<"i got value"<<i++<<package.values;
            if(ParaseCmd( package ))
            {
                qDebug()<<"time"<<cc.time<<package.values;
            }
            //            break;
            //            if(i>10)
            //                break;
        }
        //        qDebug()<<"begin parse"<<strSql<<i<<time.elapsed();
#if 1
        if(modifyIds.count())
        {
            for(int i=0;i<modifyIds.count();i++)
            {
                strSql=QString("update %1 set bvalid=0 where id=%2").arg(TABLE_NAME4).arg(modifyIds.at(i));
                bool b=query.exec(strSql);
                //                qDebug()<<"update devcie address ,"<<b<<strSql;
            }
            modifyIds.clear();
        }
#endif
        emit  updateInfo(QString::fromWCharArray(L"数据刷新条数:%1,%2").arg(nIds.count()).arg(QDateTime::currentDateTime().toString("hh-mm-ss")));
        commit();
    }
    else
    {
        qDebug()<<"error sql"<<strSql;
        rollback();
    }
    //reb
    qDebug()<<"par ok"<<time.elapsed();
#endif
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
    QString strSql=QString("select deviceNO,tableName,department,deviceType,localPosition,remark from %1 ").arg(MYSQL_deviceTable_name);
    transaction();
    if(query.exec(strSql))
    {
        deviceInfo.clear();
        while(query.next())
        {
            QString no=query.value("deviceNo").toString();
            QString tb=query.value("tableName").toString();

            deviceInfo.deviceNo<<no;
            deviceInfo.tableName<<tb;
            deviceInfo.position<<QString::fromUtf8(query.value("localPosition").toString().toUtf8());
            QString type=query.value("remark").toString();
            deviceInfo.sTypes<<type;
            deviceInfo.eTypes<<getType(type);
            QString mm=query.value("department").toString();
            deviceInfo.departments<<mm;//.toByteArray().toHex();
            deviceInfo.recentTms<<"";
        }
    }
    else
    {
        qDebug()<<"------------------not update devie info";
    }

    transaction();
    for(int i=0;i<deviceInfo.deviceNo.count();i++)
    {
        //id,no,tablename,position,type,recent time
        qDebug()<<QString("devices=%1,%2,%3,%4,%5,%6")
                  .arg(i,2)
                  .arg(deviceInfo.deviceNo.at(i),-12)
                  .arg(deviceInfo.tableName.at(i),-20)
                  .arg(deviceInfo.sTypes.at(i),-10)
                  .arg(deviceInfo.recentTms.at(i),-20)
                  .arg(deviceInfo.position.at(i),-40);
    }
    emit updateDevice();
}
QString MeasureDB::updateRecentTime(QString deviceNo,QString tableName)
{
    QSqlQuery query(m_db);
    QString strSql=QString("select tm from %1 where deviceNO=\'%2\' order by tm desc ")
            .arg(tableName)
            .arg(deviceNo);
    //qDebug()<<"recent time"<<strSql;
    if(query.exec(strSql))
    {
        QString m="NULL";
        while(query.next())
        {
            m=query.value(0).toString();
            break;
        }
        return m;
    }
    else
    {
        qDebug()<<"error get recent devcie time,sql="<<deviceNo<<tableName<<strSql;
        return "NULL";
    }
}
void MeasureDB::utf8_unicode(unsigned char *inbuf, unsigned char *outbuf, int inlen)
{
    
    int idx = 0;
    int odx = 0;
    if (inlen == -1)
        inlen = strlen((const char*)inbuf);
    //qDebug()<<"in len : " << inlen;
    while (idx < inlen)
    {
        
        if ((inbuf[idx]&0xE0) == 0xc0)
        {
            unsigned char val0 = (inbuf[idx] & 0x1f);
            unsigned char val1 = (inbuf[idx+1] & 0x3f);
            outbuf[odx++] = val0 >> 2;
            outbuf[odx++] = (val0&0x03)<<6 | val1;
            idx += 2;
        }
        else if ((inbuf[idx]&0xE0) == 0xE0)
        {
            unsigned char val0 = (inbuf[idx] & 0x0f);
            unsigned char val1 = (inbuf[idx+1] & 0x3f);
            unsigned char val2 = (inbuf[idx+2] & 0x3f);
            outbuf[odx] = (val0 <<4)| (val1>>2);
            outbuf[odx+1] = (val1&0x03)<<6 | val2;
            odx += 2;
            idx += 3;
        }
        else
        {
            // qDebug("X:%02x ", (unsigned char)inbuf[idx]);
            outbuf[odx++] = inbuf[idx++];
            
        }
    }
    outbuf[odx] = 0;
}
void MeasureDB::unicode_utf8(const unsigned int inbuf, unsigned char *outbuf, int* outlen, int mlen)
{
    int utf8_idx = 0;
    
    if (inbuf <= 0x7f && mlen >= 1)
        outbuf[utf8_idx++] = inbuf;
    else if (inbuf <= 0x7ff && mlen >= 2)
    {
        outbuf[utf8_idx++] = 0xC0 | (inbuf>>6);
        outbuf[utf8_idx++] = 0x80 | (inbuf&0x3F);
    }
    else if (inbuf <= 0xffff && mlen >= 3)
    {
        outbuf[utf8_idx++] = 0xE0 | (inbuf>>12);
        outbuf[utf8_idx++] = 0x80 | ((inbuf>>6) & 0x3f);
        outbuf[utf8_idx++] = 0x80 | (inbuf & 0x3f);
    }
    *outlen = utf8_idx;
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
            //            QByteArray mm=query.value(0).toString().toUtf8();
            //            QString cc=QString::fromUtf8(mm);
            
            //            char outbuf[255];
            //            utf8_unicode((unsigned char*)cc.toUtf8().constData(), (unsigned char*)outbuf, -1);
            //            //qDebug()<<"the outbuf " <<QObject::tr(outbuf);
            //            QString strT3 = QTextCodec::codecForName("UTF-8")->toUnicode(cc.toUtf8());
            //            QByteArray gbkstr = QTextCodec::codecForName("GB18030")->fromUnicode(strT3);
            //            qDebug()<<"fuck code "<<cc<<strT3<<QString(outbuf);
            //            wchar_t dd[100];
            //            //            dd[mm.toWCharArray(dd)]=0;
            //            //            qDebug()<<"device type"<<QString::fromWCharArray(dd);
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
            //            departments<<query.value(0).toString();
            departments<< query.value(0) .toString();
        }
        return departments;
    }
    return QStringList("");
}
//函数功能:根据设备编号,寻找设备编号对应的表格名称
QString  MeasureDB::getDeviceTable(QString device )
{
    QString table;
    int idx=deviceInfo.deviceNo.indexOf(device);
    if(idx!=(-1))
    {
        table= deviceInfo.tableName.at(idx);
    }
    else
    {
        table="";
    }
    return table;
}
void MeasureDB::slot_showTable(void)
{
    showDataTable(MeasureDB::table4);
}
//函数功能：更新模拟数据源
void MeasureDB::getDataRecords(simData &devcieInfo)
{
    QString startTm=devcieInfo.dataStartTm;
    QString endTm=devcieInfo.dataEndTm;
    QString tableName=devcieInfo.dataTable;
    QString deviceNo=devcieInfo.dataNo;
    QSqlQuery query(m_db);

    QString strSql;
#if 0
    switch(devcieInfo.eType)
    {
    case CURRENT:
    {
        strSql=QString("select * from %1 where ((tm>\'%2\' and tm <\'%3\') and deviceNo=\'%4\') ").arg(tableName).arg(startTm)
                .arg(endTm)
                .arg(deviceNo);
        //qDebug()<<"current device"<<strSql;
        break;
    }
    case WIRE:
    {
        strSql=QString("select * from %1 where ((tm>\'%2\' and tm <\'%3\') and deviceNo=\'%4\') ").arg(tableName).arg(startTm)
                .arg(endTm)
                .arg(deviceNo);
        //qDebug()<<"wire device"<<strSql;
        break;
    }
    }
#else
    strSql=QString("select * from %1 where ((tm>\'%2\' and tm <\'%3\') and deviceNo=\'%4\') ").arg(tableName).arg(startTm)
            .arg(endTm)
            .arg(deviceNo);
#endif
    //qDebug()<<"update sim data sql"<<strSql<<deviceInfo.eTypes;
    if(query.exec(strSql))
    {
        while(query.next())
        {
            switch(devcieInfo.eType)
            {
            case CURRENT:
            {
                current_Data tmp;
                tmp.tm= query.value("tm").toString();
                tmp.p1T1=query.value("t1").toFloat();
                tmp.p1H1=query.value("h1").toFloat();
                tmp.p2V1=query.value("v1").toFloat();
                tmp.p2Current=query.value("current").toFloat();
                tmp.p3T2=query.value("t2").toFloat();
                tmp.p3H2=query.value("h2").toFloat();
                tmp.p4Tmp1=query.value("tmp1").toFloat();
                tmp.p4Tmp2=query.value("tmp2").toFloat();
                devcieInfo.simCurrentValues<<tmp;
                devcieInfo.dataCount++;
                break;
            }
            case WIRE:
            {
                wire_Data tmp;
                tmp.tm= query.value("tm").toString();
                tmp.deviceNo=query.value("deviceNo").toString();
                tmp.lineNo=query.value("lineNo").toInt();
                tmp.voltage=query.value("voltage").toString();
                tmp.current=query.value("current").toString();
                tmp.temperature=query.value("temperature").toString();
                tmp.humidity=query.value("humidity").toString();
                devcieInfo.simWireValues<<tmp;
                devcieInfo.dataCount++;
                break;
            }
            case BRANCH:
            {
                branch_Data tmp;
                tmp.tm= query.value("tm").toString();
                tmp.aV=query.value("AVoltage").toInt();
                tmp.aC=query.value("ACurrent").toFloat();
                tmp.bV=query.value("BVoltage").toFloat();
                tmp.bC=query.value("BCurrent").toFloat();
                tmp.cV=query.value("CVoltage").toFloat();
                tmp.cC=query.value("CCurrent").toFloat();
                tmp.t=query.value("temperature1").toFloat();
                tmp.h=query.value("humidity1").toFloat();
                tmp.tmp1=query.value("tmp1").toString();
                tmp.tmp2=query.value("tmp2").toString();
                tmp.remark=query.value("remark").toString();
                devcieInfo.simBranchValues<<tmp;
                devcieInfo.dataCount++;
                break;
            }
            }
        }
    }
    else
    {
        qDebug()<<"error get current records"<<tableName<<strSql;
    }
}
