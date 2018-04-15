/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <expandpushbutton.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout_2;
    QGroupBox *groupBox_2;
    QLabel *label;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QLineEdit *lineEdit_ip;
    QLineEdit *lineEdit_port;
    QPushButton *pushButton_startS;
    QPushButton *test;
    QPushButton *pbtnAnalysis;
    QLabel *lblCurIndex;
    QPushButton *test_2;
    QRadioButton *rdbCurDay;
    QRadioButton *rdbCurMonth;
    QRadioButton *rdbOther;
    QPushButton *pbtnSetDate;
    QLineEdit *lneDate;
    QPushButton *pbtnReflash;
    QLabel *label_3;
    QListWidget *lsDeviceType;
    QLabel *label_2;
    QListWidget *lsDepartment;
    expandPushButton *pbtnUIExpand;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QTableView *tbIP;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(809, 645);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout_2 = new QGridLayout(centralWidget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setMinimumSize(QSize(300, 0));
        label = new QLabel(groupBox_2);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 320, 81, 31));
        label->setStyleSheet(QStringLiteral("border:2px solid red"));
        layoutWidget = new QWidget(groupBox_2);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(20, 10, 173, 86));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        lineEdit_ip = new QLineEdit(layoutWidget);
        lineEdit_ip->setObjectName(QStringLiteral("lineEdit_ip"));

        verticalLayout->addWidget(lineEdit_ip);

        lineEdit_port = new QLineEdit(layoutWidget);
        lineEdit_port->setObjectName(QStringLiteral("lineEdit_port"));

        verticalLayout->addWidget(lineEdit_port);

        pushButton_startS = new QPushButton(layoutWidget);
        pushButton_startS->setObjectName(QStringLiteral("pushButton_startS"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButton_startS->sizePolicy().hasHeightForWidth());
        pushButton_startS->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(pushButton_startS);

        test = new QPushButton(groupBox_2);
        test->setObjectName(QStringLiteral("test"));
        test->setGeometry(QRect(20, 197, 101, 41));
        sizePolicy.setHeightForWidth(test->sizePolicy().hasHeightForWidth());
        test->setSizePolicy(sizePolicy);
        pbtnAnalysis = new QPushButton(groupBox_2);
        pbtnAnalysis->setObjectName(QStringLiteral("pbtnAnalysis"));
        pbtnAnalysis->setGeometry(QRect(20, 250, 101, 41));
        sizePolicy.setHeightForWidth(pbtnAnalysis->sizePolicy().hasHeightForWidth());
        pbtnAnalysis->setSizePolicy(sizePolicy);
        lblCurIndex = new QLabel(groupBox_2);
        lblCurIndex->setObjectName(QStringLiteral("lblCurIndex"));
        lblCurIndex->setGeometry(QRect(140, 180, 51, 31));
        lblCurIndex->setStyleSheet(QStringLiteral("border:2px solid red"));
        test_2 = new QPushButton(groupBox_2);
        test_2->setObjectName(QStringLiteral("test_2"));
        test_2->setGeometry(QRect(130, 250, 101, 41));
        sizePolicy.setHeightForWidth(test_2->sizePolicy().hasHeightForWidth());
        test_2->setSizePolicy(sizePolicy);
        rdbCurDay = new QRadioButton(groupBox_2);
        rdbCurDay->setObjectName(QStringLiteral("rdbCurDay"));
        rdbCurDay->setGeometry(QRect(20, 100, 89, 16));
        rdbCurMonth = new QRadioButton(groupBox_2);
        rdbCurMonth->setObjectName(QStringLiteral("rdbCurMonth"));
        rdbCurMonth->setGeometry(QRect(20, 120, 89, 16));
        rdbOther = new QRadioButton(groupBox_2);
        rdbOther->setObjectName(QStringLiteral("rdbOther"));
        rdbOther->setGeometry(QRect(20, 140, 89, 16));
        pbtnSetDate = new QPushButton(groupBox_2);
        pbtnSetDate->setObjectName(QStringLiteral("pbtnSetDate"));
        pbtnSetDate->setGeometry(QRect(100, 100, 75, 41));
        lneDate = new QLineEdit(groupBox_2);
        lneDate->setObjectName(QStringLiteral("lneDate"));
        lneDate->setGeometry(QRect(180, 110, 81, 21));
        pbtnReflash = new QPushButton(groupBox_2);
        pbtnReflash->setObjectName(QStringLiteral("pbtnReflash"));
        pbtnReflash->setGeometry(QRect(160, 340, 75, 23));
        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(160, 380, 72, 15));
        lsDeviceType = new QListWidget(groupBox_2);
        lsDeviceType->setObjectName(QStringLiteral("lsDeviceType"));
        lsDeviceType->setGeometry(QRect(160, 400, 121, 131));
        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(20, 380, 72, 15));
        lsDepartment = new QListWidget(groupBox_2);
        lsDepartment->setObjectName(QStringLiteral("lsDepartment"));
        lsDepartment->setGeometry(QRect(20, 400, 111, 127));

        gridLayout_2->addWidget(groupBox_2, 0, 0, 1, 1);

        pbtnUIExpand = new expandPushButton(centralWidget);
        pbtnUIExpand->setObjectName(QStringLiteral("pbtnUIExpand"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(pbtnUIExpand->sizePolicy().hasHeightForWidth());
        pbtnUIExpand->setSizePolicy(sizePolicy1);
        pbtnUIExpand->setMinimumSize(QSize(0, 0));
        pbtnUIExpand->setMaximumSize(QSize(20, 16777215));
        pbtnUIExpand->setStyleSheet(QLatin1String("border:1px solid;\n"
"border-color: rgb(255, 255, 255);"));

        gridLayout_2->addWidget(pbtnUIExpand, 0, 1, 1, 1);

        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        tbIP = new QTableView(groupBox);
        tbIP->setObjectName(QStringLiteral("tbIP"));
        tbIP->setMinimumSize(QSize(300, 0));

        gridLayout->addWidget(tbIP, 0, 0, 1, 1);


        gridLayout_2->addWidget(groupBox, 0, 2, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 809, 26));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        groupBox_2->setTitle(QString());
        label->setText(QString());
        pushButton_startS->setText(QApplication::translate("MainWindow", "\345\274\200\345\220\257\346\234\215\345\212\241", 0));
        test->setText(QApplication::translate("MainWindow", "\345\217\221\351\200\201\345\221\275\344\273\244", 0));
        pbtnAnalysis->setText(QApplication::translate("MainWindow", "\345\210\206\346\236\220\346\227\242\346\234\211\346\225\260\346\215\256", 0));
        lblCurIndex->setText(QString());
        test_2->setText(QApplication::translate("MainWindow", "\346\265\213\350\257\225", 0));
        rdbCurDay->setText(QApplication::translate("MainWindow", "\345\275\223\345\244\251\346\225\260\346\215\256", 0));
        rdbCurMonth->setText(QApplication::translate("MainWindow", "\345\275\223\346\234\210\346\225\260\346\215\256", 0));
        rdbOther->setText(QApplication::translate("MainWindow", "\346\214\207\345\256\232\346\227\245\346\234\237", 0));
        pbtnSetDate->setText(QApplication::translate("MainWindow", "\350\256\276\347\275\256", 0));
        pbtnReflash->setText(QApplication::translate("MainWindow", "\350\256\276\345\244\207\345\210\267\346\226\260", 0));
        label_3->setText(QApplication::translate("MainWindow", "\344\274\240\346\204\237\345\231\250", 0));
        label_2->setText(QApplication::translate("MainWindow", "\344\276\233\347\224\265\346\256\265", 0));
        pbtnUIExpand->setText(QApplication::translate("MainWindow", ">>", 0));
        groupBox->setTitle(QApplication::translate("MainWindow", "IP", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
