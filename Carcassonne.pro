#-------------------------------------------------
#
# Project created by QtCreator 2012-11-21T19:12:17
#
#-------------------------------------------------

QT       += core gui

TARGET = Carcassonne
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    tile.cpp \
    tileonboard.cpp \
    piece.cpp \
    placedpiece.cpp \
    color.cpp \
    board.cpp \
    boardwidget.cpp \
    tilewidget.cpp \
    createbasegametiles.cpp

HEADERS  += mainwindow.h \
    tile.h \
    tileonboard.h \
    piece.h \
    color.h \
    placedpiece.h \
    board.h \
    boardwidget.h \
    tilewidget.h \
    createbasegametiles.h

INCLUDEPATH += "C:\\Program Files\\boost\\boost_1_52_0"

FORMS    += mainwindow.ui

RESOURCES += \
    tiles.qrc
