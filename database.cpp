/*
 * operator here for database
*/

#include <QtSql>
#include "database.h"
QStringList tbDisplayModel::detailinfo0514_Devcies;
#if MYSQL
//QSqlDatabase tbDisplayModel::m_db=QSqlDatabase::addDatabase("QMYSQL");
//("0600040002", "0600040006", "0600040003", "0600040004",
// "0600040005", "0600050003", "0600050002", "0600050001",
// "0600050004", "0600020004", "0600020003", "0600020002",
// "0600020001", "0600020005", "0600030001", "0600030002",
// "0600030003", "0600030004", "0600030005", "0600030006",
// "0600030007", "0600030008", "0600030009", "060003000a",
// "0600030010", "0600020010", "0600020008", "0600020007",
// "0600020009", "0600020006")
#else
QSqlDatabase tbDisplayModel::m_db=QSqlDatabase::addDatabase("QSQLITE","connectName");
#endif
tbDisplayModel::tbDisplayModel(QString tableName,QTableView *view,QObject * parent , QSqlDatabase db ) : QSqlTableModel(parent, db)
{    
   
    table.view=view;
    table.name=TABLE_NAME4;
    initialCreate(false);
    initialDb(DB_NAME);
    qDebug()<<"set  host name,database status"<<m_db.isValid()<<m_db.databaseName()<<getRecordCount();
    

    if(!m_db.tables().contains(tableName))
    {
        createTable(table);
    }
//    showtable(TABLE_NAME4);
    // cur custom code
    dataTm.clear();
    curIndex=0;
}

