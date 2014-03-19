#include "NewBoard.h"

#include "PlacedPiece.h"

#include <boost/foreach.hpp>

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
		// checkForFinishedCloisters( inRow, inCol );
		// checkForFinishedCities( inRow, inCol );
		// checkForFinishedRoads( inRow, inCol );
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