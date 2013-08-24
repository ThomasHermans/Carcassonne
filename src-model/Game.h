#ifndef GAME_H
#define GAME_H

#include "src-model/Board.h"
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
	Game();
	~Game();

	std::vector< Player > const & getPlayers();

	unsigned int getNrOfRows() const;
	unsigned int getNrOfCols() const;
	unsigned int getStartRow() const;
	unsigned int getStartCol() const;
	Player const & getCurrentPlayer() const;

	void clickTile(unsigned int inCol, unsigned int inRow);
	void placeTileOnBoard(unsigned int inCol, unsigned int inRow);
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
	void onFinishedCloister( unsigned int inCol, unsigned int inRow );

signals:
	void tileRotated(unsigned int inCol, unsigned int inRow, std::string inId, TileOnBoard::Rotation inRot);
	void tilePlaced(unsigned int col, unsigned int row, std::string id, TileOnBoard::Rotation rot);
	void tileUnplaced(unsigned int inCol, unsigned int inRow);
	void nextTile(std::string inNextId);
	void tilesLeft(unsigned int inNr);

	void piecePlaced( unsigned int inCol, unsigned int inRow, Area::Area inArea, Player const & inCurrentPlayer );
	void pieceReturned( unsigned int inCol, unsigned int inRow, Player const & inCurrentPlayer );

	void finishedCloister(unsigned int inCol, unsigned int inRow);
	void finishedCity(std::vector< std::pair< unsigned int, unsigned int > > inTiles);
	void finishedRoad(std::vector< std::pair< unsigned int, unsigned int > > inTiles);

	void currentPlayerChanged( Player const & inCurrentPlayer );
	void playerInfoChanged( Player const & inNewInfo );

	void endOfGame(unsigned int inTilesLeft);

private slots:
	void addColsLeft( unsigned inNrOfCols );
	void addRowsTop( unsigned inNrOfRows );

private:
	void pickNextTile();
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

private:
	Board mBoard;
	unsigned int mStartRow;
	unsigned int mStartCol;

	boost::optional< TileOnBoard > mCurrentPlacedTile;

	unsigned int mCurrentPlacedRow;
	unsigned int mCurrentPlacedCol;	

	std::vector< Tile > mBag;
	boost::optional< Tile > mNextTile;

	std::vector< Player > mPlayers;
	unsigned int mCurrentPlayer;
};

#endif // GAME_H
