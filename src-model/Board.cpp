#include "src-model/Board.h"

#include <algorithm>
#include <cassert>
#include <sstream>

namespace
{
	int const kExtraRowsAndCols = 1;
	unsigned const kInvalid = -1;

	FRCArea::CityArea
	oppositeSide( FRCArea::CityArea inCityArea )
	{
		switch ( inCityArea )
		{
		case FRCArea::Top:
			return FRCArea::Bottom;
		case FRCArea::Right:
			return FRCArea::Left;
		case FRCArea::Bottom:
			return FRCArea::Top;
		case FRCArea::Left:
			return FRCArea::Right;
		default:
			return FRCArea::Top;
		}
	}

	FRCArea::FieldArea
	oppositeSide( FRCArea::FieldArea inFieldArea )
	{
		switch( inFieldArea )
		{
		case FRCArea::TopLeft:
			return FRCArea::BottomLeft;
		case FRCArea::TopRight:
			return FRCArea::BottomRight;
		case FRCArea::RightTop:
			return FRCArea::LeftTop;
		case FRCArea::RightBottom:
			return FRCArea::LeftBottom;
		case FRCArea::BottomRight:
			return FRCArea::TopRight;
		case FRCArea::BottomLeft:
			return FRCArea::TopLeft;
		case FRCArea::LeftBottom:
			return FRCArea::RightBottom;
		case FRCArea::LeftTop:
			return FRCArea::RightTop;
		default:
			return FRCArea::TopLeft;
		}
	}
}

Board::Board( unsigned inSize )
:
	mNrRows( (inSize % 2 == 1) ? inSize : inSize + 1 ),
	mNrCols( (inSize % 2 == 1) ? inSize : inSize + 1 ),
    mBoard( mNrCols * mNrRows, boost::none )
{}

Board::Board( Board const & inBoard )
:
	mNrRows( inBoard.mNrRows ),
	mNrCols( inBoard.mNrCols ),
	mBoard( inBoard.mBoard )
{}

Board::~Board()
{}

Board &
Board::operator = ( Board const & inBoard )
{
	if ( this != &inBoard )
	{
		mNrRows = inBoard.mNrRows;
		mNrCols = inBoard.mNrCols;
		mBoard = inBoard.mBoard;
	}
	return *this;
}

unsigned
Board::getNrOfRows() const
{
	return mNrRows;
}

unsigned
Board::getNrOfCols() const
{
	return mNrCols;
}

bool
Board::isTile( unsigned inCol, unsigned inRow ) const
{
	if ( inCol < mNrCols && inRow < mNrRows )
	{
		return getTile( inCol, inRow );
	}
	else
	{
		return false;
	}
}

boost::optional< TileOnBoard > const &
Board::getTile( unsigned inCol, unsigned inRow ) const
{
	assert( inCol < mNrCols );
	assert( inRow < mNrRows );
	return mBoard[inRow * mNrCols + inCol];
}

boost::optional< TileOnBoard > &
Board::getTile( unsigned inCol, unsigned inRow )
{
	assert( inCol < mNrCols );
	assert( inRow < mNrRows );
	return mBoard[inRow * mNrCols + inCol];
}

