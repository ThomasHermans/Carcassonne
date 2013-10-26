#-------------------------------------------------
#
# Project created by QtCreator 2012-11-21T19:12:17
#
#-------------------------------------------------

QT       += core gui

TARGET = Carcassonne
TEMPLATE = app


SOURCES += main.cpp \
	Controller\src\GameController.cpp \
	Controller\src\StartController.cpp \
	Controller\src\SupremeController.cpp \
	test_o_matic.cpp \
	TileUnitTests.cpp \
	UnitTests.cpp \
	Model\src\Board.cpp \
	Model\src\Color.cpp \
	Model\src\CreateBaseGameTiles.cpp \
	Model\src\Game.cpp \
	Model\src\Piece.cpp \
	Model\src\PlacedPiece.cpp \
	Model\src\PlacedProject.cpp \
	Model\src\Player.cpp \
	Model\src\TileOnBoard.cpp \
	Model\src\Tile.cpp \
	View\src\AllScoresWidget.cpp \
	View\src\BoardView.cpp \
	View\src\DragData.cpp \
	View\src\DragMeepleLabel.cpp \
	View\src\GameWindow.cpp \
	View\src\StartScreen.cpp \
	View\src\StartScreenRow.cpp \
	View\src\TileItem.cpp \
	View\src\UserInfoWidget.cpp \

HEADERS += \
	Controller\GameController.h \
	Controller\SupremeController.h \
	Controller\src\StartController.h \
	test_o_matic.hpp \
	UnitTests.h \
	Model\Color.h \
	Model\Game.h \
	Model\Player.h \
	Model\src\Board.h \
	Model\src\CreateBaseGameTiles.h \
	Model\src\Piece.h \
	Model\src\PlacedPiece.h \
	Model\src\Tile.h \
	Model\src\TileOnBoard.h \
	View\DragData.h \
	View\GameWindow.h \
	View\StartScreen.h \
	View\src\AllScoresWidget.h \
	View\src\BoardView.h \
	View\src\DragMeepleLabel.h \
	View\src\StartScreenRow.h \
	View\src\TileItem.h \
	View\src\Typedefs.h \
	View\src\UserInfoWidget.h \

INCLUDEPATH += "C:\\Program Files\\boost\\boost_1_52_0"

FORMS    += mainwindow.ui

RESOURCES += \
	Resources.qrc
