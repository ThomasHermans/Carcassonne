#ifndef GAME_H
#define GAME_H

#include "src-model/Board.h"
#include "src-model/PlacedProject.h"
#include "src-model/Player.h"
#include "src-model/Tile.h"

#include <QObject>

#include <boost/optional/optional.hpp>

#include <set>
#include <vector>

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

	void clickTile( unsigned inCol, unsigned inRow );
	void placeTileOnBoard( unsigned inCol, unsigned inRow );
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

public slots:
	void onEndCurrentTurn();
	void onFinishedCloister( unsigned inCol, unsigned inRow );

signals:
	void tileRotated( unsigned inCol, unsigned inRow, std::string inId, TileOnBoard::Rotation inRot );
	void tilePlaced( unsigned col, unsigned row, std::string id, TileOnBoard::Rotation rot );
	void tileUnplaced( unsigned inCol, unsigned inRow );
	void nextTile( std::string inNextId );
	void tilesLeft( unsigned inNr );

	void piecePlaced( unsigned inCol, unsigned inRow, Area::Area inArea, Player const & inCurrentPlayer );
	void pieceReturned( unsigned inCol, unsigned inRow, Area::Area inArea, Player const & inCurrentPlayer );

	void finishedCloister( unsigned inCol, unsigned inRow );

	void currentPlayerChanged( Player const & inCurrentPlayer );
	void playerInfoChanged( Player const & inNewInfo );

	void endOfGame( unsigned inTilesLeft );

private slots:
	void addColsLeft( unsigned inNrOfCols );
	void addRowsTop( unsigned inNrOfRows );

	void onFinishedRoad( std::vector< PlacedRoad > const & inRoad );
	void onFinishedCity( std::vector< PlacedCity > const & inCity );

private:
	void pickNextTile();
	void updateOccupations();

	void rotateCurrentTile();

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

	boost::optional< TileOnBoard > mCurrentPlacedTile;

	unsigned mCurrentPlacedRow;
	unsigned mCurrentPlacedCol;	

	std::vector< Tile > mBag;
	boost::optional< Tile > mNextTile;

	std::vector< Player > mPlayers;
	unsigned mCurrentPlayer;
	unsigned mPiecesPlacedInCurrentTurn;
};

#endif // GAME_H
