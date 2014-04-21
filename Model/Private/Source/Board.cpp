#include "Board.h"

#include "PlacedPiece.h"

#include <boost/foreach.hpp>

namespace
{
	bool
	upperLeftCompare( Model::PlacedCity const & inLeft, Model::PlacedCity const & inRight )
	{
		if ( inLeft.row != inRight.row )
		{
			return inLeft.row < inRight.row;
		}
		else if ( inLeft.col != inRight.col )
		{
			return inLeft.col < inRight.col;
		}
		else
		{
			return inLeft.area < inRight.area;
		}
	}
}

Model::Board::Board()
:
	mTiles( 1, boost::none ),
	mStartRow( 0 ),
	mStartCol( 0 ),
	mNrRows( 1 ),
	mNrCols( 1 )
{
}

std::size_t
Model::Board::getNrOfTiles() const
{
	std::size_t nrOfTiles = 0;
	for ( Tiles::const_iterator it = mTiles.begin(); it != mTiles.end(); ++it )
	{
		if ( *it )
		{
			++nrOfTiles;
		}
	}
	return nrOfTiles;
}

int
Model::Board::getTopRow() const
{
	return - mStartRow;
}

int
Model::Board::getBottomRow() const
{
	return mNrRows - mStartRow - 1;
}

int
Model::Board::getLeftCol() const
{
	return - mStartCol;
}

int
Model::Board::getRightCol() const
{
	return mNrCols - mStartCol - 1;
}

bool
Model::Board::isTile( int inRow, int inCol ) const
{
	return getTile( inRow, inCol );
}

boost::optional< Model::TileOnBoard >
Model::Board::getTile( int inRow, int inCol ) const
{
	int const index = getIndex( inRow, inCol );
	if ( index >= 0 && index < int( mTiles.size() ) )
	{
		return mTiles[ index ];
	}
	else
	{
		return boost::none;
	}
}

boost::optional< Model::TileOnBoard > &
Model::Board::getTile( int inRow, int inCol )
{
	ensureTile( inRow, inCol );
	int const index = getIndex( inRow, inCol );
	return mTiles[ index ];
}

bool
Model::Board::placeStartTile( TileOnBoard const & inTile )
{
	if ( getNrOfTiles() == 0 )
	{
		return placeTile( inTile, 0, 0 );
	}
	else
	{
		return false;
	}
}

bool
Model::Board::isValidTilePlacement( TileOnBoard const & inTile, int inRow, int inCol ) const
{
	bool const tile = isTile( inRow, inCol );
	bool const neighbor = hasNeighbor( inRow, inCol );
	bool const sides = sidesMatch( inTile, inRow, inCol );
	return !tile && neighbor && sides;
}

bool
Model::Board::placeValidTile( TileOnBoard const & inTile, int inRow, int inCol )
{
	if ( isValidTilePlacement( inTile, inRow, inCol ) )
	{
		return placeTile( inTile, inRow, inCol );
	}
	else
	{
		return false;
	}
}

bool
Model::Board::isPossibleTile( Tile const & inTile ) const
{
	int const topRow = getTopRow() - 1;
	int const bottomRow = getBottomRow() + 1;
	int const leftCol = getLeftCol() - 1;
	int const rightCol = getRightCol() + 1;
	for ( int row = topRow; row <= bottomRow; ++row )
	{
		for ( int col = leftCol; col <= rightCol; ++col )
		{
			Rotation rot = kCw0;
			for ( std::size_t i = 0; i < 4; ++i )
			{
				TileOnBoard const tile( inTile, rot );
				if ( isValidTilePlacement( tile, row, col ) )
				{
					return true;
				}
				rot = rotateCW( rot );
			}
		}
	}
	return false;
}

bool
Model::Board::isOccupiedRoad( PlacedRoad const & inRoad ) const
{
	if ( !isTile( inRoad.row, inRoad.col ) )
	{
		return false;
	}
	ContiguousRoad const thisRoad = getTile( inRoad.row, inRoad.col )->getContiguousRoad( inRoad.area );
	std::vector< PlacedRoad > queue;
	BOOST_FOREACH( Area::Area const & area, thisRoad )
	{
		queue.push_back( PlacedRoad( inRoad.row, inRoad.col, area ) );
	}
	for ( std::size_t index = 0; index < queue.size(); ++index )
	{
		PlacedRoad const road = queue[index];
		if ( getTile( road.row, road.col )->hasPiece( road.area ) )
		{
			return true;
		}
		PlacedRoad const neighbor = getNeighbor( road );
		if ( isTile( neighbor.row, neighbor.col ) )
		{
			if ( std::find( queue.begin(), queue.end(), neighbor ) == queue.end() )
			{
				ContiguousRoad const neighborRoad = getTile( neighbor.row, neighbor.col )->getContiguousRoad( neighbor.area );
				BOOST_FOREACH( Area::Area const & area, neighborRoad )
				{
					queue.push_back( PlacedRoad( neighbor.row, neighbor.col, area ) );
				}
			}
		}
	}
	return false;
}

