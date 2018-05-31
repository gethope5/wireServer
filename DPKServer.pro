#-------------------------------------------------
#
# Project created by QtCreator 2017-02-16T15:24:43
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QT       += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DPKServer-0531
TEMPLATE = app

CONFIG += console

SOURCES += main.cpp\
        mainwindow.cpp \
    expandpushbutton.cpp \
    database.cpp \
#    subsqlmodel.cpp \
    setdatedlg.cpp

HEADERS  += mainwindow.h \
    expandpushbutton.h \
    database.h \
    headDatabase.h \
#    subsqlmodel.h \
    setdatedlg.h

FORMS    += mainwindow.ui \
    setdatedlg.ui