void tbDisplayModel::initialCreate(bool f)
{
    //#define CREATE_SQL4      "deviceType varchar(10),serialNumber varchar(20),ipAddress varchar(20),ipPort varchar(20),bValid bool"
    //#define TABLE_TITLE4   romWCharArray(L"ID1,时间1,设备ip地址1,设备ip端口1,当前状态0,数据包1")
    //#define TABLE_NAME4 "deviceaddress"
    //1
    fieldInfo tmpField={"id",QString::fromWCharArray(L"ID"),true,"int","123"};
    table.fields<<tmpField;
    //2
    tmpField={"tm",QString::fromWCharArray(L"时间"),true,"varchar(30)","2018"};
    table.fields<<tmpField;
    //3
    tmpField={"ip",QString::fromWCharArray(L"设备ip地址"),true,"varchar(10)","1.23"};
    table.fields<<tmpField;
    //4
    tmpField={"port",QString::fromWCharArray(L"设备ip端口"),true,"varchar(20)","3.2"};
    table.fields<<tmpField;
    //5
    tmpField={"bValid",QString::fromWCharArray(L"当前状态0"),true,"bool","1"};
    table.fields<<tmpField;
    //6
    tmpField={"deviceInfo",QString::fromWCharArray(L"数据包1"),true,"varchar(200)","1"};
    table.fields<<tmpField;
    //
    tmpField={"packages",QString::fromWCharArray(L"数据包1"),true,"varchar(200)","1"};
    table.fields<<tmpField;
}
void tbDisplayModel::initialDb( const QString& dbName)
{
#if MYSQL


    QSqlError err;
#if LOCALDEBUG
    m_db = QSqlDatabase::addDatabase("QMYSQL");
    m_db.setHostName("127.0.0.1");
    m_db.setPort(3307);
    m_db.setDatabaseName("zbwdb");
    m_db.setUserName("root");
    m_db.setPassword("mytianjun-mysql");
#else
    m_db = QSqlDatabase::addDatabase("QMYSQL");
    m_db.setHostName("120.25.233.115");
    m_db.setPort(3306);
    m_db.setDatabaseName("electricdata");
    m_db.setUserName("root");
    m_db.setPassword("mytianjun-mysql");
#endif
#else
    QString connectName= QString("mdb_connection1");
    m_db.setDatabaseName(dbName);
#endif
    if( m_db.open())
    {
        //QMessageBox::critical(this,"错误提示",m_db.lastError().text());
        //return false;
        //        ui->statusBar->showMessage(QString::fromWCharArray(L"数据库打开"));
        //        dbConnect->setText(QString::fromWCharArray(L"数据库打开"));
        qDebug()<<"database is connected,show all tables="<<m_db.tables();
        emit db_status(true);
    }
    else
    {
        qDebug()<<"can not open db..........";
        err = m_db.lastError();
        m_db = QSqlDatabase();
        //        ui->statusBar->showMessage(QString::fromWCharArray(L"连接数据库失败")+m_db.lastError().text());
        //        dbConnect->setText(QString::fromWCharArray(L"数据库失败")+m_db.lastError().text());
        qDebug()<<"data is error";
        emit db_status(false);
    }

}
void tbDisplayModel::showTables(void)
{
#if showALL
    showDataTable(table1);
    showDataTable(table2);
    showDataTable(table3);
#endif
}
void tbDisplayModel::connectDB(void)
{

#if LOCALDEBUG
    m_db = QSqlDatabase::addDatabase("QMYSQL");
    m_db.setHostName("127.0.0.1");
    m_db.setPort(3307);
    m_db.setDatabaseName("zbwdb");
    m_db.setUserName("root");
    m_db.setPassword("mytianjun-mysql");
#else

    m_db = QSqlDatabase::addDatabase("QMYSQL");
    m_db.setHostName("120.25.233.115");
    m_db.setPort(3306);
    m_db.setDatabaseName("electricdata");
    m_db.setUserName("root");
    m_db.setPassword("mytianjun-mysql");
#endif
    if( m_db.open())
    {
        //QMessageBox::critical(this,"错误提示",m_db.lastError().text());
        //return false;
        //        ui->statusBar->showMessage(QString::fromWCharArray(L"数据库打开"));
        //        dbConnect->setText(QString::fromWCharArray(L"数据库打开"));
        qDebug()<<"database is connected,show all tables="<<m_db.tables();
    }
    else
    {
        //        ui->statusBar->showMessage(QString::fromWCharArray(L"连接数据库失败")+m_db.lastError().text());
        //        dbConnect->setText(QString::fromWCharArray(L"数据库失败")+m_db.lastError().text());
        qDebug()<<"data is error";
    }
}
void tbDisplayModel::addConnection(QString dbName)
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
}
void tbDisplayModel::isOpenDB(void)
{
    if(!m_db.isOpen())
        qDebug()<<"again open status="<<m_db.open();
}
void tbDisplayModel::isCloseDB(void)
{
    Commit();
    m_db.close();
}
QString tbDisplayModel::parseTmp(QString &tmp)
{
    if(tmp.endsWith(","))
    {
        return tmp.left(tmp.count()-1);
    }
    else
        return tmp;
}
bool tbDisplayModel::insertRecord(tableInfo table,int startIndex)
{
    QString fieldTmp,valueTmp;
    if(table.fields.count()&&(startIndex<table.fields.count()))
    {
        for(int i=startIndex;i<table.fields.count();i++)
        {
            //            qDebug()<<"cur insert"<<i<<table.fields.at(i).value;
            fieldTmp+=table.fields.at(i).fieldName+",";
            if(table.fields.at(i).dataType.contains("char"))
            {
                valueTmp+="\'"+table.fields.at(i).value+"\',";
            }
            else
            {
                valueTmp+=table.fields.at(i).value+",";
            }
        }
        fieldTmp=parseTmp(fieldTmp);
        valueTmp=parseTmp(valueTmp);
        QString strSql=QString("insert into %1 (%2) values (%3)").arg(table.name).arg(fieldTmp).arg(valueTmp);
        QSqlQuery query(currDatabase());
        bool flag=query.exec(strSql);
//        qDebug()<<(strSql)<<flag;
        return flag;
    }
    else
    {
        return false;
    }
}
bool tbDisplayModel::createTable(tableInfo &table)
{
    if(currDatabase().tables().contains(table.name))
    {
        qDebug()<<QString("table %1 is exist").arg(table.name);
        return true;
    }
    else
    {
        QSqlQuery query(currDatabase());
        QString strCreateTmp;
        for(int i=0;i<table.fields.count();i++)
        {
            strCreateTmp+=table.fields.at(i).fieldName+" "+table.fields.at(i).dataType+",";
        }
        strCreateTmp=parseTmp(strCreateTmp);
        QString strSql=QString("create table %1(%2)").arg(table.name).arg(strCreateTmp);
        qDebug()<<"create table,sql string "<<strSql;
        return query.exec(strSql);
    }
}
void tbDisplayModel::showtable(QString strName,QString filter)
{
    if(!currDatabase().isOpen())
        currDatabase().open();

#if 1
    QSqlQuery query(currDatabase());
    QString strSql;
    if(filter.isNull())
    {
        strSql=QString("select * from %1").arg(table.name);
    }
    else
    {
        strSql=QString("select * from %1 where tm like \'%2%\'").arg(table.name).arg(filter);
    }
    //    qDebug()<<"string sql"<<strSql;
    if(query.exec(strSql))
    {
        this->setQuery(query);
    }
#else
    this->setTable(table.name);
    qDebug()<<"cur table name="<<this->tableName();
#endif

    this->setEditStrategy(QSqlTableModel::OnRowChange);
    this->setSort(0,Qt::AscendingOrder);
    this->select();
    for(int i=0;i<table.fields.count();i++)
    {
        this->setHeaderData(i,Qt::Horizontal,table.fields.at(i).fieldTitle);
    }
    //    while (this->canFetchMore())
    //    {
    //        this->fetchMore();
    //    }
    //    qDebug()<<"cur index"<<curIndex<<dd.at(curIndex).first.name<<table.name<<dd.count();
    table.view->setModel(this);

    table.view->selectRow(this->rowCount()-1);

    for(int i=0;i<table.fields.count();i++)
    {
        if(!table.fields.at(i).bVisible)
        {
            table.view->setColumnHidden(i,true);//x倾角
        }
    }
}
QByteArray tbDisplayModel::parseOnePackage(int index,IPOrignal &oneRecord)
{
    QByteArray tmp;
    isOpenDB();
    QSqlQuery query(m_db);
    //id,time,ip,port,bvalid,deviceinfo,packages
    QString strval = QString("select tm,ip,port,packages from %1 where id=%2").arg(table.name).arg(index);

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


//#define CREATE_SQL4      "id INTEGER PRIMARY KEY AUTOINCREMENT, time varchar(30),deviceType varchar(10),serialNumber varchar(20),ipAddress varchar(20),ipPort varchar(20),bValid bool"
//#define TABLE_TITLE4     "ID1,时间1,设备ip地址1,设备ip端口1,当前状态1,数据包1"
//#define TABLE_NAME4 "connectedClients"
bool tbDisplayModel::insertIPPackageRecord(IPOrignal &dval)
{
    isOpenDB();
    //    qDebug()<<"db="<<m_db.isValid()<<m_db.tables();
    QSqlQuery query(m_db);
    bool flag;
#if 0
    QString strval = QString::fromWCharArray(L"INSERT INTO %1 VALUES(:id1, :id2, :id3, :id4, :id5, :id6, :id7)").arg(TABLE_NAME4);
    query.prepare(strval);
    qDebug()<<"ip record sql"<<strval;
    query.bindValue(":id2",QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss"));
    query.bindValue(":id3",dval.ipAddress);
    query.bindValue(":id4",dval.ipPort);
    query.bindValue(":id5", "dval.");
    query.bindValue(":id6", "dval.bValid");
    query.bindValue(":id7", dval.packages);
    flag=query.exec();
#else
    QString sqlBuffer=QString("insert into %1 (tm,ip,port,bValid,deviceInfo,packages) values (\'%2\',\'%3\',\'%4\',%5,\'%6\',\'%7\')").arg(TABLE_NAME4)
            .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss"))
            .arg(dval.ipAddress)
            .arg(dval.ipPort)
            .arg(1)
            .arg("f")
            .arg(dval.packages);

    flag=query.exec(sqlBuffer);
    qDebug()<<"sql="<<sqlBuffer;
#endif
    //    isCloseDB();
    return flag;
}
void tbDisplayModel::Transaction(void)
{
    if(m_db.isOpen())
        m_db.transaction();
}
void tbDisplayModel::Commit(void)
{
    if(m_db.isOpen())
        m_db.commit();
}
bool tbDisplayModel::setData ( const QModelIndex & index, const QVariant & value, int role)
{
    return QSqlTableModel::setData (index, value, role);
}
tbDisplayModel::~tbDisplayModel()
{
    if(m_db.isOpen())
        m_db.close();
    if(m_db.isOpen())
    {
        qDebug()<<"~~~~"<<m_db.connectionName();
        QSqlDatabase::removeDatabase(m_db.connectionName());
    }
}
QSqlDatabase tbDisplayModel::currDatabase(void)
{
    return m_db;
}


bool tbDisplayModel::ParaseCmd(QByteArray packages, qint16 &deviceNo)
{
    int i=0;
    int  nEndIndex;
    int nFirstIndex;
    bool flag=false;
    qDebug()<<"dd"<<packages;
    do
    {
        nEndIndex=packages.indexOf("0d0a");
        nFirstIndex=packages.indexOf("fe");
        qDebug()<<"index of"<<nEndIndex<<nFirstIndex;
        if((nFirstIndex!=-1)&&((nEndIndex-nFirstIndex)==28))
        {
            flag=ParaseSingleCmd(packages.mid(nFirstIndex,nEndIndex-nFirstIndex+4),deviceNo);
            i++;
            qDebug()<<i<<flag;
        }
        packages=packages.right(packages.count()-nEndIndex-4);
    }while((nEndIndex!=-1)&&(nFirstIndex!=-1));
    dataTm.clear();
    //    qDebug()<<"package counts="<<i;
    return flag;
}


bool tbDisplayModel::ParaseSingleCmd(const QByteArray &singleCmd,qint16 &deviceNo)
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
            InsertDB(cmd,dataTm);//插入数据库

            //更新设备状态
            UpdataStatursDB(cmd);
        }
        return true;
    }
    else
    {
        return false;
    }
}
QStringList tbDisplayModel::tables(void)
{
    return m_db.tables();
}
bool tbDisplayModel::InsertDB(const CCommand cmd,QString tm)
{
    QSqlQuery sqlQuery(m_db);
    static QString wendu=0;
    static QString shidu=0;
    if(cmd.lineNo)
    {
#if 1
        //0318先通过设备号查找对应的表格，在进行数据插入操作
        QString strSql;
        QString deviceNo=cmd.sRelayNo+cmd.sNo;
        QString tableName=getTableName(deviceNo);
        if(tableName.isNull())
        {
            qDebug()<<QString::fromWCharArray(L"未找到设备类型");
            return false;
        }
        if(tm.isNull())
        {
            strSql=QString("insert into %7 ( tm,deviceNO,lineNo,voltage,current,temperature,humidity) values (now(),\'%1\',%2,%3,%4,%5,%6)")
                    .arg(deviceNo).arg(cmd.lineNo).arg(QString::number(cmd.cv/1000.000,'f',3)).arg(QString::number(cmd.hi/1000.000,'f',3)).arg(wendu).arg(shidu)
                    .arg(tableName);
        }
        else
        {
            strSql=QString("insert into %8( tm,deviceNO,lineNo,voltage,current,temperature,humidity) values (\'%1\',\'%2\',%3,%4,%5,%6,%7)")
                    .arg(dataTm).arg(deviceNo).arg(cmd.lineNo).arg(QString::number(cmd.cv/1000.000,'f',3)).arg(QString::number(cmd.hi/1000.000,'f',3)).arg(wendu).arg(shidu)
                    .arg(tableName);
        }
        //        qDebug()<<"insert data"<<strSql;
        return sqlQuery.exec(strSql);
#else
        QString strSql;
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
        //        qDebug()<<"insert data"<<strSql;
        return sqlQuery.exec(strSql);
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
void tbDisplayModel::UpdataStatursDB(CCommand command)
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
void tbDisplayModel::parseAll(void)
{
    //    m_db.transaction();
    QSqlQuery query(m_db);
    if(query.exec(QString("select *from %1").arg(table.name)))
    {
        while(query.next())
        {
            qint16 d;
            IPOrignal cc;
            qDebug()<<"index"<<curIndex;
            QByteArray mm=parseOnePackage(curIndex++,cc);

            dataTm=cc.time;
            if( ParaseCmd( mm ,d))
            {
                qDebug()<<"time"<<cc.time;
            }
        }
    }
    //    m_db.commit();
}
QString tbDisplayModel::getCurIndex(void)
{
    return QString::number(curIndex);
}
QStringList tbDisplayModel::getALLDeviceNo(void)
{
    QSqlQuery query(m_db);
//    QString strSql=QString("Select distinct deviceNo  from %1").arg("detailinfo0514");
    QString strSql=QString("Select  deviceId  from %1").arg("deviceInfo");

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
//函数功能：设备
bool tbDisplayModel::insertDeviceStatus(void)
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

    //    QFile file("utf8.txt");
    //    QByteArray utfstr;
    //    if(file.open(QFile::ReadWrite))
    //    {
    //        utfstr=(file.readAll());
    //        file.close();
    //    }
    //    if(!utfstr.isEmpty())
    //    {
    //        ////    QByteArray dd=QString(QString::fromWCharArray(L"成都").toUtf8());
    //        QString strsql;//=QString::fromWCharArray(L"insert into deviecestaturs0514 (deviceNo) values(\'%1\')").arg(QString::fromWCharArray(,utfstr.to));//.arg("成都"/*QString::fromWCharArray(L"sfjs你").toUtf8()*/);

    //        qDebug()<<"sql"<<strsql;
    //        return query.exec(strsql);
    //    }
    //    return true;
}
//函数功能：对table对象的记录数进行统计
long tbDisplayModel::getRecordCount(void)
{
    long tmp=0;
    QSqlQuery query(m_db);
    QString strSql=QString("select count(*) from %1").arg(table.name);
    qDebug()<<"get record count sql="<<strSql;
    if(query.exec(strSql))
    {
        while(query.next())
        {
            tmp=query.value(0).toLongLong();
        }
        qDebug()<<"recoud count="<<tmp;
    }
    return tmp;
}
//函数功能：根据设备id查询id的数据存储的表格
QString tbDisplayModel::getTableName(QString id)
{
    QSqlQuery query(m_db);
    QString strSql=QString("select tableName from %1 where deviceId=\'%2\'").arg("deviceInfo").arg(id);
//    qDebug()<<"found tablename ,sql="<<strSql;
    if(query.exec(strSql))
    {
        while(query.next())
        {
            return query.value(0).toString();
        }
    }
    return "";
}
//函数功能：根据供电段的名称查询该供电段所属的传感器类型
QStringList tbDisplayModel::getDeviceType(QString department)
{
    QSqlQuery query(m_db);
    QString strSql=QString("select distinct deviceType from %1 where department= \'%2\'").arg("deviceInfo").arg(department);
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
QStringList tbDisplayModel::getDeparment(void)
{
    QSqlQuery query(m_db);
    QString strSql=QString("select DISTINCT department from %1 ").arg("deviceInfo");
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
