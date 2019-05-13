#-------------------------------------------------
#
# Project created by QtCreator 2018-04-28T09:17:57
#
#-------------------------------------------------

QT       += core gui serialport sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HeatCounter
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include( $${PWD}/_include/nayk.pri )

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    global.cpp \
    smartstend.cpp \
    wmbus.cpp \
    formcalibration.cpp \
    formparamsedit.cpp \
    formlog.cpp \
    objectthread.cpp \
    gefest.cpp \
    dialogupdatenote.cpp \
    dialoglogindatabase.cpp \
    dialogwritingdb.cpp \
    formviewtable.cpp \
    form.cpp \
    formconnectionparams.cpp \
    formusercontrol.cpp \
    dialogsaveuserchanges.cpp \
    dialogisrunwithoutconnect.cpp

HEADERS += \
        mainwindow.h \
    global.h \
    smartstend.h \
    wmbus.h \
    formcalibration.h \
    formparamsedit.h \
    formlog.h \
    objectthread.h \
    gefest.h \
    dialogupdatenote.h \
    dialoglogindatabase.h \
    dialogwritingdb.h \
    formviewtable.h \
    form.h \
    formconnectionparams.h \
    formusercontrol.h \
    dialogsaveuserchanges.h \
    dialogisrunwithoutconnect.h

FORMS += \
        mainwindow.ui \
    formcalibration.ui \
    formparamsedit.ui \
    formlog.ui \
    dialogupdatenote.ui \
    dialoglogindatabase.ui \
    dialogwritingdb.ui \
    formviewtable.ui \
    form.ui \
    formconnectionparams.ui \
    formusercontrol.ui \
    dialogsaveuserchanges.ui \
    dialogisrunwithoutconnect.ui
