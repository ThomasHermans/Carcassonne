#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "src-view/BoardView.h"
#include "src-view/TileItem.h"
#include "src-view/Typedefs.h"

#include <QMainWindow>

#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QScrollArea>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>

#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QGraphicsScene>

class DragData;
class DragMeepleLabel;

struct GuiPlacedPiece
{
	QGraphicsPolygonItem* mItem;
	int mX;
	int mY;
	QColor mColor;
	
	GuiPlacedPiece( QGraphicsPolygonItem* inItem, int inX, int inY, QColor inColor )
	:
	mItem( inItem ),
	mX( inX ),
	mY( inY ),
	mColor( inColor )
	{}
};

class GameWindow : public QMainWindow
{
	Q_OBJECT
public:
	explicit GameWindow( QWidget *parent = 0 );
	~GameWindow();

	void clearTile( int x, int y );
	void rotateTile( int x, int y, std::string inId, int inRotation );
	void displayTilesLeft( unsigned int inNr );

	void setActivePlayer
	(
		std::string const & inName,
		Dragging::Color inColor,
		int inScore,
		int inPiecesLeft
	);
	void setMeepleLeft( int inPiecesLeft );

	void finishCloister( int inX, int inY );
	void finishCity( int inLeft, int inRight, int inTop, int inBottom );
	void finishRoad( int inLeft, int inRight, int inTop, int inBottom );
	
signals:
	void clicked( int x, int y );
	void endCurrentTurn();
	void tryToPlacePiece();
	void tryToPlacePiece( DragData const & inData, int inX, int inY );
	
public slots:
	void setTile( int inX, int inY, std::string inId, int inRotation );
	void setNextTile( std::string inId );
	void fadeNextTile();
	void onClicked( int x, int y );
	void placePiece( int inX, int inY, QColor inColor );
	void returnPiece( int inX, int inY, QColor inColor );

private:
	void updateSceneRect();

private:
	QGraphicsScene *mBoardScene;
	BoardView *mBoardView;
	std::vector< TileItem* > mTiles;
	std::vector< GuiPlacedPiece > mMeeples;
	QVBoxLayout *mSideBarLayout;
	QLabel *mTilesLeft;
	QLabel *mPickedTileLabel;
	QLabel *mActiveUserNameLabel;
	QLabel *mActiveUserScoreLabel;
	QLabel *mActiveUserMeepleLeftLabel;
	DragMeepleLabel * mActiveUserDragFollowerLabel;
	QPushButton *mEndTurnButton;
	QPushButton *mTryToPlacePieceButton;
};

#endif // GAMEWINDOW_H
