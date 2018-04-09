#ifndef SUBSQLMODEL_H
#define SUBSQLMODEL_H

#include <QObject>
#include <QSqlTableModel>
#include <qdebug.h>
//class subSqlModel : public QSqlTableModel
//{
//    Q_OBJECT
//public:
//    explicit subSqlModel(QObject *parent = 0);

//signals:

//public slots:
//};

class subSqlModel : public QSqlTableModel
{
    Q_OBJECT

public:
    subSqlModel (  QSqlDatabase db = QSqlDatabase(),QObject * parent = 0 );

    void updateRowIndex(int row, bool insert);
    void setPrimKeyAsRow(int row);

    QVariant data(const QModelIndex &item, int role) const;
    bool DeleteRecord(int row);
    virtual bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );
//  virtual bool deleteRowFromTable(int row);
    virtual void setquery(const QSqlQuery &query);
protected:

};
#endif // SUBSQLMODEL_H
