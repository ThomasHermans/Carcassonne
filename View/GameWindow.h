#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "BoardView.h"
#include "TileItem.h"

#include "View/Typedefs.h"

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

#include <map>

class AllScoresWidget;
class DragMeepleLabel;
class DragTileLabel;
class UserInfoWidget;

class QStackedWidget;

namespace Dragging
{
	class PieceData;
}

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

	void addPlayer
	(
		std::string const & inName,
		View::Color inColor,
		unsigned inNumberOfFollowers
	);

	void clearTile( int x, int y );
	void rotateTile( int inX, int inY, std::string const & inId, int inRotation );
	void displayTilesLeft( unsigned int inNr );

	void setActivePlayer( std::string const & inName );
	void setScore( std::string const & inName, unsigned inScore );
	void setFollowersLeft( std::string const & inName, unsigned inNumberOfFollowers );
	void setNextTile( std::string const & inId );

	void finishCloister( int inX, int inY );
	
signals:
	void clicked( int inX, int inY );
	void tileDropped( int inX, int inY, std::string const & inId, View::Rotation inRotation );
	void endCurrentTurn();
	void tryToPlacePiece( Dragging::PieceData const & inData, int inX, int inY );
	
public slots:
	void setTile( int inX, int inY, std::string const & inId, double inRotation );
	void fadeNextTile();
	void onClicked( int x, int y );
	void onDroppedTile( int inX, int inY, std::string const & inTileId, View::Rotation inRotation );
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
	DragTileLabel *mPickedTileLabel;
	QStackedWidget * mUserInfo;
	std::map< std::string, UserInfoWidget * > mUserInfoMap;
	QPushButton *mEndTurnButton;
	AllScoresWidget * mAllScoresWidget;
};

#endif // GAMEWINDOW_H
