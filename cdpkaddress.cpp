#include "cdpkaddress.h"

CDPKAddress::CDPKAddress()
{
    dpsAddress = 0;
    dpsNo = 0 ;
}

void CDPKAddress::bindNoAddress(QTcpSocket *client, qint16 no)
{
    dpsAddress = client;
    dpsNo = no;
}

