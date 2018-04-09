#ifndef SETDATEDLG_H
#define SETDATEDLG_H

#include <QDialog>
#include <qdebug.h>
namespace Ui {
class setDateDlg;
}

class setDateDlg : public QDialog
{
    Q_OBJECT

public:
    explicit setDateDlg(QWidget *parent = 0);
    ~setDateDlg();
private:
    Ui::setDateDlg *ui;
public slots:
    QString getData(void);

};

#endif // SETDATEDLG_H
