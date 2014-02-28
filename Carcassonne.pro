#-------------------------------------------------
#
# Project created by QtCreator 2012-11-21T19:12:17
#
#-------------------------------------------------

QT       += core gui

TARGET = Carcassonne
TEMPLATE = app


SOURCES += main.cpp \
	Controller\Private\Source\GameController.cpp \
	Controller\Private\Source\ModelViewGlue.cpp \
	Controller\Private\Source\StartController.cpp \
	Controller\Private\Source\Settings.cpp \
	Controller\Private\Source\SupremeController.cpp \
	test_o_matic.cpp \
	TileUnitTests.cpp \
	UnitTests.cpp \
	Model\Private\Source\Area.cpp \
	Model\Private\Source\Board.cpp \
	Model\Private\Source\Color.cpp \
	Model\Private\Source\CreateBaseGameTiles.cpp \
	Model\Private\Source\Game.cpp \
	Model\Private\Source\Piece.cpp \
	Model\Private\Source\PlacedPiece.cpp \
	Model\Private\Source\PlacedProject.cpp \
	Model\Private\Source\Player.cpp \
	Model\Private\Source\Rotation.cpp \
	Model\Private\Source\TileOnBoard.cpp \
	Model\Private\Source\Tile.cpp \
	View\Private\Source\AllScoresWidget.cpp \
	View\Private\Source\BoardView.cpp \
	View\Private\Source\DragData.cpp \
	View\Private\Source\DragMeepleLabel.cpp \
	View\Private\Source\DragTileLabel.cpp \
	View\Private\Source\GameWindow.cpp \
	View\Private\Source\MeepleUtils.cpp \
	View\Private\Source\QtGlue.cpp \
	View\Private\Source\StartScreen.cpp \
	View\Private\Source\StartScreenRow.cpp \
	View\Private\Source\TileItem.cpp \
	View\Private\Source\TileUtils.cpp \
        View\Private\Source\UserInfoWidget.cpp

HEADERS += \
	Controller\GameController.h \
	Controller\SupremeController.h \
	Controller\Private\Include\ModelViewGlue.h \
	Controller\Private\Include\Settings.h \
	Controller\Private\Include\StartController.h \
	test_o_matic.hpp \
	UnitTests.h \
	Model\Area.h \
	Model\Color.h \
	Model\Game.h \
	Model\Piece.h \
	Model\Player.h \
	Model\Rotation.h \
	Model\Private\Include\Board.h \
	Model\Private\Include\CreateBaseGameTiles.h \
	Model\Private\Include\PlacedPiece.h \
	Model\Private\Include\PlacedProject.h \
	Model\Private\Include\Tile.h \
	Model\Private\Include\TileOnBoard.h \
	View\DragData.h \
	View\GameWindow.h \
	View\StartScreen.h \
        View\Typedefs.h \
	View\Private\Include\AllScoresWidget.h \
	View\Private\Include\BoardView.h \
	View\Private\Include\DragMeepleLabel.h \
	View\Private\Include\DragTileLabel.h \
	View\Private\Include\MeepleUtils.h \
	View\Private\Include\QtGlue.h \
	View\Private\Include\StartScreenRow.h \
	View\Private\Include\TileItem.h \
	View\Private\Include\TileUtils.h \
        View\Private\Include\UserInfoWidget.h

INCLUDEPATH += \
	"C:\\Program Files\\boost\\boost_1_52_0" \
	"Controller\\Private\\Include" \
	"Model\\Private\\Include" \
        "View\\Private\\Include"

RESOURCES += \
	Resources\Resources.qrc

RC_FILE = Carcassonne.rc
