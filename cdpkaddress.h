#ifndef CDPKADDRESS_H
#define CDPKADDRESS_H
#include <QTcpSocket>
#include <QVector>
#include <QObject>


struct CDPKAddress
{
    void bindNoAddress(QTcpSocket * client,qint16 no);
    QTcpSocket *dpsAddress;
    qint16  dpsNo;

};

#endif // CDPKADDRESS_H
