#-------------------------------------------------
#
# Project created by QtCreator 2017-09-02T21:55:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SocialNetwork
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    person.cpp \
    renderarea.cpp \
    adddialog.cpp \
    dialog_in_ar.cpp \
    network.cpp

HEADERS  += mainwindow.h \
    def.h \
    linklist.h \
    person.h \
    renderarea.h \
    adddialog.h \
    dialog_in_ar.h \
    network.h

FORMS    += mainwindow.ui \
    adddialog.ui \
    dialog_in_ar.ui \
    network.ui

RC_FILE=images/logo.rc

DISTFILES += \
    images/logo.ico \
    images/logo.rc
