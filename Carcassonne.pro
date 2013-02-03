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
    Color.cpp \
    CreateBaseGameTiles.cpp \
    Game.cpp \
    GameController.cpp \
    GameWindow.cpp \
    Piece.cpp \
    PlacedPiece.cpp \
    TileOnBoard.cpp \
    Tile.cpp \
    Board.cpp

HEADERS  += \
    test_o_matic.hpp \
    UnitTests.h \
    BoardView.h \
    TileItem.h \
    GuiConstants.h \
    Board.h \
    Color.h \
    Game.h \
    GameController.h \
    GameWindow.h \
    Piece.h \
    PlacedPiece.h \
    Tile.h \
    TileOnBoard.h \
    CreateBaseGameTiles.h

INCLUDEPATH += "C:\\Program Files\\boost\\boost_1_52_0"

FORMS    += mainwindow.ui

RESOURCES += \
    tiles.qrc
