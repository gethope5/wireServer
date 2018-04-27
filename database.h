#ifndef _DATA_BASE_H_
#define _DATA_BASE_H_
#include <QtSql>
#include <QtGui>
#include "headDatabase.h"
#if XLS
#include "subexcelsave.h"
#endif
#include <qtableview.h>
#include <qfiledialog.h>
#include <qtextcodec.h>
#define showALL 0
//表示设备信息的本地mysql数据表格
#define MYSQL_deviceTable_name "deviceinfo"
class subSqlModel;
class MeasureDB:public QObject
{
    Q_OBJECT
public:
    MeasureDB(QTableView *m_tableView,QSqlDatabase &db);
    MeasureDB(QVector<QTableView *> tableView);
    bool insertIPPackageRecord(IPOrignal &dval);
    ~MeasureDB();
    bool createTable(tableInfo);
    bool addConnection(void);
    void addConnection(QString dbName);
    QSqlDatabase m_db;
    void showDataTable(tableInfo &tableName,QString filter=QDateTime::currentDateTime().toString("yyyy-MM-dd"));
    bool modifyRecord(int id, const QString& station,const QString& pid,const int structFlg);
    bool insertRecordWireData(wireOrignalData & dval);
    void Transaction(void);
    void Commit(void);
    QSqlDatabase currDatabase(void);
    QString curDataName;
    void exportCSVFile(QString);
    void isOpenDB(void);
    void isCloseDB(void);
    wireOrignalData parseWirePackage(wireCmdPackage);
    QByteArray parseOnePackage(int index,IPOrignal &oneRecord);

    bool ParaseSingleCmd(const QByteArray &singleCmd,qint16 &deviceNo);
    bool ParaseCmd(QByteArray packages, qint16 &deviceNo);
    void UpdataStatursDB(CCommand command);
    void parseAll(void);
    void tran(void);
    void commit(void);
    static QString tbPackagesName;
    static QString tbDetectDataName;

    static tableInfo table1;
    static tableInfo table3;
    static tableInfo table4;
    QStringList getALLDeviceNo(void);

    void showTables(void);
    QString getCurIndex(void);
    bool insertDeviceStatus(void);
    QString getTableName(QString devieNo);
    void udapteTableName(void);
    static QPair<QStringList,QStringList> deviceTables;//deviceId,tableName
    QStringList getDeviceType(QString department);
    QStringList getDeparment(void);
private:
    bool insertWireDb(const CCommand cmd,QString tm);
    bool insertBDb(const CCommand cmd);
    bool insertCurrentDb(const CCommand cmd);
    bool displayType;//true,一个tableview显示多个table；false，一个table对应一个tableview
    QString dataTm;
    int curIndex;
    B_Data curBValue;
    Current_Data curCurrentValue;
signals:
    void updateInfo(QString);
};

#endif


