#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "BoardView.h"
#include "TileItem.h"

#include "View/Typedefs.h"

#include "Meeple.h"

#include "Utils/Location.h"
#include "Utils/Typedefs.h"

#include <boost/optional.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/signals2/signal.hpp>

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
		 *	Add a player to the game with the given name and color.
		 */
		void
		addPlayer( std::string const & inName, Color inColor );

		/**
		 *	Place a tile on the board at the specified location.
		 *
		 *	@param inLocation Where should the tile be placed (scene coordinates).
		 *	@param inId Which tile should be placed?
		 *	@param inRotation How should the tile be rotated?
		 *	@param inColor Which player placed this tile?
		 */
		void
		setTile
		(
			Utils::Location const & inLocation,
			std::string const & inId,
			Rotation inRotation,
			boost::optional< Color > const & inColor
		);

		/**
		 *	Clear a tile from the board at the specified location.
		 */
		void
		clearTile( Utils::Location const & inLocation );

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
		void setPlayerSupply( std::string const & inName, Meeple::MeepleType inMeepleType, std::size_t inAmount );
		
		/**
		 *	Show the specified tile as the next to be played tile.
		 */
		void setNextTile( std::string const & inId );
		
		/**
		 *	Place a piece on the board.
		 */
		void placePiece( int inX, int inY, Meeple const & inPiece );

		/**
		 *	Remove a piece from the board.
		 */
		void removePiece( int inX, int inY, Meeple const & inPiece );

		/**
		 *	Show the specified locations as possible locations for the current tile.
		 */
		void setPossibleLocations( Utils::Locations const & inLocations );

		void showMessage( std::string const & inMessage );

		boost::signals2::signal< void ( Utils::Location const &, std::string const & inId, Rotation inRotation ) > tilePlaced;
		boost::signals2::signal< void ( int x, int y, Meeple const & ) > piecePlaced;
		boost::signals2::signal< void () > noPiecePlaced;
		
	signals:
		void tileDropped( int inX, int inY, std::string const & inId, Rotation inRotation );
		void endCurrentTurn();
		
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
		void onDroppedPiece( Dragging::PieceData const & inData, int inX, int inY );

	private:
		void updateSceneRect();
		void toggleRecentTileBorders();
		void toggleRemovedMeeple();

	private:
		QGraphicsScene * mBoardScene;
		BoardView * mBoardView;
		std::vector< TileItem * > mTiles;
		std::vector< GuiPlacedPiece > mMeeples;
		std::vector< std::pair< Color, QGraphicsPathItem * > > mLastPlacedTiles;
		boost::scoped_ptr< QGraphicsPathItem > mPossibleLocations;
		QLabel * mTilesLeft;
		DragTileLabel * mPickedTileLabel;
		QLabel * mMessageLabel;
		QStackedWidget * mUserInfo;
		std::map< std::string, UserInfoWidget * > mUserInfoMap;
		AllScoresWidget * mAllScoresWidget;

		bool mShowRecentTileBorders;
		bool mShowRemovedMeeple;
	};
}

#endif // GAMEWINDOW_H
