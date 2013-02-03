#-------------------------------------------------
#
# Project created by QtCreator 2012-11-21T19:12:17
#
#-------------------------------------------------

QT       += core gui

TARGET = Carcassonne
TEMPLATE = app


SOURCES += main.cpp\
    test_o_matic.cpp \
    TileUnitTests.cpp \
    UnitTests.cpp \
    BoardView.cpp \
    TileItem.cpp \
    Board.cpp \
    Color_.cpp \
    CreateBaseGameTiles_.cpp \
    Game_.cpp \
    GameController_.cpp \
    GameWindow_.cpp \
    Piece_.cpp \
    PlacedPiece_.cpp \
    Tile_.cpp \
    TileOnBoard_.cpp

HEADERS  += \
    test_o_matic.hpp \
    UnitTests.h \
    BoardView.h \
    TileItem.h \
    GuiConstants.h \
    Board_.h \
    Color_.h \
    CreateBaseGameTiles_.h \
    Game_.h \
    GameController_.h \
    GameWindow_.h \
    Piece_.h \
    PlacedPiece_.h \
    Tile_.h \
    TileOnBoard_.h

INCLUDEPATH += "C:\\Program Files\\boost\\boost_1_52_0"

FORMS    += mainwindow.ui

RESOURCES += \
    tiles.qrc