bool
Board::isPossibleTile( Tile const & inTile )
{
	for ( unsigned row = 0; row < mNrRows; ++row )
	{
		for ( unsigned col = 0; col < mNrCols; ++col )
		{
			for ( unsigned rot = 0; rot < 4; ++rot )
			{
				TileOnBoard tileOnBoard( inTile, TileOnBoard::Rotation(rot * 3) );
				if ( isValidTilePlacement( tileOnBoard, col, row ) )
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool
Board::isEmptySpot( unsigned inCol, unsigned inRow ) const
{
	return !isTile( inCol, inRow );
}

bool
Board::isValidTilePlacement( TileOnBoard const & inTile, unsigned inCol, unsigned inRow ) const
{
	if ( isEmptySpot( inCol, inRow ) )
	{
		return isValidAlternateTilePlacement( inTile, inCol, inRow );
	}
	return false;
}

bool
Board::isValidAlternateTilePlacement( TileOnBoard const & inTile, unsigned inCol, unsigned inRow ) const
{
	// Check if one of four neighbors is placed
	int nrOfNeighbors = 0;
	if ( (inRow > 0) && isTile( inCol, inRow - 1 ) )
	{
		++nrOfNeighbors;
	}
	if ( (inRow + 1 < mNrRows) && isTile( inCol, inRow + 1 ) )
	{
		++nrOfNeighbors;
	}
	if ( (inCol > 0) && isTile( inCol - 1, inRow ) )
	{
		++nrOfNeighbors;
	}
	if ( (inCol + 1 < mNrCols) && isTile( inCol + 1, inRow ) )
	{
		++nrOfNeighbors;
	}
	if ( nrOfNeighbors == 0 )
	{
		return false;
	}
	// Check if sides match with four neighbors
	if ( ( inRow > 0 )
			&& isTile( inCol, inRow - 1 )
			&& !inTile.matchesBelow( getTile( inCol, inRow - 1 ).get() ) )
	{
		return false;
	}
	if ( ( inRow + 1 < mNrRows )
			&& isTile( inCol, inRow + 1 )
			&& !inTile.matchesAbove( getTile( inCol, inRow + 1 ).get() ) )
	{
		return false;
	}
	if ( ( inCol > 0 )
			&& isTile( inCol - 1, inRow )
			&& !inTile.matchesRightOf( getTile( inCol - 1, inRow ).get() ) )
	{
		return false;
	}
	if ( ( inCol + 1 < mNrCols )
			&& isTile( inCol + 1, inRow )
			&& !inTile.matchesLeftOf( getTile( inCol + 1, inRow ).get() ) )
	{
		return false;
	}
	return true;
}

bool
Board::placeValidTile( TileOnBoard const & inTile, unsigned inCol, unsigned inRow )
{
	if ( isValidTilePlacement( inTile, inCol, inRow ) )
	{
		placeTile( inTile, inCol, inRow );
		return true;
	}
	else
	{
		return false;
	}
}

unsigned
Board::placeStartTile( TileOnBoard const & inTile )
{
	bool empty = true;
	for ( unsigned row = 0; row < mNrRows; ++row )
	{
		for ( unsigned col = 0; col < mNrCols; ++col )
		{
			if ( !isEmptySpot( col, row ) )
			{
				empty = false;
				break;
			}
		}
	}
	if ( !empty )
	{
		return kInvalid;
	}
	unsigned col = mNrCols / 2;
	unsigned row = mNrRows / 2;
	placeTile( inTile, col, row );
	return row * mNrCols + col;
}

bool
Board::isOccupied( unsigned inCol, unsigned inRow, Area::Area inArea ) const
{
	if ( !isTile( inCol, inRow ) )
	{
		return false;
	}
	if ( getTile( inCol, inRow )->isRoad( inArea ) )
	{
		return isOccupiedRoad( inCol, inRow, FRCArea::RoadArea( inArea ) );
	}
	else
	{
		// TODO: expand for cities, fields and cloisters
		return false;
	}
}

bool
Board::isOccupiedRoad( unsigned inCol, unsigned inRow, FRCArea::RoadArea inArea ) const
{
	if ( !isTile( inCol, inRow ) )
	{
		return false;
	}
	Tile::ContiguousRoad road = getTile( inCol, inRow )->getContiguousRoad( inArea );
	std::vector< PlacedRoad > toCheckQueue;
	for ( Tile::ContiguousRoad::const_iterator it = road.begin(); it != road.end(); ++it )
	{
		toCheckQueue.push_back( PlacedRoad( inCol, inRow, *it ) );
	}
	unsigned index = 0;
	while ( index < toCheckQueue.size() )
	{
        if ( isTile( toCheckQueue[index].col, toCheckQueue[index].row )
        	&& getTile( toCheckQueue[index].col, toCheckQueue[index].row )->hasPiece( Area::Area( toCheckQueue[index].area ) ) )
		{
			return true;
		}
		PlacedRoad neighbor = getNeighbor( toCheckQueue[index] );
		if ( isTile( neighbor.col, neighbor.row ) )
		{
			if ( std::find( toCheckQueue.begin(), toCheckQueue.end(), neighbor ) == toCheckQueue.end() )
			{
				Tile::ContiguousRoad contRoad = getTile( neighbor.col, neighbor.row )->getContiguousRoad( neighbor.area );
				for ( Tile::ContiguousRoad::const_iterator contRoadIt = contRoad.begin(); contRoadIt != contRoad.end(); ++contRoadIt )
				{
					toCheckQueue.push_back( PlacedRoad( neighbor.col, neighbor.row, *contRoadIt ) );
				}
			}
		}
		++index;
	}
	return false;
}

std::vector< PlacedPiece >
Board::removePieces( unsigned inCol, unsigned inRow, Area::Area inArea )
{
	boost::optional< TileOnBoard > & tile = getTile( inCol, inRow );
	if ( tile )
	{
		return tile->removePieces( inArea );
	}
	else
	{
		return std::vector< PlacedPiece >();
	}
}

void
Board::checkForFinishedCities( unsigned inCol, unsigned inRow )
{
	if ( !isTile( inCol, inRow ) )
	{
		return;
	}
	std::vector< Tile::ContiguousCity > contiguousCities = getTile( inCol, inRow )->getContiguousCities();
	for ( unsigned city = 0; city < contiguousCities.size(); ++city )
	{
		// Start a tempQueue and add all CityAreas from this ContCity to it
		std::vector< LocatedCity > tempQueue;
		for ( unsigned i = 0; i < contiguousCities[city].size(); ++i )
		{
			tempQueue.push_back
			(
				LocatedCity( inRow * mNrCols + inCol, contiguousCities[city][i] )
			);
		}
		// Go over tempQueue, adding ContinuationCityAreas to it as we go
		unsigned i = 0;
		bool finished = true;
		while ( i < tempQueue.size() )
		{
			LocatedCity currentCity = tempQueue[i];
			unsigned neighborLocation = getNeighborLocation( currentCity );
			if ( neighborLocation < mNrCols * mNrRows && mBoard[neighborLocation] )
			{
				// If not already in tempQueue, add continuation and all of its contiguous CityAreas to tempQueue
				FRCArea::CityArea neighborSide = oppositeSide( currentCity.second );
				if ( std::find( tempQueue.begin(), tempQueue.end(), LocatedCity( neighborLocation, neighborSide ) ) == tempQueue.end() )
				{
					Tile::ContiguousCity contCity = mBoard[neighborLocation]->getContiguousCity( neighborSide );
					for (unsigned j = 0; j < contCity.size(); ++j)
					{
						tempQueue.push_back( LocatedCity( neighborLocation, contCity[j] ) );
					}
				}
			}
			else
			{
				// No continuation means unfinished city
				finished = false;
				break;
			}
			++i;
		}
		if ( finished )
		{
			// Emit signal finishedCity( tempQueue )
			std::vector< std::pair< unsigned, unsigned > > tiles;
			for ( unsigned tile = 0; tile < tempQueue.size(); ++tile )
			{
				unsigned col = tempQueue[tile].first % mNrCols;
				unsigned row = tempQueue[tile].first / mNrCols;
				tiles.push_back( std::make_pair( col, row ) );
			}
			emit finishedCity( tiles );
		}
	}
}

void
Board::checkForFinishedRoads( unsigned inCol, unsigned inRow )
{
	if ( !isTile( inCol, inRow ) )
	{
		return;
	}
	std::vector< Tile::ContiguousRoad > contiguousRoads = getTile( inCol, inRow )->getContiguousRoads();
	for ( unsigned road = 0; road < contiguousRoads.size(); ++road )
	{
		// Start a tempQueue and add all RoadAreas from this ContRoad to it
		std::vector< PlacedRoad > tempQueue;
		for ( unsigned i = 0; i < contiguousRoads[road].size(); ++i )
		{
			tempQueue.push_back
			(
				PlacedRoad( inCol, inRow, contiguousRoads[road][i] )
			);
		}
		// Go over tempQueue, adding ContinuationRoadAreas to it as we go
		unsigned i = 0;
		bool finished = true;
		while ( i < tempQueue.size() )
		{
			PlacedRoad currentRoad = tempQueue[i];
			PlacedRoad neighbor = getNeighbor( currentRoad );
			if ( isTile( neighbor.col, neighbor.row ) )
			{
				// If not already in tempQueue, add continuation and all of its contiguous RoadAreas to tempQueue
				if ( std::find( tempQueue.begin(), tempQueue.end(), neighbor ) == tempQueue.end() )
				{
					Tile::ContiguousRoad contRoad = getTile( neighbor.col, neighbor.row)->getContiguousRoad( neighbor.area );
					for ( Tile::ContiguousRoad::const_iterator it = contRoad.begin(); it != contRoad.end(); ++it )
					{
						tempQueue.push_back( PlacedRoad( neighbor.col, neighbor.row, *it ) );
					}
				}
			}
			else
			{
				// No continuation means unfinished road
				finished = false;
				break;
			}
			++i;
		}
		if ( finished )
		{
			emit finishedRoad( tempQueue );
		}
	}
}

void
Board::checkForFinishedCloisters( unsigned inCol, unsigned inRow )
{
	unsigned leftCol = std::max( inCol, (unsigned)1 ) - 1;
	unsigned rightCol = std::min( inCol + 1, mNrCols );
	unsigned topRow = std::max( inRow, (unsigned)1 ) - 1;
	unsigned bottomRow = std::min( inRow + 1, mNrRows );
	for ( unsigned r = topRow; r <= bottomRow; ++r )
	{
		for ( unsigned c = leftCol; c <= rightCol; ++c )
		{
			if ( isFinishedCloister( c, r ) )
			{
				emit finishedCloister( c, r );
			}
		}
	}
}

bool
Board::isFinishedCloister( unsigned inCol, unsigned inRow ) const
{
    if ( !isTile( inCol, inRow ) )
	{
		return false;
	}
	if ( getTile( inCol, inRow )->getCenter() == Tile::Cloister )
	{
		if ( isFullySurrounded( inCol, inRow ) )
		{
			return true;
		}
	}
	return false;
}

bool
Board::isFullySurrounded( unsigned inCol, unsigned inRow ) const
{
	if ( inCol <= 0 || inCol >= mNrCols - 1 || inRow <= 0 || inRow >= mNrRows - 1 )
	{
		return false;
	}
	for ( unsigned row = inRow - 1; row <= inRow + 1; ++row )
	{
		for ( unsigned col = inCol - 1; col <= inCol + 1; ++col )
		{
			if ( !isTile( col, row ) )
			{
				return false;
			}
		}
	}
	return true;
}

std::string
Board::toString() const
{
	std::stringstream res;
	for ( unsigned r = 0; r < mNrRows; ++r )
	{
		for ( unsigned c = 0; c < mNrCols; ++c )
		{
			res << shortPrint( c, r ) << "\t";
		}
		res << std::endl;
	}
	return res.str();
}

std::string
Board::shortPrint( unsigned inCol, unsigned inRow ) const
{
	std::stringstream res;
	if ( isTile( inCol, inRow ) )
	{
		res << getTile( inCol, inRow )->getID() << ( getTile( inCol, inRow )->getRotation() * 30 );
	}
	else
	{
		res << "/";
	}
	return res.str();
}

bool
Board::placeTile( const TileOnBoard &inTile, unsigned inCol, unsigned inRow )
{
	if ( inCol < mNrCols && inRow < mNrRows && !isTile( inCol, inRow ) )
	{
		getTile( inCol, inRow ) = boost::optional< TileOnBoard >( inTile );
		widenUp( inCol, inRow );
		return true;
	}
	return false;
}

bool
Board::removeTile( unsigned inCol, unsigned inRow )
{
	if ( inCol < mNrCols && inRow < mNrRows && isTile( inCol, inRow ) )
	{
		getTile( inCol, inRow ) = boost::none;
		return true;
	}
	return false;
}

void
Board::widenUp( unsigned inCol, unsigned inRow )
{
	if ( inCol == 0 )
	{
		addColsLeft( kExtraRowsAndCols );
		emit colsAddedLeft( kExtraRowsAndCols );
	}
	else if ( inCol == getNrOfCols() - 1 )
	{
		addColsRight( kExtraRowsAndCols );
	}
	else if ( inRow == 0 )
	{
		addRowsOnTop( kExtraRowsAndCols );
		emit rowsAddedTop( kExtraRowsAndCols );
	}
	else if ( inRow == getNrOfRows() - 1 )
	{
		addRowsBelow( kExtraRowsAndCols );
	}
}

void
Board::addRowsOnTop( unsigned inNrOfRows )
{
	mBoard.insert( mBoard.begin(), mNrCols * inNrOfRows, boost::none );
	mNrRows += inNrOfRows;
}

void
Board::addRowsBelow( unsigned inNrOfRows )
{
	mBoard.insert( mBoard.end(), mNrCols * inNrOfRows, boost::none );
	mNrRows += inNrOfRows;
}

void
Board::addColsLeft( unsigned inNrOfCols )
{
	std::vector< boost::optional< TileOnBoard > >::iterator it = mBoard.begin();
	for( unsigned i = 0; i < mNrRows; ++i )
	{
		for ( unsigned j = 0; j < inNrOfCols; ++j )
		{
			it = mBoard.insert( it, boost::none );
			++it;
		}
		for ( unsigned j = 0; j < mNrCols; ++j )
		{
			++it;
		}
	}
	mNrCols += inNrOfCols;
}

void
Board::addColsRight( unsigned inNrOfCols )
{
	std::vector< boost::optional< TileOnBoard > >::iterator it = mBoard.begin();
	for ( unsigned i = 0; i < mNrRows; ++i)
	{
		for ( unsigned j = 0; j < mNrCols; ++j )
		{
			++it;
		}
		for ( unsigned j = 0; j < inNrOfCols; ++j )
		{
			it = mBoard.insert( it, boost::none );
			++it;
		}
	}
	mNrCols += inNrOfCols;
}

bool
Board::isContinueued( LocatedCity inLocatedCity ) const
{
	unsigned neighbor = getNeighborLocation( inLocatedCity );
	if ( mBoard[neighbor] )
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool
Board::isContinueued( LocatedField inLocatedField ) const
{
	unsigned neighbor = getNeighborLocation( inLocatedField );
	if ( mBoard[neighbor] )
	{
		return true;
	}
	else
	{
		return false;
	}
}

unsigned
Board::getNeighborLocation( LocatedCity inLocatedCity ) const
{
	unsigned location = inLocatedCity.first;
	FRCArea::CityArea cityArea = inLocatedCity.second;
	unsigned neighborLocation = (unsigned) -1;
	switch ( cityArea )
	{
		case FRCArea::Top:
		{
			if ( location > mNrCols )
			{
				neighborLocation = location - mNrCols;
			}
			break;
		}
		case FRCArea::Right:
		{
			if ( location % mNrCols < mNrCols - 1 )
			{
				neighborLocation = location + 1;
			}
			break;
		}
		case FRCArea::Bottom:
		{
			if ( location < (mNrRows - 1) * mNrCols )
			{
				neighborLocation = location + mNrCols;
			}
			break;
		}
		case FRCArea::Left:
		{
			if ( location % mNrCols > 0 )
			{
				neighborLocation = location - 1;
			}
			break;
		}
	}
	return neighborLocation;
}

unsigned
Board::getNeighborLocation( LocatedField inLocatedField ) const
{
	unsigned location = inLocatedField.first;
	FRCArea::FieldArea fieldArea = inLocatedField.second;
	switch ( fieldArea )
	{
		case FRCArea::TopLeft:
		case FRCArea::TopRight:
			return (location - mNrCols);
		case FRCArea::RightTop:
		case FRCArea::RightBottom:
			return (location + 1);
		case FRCArea::BottomRight:
		case FRCArea::BottomLeft:
			return (location + mNrCols);
		case FRCArea::LeftBottom:
		case FRCArea::LeftTop:
			return (location - 1);
		default:
			return (unsigned)-1;
	}
}