bool
Model::Board::isOccupiedCity( PlacedCity const & inCity ) const
{
	if ( !isTile( inCity.row, inCity.col ) )
	{
		return false;
	}
	ContiguousCity const thisCity = getTile( inCity.row, inCity.col )->getContiguousCity( inCity.area );
	std::vector< PlacedCity > queue;
	BOOST_FOREACH( Area::Area const & area, thisCity )
	{
		queue.push_back( PlacedCity( inCity.row, inCity.col, area ) );
	}
	for ( std::size_t index = 0; index < queue.size(); ++index )
	{
		PlacedCity const city = queue[index];
		if ( getTile( city.row, city.col )->hasPiece( city.area ) )
		{
			return true;
		}
		PlacedCity const neighbor = getNeighbor( city );
		if ( isTile( neighbor.row, neighbor.col ) )
		{
			if ( std::find( queue.begin(), queue.end(), neighbor ) == queue.end() )
			{
				ContiguousCity const neighborCity = getTile( neighbor.row, neighbor.col )->getContiguousCity( neighbor.area );
				BOOST_FOREACH( Area::Area const & area, neighborCity )
				{
					queue.push_back( PlacedCity( neighbor.row, neighbor.col, area ) );
				}
			}
		}
	}
	return false;
}

bool
Model::Board::isOccupiedField( PlacedField const & inField ) const
{
	if ( !isTile( inField.row, inField.col ) )
	{
		return false;
	}
	ContiguousField const thisField = getTile( inField.row, inField.col )->getContiguousField( inField.area );
	std::vector< PlacedField > queue;
	BOOST_FOREACH( Area::Area const & area, thisField )
	{
		queue.push_back( PlacedField( inField.row, inField.col, area ) );
	}
	for ( std::size_t index = 0; index < queue.size(); ++index )
	{
		PlacedField const city = queue[index];
		if ( getTile( city.row, city.col )->hasPiece( city.area ) )
		{
			return true;
		}
		PlacedField const neighbor = getNeighbor( city );
		if ( isTile( neighbor.row, neighbor.col ) )
		{
			if ( std::find( queue.begin(), queue.end(), neighbor ) == queue.end() )
			{
				ContiguousField const neighborField = getTile( neighbor.row, neighbor.col )->getContiguousField( neighbor.area );
				BOOST_FOREACH( Area::Area const & area, neighborField )
				{
					queue.push_back( PlacedField( neighbor.row, neighbor.col, area ) );
				}
			}
		}
	}
	return false;
}

std::vector< Model::PlacedPiece >
Model::Board::removePieces( PlacedProject inArea )
{
	if ( isTile( inArea.row, inArea.col ) )
	{
		return getTile( inArea.row, inArea.col )->removePieces( inArea.area );
	}
	else
	{
		return std::vector< PlacedPiece >();
	}
}

bool
Model::Board::isFinishedCity( PlacedCity const & inCity ) const
{
	if ( !isCity( inCity ) )
	{
		return false;
	}
	ContiguousCity const city = getTile( inCity.row, inCity.col )->getContiguousCity( inCity.area );
	// Check if this city is unfinished
	// Create a queue and add all areas from this city to it
	std::vector< PlacedCity > queue;
	BOOST_FOREACH( Area::Area const area, city )
	{
		queue.push_back( PlacedCity( inCity.row, inCity.col, area ) );
	}
	// Go over the queue, adding continuations as we encounter them
	// When a continuation is missing, this city is not finished
	bool finished = true;
	for ( std::size_t i = 0; i < queue.size(); ++i )
	{
		PlacedCity const neighbor = getNeighbor( queue[i] );
		if ( isTile( neighbor.row, neighbor.col ) )
		{
			// Add continuation to the queue if it is not there yet
			if ( std::find( queue.begin(), queue.end(), neighbor ) == queue.end() )
			{
				ContiguousCity const neighborCity = getTile( neighbor.row, neighbor.col )->getContiguousCity( neighbor.area );
				BOOST_FOREACH( Area::Area const area, neighborCity )
				{
					queue.push_back( PlacedCity( neighbor.row, neighbor.col, area ) );
				}
			}
		}
		else
		{
			// No continuation means unfinished city
			finished = false;
			break;
		}
	}
	return finished;
}

