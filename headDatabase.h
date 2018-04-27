#ifndef HEADDATABASE_H
#define HEADDATABASE_H
//#include "qdatetime.h"
#include <QtGui>
#include <qtableview.h>
//#include "database.h"
//#include "subsqlmodel.h"
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
    int id;                 //1     id
    QString time;           //2     时间
    QString deviceType;     //4     设备类型
    QString serialNumber;   //5     设备编号
    QString sensorNumber;       //6     传感器编号
    QString voltage;        //7     设备电压
    QString eleCurrent;     //8     设备电流
    QString temperature;    //9     设备温度
    QString humidity;       //10     设备湿度
}wireOrignalData;
//                       "ID1,                                  时间1,            设备类型1,              设备编号1,                  传感器编号1,             设备电压1,           设备电流1,               设备温度1,               设备湿度1"
#define CREATE_SQL1      "id INTEGER PRIMARY KEY AUTOINCREMENT, time varchar(30),deviceType varchar(10),serialNumber varchar(20),sersorNumber varchar(20),voltage varchar(20),eleCurrent varchar(20),temperature varchar(10),humidity varchar(10)"
#define TABLE_TITLE1     "ID1,时间1,设备类型1,设备编号1,传感器编号1,电压1,电流1,温度1,湿度1"
#define TABLE_NAME1 "wireOrignal"


typedef struct
{
    int id;                 //1     id
    QString time;           //2     连接时间
    QString Type;           //4     设备类型
    QString serialNumber;   //5     设备编号
    QString ipAddress;      //6     设备ip地址
    QString ipPort;         //7     设备ip端口
    bool    bValid;         //8     当前有效状态
    QString packages;       //9     数据包
}IPOrignal;
#define CREATE_SQL4      "id INTEGER PRIMARY KEY AUTOINCREMENT, time varchar(30),deviceType varchar(10),serialNumber varchar(20),ipAddress varchar(20),ipPort varchar(20),bValid bool"
#define TABLE_TITLE4     QString::fromWCharArray(L"ID1,时间1,设备ip地址1,设备ip端口1,当前状态0,数据包1")
#define TABLE_NAME4 "deviceaddress"

struct tableInfo
{
    QString createSQL;      //1表格创建SQL语句
    QString titleName;      //2表格字段中文名
    QString tableName;      //3表格名称
    QTableView *tableView;
};

//电屏铠数据解析包
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
struct B_Data
{
  float b1;             //1
  float b2;
  float temperature;
  float temperatureIn;
  float voltage;        //0x43
  float voltage2;
  float humidity;       //0x40
  float humidityIn;
  char package;
  B_Data()
  {
      b1=0;             //1
      b2=0;
      temperature=0;
      temperatureIn=0;
      voltage=0;        //0x43
      voltage2=0;
      humidity=0;       //0x40
      humidityIn=0;
      package=0;
  }
};
struct Current_Data
{
  float p1T1;             //1
  float p1H1;
  float p2V1;
  float p2Current;
  float p3T2;        //0x43
  float p3H2;
  float p4Tmp1;       //0x40
  float p4Tmp2;
  char package;
  Current_Data()
  {
      p1T1=0;
      p1H1=0;
      p2V1=0;
      p2Current=0;
      p3T2=0;
      p3H2=0;
      p4Tmp1=0;
      p4Tmp2=0;
      package=0;
  }
};
#define DB_PATHS  "./cc1.db"
//添加设备
#define ADD_DEVICE_UTF8 "\346\267\273\345\212\240\350\256\276\345\244\207"
//当前连接数
#define CUR_CONNECTS_UTF8 "\345\275\223\345\211\215\350\277\236\346\216\245\346\225\260:"
#define Wire_Type 0x06      //电屏铠
#define B_Type 0x0e         //B值监测
#define CURRENT_Type 0x03   //泄露电流

//#define "bdetectkm"
#endif // HEADDATABASE_H
