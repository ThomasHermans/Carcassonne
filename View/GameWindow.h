#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "BoardView.h"
#include "TileItem.h"

#include "View/Typedefs.h"

#include "Utils/Typedefs.h"

#include <boost/scoped_ptr.hpp>

#include <QMainWindow>

#include <QLabel>
#include <QMenuBar>
#include <QScrollArea>
#include <QStatusBar>
#include <QToolBar>

#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QGraphicsScene>

#include <map>

QT_BEGIN_NAMESPACE
	class QGraphicsPathItem;
	class QStackedWidget;
QT_END_NAMESPACE

namespace Dragging
{
	class PieceData;
}

namespace View
{
	class AllScoresWidget;
	class DragMeepleLabel;
	class DragTileLabel;
	struct GuiPlacedPiece;
	class Meeple;
	class UserInfoWidget;

	class GameWindow : public QMainWindow
	{
		Q_OBJECT
	public:
		/**
		 *	Constructor of a main game window.
		 */
		explicit GameWindow( QWidget *parent = 0 );

		/**
		 *	Destructor.
		 */
		~GameWindow();

		/**
		 *	Add a player to the game with the given name, color
		 *	and number of basic followers to start with.
		 */
		void
		addPlayer
		(
			std::string const & inName,
			View::Color inColor
		);

		/**
		 *	Place a tile on the board at the specified location in scene
		 *	coordinates.
		 */
		void
		setTile( int inX, int inY, std::string const & inId, Rotation inRotation );

		/**
		 *	Clear a tile from the board at the specified location.
		 */
		void
		clearTile( int x, int y );

		/**
		 *	Rotate the tile at the specified location.
		 *	The tile at the specified location will be replaced with a tile with
		 *	the given id and given rotation.
		 */
		void rotateTile( int inX, int inY, std::string const & inId, Rotation inRotation );

		/**
		 *	Display the number of tiles there are left in the bag.
		 */
		void displayTilesLeft( unsigned inNr );

		/**
		 *	Set the specified player as active player.
		 */
		void setActivePlayer( std::string const & inName );

		/**
		 *	Set the score for the specified player.
		 */
		void setScore( std::string const & inName, std::size_t inScore );

		/**
		 *	Set the amount of followers the specified player has left.
		 */
		void setFollowersLeft( std::string const & inName, std::size_t inNumberOfFollowers );

		void enableLargeFollowers( std::string const & inName );

		/**
		 *	Set the amount of large followers the specified player has left.
		 */
		void setLargeFollowersLeft( std::string const & inName, std::size_t inNumberOfLargeFollowers );
		
		/**
		 *	Show the specified tile as the next to be played tile.
		 */
		void setNextTile( std::string const & inId );
		
		/**
		 *	Place a piece on the board.
		 */
		void placePiece( int inX, int inY, View::Meeple const & inPiece );

		/**
		 *	Remove a piece from the board.
		 */
		void returnPiece( int inX, int inY, View::Meeple const & inPiece );

		/**
		 *	Show the specified locations as possible locations for the current tile.
		 */
		void setPossibleLocations( Utils::Locations const & inLocations );
		
	signals:
		void clicked( int inX, int inY );
		void tileDropped( int inX, int inY, std::string const & inId, View::Rotation inRotation );
		void endCurrentTurn();
		void tryToPlacePiece( Dragging::PieceData const & inData, int inX, int inY );
		
	public slots:
		/**
		 *	Fade the tile shown as next tile, since it has been placed on the board.
		 */
		void fadeNextTile();

		/**
		 *	End the current turn.
		 */
		void onEndCurrentTurn();

		void onDroppedTile( int inX, int inY, std::string const & inTileId, View::Rotation inRotation );

	private:
		void updateSceneRect();

	private:
		QGraphicsScene *mBoardScene;
		BoardView *mBoardView;
		std::vector< TileItem* > mTiles;
		std::vector< GuiPlacedPiece > mMeeples;
		boost::scoped_ptr< QGraphicsPathItem > mPossibleLocations;
		QLabel *mTilesLeft;
		DragTileLabel *mPickedTileLabel;
		QStackedWidget * mUserInfo;
		std::map< std::string, UserInfoWidget * > mUserInfoMap;
		AllScoresWidget * mAllScoresWidget;
	};
}

#endif // GAMEWINDOW_H
