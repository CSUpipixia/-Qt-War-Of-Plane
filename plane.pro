#-------------------------------------------------
#
# Project created by QtCreator 2019-07-04T20:08:21
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia
QT       += sql
QT       += network
QT       += widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = plane
TEMPLATE = app


SOURCES += main.cpp \
    object.cpp \
    plane.cpp \
    myplane.cpp \
    enemyplane.cpp \
    bullet.cpp \
    control.cpp \
    lifesupply.cpp \
    skillprop.cpp \
    logindialog.cpp \
    account.cpp \
    dbhelper.cpp \
    missile.cpp

HEADERS  += \
    object.h \
    plane.h \
    myplane.h \
    enemyplane.h \
    bullet.h \
    control.h \
    lifesupply.h \
    skillprop.h \
    logindialog.h \
    account.h \
    dbhelper.h \
    missile.h

CONFIG += C++11
CONFIG += resources_big

FORMS    +=

RESOURCES += \
    images.qrc \
    qss.qrc
