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
    static QString tbPackagesName;
    static QString tbDetectDataName;

    static tableInfo table1;
    static tableInfo table3;
    static tableInfo table4;
    static deviceInfos deviceInfo; //存储当前所有设备的基础信息，包括编号、表格名称、位置、类型(存储于表格的remark列)

    QStringList getALLDeviceNo(void);

    void showTables(void);
    QString getCurIndex(void);
    QString getTableName(QString devieNo);
    void udapteTableName(void);
//    QVector<QPair<>
    QStringList getDeviceType(QString department);
    QStringList getDeparment(void);
    QStringList   speicalCurrentDevice;
    void run(void);
    IPOrignal curIPData;
    long wireCount;
    long currentCount;
    long angleCount;
    QTimer *timer;
    void transaction(void);
    void commit(void);
    void rollback(void);
    bool insertWireRec(wire_Data & dval,QString table);
    bool insertCurrentRec(QString table,QString deviceNO,current_Data curCurrentValue,QString tm);
    bool insertBranchRec(QString table,QString deviceNo,branch_Data curCurrentValue,QString tm);
    void getDataRecords(simData &devcieInfo);
    QString  getDeviceTable(QString device );
    eDeviceType getType(QString curDeviceID);
private slots:
    void slot_showTable(void);
private:
    bool updateWireDb(const CCommand cmd,QString tm,originalPackage pac);
    bool insertBDb(const CCommand cmd,originalPackage pac);
    bool updateCurrentDb(const CCommand cmd,originalPackage pac);
    bool insertAngleDb(const CCommand cmd,originalPackage pac);
    bool updateBranchDb(const CCommand cmd,originalPackage pac);
    bool insertForceDb(const CCommand cmd,originalPackage pac);
    bool insertAirDb(const CCommand cmd,originalPackage pac);

    bool displayType;//true,一个tableview显示多个table；false，一个table对应一个tableview

    QString dataTm;
    originalPackage package;
    int curIndex;
    B_Data curBValue;
    branch_Data curBranch;
    current_Data curCurrentValue;
    angle_Data curAngleValue;
    force_Data curForceValue;
    air_Data curAirValue;
    QString getCurTable(QString curDeviceID);
    QVector<long> modifyIds;
    QString updateRecentTime(QString deviceNo,QString tableName);
    void utf8_unicode( unsigned char *inbuf, unsigned char *outbuf, int inlen);
    void unicode_utf8(const unsigned int inbuf, unsigned char *outbuf, int* outlen, int mlen);


signals:
    void updateInfo(QString);
    void updateDevice(void);
};

#endif


