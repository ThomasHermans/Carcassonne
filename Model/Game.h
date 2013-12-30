#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include "PlacedProject.h"
#include "Tile.h"

#include "Model/Player.h"

#include <QObject>

#include <boost/optional/optional.hpp>

#include <set>
#include <vector>

namespace Model
{
	class Game : public QObject
	{
		Q_OBJECT
	public:
		Game( QObject * inParent = 0 );
		Game( std::string const & inTiles, QObject * inParent = 0 );
		Game( std::vector< Player > const & inPlayers, QObject * inParent = 0 );
		~Game();

		std::vector< Player > const & getPlayers();

		unsigned getNrOfRows() const;
		unsigned getNrOfCols() const;
		unsigned getStartRow() const;
		unsigned getStartCol() const;
		Player const & getCurrentPlayer() const;

		void rotateTile( unsigned inCol, unsigned inRow );
		void dropTile( unsigned inCol, unsigned inRow, std::string const & inTileId, Model::Rotation inRotation );
		void placeTileOnBoard( unsigned inCol, unsigned inRow, Model::Rotation inRotation = Model::kCw0 );
		void placeStartTileOnBoard();

		boost::optional< Tile > getNextTile() const;

		void tryToPlacePiece
		(
			Color::Color inColor,
			Piece::PieceType inType,
			unsigned inCol,
			unsigned inRow,
			Area::Area inArea
		);

		void endTurn();

		void calculateEndPoints();

	public slots:
		void onEndCurrentTurn();

	signals:
		void tileRotated( unsigned inCol, unsigned inRow, std::string inId, Model::Rotation inRot );
		void tilePlaced( unsigned col, unsigned row, std::string id, Model::Rotation rot );
		void tileUnplaced( unsigned inCol, unsigned inRow );
		void nextTile( std::string inNextId );
		void tilesLeft( unsigned inNr );

		void piecePlaced( unsigned inCol, unsigned inRow, Model::Area::Area inArea, Model::Player const & inCurrentPlayer );
		void pieceReturned( unsigned inCol, unsigned inRow, Model::Area::Area inArea, Model::Player const & inCurrentPlayer );

		void finishedCloister( unsigned inCol, unsigned inRow );

		void currentPlayerChanged( Model::Player const & inCurrentPlayer );
		void playerInfoChanged( Model::Player const & inNewInfo );

		void endOfGame( unsigned inTilesLeft );

	private slots:
		void addColsLeft( unsigned inNrOfCols );
		void addRowsTop( unsigned inNrOfRows );

		void onFinishedCloister( unsigned inCol, unsigned inRow );
		void onFinishedRoad( std::vector< PlacedRoad > const & inRoad );
		void onFinishedCity( std::vector< PlacedCity > const & inCity );

	private:
		void pickNextTile();
		void updateOccupations();

		Player & getPlayer( Color::Color inColor );

		void
		returnPieces
		(
			std::vector< PlacedPiece > const & inPieces,
			unsigned inCol,
			unsigned inRow
		);

		void
		awardPoints
		(
			std::set< Color::Color > const & inColors,
			unsigned inPoints
		);

		bool isEmptySpot( unsigned inCol, unsigned inRow ) const;
		bool isCurrentSpot( unsigned inCol, unsigned inRow ) const;
		bool isOccupied( Area::Area inArea ) const;

		// Initialization
		void connectPlayerSignals();
		void connectGameSignals();

	private:
		Board mBoard;
		unsigned mStartRow;
		unsigned mStartCol;

		boost::optional< Model::TileOnBoard > mCurrentPlacedTile;

		unsigned mCurrentPlacedRow;
		unsigned mCurrentPlacedCol;	

		std::vector< Tile > mBag;
		boost::optional< Tile > mNextTile;

		std::vector< Player > mPlayers;
		unsigned mCurrentPlayer;
		unsigned mPiecesPlacedInCurrentTurn;
	};
}

#endif // GAME_H
