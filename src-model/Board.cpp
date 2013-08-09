#include "src-model/Board.h"

#include <algorithm>
#include <sstream>

namespace
{
	int const kExtraRowsAndCols = 1;

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

Board::Board( unsigned int inSize )
:
	mNrRows( (inSize % 2 == 1) ? inSize : inSize + 1 ),
	mNrCols( (inSize % 2 == 1) ? inSize : inSize + 1 ),
    mBoard( mNrCols * mNrRows, boost::optional< TileOnBoard >() )
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

unsigned int Board::getNrOfRows() const
{
	return mNrRows;
}

unsigned int
Board::getNrOfCols() const
{
	return mNrCols;
}

boost::optional< TileOnBoard >
Board::getTile(unsigned int inCol, unsigned int inRow) const
{
	return mBoard[inRow * mNrCols + inCol];
}

void
Board::addRowsOnTop(unsigned int inNrOfRows)
{
	mBoard.insert(mBoard.begin(), mNrCols * inNrOfRows, boost::optional< TileOnBoard >());
	mNrRows += inNrOfRows;
}

void
Board::addRowsBelow(unsigned int inNrOfRows)
{
	mBoard.insert(mBoard.end(), mNrCols * inNrOfRows, boost::optional< TileOnBoard >());
	mNrRows += inNrOfRows;
}

void
Board::addColsLeft(unsigned int inNrOfCols)
{
	std::vector< boost::optional< TileOnBoard > >::iterator it = mBoard.begin();
	for(unsigned int i = 0; i < mNrRows; ++i)
	{
		for (unsigned int j = 0; j < inNrOfCols; ++j)
		{
			it = mBoard.insert(it, boost::optional< TileOnBoard >());
			++it;
		}
		for (unsigned int j = 0; j < mNrCols; ++j)
		{
			++it;
		}
	}
	mNrCols += inNrOfCols;
}

void
Board::addColsRight(unsigned int inNrOfCols)
{
	std::vector< boost::optional< TileOnBoard > >::iterator it = mBoard.begin();
	for(unsigned int i = 0; i < mNrRows; ++i)
	{
		for (unsigned int j = 0; j < mNrCols; ++j)
		{
			++it;
		}
		for (unsigned int j = 0; j < inNrOfCols; ++j)
		{
			it = mBoard.insert(it, boost::optional< TileOnBoard >());
			++it;
		}
	}
	mNrCols += inNrOfCols;
}

bool
Board::isPossibleTile(Tile inTile)
{
	bool possible = false;
	for (unsigned int row = 0; row < mNrRows; ++row)
	{
		for(unsigned int col = 0; col < mNrCols; ++col)
		{
			for (unsigned int rot = 0; rot < 4; ++rot)
			{
				TileOnBoard tileOnBoard(inTile, TileOnBoard::Rotation(rot * 3));
				if (isValidTilePlacement(tileOnBoard, col, row))
				{
					possible = true;
					break;
				}
			}
		}
	}
	return possible;
}

bool
Board::isEmptySpot(unsigned int inCol, unsigned int inRow) const
{
	if (inCol < 0 || inCol >= mNrCols || inRow < 0 || inRow >= mNrRows)
	{
		return false;
	}
	// Check if no tile placed there yet
	if (mBoard[inRow * mNrCols + inCol])
	{
		return false;
	}
	return true;
}

bool
Board::isValidTilePlacement(const TileOnBoard &inTile, unsigned int inCol, unsigned int inRow) const
{
	if ( isEmptySpot( inCol, inRow ) )
	{
		return isValidAlternateTilePlacement(inTile, inCol, inRow);
	}
	return false;
}

bool
Board::isValidAlternateTilePlacement(const TileOnBoard &inTile, unsigned int inCol, unsigned int inRow) const
{
	bool valid = true;
	// Check if one of four neighbors is placed
	int nrOfNeighbors = 0;
	if ((inRow > 0) && mBoard[(inRow - 1) * mNrCols + inCol])
	{
		++nrOfNeighbors;
	}
	if ((inRow + 1 < mNrRows) && mBoard[(inRow + 1) * mNrCols + inCol])
	{
		++nrOfNeighbors;
	}
	if ((inCol > 0) && mBoard[inRow * mNrCols + inCol - 1])
	{
		++nrOfNeighbors;
	}
	if ((inCol + 1 < mNrCols) && mBoard[inRow * mNrCols + inCol + 1])
	{
		++nrOfNeighbors;
	}
	if (nrOfNeighbors == 0)
	{
		valid = false;
	}
	// Check if sides match with four neighbors
	if ((inRow > 0)
			&& mBoard[(inRow - 1) * mNrCols + inCol]
			&& !inTile.matchesBelow( mBoard[(inRow - 1) * mNrCols + inCol].get() ) )
	{
		valid = false;
	}
	if ((inRow + 1 < mNrRows)
			&& mBoard[(inRow + 1) * mNrCols + inCol]
			&& !inTile.matchesAbove( mBoard[(inRow + 1) * mNrCols + inCol].get() ) )
	{
		valid = false;
	}
	if ((inCol > 0)
			&& mBoard[inRow * mNrCols + inCol - 1]
			&& !inTile.matchesRightOf( mBoard[inRow * mNrCols + inCol - 1].get() ) )
	{
		valid = false;
	}
	if ((inCol + 1 < mNrCols)
			&& mBoard[inRow * mNrCols + inCol + 1]
			&& !inTile.matchesLeftOf( mBoard[inRow * mNrCols + inCol + 1].get() ) )
	{
		valid = false;
	}
	return valid;
}

bool
Board::placeValidTile(const TileOnBoard &inTile, unsigned int inCol, unsigned int inRow)
{
	if (isValidTilePlacement(inTile, inCol, inRow))
	{
		placeTile(inTile, inCol, inRow);
		checkForOccupiedRoads(inCol, inRow);
		checkForOccupiedCities(inCol, inRow);
		checkForOccupiedFields(inCol, inRow);
		return true;
	}
	else
	{
		return false;
	}
}

unsigned int
Board::placeStartTile(const TileOnBoard &inTile)
{
	bool empty = true;
	for ( unsigned int r = 0; r < mNrRows; ++r )
	{
		for ( unsigned int c = 0; c < mNrCols; ++c )
		{
			if ( mBoard[r * mNrCols + c] )
			{
				empty = true;
				break;
			}
		}
	}
	if ( !empty )
	{
		return (unsigned int)-1;
	}
	unsigned int col = mNrCols / 2;
	unsigned int row = mNrRows / 2;
	placeTile(inTile, col, row);
	return row * mNrCols + col;
}

std::vector< PlacedPiece >
Board::removePieces( unsigned inCol, unsigned inRow, Area::Area inArea )
{
	boost::optional< TileOnBoard > tile = getTile( inCol, inRow );
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
Board::checkForFinishedCloisters(unsigned int inCol, unsigned int inRow)
{
	unsigned int leftCol = std::max( inCol, (unsigned int)1 ) - 1;
	unsigned int rightCol = std::min( inCol + 1, mNrCols );
	unsigned int topRow = std::max( inRow, (unsigned int)1 ) - 1;
	unsigned int bottomRow = std::min( inRow + 1, mNrRows );
	for ( unsigned int r = topRow; r <= bottomRow; ++r )
	{
		for ( unsigned int c = leftCol; c <= rightCol; ++c )
		{
			if ( isFinishedCloister( c, r ) )
			{
				emit finishedCloister( c, r );
			}
		}
	}
}

bool
Board::isFinishedCloister( unsigned int inCol, unsigned int inRow ) const
{
	if ( !mBoard[inRow * mNrCols + inCol])
	{
		return false;
	}
	TileOnBoard thisTile = mBoard[inRow * mNrCols + inCol].get();
	if ( thisTile.getCenter() == Tile::Cloister )
	{
		if ( isFullySurrounded( inCol, inRow ) )
		{
			return true;
		}
	}
	return false;
}

bool
Board::isFullySurrounded( unsigned int inCol, unsigned int inRow ) const
{
	if ( inCol <= 0 || inCol >= mNrCols - 1 || inRow <= 0 || inRow >= mNrRows - 1 )
	{
		return false;
	}
	for ( unsigned int r = inRow - 1; r <= inRow + 1; ++r )
	{
		for ( unsigned int c = inCol - 1; c <= inCol + 1; ++c )
		{
			if ( !mBoard[r * mNrCols + c] )
			{
				return false;
			}
		}
	}
	return true;
}

void
Board::checkForFinishedCities( unsigned int inCol, unsigned int inRow )
{
	if ( !mBoard[inRow * mNrCols + inCol] )
	{
		return;
	}
	TileOnBoard thisTile = mBoard[inRow * mNrCols + inCol].get();
	std::vector< Tile::ContiguousCity > contiguousCities = thisTile.getContiguousCities();
	for ( unsigned int city = 0; city < contiguousCities.size(); ++city )
	{
		// Start a tempQueue and add all CityAreas from this ContCity to it
		std::vector< LocatedCity > tempQueue;
		for ( unsigned int i = 0; i < contiguousCities[city].size(); ++i )
		{
			tempQueue.push_back
			(
				LocatedCity( inRow * mNrCols + inCol, contiguousCities[city][i])
			);
		}
		// Go over tempQueue, adding ContinuationCityAreas to it as we go
		unsigned int i = 0;
		bool finished = true;
		while ( i < tempQueue.size() )
		{
			LocatedCity currentCity = tempQueue[i];
			unsigned int neighborLocation = getNeighborLocation( currentCity );
			if ( neighborLocation < mNrCols * mNrRows && mBoard[neighborLocation] )
			{
				// If not already in tempQueue, add continuation and all of its contiguous CityAreas to tempQueue
				FRCArea::CityArea neighborSide = oppositeSide( currentCity.second );
				if ( std::find(tempQueue.begin(), tempQueue.end(), LocatedCity(neighborLocation, neighborSide)) == tempQueue.end() )
				{
					Tile::ContiguousCity contCity = mBoard[neighborLocation]->getContiguousCity(neighborSide);
					for (unsigned int j = 0; j < contCity.size(); ++j)
					{
						tempQueue.push_back(LocatedCity(neighborLocation, contCity[j]));
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
			std::vector< std::pair< unsigned int, unsigned int > > tiles;
			for (unsigned int tile = 0; tile < tempQueue.size(); ++tile)
			{
				unsigned int col = tempQueue[tile].first % mNrCols;
				unsigned int row = tempQueue[tile].first / mNrCols;
				tiles.push_back( std::pair< unsigned int, unsigned int >( col, row ) );
			}
			emit finishedCity( tiles );
		}
	}
}

void
Board::checkForFinishedRoads( unsigned int inCol, unsigned int inRow )
{
	if ( !mBoard[inRow * mNrCols + inCol] )
	{
		return;
	}
	TileOnBoard thisTile = mBoard[inRow * mNrCols + inCol].get();
	std::vector< Tile::ContiguousRoad > contiguousRoads = thisTile.getContiguousRoads();
	for ( unsigned int road = 0; road < contiguousRoads.size(); ++road )
	{
		// Start a tempQueue and add all RoadAreas from this ContRoad to it
		std::vector< LocatedRoad > tempQueue;
		for ( unsigned int i = 0; i < contiguousRoads[road].size(); ++i )
		{
			tempQueue.push_back
			(
				LocatedRoad( inRow * mNrCols + inCol, contiguousRoads[road][i])
			);
		}
		// Go over tempQueue, adding ContinuationRoadAreas to it as we go
		unsigned int i = 0;
		bool finished = true;
		while ( i < tempQueue.size() )
		{
			LocatedRoad currentRoad = tempQueue[i];
			unsigned int neighborLocation = getNeighborLocation( currentRoad );
			if ( neighborLocation < mNrCols * mNrRows && mBoard[neighborLocation] )
			{
				// If not already in tempQueue, add continuation and all of its contiguous RoadAreas to tempQueue
				FRCArea::RoadArea neighborSide = oppositeSide( currentRoad.second );
				if ( std::find(tempQueue.begin(), tempQueue.end(), LocatedRoad(neighborLocation, neighborSide)) == tempQueue.end() )
				{
					Tile::ContiguousRoad contRoad = mBoard[neighborLocation]->getContiguousRoad(neighborSide);
					for (unsigned int j = 0; j < contRoad.size(); ++j)
					{
						tempQueue.push_back(LocatedRoad(neighborLocation, contRoad[j]));
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
			// Emit signal finishedRoad( tempQueue )
			std::vector< std::pair< unsigned int, unsigned int > > tiles;
			for (unsigned int tile = 0; tile < tempQueue.size(); ++tile)
			{
				unsigned int col = tempQueue[tile].first % mNrCols;
				unsigned int row = tempQueue[tile].first / mNrCols;
				tiles.push_back( std::pair< unsigned int, unsigned int >( col, row ) );
			}
			emit finishedRoad( tiles );
		}
	}
}

std::string
Board::toString() const
{
	std::stringstream res;
	for (unsigned int r = 0; r < mNrRows; ++r)
	{
		for (unsigned int c = 0; c < mNrCols; ++c)
		{
			res << shortPrint(c, r) << "\t";
		}
		res << std::endl;
	}
	return res.str();
}

std::string
Board::shortPrint(unsigned int inCol, unsigned int inRow) const
{
	std::stringstream res;
	if (mBoard[inRow * mNrCols + inCol])
	{
		res << mBoard[inRow * mNrCols + inCol]->getID() << (mBoard[inRow * mNrCols + inCol]->getRotation() * 30);
	}
	else
	{
		res << "/";
	}
	return res.str();
}

bool
Board::placeTile(const TileOnBoard &inTile, unsigned int inCol, unsigned int inRow)
{
	bool placed = false;
	if ((inCol >= 0) && (inCol < mNrCols) && (inRow >= 0) && (inRow < mNrRows))
	{
		mBoard[inRow * mNrCols + inCol] = boost::optional< TileOnBoard >( inTile );
		widenUp( inCol, inRow );
		placed = true;
	}
	return placed;
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

bool
Board::isContinueued( LocatedCity inLocatedCity ) const
{
	unsigned int neighbor = getNeighborLocation( inLocatedCity );
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
	unsigned int neighbor = getNeighborLocation( inLocatedField );
	if ( mBoard[neighbor] )
	{
		return true;
	}
	else
	{
		return false;
	}
}

unsigned int
Board::getNeighborLocation( LocatedCity inLocatedCity ) const
{
	unsigned int location = inLocatedCity.first;
	FRCArea::CityArea cityArea = inLocatedCity.second;
	unsigned int neighborLocation = (unsigned int) -1;
	switch ( cityArea )
	{
	case FRCArea::Top:
		if ( location > mNrCols )
		{
			neighborLocation = location - mNrCols;
		}
		break;
	case FRCArea::Right:
		if ( location % mNrCols < mNrCols - 1 )
		{
			neighborLocation = location + 1;
		}
		break;
	case FRCArea::Bottom:
		if ( location < (mNrRows - 1) * mNrCols )
		{
			neighborLocation = location + mNrCols;
		}
		break;
	case FRCArea::Left:
		if ( location % mNrCols > 0 )
		{
			neighborLocation = location - 1;
		}
		break;
	}
	return neighborLocation;
}

unsigned int
Board::getNeighborLocation( LocatedField inLocatedField ) const
{
	unsigned int location = inLocatedField.first;
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
		return (unsigned int)-1;
	}
}

void
Board::checkForOccupiedRoads(unsigned int inCol, unsigned int inRow)
{
	if ( !mBoard[inRow * mNrCols + inCol] )
	{
		return;
	}
	std::vector< Tile::ContiguousRoad > toBeUpdatedRoads = mBoard[inRow * mNrCols + inCol]->getContiguousRoads();
	for ( unsigned int road = 0; road < toBeUpdatedRoads.size(); ++road )
	{
		// Start a tempQueue and add all RoadAreas from this ContRoad to it
		std::vector< LocatedRoad > tempQueue;
		for ( unsigned int i = 0; i < toBeUpdatedRoads[road].size(); ++i )
		{
			tempQueue.push_back
			(
				LocatedRoad(inRow * mNrCols + inCol, toBeUpdatedRoads[road][i])
			);
		}
		// Go over tempQueue, adding ContinuationRoadAreas to it as we go
		unsigned int i = 0;
		bool occupied = false;
		while ( i < tempQueue.size() )
		{
			// Pick RoadArea i from tempQueue
			LocatedRoad currentRoad = tempQueue[i];
			unsigned int neighborLocation = getNeighborLocation( currentRoad );
			if ( neighborLocation < mNrCols * mNrRows && mBoard[neighborLocation] )
			{
				FRCArea::RoadArea neighborSide = oppositeSide( currentRoad.second );
				// If neighbor is occupied, set occupied true
				if ( mBoard[neighborLocation]->isRoadOccupied( neighborSide ) )
				{
					occupied = true;
				}
				// Else if neighbor is not yet in tempQueue
				else if ( std::find(tempQueue.begin(), tempQueue.end(), LocatedRoad( neighborLocation, neighborSide )) == tempQueue.end() )
				{
					Tile::ContiguousRoad contRoad = mBoard[neighborLocation]->getContiguousRoad(neighborSide);
					for ( unsigned int j = 0; j < contRoad.size(); ++j )
					{
						tempQueue.push_back
						(
							LocatedRoad( neighborLocation, contRoad[j] )
						);
					}
				}
			}
			++i;
		}
		// If occupied == true:
		// Mark all RoadAreas in tempQueue as occupied
		if (occupied)
		{
			for (i = 0; i < tempQueue.size(); ++i)
			{
				mBoard[tempQueue[i].first]->occupyRoad(tempQueue[i].second);
			}
		}
	}
}

void
Board::checkForOccupiedCities(unsigned int inCol, unsigned int inRow)
{
	if ( !mBoard[inRow * mNrCols + inCol] )
	{
		return;
	}
	std::vector< Tile::ContiguousCity > toBeUpdatedCities = mBoard[inRow * mNrCols + inCol]->getContiguousCities();
	for ( unsigned int city = 0; city < toBeUpdatedCities.size(); ++city )
	{
		// Start a tempQueue and add all CityAreas from this ContCity to it
		std::vector< LocatedCity > tempQueue;
		for ( unsigned int i = 0; i < toBeUpdatedCities[city].size(); ++i )
		{
			tempQueue.push_back
			(
				LocatedCity( inRow * mNrCols + inCol, toBeUpdatedCities[city][i] )
			);
		}
		// Go over tempQueue, adding ContinuationCityAreas to it as we go
		unsigned int i = 0;
		bool occupied = false;
		while ( i < tempQueue.size() )
		{
			// Pick CityArea i from tempQueue
			LocatedCity currentCity = tempQueue[i];
			unsigned int neighborLocation = getNeighborLocation( currentCity );
			if ( neighborLocation < mNrCols * mNrRows && mBoard[neighborLocation] )
			{
				FRCArea::CityArea neighborSide = oppositeSide( currentCity.second );
				// If neighbor is occupied, set occupied true
				if ( mBoard[neighborLocation]->isCityOccupied( neighborSide ) )
				{
					occupied = true;
				}
				// Else if neighbor is not yet in tempQueue
				else if ( std::find(tempQueue.begin(), tempQueue.end(), LocatedCity( neighborLocation, neighborSide )) == tempQueue.end() )
				{
					Tile::ContiguousCity contCity = mBoard[neighborLocation]->getContiguousCity(neighborSide);
					for ( unsigned int j = 0; j < contCity.size(); ++j )
					{
						tempQueue.push_back
						(
							LocatedCity( neighborLocation, contCity[j] )
						);
					}
				}
			}
			++i;
		}
		// If occupied == true:
		// Mark all CityAreas in tempQueue as occupied
		if (occupied)
		{
			for (i = 0; i < tempQueue.size(); ++i)
			{
				mBoard[tempQueue[i].first]->occupyCity(tempQueue[i].second);
			}
		}
	}
}

void
Board::checkForOccupiedFields(unsigned int inCol, unsigned int inRow)
{
	if ( !mBoard[inRow * mNrCols + inCol] )
	{
		return;
	}
	std::vector< Tile::ContiguousField > toBeUpdatedFields = mBoard[inRow * mNrCols + inCol]->getContiguousFields();
	for ( unsigned int field = 0; field < toBeUpdatedFields.size(); ++field )
	{
		// Start a tempQueue and add all FieldAreas from this ContField to it
		std::vector< LocatedField > tempQueue;
		for ( unsigned int i = 0; i < toBeUpdatedFields[field].size(); ++i )
		{
			tempQueue.push_back
			(
				LocatedField(inRow * mNrCols + inCol, toBeUpdatedFields[field][i])
			);
		}
		// Go over tempQueue, adding ContinuationFieldAreas to it as we go
		unsigned int i = 0;
		bool occupied = false;
		while ( i < tempQueue.size() )
		{
			// Pick FieldArea i from tempQueue
			LocatedField currentField = tempQueue[i];
			unsigned int neighborLocation = getNeighborLocation( currentField );
			if ( neighborLocation < mNrCols * mNrRows && mBoard[neighborLocation] )
			{
				FRCArea::FieldArea neighborSide = oppositeSide( currentField.second );
				// If neighbor is occupied, set occupied true
				if ( mBoard[neighborLocation]->isFieldOccupied( neighborSide ) )
				{
					occupied = true;
				}
				// Else if neighbor is not yet in tempQueue
				else if ( std::find(tempQueue.begin(), tempQueue.end(), LocatedField( neighborLocation, neighborSide )) == tempQueue.end() )
				{
					Tile::ContiguousField contField = mBoard[neighborLocation]->getContiguousField( neighborSide );
					for ( unsigned int j = 0; j < contField.size(); ++j )
					{
						tempQueue.push_back
						(
							LocatedField( neighborLocation, neighborSide )
						);
					}
				}
			}
			++i;
		}
		// If occupied == true:
		// Mark all FieldAreas in tempQueue as occupied
		if (occupied)
		{
			for (i = 0; i < tempQueue.size(); ++i)
			{
				mBoard[tempQueue[i].first]->occupyField(tempQueue[i].second);
			}
		}
	}
}

