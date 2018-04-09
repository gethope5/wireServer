﻿#include "subsqlmodel.h"

//subSqlModel::subSqlModel(QObject *parent) : QObject(parent)
//{

//}


subSqlModel::subSqlModel( QSqlDatabase db,QObject *parent)
    : QSqlTableModel(parent,db)
{


}

#if 1
QVariant subSqlModel::data(const QModelIndex &index, int role) const
{
    QVariant value = QSqlTableModel::data(index, role);
    if (value.isValid() && role == Qt::DisplayRole)
    {

    }
    if (role ==  Qt::EditRole && index.column() == 0)
    {
        //qDebug()<<"get data" << value.toString();
        // return QVariant(20);
    }
//    if (role == Qt::TextColorRole && index.column() == 0)
//        return qVariantFromValue(QColor(Qt::blue));
    return value;
}
#endif
void subSqlModel::setPrimKeyAsRow(int row)
{
    QSqlTableModel::setData(index(row, 0), QVariant(row+1), Qt::EditRole);
}

void subSqlModel::updateRowIndex(int row, bool insert)
{
    int i;
    qDebug()<<"the total row count is : "<<rowCount()-1<<" and row is : " << row;
    if (insert)
    {
        //        for (i = rowCount()-1 ; i >= row; i--)
        for (i = 100 ; i >= row; i--)
        {
            QModelIndex nItem = index(i,0);
            int value = nItem.data().toInt()+1;
            qDebug()<<"the (row val) : "<<i<<value<<"column count"<<nItem.column()<<
                      setData (nItem, QVariant(value), Qt::EditRole);
        }
    }
    else
    {
        for (i = row; i < rowCount(); i++)
        {
            QModelIndex nItem = index(i,0);
            int value = nItem.data().toInt()-1;
            QSqlTableModel::setData (nItem, QVariant(value), Qt::EditRole);
        }
    }


}

#if 1
bool subSqlModel::setData ( const QModelIndex & index, const QVariant & value, int role)
{

    if (index.column() == 0)
    {
        //        qDebug()<<"val: "<<value.toString();
        //        return false;
    }
    return QSqlTableModel::setData (index, value, role);
}
#endif
bool subSqlModel::DeleteRecord(int row)
{
    return deleteRowFromTable(row);
}
void subSqlModel::setquery(const QSqlQuery &query)
{
    QSqlTableModel::setQuery(query);
}
