#include "setdatedlg.h"
#include "ui_setdatedlg.h"

setDateDlg::setDateDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setDateDlg)
{
    ui->setupUi(this);
    connect(ui->pbtnCancel,SIGNAL(clicked()),this,SLOT(reject()));
    connect(ui->pbtnOk,SIGNAL(clicked()),this,SLOT(getData()));
    this->setWindowTitle(QString::fromWCharArray(L"请选择日期..."));
}

setDateDlg::~setDateDlg()
{
    delete ui;
}
QString setDateDlg::getData(void)
{
//    qDebug()<<"date"    <<ui->calendarWidget->monthShown()<<ui->calendarWidget->yearShown()<<ui->calendarWidget->selectedDate().toString();
    accept();
    return ui->calendarWidget->selectedDate().toString("yyyy-MM-dd");
}
