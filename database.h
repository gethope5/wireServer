#ifndef _DATA_BASE_H_
#define _DATA_BASE_H_

#include <QString>
#include <QStringList>

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <qtableview.h>
#include <QSqlTableModel>
#include <qcolor.h>
#include <QPair>
#define LOCALDEBUG 0

#include "headDatabase.h"
#define MYSQL 1
#define TABLE_NAME4 "deviceaddress"
#define DB_NAME "testDb.db"
struct fieldInfo
{
    QString fieldName;
    QString fieldTitle;
    bool bVisible;
    QString dataType;
    QString value;
};
struct tableInfo
{
    QString name;
    QTableView *view;
    QVector<fieldInfo> fields;
    tableInfo()
    {
        view=NULL;
    }
};
class tbDisplayModel : public QSqlTableModel
{
    Q_OBJECT

public:

    tbDisplayModel (QString tableName,QTableView *table=0, QObject * parent = 0, QSqlDatabase db = QSqlDatabase() ) ;
    ~tbDisplayModel();

	tableInfo table;
    void Commit(void);
	void isOpenDB(void);
	void isCloseDB(void);
	void Transaction(void);
	void initialCreate(bool f=true);
    void initialDb( const QString& dbName);
	void showtable(QString strName,QString filter="");
    QStringList tables(void);
    QSqlDatabase currDatabase(void);
    virtual bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );
	long getRecordCount(void);
	  bool insertRecord(tableInfo table,int startIndex=0);
    bool insertIPPackageRecord(IPOrignal &dval);
    bool createTable(tableInfo &);
    void connectDB(void);

    void addConnection(QString dbName);
	
			 
    QByteArray parseOnePackage(int index,IPOrignal &oneRecord );

    bool ParaseSingleCmd(const QByteArray &singleCmd,qint16 &deviceNo);
    bool ParaseCmd(QByteArray packages, qint16 &deviceNo);
    void UpdataStatursDB(CCommand command);
    void parseAll(void);
   


    
    QStringList getALLDeviceNo(void);
	QString getCurIndex(void);
    bool insertDeviceStatus(void);
    void showTables(void);	
    static QStringList detailinfo0514_Devcies;
    QString getTableName(QString id);
    QStringList getDeviceType(QString department);
    QStringList getDeparment(void);

private:
    bool InsertDB(const CCommand cmd,QString tm);

    bool displayType;//true,一个tableview显示多个table；false，一个table对应一个tableview
    QString dataTm;
    int curIndex;
    QSqlDatabase m_db;
    QString parseTmp(QString &tmp);
    
signals:
    //    void updateInfo(QString);
    void db_status(bool);
};

#endif