Model::PlacedCity
Model::Board::getIdentifierCity( PlacedCity const & inCity ) const
{
	if ( !isCity( inCity ) )
	{
		return inCity;
	}
	std::vector< PlacedCity > completeCity = getCompleteCity( inCity );
	std::sort( completeCity.begin(), completeCity.end(), &upperLeftCompare );
	return completeCity.front();
}

std::size_t
Model::Board::getNrOfSurroundingTiles( int inRow, int inCol ) const
{
	if ( !isTile( inRow, inCol ) )
	{
		return 0;
	}
	std::size_t nr = 0;
	for ( int row = inRow - 1; row <= inRow + 1; ++row )
	{
		for ( int col = inCol - 1; col <= inCol + 1; ++col )
		{
			if ( isTile( row, col ) )
			{
				++nr;
			}
		}
	}
	return nr;
}

std::vector< Model::PlacedCity >
Model::Board::getCompleteCity( PlacedCity const & inCity ) const
{
	if ( !isCity( inCity ) )
	{
		return std::vector< PlacedCity >();
	}
	std::vector< PlacedCity > completeCity;
	ContiguousCity const city = getTile( inCity.row, inCity.col )->getContiguousCity( inCity.area );
	BOOST_FOREACH( Area::Area area, city )
	{
		completeCity.push_back( PlacedCity( inCity.row, inCity.col, area ) );
	}
	for ( std::size_t i = 0; i < completeCity.size(); ++i )
	{
		PlacedCity const thisCity = completeCity[i];
		PlacedCity const neighbor = getNeighbor( thisCity );
		if ( isTile( neighbor.row, neighbor.col ) )
		{
			if ( std::find( completeCity.begin(), completeCity.end(), neighbor ) == completeCity.end() )
			{
				ContiguousCity const neighborCity = getTile( neighbor.row, neighbor.col )->getContiguousCity( neighbor.area );
				BOOST_FOREACH( Area::Area area, neighborCity )
				{
					completeCity.push_back( PlacedCity( neighbor.row, neighbor.col, area ) );
				}
			}
		}
	}
	return completeCity;
}

std::vector< Model::PlacedRoad >
Model::Board::getCompleteRoad( PlacedRoad const & inRoad ) const
{
	if ( !isRoad( inRoad ) )
	{
		return std::vector< PlacedRoad >();
	}
	std::vector< PlacedRoad > completeRoad;
	ContiguousRoad const road = getTile( inRoad.row, inRoad.col )->getContiguousRoad( inRoad.area );
	BOOST_FOREACH( Area::Area area, road )
	{
		completeRoad.push_back( PlacedRoad( inRoad.row, inRoad.col, area ) );
	}
	for ( std::size_t i = 0; i < completeRoad.size(); ++i )
	{
		PlacedRoad const thisRoad = completeRoad[i];
		PlacedRoad const neighbor = getNeighbor( thisRoad );
		if ( isTile( neighbor.row, neighbor.col ) )
		{
			if ( std::find( completeRoad.begin(), completeRoad.end(), neighbor ) == completeRoad.end() )
			{
				ContiguousRoad const neighborRoad = getTile( neighbor.row, neighbor.col )->getContiguousRoad( neighbor.area );
				BOOST_FOREACH( Area::Area area, neighborRoad )
				{
					completeRoad.push_back( PlacedRoad( neighbor.row, neighbor.col, area ) );
				}
			}
		}
	}
	return completeRoad;
}

std::vector< Model::PlacedField >
Model::Board::getCompleteField( PlacedField const & inField ) const
{
	if ( !isField( inField ) )
	{
		return std::vector< PlacedField >();
	}
	std::vector< PlacedField > completeField;
	ContiguousField const field = getTile( inField.row, inField.col )->getContiguousField( inField.area );
	BOOST_FOREACH( Area::Area area, field )
	{
		completeField.push_back( PlacedField( inField.row, inField.col, area ) );
	}
	for ( std::size_t i = 0; i < completeField.size(); ++i )
	{
		PlacedField const thisField = completeField[i];
		PlacedField const neighbor = getNeighbor( thisField );
		if ( isTile( neighbor.row, neighbor.col ) )
		{
			if ( std::find( completeField.begin(), completeField.end(), neighbor ) == completeField.end() )
			{
				ContiguousField const neighborField = getTile( neighbor.row, neighbor.col )->getContiguousField( neighbor.area );
				BOOST_FOREACH( Area::Area area, neighborField )
				{
					completeField.push_back( PlacedField( neighbor.row, neighbor.col, area ) );
				}
			}
		}
	}
	return completeField;
}

