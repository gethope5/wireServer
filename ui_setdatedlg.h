/********************************************************************************
** Form generated from reading UI file 'setdatedlg.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETDATEDLG_H
#define UI_SETDATEDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCalendarWidget>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_setDateDlg
{
public:
    QPushButton *pbtnCancel;
    QCalendarWidget *calendarWidget;
    QPushButton *pbtnOk;

    void setupUi(QDialog *setDateDlg)
    {
        if (setDateDlg->objectName().isEmpty())
            setDateDlg->setObjectName(QStringLiteral("setDateDlg"));
        setDateDlg->resize(400, 300);
        pbtnCancel = new QPushButton(setDateDlg);
        pbtnCancel->setObjectName(QStringLiteral("pbtnCancel"));
        pbtnCancel->setGeometry(QRect(210, 242, 91, 31));
        calendarWidget = new QCalendarWidget(setDateDlg);
        calendarWidget->setObjectName(QStringLiteral("calendarWidget"));
        calendarWidget->setGeometry(QRect(70, 20, 248, 197));
        pbtnOk = new QPushButton(setDateDlg);
        pbtnOk->setObjectName(QStringLiteral("pbtnOk"));
        pbtnOk->setGeometry(QRect(90, 240, 91, 31));

        retranslateUi(setDateDlg);

        QMetaObject::connectSlotsByName(setDateDlg);
    } // setupUi

    void retranslateUi(QDialog *setDateDlg)
    {
        setDateDlg->setWindowTitle(QApplication::translate("setDateDlg", "Dialog", 0));
        pbtnCancel->setText(QApplication::translate("setDateDlg", "Cancel", 0));
        pbtnOk->setText(QApplication::translate("setDateDlg", "OK", 0));
    } // retranslateUi

};

namespace Ui {
    class setDateDlg: public Ui_setDateDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETDATEDLG_H
