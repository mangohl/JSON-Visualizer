#-------------------------------------------------
#
# Project created by QtCreator 2020-01-09T18:51:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TreeView
TEMPLATE = app


SOURCES += main.cpp \
    treeview.cpp \
    node.cpp \
    edge.cpp \
    treeviewwidget.cpp

HEADERS  += \
    treeview.h \
    node.h \
    edge.h \
    treeviewwidget.h

FORMS    += \
    treeviewwidget.ui

RESOURCES += \
    file.qrc