bool
Model::Board::hasInn( PlacedRoad const & inRoad ) const
{
	if ( !isTile( inRoad.row, inRoad.col ) )
	{
		return false;
	}
	return getTile( inRoad.row, inRoad.col )->hasInn( inRoad.area );
}

int
Model::Board::getIndex( int inRow, int inCol ) const
{
	int const internalRow = mStartRow + inRow;
	int const internalCol = mStartCol + inCol;
	if ( internalRow < 0 || internalRow >= mNrRows || internalCol < 0 || internalCol >= mNrCols )
	{
		return -1;
	}
	else
	{
		return ( ( mStartRow + inRow ) * mNrCols + ( mStartCol + inCol ) );
	}
}

void
Model::Board::ensureTile( int inRow, int inCol )
{
	int const internalRow = mStartRow + inRow;
	if ( internalRow < 0 )
	{
		addRowsTop( -internalRow );
	}
	else if ( internalRow >= mNrRows )
	{
		addRowsBottom( internalRow - mNrRows + 1 );
	}
	int const internalCol = mStartCol + inCol;
	if ( internalCol < 0 )
	{
		addColsLeft( -internalCol );
	}
	else if ( internalCol >= mNrCols )
	{
		addColsRight( internalCol - mNrCols + 1 );
	}
}

void
Model::Board::addRowsTop( int inNrRows )
{
	mTiles.insert( mTiles.begin(), mNrCols * inNrRows, boost::none );
	mStartRow += inNrRows;
	mNrRows += inNrRows;
}

void
Model::Board::addRowsBottom( int inNrRows )
{
	mTiles.insert( mTiles.end(), mNrCols * inNrRows, boost::none );
	mNrRows += inNrRows;
}

void
Model::Board::addColsLeft( int inNrCols )
{
	Tiles::iterator it = mTiles.begin();
	for ( int row = 0; row < mNrRows; ++row )
	{
		for ( int newCol = 0; newCol < inNrCols; ++newCol )
		{
			it = mTiles.insert( it, boost::none );
			++it;
		}
		for ( int existingCol = 0; existingCol < mNrCols; ++existingCol )
		{
			++it;
		}
	}
	mStartCol += inNrCols;
	mNrCols += inNrCols;
}

void
Model::Board::addColsRight( int inNrCols )
{
	Tiles::iterator it = mTiles.begin();
	for ( int row = 0; row < mNrRows; ++row )
	{
		for ( int existingCol = 0; existingCol < mNrCols; ++existingCol )
		{
			++it;
		}
		for ( int newCol = 0; newCol < inNrCols; ++newCol )
		{
			it = mTiles.insert( it, boost::none );
			++it;
		}
	}
	mNrCols += inNrCols;
}

bool
Model::Board::placeTile( TileOnBoard const & inTile, int inRow, int inCol )
{
	ensureTile( inRow, inCol );
	if ( !isTile( inRow, inCol ) )
	{
		getTile( inRow, inCol ) = inTile;
		checkForFinishedCloisters( inRow, inCol );
		checkForFinishedCities( inRow, inCol );
		checkForFinishedRoads( inRow, inCol );
		return true;
	}
	else
	{
		return false;
	}
}

bool
Model::Board::hasNeighbor( int inRow, int inCol ) const
{
	return
	(
		isTile( inRow, inCol - 1 ) || isTile( inRow, inCol + 1 )
		|| isTile( inRow - 1, inCol ) || isTile( inRow + 1, inCol )
	);
}

bool
Model::Board::sidesMatch( TileOnBoard const & inTile, int inRow, int inCol ) const
{
	if ( isTile( inRow, inCol - 1 ) && !getTile( inRow, inCol - 1 )->matchesLeftOf( inTile ) )
	{
		return false;
	}
	if ( isTile( inRow, inCol + 1 ) && !getTile( inRow, inCol + 1 )->matchesRightOf( inTile ) )
	{
		return false;
	}
	if ( isTile( inRow - 1, inCol ) && !getTile( inRow - 1, inCol )->matchesAbove( inTile ) )
	{
		return false;
	}
	if ( isTile( inRow + 1, inCol ) && !getTile( inRow + 1, inCol )->matchesBelow( inTile ) )
	{
		return false;
	}
	return true;
}

