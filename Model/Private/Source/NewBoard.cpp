#include "NewBoard.h"

#include "PlacedPiece.h"

#include <boost/foreach.hpp>

namespace
{
	bool
	upperLeftCompare( Model::NewPlacedCity const & inLeft, Model::NewPlacedCity const & inRight )
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

Model::NewBoard::NewBoard()
:
	mTiles( 1, boost::none ),
	mStartRow( 0 ),
	mStartCol( 0 ),
	mNrRows( 1 ),
	mNrCols( 1 )
{
}

std::size_t
Model::NewBoard::getNrOfTiles() const
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

bool
Model::NewBoard::isTile( int inRow, int inCol ) const
{
	return getTile( inRow, inCol );
}

boost::optional< Model::TileOnBoard >
Model::NewBoard::getTile( int inRow, int inCol ) const
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
Model::NewBoard::getTile( int inRow, int inCol )
{
	ensureTile( inRow, inCol );
	int const index = getIndex( inRow, inCol );
	return mTiles[ index ];
}

bool
Model::NewBoard::placeStartTile( TileOnBoard const & inTile )
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
Model::NewBoard::isValidTilePlacement( TileOnBoard const & inTile, int inRow, int inCol ) const
{
	bool const tile = isTile( inRow, inCol );
	bool const neighbor = hasNeighbor( inRow, inCol );
	bool const sides = sidesMatch( inTile, inRow, inCol );
	return !tile && neighbor && sides;
}

bool
Model::NewBoard::placeValidTile( TileOnBoard const & inTile, int inRow, int inCol )
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
Model::NewBoard::isOccupiedRoad( NewPlacedRoad const & inRoad ) const
{
	if ( !isTile( inRoad.row, inRoad.col ) )
	{
		return false;
	}
	ContiguousRoad const thisRoad = getTile( inRoad.row, inRoad.col )->getContiguousRoad( inRoad.area );
	std::vector< NewPlacedRoad > queue;
	BOOST_FOREACH( Area::Area const & area, thisRoad )
	{
		queue.push_back( NewPlacedRoad( inRoad.row, inRoad.col, area ) );
	}
	for ( std::size_t index = 0; index < queue.size(); ++index )
	{
		NewPlacedRoad const road = queue[index];
		if ( getTile( road.row, road.col )->hasPiece( road.area ) )
		{
			return true;
		}
		NewPlacedRoad const neighbor = getNeighbor( road );
		if ( isTile( neighbor.row, neighbor.col ) )
		{
			if ( std::find( queue.begin(), queue.end(), neighbor ) == queue.end() )
			{
				ContiguousRoad const neighborRoad = getTile( neighbor.row, neighbor.col )->getContiguousRoad( neighbor.area );
				BOOST_FOREACH( Area::Area const & area, neighborRoad )
				{
					queue.push_back( NewPlacedRoad( neighbor.row, neighbor.col, area ) );
				}
			}
		}
	}
	return false;
}

bool
Model::NewBoard::isOccupiedCity( NewPlacedCity const & inCity ) const
{
	if ( !isTile( inCity.row, inCity.col ) )
	{
		return false;
	}
	ContiguousCity const thisCity = getTile( inCity.row, inCity.col )->getContiguousCity( inCity.area );
	std::vector< NewPlacedCity > queue;
	BOOST_FOREACH( Area::Area const & area, thisCity )
	{
		queue.push_back( NewPlacedCity( inCity.row, inCity.col, area ) );
	}
	for ( std::size_t index = 0; index < queue.size(); ++index )
	{
		NewPlacedCity const city = queue[index];
		if ( getTile( city.row, city.col )->hasPiece( city.area ) )
		{
			return true;
		}
		NewPlacedCity const neighbor = getNeighbor( city );
		if ( isTile( neighbor.row, neighbor.col ) )
		{
			if ( std::find( queue.begin(), queue.end(), neighbor ) == queue.end() )
			{
				ContiguousCity const neighborCity = getTile( neighbor.row, neighbor.col )->getContiguousCity( neighbor.area );
				BOOST_FOREACH( Area::Area const & area, neighborCity )
				{
					queue.push_back( NewPlacedCity( neighbor.row, neighbor.col, area ) );
				}
			}
		}
	}
	return false;
}

bool
Model::NewBoard::isOccupiedField( NewPlacedField const & inField ) const
{
	if ( !isTile( inField.row, inField.col ) )
	{
		return false;
	}
	ContiguousField const thisField = getTile( inField.row, inField.col )->getContiguousField( inField.area );
	std::vector< NewPlacedField > queue;
	BOOST_FOREACH( Area::Area const & area, thisField )
	{
		queue.push_back( NewPlacedField( inField.row, inField.col, area ) );
	}
	for ( std::size_t index = 0; index < queue.size(); ++index )
	{
		NewPlacedField const city = queue[index];
		if ( getTile( city.row, city.col )->hasPiece( city.area ) )
		{
			return true;
		}
		NewPlacedField const neighbor = getNeighbor( city );
		if ( isTile( neighbor.row, neighbor.col ) )
		{
			if ( std::find( queue.begin(), queue.end(), neighbor ) == queue.end() )
			{
				ContiguousField const neighborField = getTile( neighbor.row, neighbor.col )->getContiguousField( neighbor.area );
				BOOST_FOREACH( Area::Area const & area, neighborField )
				{
					queue.push_back( NewPlacedField( neighbor.row, neighbor.col, area ) );
				}
			}
		}
	}
	return false;
}

std::vector< Model::PlacedPiece >
Model::NewBoard::removePieces( NewPlacedProject inArea )
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
Model::NewBoard::isFinishedCity( NewPlacedCity const & inCity ) const
{
	if ( !isCity( inCity ) )
	{
		return false;
	}
	ContiguousCity const city = getTile( inCity.row, inCity.col )->getContiguousCity( inCity.area );
	// Check if this city is unfinished
	// Create a queue and add all areas from this city to it
	std::vector< NewPlacedCity > queue;
	BOOST_FOREACH( Area::Area const area, city )
	{
		queue.push_back( NewPlacedCity( inCity.row, inCity.col, area ) );
	}
	// Go over the queue, adding continuations as we encounter them
	// When a continuation is missing, this city is not finished
	bool finished = true;
	for ( std::size_t i = 0; i < queue.size(); ++i )
	{
		NewPlacedCity const neighbor = getNeighbor( queue[i] );
		if ( isTile( neighbor.row, neighbor.col ) )
		{
			// Add continuation to the queue if it is not there yet
			if ( std::find( queue.begin(), queue.end(), neighbor ) == queue.end() )
			{
				ContiguousCity const neighborCity = getTile( neighbor.row, neighbor.col )->getContiguousCity( neighbor.area );
				BOOST_FOREACH( Area::Area const area, neighborCity )
				{
					queue.push_back( NewPlacedCity( neighbor.row, neighbor.col, area ) );
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

Model::NewPlacedCity
Model::NewBoard::getIdentifierCity( NewPlacedCity const & inCity ) const
{
	if ( !isCity( inCity ) )
	{
		return inCity;
	}
	ContiguousCity const city = getTile( inCity.row, inCity.col )->getContiguousCity( inCity.area );
	// Find the complete city for inCity
	std::vector< NewPlacedCity > completeCity;
	BOOST_FOREACH( Area::Area area, city )
	{
		completeCity.push_back( NewPlacedCity( inCity.row, inCity.col, area ) );
	}
	for ( std::size_t i = 0; i < completeCity.size(); ++i )
	{
		NewPlacedCity const thisCity = completeCity[i];
		NewPlacedCity const neighbor = getNeighbor( thisCity );
		if ( isTile( neighbor.row, neighbor.col ) )
		{
			if ( std::find( completeCity.begin(), completeCity.end(), neighbor ) == completeCity.end() )
			{
				ContiguousCity const neighborCity = getTile( neighbor.row, neighbor.col )->getContiguousCity( neighbor.area );
				BOOST_FOREACH( Area::Area area, neighborCity )
				{
					completeCity.push_back( NewPlacedCity( neighbor.row, neighbor.col, area ) );
				}
			}
		}
	}
	std::sort( completeCity.begin(), completeCity.end(), &upperLeftCompare );
	return completeCity.front();
}

std::size_t
Model::NewBoard::getNrOfSurroundingTiles( int inRow, int inCol ) const
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

int
Model::NewBoard::getIndex( int inRow, int inCol ) const
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
Model::NewBoard::ensureTile( int inRow, int inCol )
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
Model::NewBoard::addRowsTop( int inNrRows )
{
	mTiles.insert( mTiles.begin(), mNrCols * inNrRows, boost::none );
	mStartRow += inNrRows;
	mNrRows += inNrRows;
}

void
Model::NewBoard::addRowsBottom( int inNrRows )
{
	mTiles.insert( mTiles.end(), mNrCols * inNrRows, boost::none );
	mNrRows += inNrRows;
}

void
Model::NewBoard::addColsLeft( int inNrCols )
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
Model::NewBoard::addColsRight( int inNrCols )
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
Model::NewBoard::placeTile( TileOnBoard const & inTile, int inRow, int inCol )
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
Model::NewBoard::hasNeighbor( int inRow, int inCol ) const
{
	return
	(
		isTile( inRow, inCol - 1 ) || isTile( inRow, inCol + 1 )
		|| isTile( inRow - 1, inCol ) || isTile( inRow + 1, inCol )
	);
}

bool
Model::NewBoard::sidesMatch( TileOnBoard const & inTile, int inRow, int inCol ) const
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
Model::NewBoard::checkForFinishedCities( int inRow, int inCol )
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
		std::vector< NewPlacedCity > queue;
		BOOST_FOREACH( Area::Area const area, city )
		{
			queue.push_back( NewPlacedCity( inRow, inCol, area ) );
		}
		// Go over the queue, adding continuations as we encounter them
		// When a continuation is missing, this city is not finished
		bool finished = true;
		for ( std::size_t i = 0; i < queue.size(); ++i )
		{
			NewPlacedCity const neighbor = getNeighbor( queue[i] );
			if ( isTile( neighbor.row, neighbor.col ) )
			{
				// Add continuation to the queue if it is not there yet
				if ( std::find( queue.begin(), queue.end(), neighbor ) == queue.end() )
				{
					ContiguousCity const neighborCity = getTile( neighbor.row, neighbor.col )->getContiguousCity( neighbor.area );
					BOOST_FOREACH( Area::Area const area, neighborCity )
					{
						queue.push_back( NewPlacedCity( neighbor.row, neighbor.col, area ) );
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
Model::NewBoard::checkForFinishedRoads( int inRow, int inCol )
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
		std::vector< NewPlacedRoad > queue;
		BOOST_FOREACH( Area::Area const area, road )
		{
			queue.push_back( NewPlacedRoad( inRow, inCol, area ) );
		}
		// Go over the queue, adding continuations as we encounter them
		// When a continuation is missing, this road is not finished
		bool finished = true;
		for ( std::size_t i = 0; i < queue.size(); ++i )
		{
			NewPlacedRoad const neighbor = getNeighbor( queue[i] );
			if ( isTile( neighbor.row, neighbor.col ) )
			{
				// Add continuation to the queue if it is not there yet
				if ( std::find( queue.begin(), queue.end(), neighbor ) == queue.end() )
				{
					ContiguousRoad const neighborRoad = getTile( neighbor.row, neighbor.col )->getContiguousRoad( neighbor.area );
					BOOST_FOREACH( Area::Area const area, neighborRoad )
					{
						queue.push_back( NewPlacedRoad( neighbor.row, neighbor.col, area ) );
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
Model::NewBoard::checkForFinishedCloisters( int inRow, int inCol )
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
Model::NewBoard::isFinishedCloister( int inRow, int inCol ) const
{
	return
	(
		isTile( inRow, inCol )
		&& getTile( inRow, inCol )->getCenter() == Tile::kCenterCloister
		&& isFullySurrounded( inRow, inCol )
	);
}

bool
Model::NewBoard::isFullySurrounded( int inRow, int inCol ) const
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
Model::NewBoard::isCity( NewPlacedCity const & inCity ) const
{
	return isTile( inCity.row, inCity.col ) && getTile( inCity.row, inCity.col )->isCity( inCity.area );
}