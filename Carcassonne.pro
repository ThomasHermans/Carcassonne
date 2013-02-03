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
    createbasegametiles.cpp \
    test_o_matic.cpp \
    TileUnitTests.cpp \
    UnitTests.cpp \
    BoardView.cpp \
    TileItem.cpp \
    GameWindow.cpp \
    Game.cpp \
    GameController.cpp \
    Board.cpp

HEADERS  += mainwindow.h \
    tile.h \
    tileonboard.h \
    piece.h \
    color.h \
    placedpiece.h \
    createbasegametiles.h \
    test_o_matic.hpp \
    UnitTests.h \
    BoardView.h \
    TileItem.h \
    GameWindow.h \
    GuiConstants.h \
    Game.h \
    GameController.h \
    Board.h

INCLUDEPATH += "C:\\Program Files\\boost\\boost_1_52_0"

FORMS    += mainwindow.ui

RESOURCES += \
    tiles.qrc
