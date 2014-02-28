#ifndef BOARD_H
#define BOARD_H

#include "PlacedProject.h"
#include "Tile.h"
#include "TileOnBoard.h"

#include <QObject>

#include <boost/optional/optional.hpp>

namespace Model
{
	class Board : public QObject
	{
		Q_OBJECT
		
	public:
		Board( unsigned inSize = 5 );

		unsigned getNrOfRows() const;
		unsigned getNrOfCols() const;
		bool isTile( unsigned inCol, unsigned inRow ) const;
		boost::optional< Model::TileOnBoard > const & getTile( unsigned inCol, unsigned inRow ) const;
		boost::optional< Model::TileOnBoard > & getTile( unsigned inCol, unsigned inRow );

		bool isPossibleTile( Tile const & inTile );
		bool isEmptySpot( unsigned inCol, unsigned inRow ) const;
		bool isValidTilePlacement( Model::TileOnBoard const & inTile, unsigned inCol, unsigned inRow ) const;
		bool isValidAlternateTilePlacement( Model::TileOnBoard const & inTile, unsigned inCol, unsigned inRow ) const;
		bool placeValidTile( Model::TileOnBoard const & inTile, unsigned inCol, unsigned inRow );
		unsigned placeStartTile( Model::TileOnBoard const & inTile );

		bool isOccupiedRoad( unsigned inCol, unsigned inRow, Area::Area inArea ) const;
		bool isOccupiedCity( unsigned inCol, unsigned inRow, Area::Area inArea ) const;
		bool isOccupiedField( PlacedField const & inField ) const;
		std::vector< PlacedPiece > removePieces( unsigned inCol, unsigned inRow, Area::Area inArea );

		void checkForFinishedCities( unsigned inCol, unsigned inRow );
		void checkForFinishedRoads( unsigned inCol, unsigned inRow );
		void checkForFinishedCloisters( unsigned inCol, unsigned inRow );
		bool isFinishedCity( unsigned inCol, unsigned inRow, Area::Area inArea ) const;
		PlacedCity getUpperLeftPlacedCity( unsigned inCol, unsigned inRow, Area::Area inArea ) const;
		bool isFinishedCloister( unsigned inCol, unsigned inRow ) const;
		bool isFullySurrounded( unsigned inCol, unsigned inRow ) const;

		std::string toString() const;
		std::string shortPrint( unsigned inCol, unsigned inRow ) const;

		unsigned getPointsForCloister( unsigned inCol, unsigned inRow ) const;
		std::vector< PlacedRoad > getCompleteRoad( PlacedRoad const & inPlacedRoad ) const;
		std::vector< PlacedCity > getCompleteCity( PlacedCity const & inPlacedCity ) const;
		std::vector< PlacedField > getCompleteField( PlacedField const & inPlacedField ) const;

	signals:
		void finishedCloister( unsigned inCol, unsigned inRow );
		void finishedCity( std::vector< PlacedCity > inTiles );
		void finishedRoad( std::vector< PlacedRoad > inTiles );

		void colsAddedLeft( unsigned inNrOfCols );
		void rowsAddedTop( unsigned inNrOfRows );
		void dimensionsChanged();

	private:
		bool placeTile( Model::TileOnBoard const & inTile, unsigned inCol, unsigned inRow );
		bool removeTile( unsigned inCol, unsigned inRow );

		void widenUp( unsigned inCol, unsigned inRow );
		void addRowsOnTop( unsigned inNrOfRows );
		void addRowsBelow( unsigned inNrOfRows );
		void addColsLeft( unsigned inNrOfCols );
		void addColsRight( unsigned inNrOfCols );

	private:
		unsigned mNrRows;
		unsigned mNrCols;
		std::vector< boost::optional< Model::TileOnBoard > > mBoard; // one vector of size cols * rows might be easier to use than nested vectors
	};
}

#endif // BOARD_H
