#ifndef HEADDATABASE_H
#define HEADDATABASE_H
//#include "qdatetime.h"
#include <QtGui>
#include <qtableview.h>
//#include "database.h"
#include "subsqlmodel.h"
enum eCONNECTSTATUS
{
    DEVICE_CONNECTED,
    DEVICE_DISCONNECT,
    DEVICE_ERROR,
    SERVER_CONNECTED,
    SERVER_DISCONNECT,
};


typedef struct
{
    int nId;                //1序号
    QString sDeviceName;    //2设备名称
    QString sSN;            //3设备序号
    QString sDepartment;    //4所属路局
    QString sPosition;      //5安装位置
    QString installTime;    //6安装时间
    QString addTime;        //7添加时间
    QString wire1Name;      //8线路1名称
    QString wire2Name;      //9线路2名称
    QString wire3Name;      //10线路3名称
    QString wire4Name;      //11线路4名称
    QString wire5Name;      //12线路5名称
    QString wire6Name;      //13线路6名称
}deviceInfo;

#define CREATE_SQL2      "id INTEGER PRIMARY KEY AUTOINCREMENT,deviceName varchar(30),SN varchar(30),department varchar(30),position varchar(30),installTm varchar(30),addTm varchar(30),w1Name varchar(30),w2Name varchar(30),w3Name varchar(30),w4Name varchar(30),w5 varchar(30),w6Name varchar(30)"
#define TABLE_TITLE2     "ID1,设备名称1,设备序号1,所属路局1,安装位置1,安装时间1,添加时间1,线路11,线路21,线路31,线路41,线路51,线路61"
#define TABLE_NAME2 "wireDeviceInfo"

struct IPOrignal
{
    int id;                 //1     id
    QString time;           //2     连接时间
    QString Type;           //4     设备类型
    QString serialNumber;   //5     设备编号
    QString ipAddress;      //6     设备ip地址
    QString ipPort;         //7     设备ip端口
    bool    bValid;         //8     当前有效状态
    QString packages;       //9     数据包
    IPOrignal()
    {}
};




typedef struct
{
    int id;                 //1 ID
    QString originalData;   //2 原始数据
}wireOriginal;
typedef struct
{
    QString poleindex;
    QString station;
}tableStruct_t;

struct wireCmdPackage
{
    qint8 start;
    qint32 relayNo;
    qint8 length;   //数据长度
    qint8 key;      //特征码
    qint8 type;     //装置类型
    qint16 no;      //设备序号
    qint16 cv;      //温度或电压
    qint16 hi;      //湿度或电流
    qint8 cks;      //校验和
    qint16 enter;//处理多余的回车换行
};
struct CCommand
{
    QString sRelayNo;   //集中器编号
    qint8 length;
//    QString sLength;    //数据长度
    qint8 key;
//    QString sKey;       //特征码
    qint8 type;
//    QString sType;      //装置类型
    qint16 no;
    QString sNo;        //装置编号
    qint16 cv;//温度或电压
//    QString sCv;        //
    qint16 hi;//湿度或电流
//    QString sHi;
    qint8 cks;//校验和
//    QString sCks;
    qint16 enter;//处理多余的回车换行
    QDateTime tm;//时间
    qint8 lineNo;//线路编号
};
#define DB_PATHS  "./cc1.db"
//添加设备
#define ADD_DEVICE_UTF8 "\346\267\273\345\212\240\350\256\276\345\244\207"
//当前连接数
#define CUR_CONNECTS_UTF8 "\345\275\223\345\211\215\350\277\236\346\216\245\346\225\260:"
#define Wire_Type 0x06
#endif // HEADDATABASE_H
