#include "NewBoard.h"

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
	std::size_t const index = getIndex( inRow, inCol );
	if ( index >= 0 && index < mTiles.size() )
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
	std::size_t const index = getIndex( inRow, inCol );
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

std::size_t
Model::NewBoard::getIndex( int inRow, int inCol ) const
{
	return ( ( mStartRow + inRow ) * mNrCols + ( mStartCol + inCol ) );
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