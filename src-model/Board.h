#ifndef BOARD_H
#define BOARD_H

#include "src-model/Tile.h"
#include "src-model/TileOnBoard.h"

#include <QObject>

#include <boost/optional/optional.hpp>

class Board : public QObject
{
	Q_OBJECT

private:
	typedef std::pair< unsigned, FRCArea::CityArea > LocatedCity;
	typedef std::pair< unsigned, FRCArea::RoadArea > LocatedRoad;
	typedef std::pair< unsigned, FRCArea::FieldArea > LocatedField;
	
public:
	Board( unsigned inSize = 5 );
	Board( Board const & inBoard );
	~Board();
	Board & operator = ( Board const & inBoard );

	unsigned getNrOfRows() const;
	unsigned getNrOfCols() const;
	bool isTile( unsigned inCol, unsigned inRow ) const;
	boost::optional< TileOnBoard > const & getTile( unsigned inCol, unsigned inRow ) const;
	boost::optional< TileOnBoard > & getTile( unsigned inCol, unsigned inRow );

	bool isPossibleTile( Tile const & inTile );
	bool isEmptySpot( unsigned inCol, unsigned inRow ) const;
	bool isValidTilePlacement( TileOnBoard const & inTile, unsigned inCol, unsigned inRow ) const;
	bool isValidAlternateTilePlacement( TileOnBoard const & inTile, unsigned inCol, unsigned inRow ) const;
	bool placeValidTile( TileOnBoard const & inTile, unsigned inCol, unsigned inRow );
	unsigned placeStartTile( TileOnBoard const & inTile );

	bool isOccupied( unsigned inCol, unsigned inRow, Area::Area inArea ) const;
	bool isOccupiedRoad( unsigned inCol, unsigned inRow, FRCArea::RoadArea inArea ) const;
	std::vector< PlacedPiece > removePieces( unsigned inCol, unsigned inRow, Area::Area inArea );

	void checkForFinishedCities( unsigned inCol, unsigned inRow );
	void checkForFinishedRoads( unsigned inCol, unsigned inRow );
	void checkForFinishedCloisters( unsigned inCol, unsigned inRow );
	bool isFinishedCloister( unsigned inCol, unsigned inRow ) const;
	bool isFullySurrounded( unsigned inCol, unsigned inRow ) const;

	std::string toString() const;
	std::string shortPrint( unsigned inCol, unsigned inRow ) const;

signals:
	void finishedCloister( unsigned inCol, unsigned inRow );
	void finishedCity( std::vector< std::pair< unsigned, unsigned > > inTiles );
	void finishedRoad( std::vector< std::pair< unsigned, unsigned > > inTiles );

	void colsAddedLeft( unsigned inNrOfCols );
	void rowsAddedTop( unsigned inNrOfRows );

private:
	bool placeTile( TileOnBoard const & inTile, unsigned inCol, unsigned inRow );
	bool removeTile( unsigned inCol, unsigned inRow );

	void widenUp( unsigned inCol, unsigned inRow );
	void addRowsOnTop( unsigned inNrOfRows );
	void addRowsBelow( unsigned inNrOfRows );
	void addColsLeft( unsigned inNrOfCols );
	void addColsRight( unsigned inNrOfCols );

	bool isContinueued( LocatedCity inLocatedCity ) const;
	bool isContinueued( LocatedField inLocatedField ) const;
	unsigned getNeighborLocation( LocatedCity inLocatedCity ) const;
	unsigned getNeighborLocation( LocatedField inLocatedField ) const;

private:
	unsigned mNrRows;
	unsigned mNrCols;
	std::vector< boost::optional< TileOnBoard > > mBoard; // one vector of size cols * rows might be easier to use than nested vectors
};

#endif // BOARD_H