void
Model::Board::checkForFinishedCities( int inRow, int inCol )
{
	if ( !isTile( inRow, inCol ) )
	{
		return;
	}
	std::vector< ContiguousCity > const cities = getTile( inRow, inCol )->getContiguousCities();
	BOOST_FOREACH( ContiguousCity const & city, cities )
	{
		// Check if this city is unfinished
		// Create a queue and add all areas from this city to it
		std::vector< PlacedCity > queue;
		BOOST_FOREACH( Area::Area const area, city )
		{
			queue.push_back( PlacedCity( inRow, inCol, area ) );
		}
		// Go over the queue, adding continuations as we encounter them
		// When a continuation is missing, this city is not finished
		bool finished = true;
		for ( std::size_t i = 0; i < queue.size(); ++i )
		{
			PlacedCity const neighbor = getNeighbor( queue[i] );
			if ( isTile( neighbor.row, neighbor.col ) )
			{
				// Add continuation to the queue if it is not there yet
				if ( std::find( queue.begin(), queue.end(), neighbor ) == queue.end() )
				{
					ContiguousCity const neighborCity = getTile( neighbor.row, neighbor.col )->getContiguousCity( neighbor.area );
					BOOST_FOREACH( Area::Area const area, neighborCity )
					{
						queue.push_back( PlacedCity( neighbor.row, neighbor.col, area ) );
					}
				}
			}
			else
			{
				// No continuation means unfinished city
				finished = false;
				break;
			}
		}
		if ( finished )
		{
			finishedCity( queue );
		}
	}
}

void
Model::Board::checkForFinishedRoads( int inRow, int inCol )
{
	if ( !isTile( inRow, inCol ) )
	{
		return;
	}
	std::vector< ContiguousRoad > const roads = getTile( inRow, inCol )->getContiguousRoads();
	BOOST_FOREACH( ContiguousRoad const & road, roads )
	{
		// Check if this road is unfinished
		// Create a queue and add all areas from this road to it
		std::vector< PlacedRoad > queue;
		BOOST_FOREACH( Area::Area const area, road )
		{
			queue.push_back( PlacedRoad( inRow, inCol, area ) );
		}
		// Go over the queue, adding continuations as we encounter them
		// When a continuation is missing, this road is not finished
		bool finished = true;
		for ( std::size_t i = 0; i < queue.size(); ++i )
		{
			PlacedRoad const neighbor = getNeighbor( queue[i] );
			if ( isTile( neighbor.row, neighbor.col ) )
			{
				// Add continuation to the queue if it is not there yet
				if ( std::find( queue.begin(), queue.end(), neighbor ) == queue.end() )
				{
					ContiguousRoad const neighborRoad = getTile( neighbor.row, neighbor.col )->getContiguousRoad( neighbor.area );
					BOOST_FOREACH( Area::Area const area, neighborRoad )
					{
						queue.push_back( PlacedRoad( neighbor.row, neighbor.col, area ) );
					}
				}
			}
			else
			{
				// No continuation means unfinished road
				finished = false;
				break;
			}
		}
		if ( finished )
		{
			finishedRoad( queue );
		}
	}
}

void
Model::Board::checkForFinishedCloisters( int inRow, int inCol )
{
	for ( int row = inRow - 1; row <= inRow + 1; ++row )
	{
		for ( int col = inCol - 1; col <= inCol + 1; ++col )
		{
			if ( isFinishedCloister( row, col ) )
			{
				finishedCloister( row, col );
			}
		}
	}
}

bool
Model::Board::isFinishedCloister( int inRow, int inCol ) const
{
	return
	(
		isTile( inRow, inCol )
		&& getTile( inRow, inCol )->getCenter() == Tile::kCenterCloister
		&& isFullySurrounded( inRow, inCol )
	);
}

bool
Model::Board::isFullySurrounded( int inRow, int inCol ) const
{
	for ( int row = inRow - 1; row <= inRow + 1; ++row )
	{
		for ( int col = inCol - 1; col <= inCol + 1; ++col )
		{
			if ( !isTile( row, col ) )
			{
				return false;
			}
		}
	}
	return true;
}

bool
Model::Board::isCity( PlacedCity const & inCity ) const
{
	return isTile( inCity.row, inCity.col ) && getTile( inCity.row, inCity.col )->isCity( inCity.area );
}

bool
Model::Board::isRoad( PlacedRoad const & inRoad ) const
{
	return isTile( inRoad.row, inRoad.col ) && getTile( inRoad.row, inRoad.col )->isRoad( inRoad.area );
}

bool
Model::Board::isField( PlacedField const & inField ) const
{
	return isTile( inField.row, inField.col ) && getTile( inField.row, inField.col )->isField( inField.area );
}