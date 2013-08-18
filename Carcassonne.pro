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
    src-model\Board.cpp \
    src-model\Color.cpp \
    src-model\CreateBaseGameTiles.cpp \
    src-model\Game.cpp \
    src-model\Piece.cpp \
    src-model\PlacedPiece.cpp \
    src-model\Player.cpp \
    src-model\TileOnBoard.cpp \
    src-model\Tile.cpp \
    src-view\BoardView.cpp \
    src-view\DragData.cpp \
    src-view\DragMeepleLabel.cpp \
    src-view\GameWindow.cpp \
    src-view\TileItem.cpp \
    GameController.cpp    

HEADERS  += \
    test_o_matic.hpp \
    UnitTests.h \
    src-model\Board.h \
    src-model\Color.h \
    src-model\CreateBaseGameTiles.h \
    src-model\Game.h \
    src-model\Piece.h \
    src-model\PlacedPiece.h \
    src-model\Player.h \
    src-model\Tile.h \
    src-model\TileOnBoard.h \
    src-view\BoardView.h \
    src-view\DragData.h \
    src-view\DragMeepleLabel.h \
    src-view\GameWindow.h \
    src-view\TileItem.h \
    src-view\Typedefs.h \
    GameController.h

INCLUDEPATH += "C:\\Program Files\\boost\\boost_1_52_0"

FORMS    += mainwindow.ui

RESOURCES += \
    tiles.qrc
