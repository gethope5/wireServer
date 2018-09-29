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
#define jiaozuoCurrent_Device1  "030007000703"
#define jiaozuoCurrent_Device2  "030007000603"
#define showALL 0
//表示设备信息的本地mysql数据表格
#define MYSQL_deviceTable_name "deviceinfo"

class subSqlModel;
#if sub_thread
class MeasureDB:public QThread
{
    Q_OBJECT
public:
    MeasureDB(QTableView *m_tableView,QSqlDatabase &db,QObject *parent = 0 );
    MeasureDB(QVector<QTableView *> tableView);
#else
class MeasureDB:public QObject
{
    Q_OBJECT
public:
    MeasureDB(QTableView *m_tableView,QSqlDatabase &db);
    MeasureDB(QVector<QTableView *> tableView);
    static bool bParse;
#endif
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
    QByteArray parseOnePackage(int index,IPOrignal &oneRecord);

    bool parseOneCmd(const originalPackage &singleCmd );
    bool ParaseCmd(originalPackage & packages );
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
    QString getTableName(QString devieNo);
    void udapteTableName(void);
    static QPair<QStringList,QStringList> deviceTables;//deviceId,tableName
    static QStringList deviceName;
    static QStringList deviceTypes;
    QStringList getDeviceType(QString department);
    QStringList getDeparment(void);
    QStringList   speicalCurrentDevice;
    void run(void);
    IPOrignal curIPData;
    long wireCount;
    long currentCount;
    long angleCount;
    QTimer *timer;
    void startUpdateTable(void);
    eDeviceType getCurType(QString curDeviceID);
private slots:
    void slot_showTable(void);
private:
    bool insertWireDb(const CCommand cmd,QString tm,originalPackage pac);
    bool insertBDb(const CCommand cmd,originalPackage pac);
    bool insertCurrentDb(const CCommand cmd,originalPackage pac);
    bool insertAngleDb(const CCommand cmd,originalPackage pac);
    bool insertBranchDb(const CCommand cmd,originalPackage pac);
    bool insertForceDb(const CCommand cmd,originalPackage pac);
    bool insertAirDb(const CCommand cmd,originalPackage pac);

    bool displayType;//true,一个tableview显示多个table；false，一个table对应一个tableview

    QString dataTm;
    originalPackage package;
    int curIndex;
    B_Data curBValue;
    branch_Data curBranch;
    Current_Data curCurrentValue;
    angle_Data curAngleValue;
    force_Data curForceValue;
    air_Data curAirValue;
    QString getCurTable(QString curDeviceID);
    void rollback(void);
    QVector<long> modifyIds;
signals:
    void updateInfo(QString);
};

#endif


