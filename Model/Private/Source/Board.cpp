#include "Board.h"

#include <algorithm>
#include <cassert>
#include <sstream>

namespace
{
	int const kExtraRowsAndCols = 1;
	unsigned const kInvalid = -1;

	bool
	upperLeftIncreasingCompare( PlacedCity const & inFirst, PlacedCity const & inSecond )
	{
		return
		(
			inFirst.row < inSecond.row
			||
			(
				inFirst.row == inSecond.row
				&&
				inFirst.col < inSecond.col
			)
			||
			(
				inFirst.row == inSecond.row
				&&
				inFirst.col == inSecond.col
				&&
				inFirst.area < inSecond.area
			)
		);
	}

	unsigned
	makeUneven( unsigned inNumber )
	{
		if ( inNumber % 2 == 1 )
		{
			return inNumber;
		}
		else
		{
			return inNumber + 1;
		}
	}
}

Board::Board( unsigned inSize )
:
	mNrRows( makeUneven( inSize ) ),
	mNrCols( makeUneven( inSize ) ),
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

boost::optional< Model::TileOnBoard > const &
Board::getTile( unsigned inCol, unsigned inRow ) const
{
	assert( inCol < mNrCols );
	assert( inRow < mNrRows );
	return mBoard[inRow * mNrCols + inCol];
}

boost::optional< Model::TileOnBoard > &
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
				Model::TileOnBoard tileOnBoard( inTile, Model::Rotation(rot * 3) );
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
Board::isValidTilePlacement( Model::TileOnBoard const & inTile, unsigned inCol, unsigned inRow ) const
{
	if ( isEmptySpot( inCol, inRow ) )
	{
		return isValidAlternateTilePlacement( inTile, inCol, inRow );
	}
	return false;
}

bool
Board::isValidAlternateTilePlacement( Model::TileOnBoard const & inTile, unsigned inCol, unsigned inRow ) const
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
Board::placeValidTile( Model::TileOnBoard const & inTile, unsigned inCol, unsigned inRow )
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
Board::placeStartTile( Model::TileOnBoard const & inTile )
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
Board::isOccupiedRoad( unsigned inCol, unsigned inRow, Area::Area inArea ) const
{
	if ( !isTile( inCol, inRow ) )
	{
		return false;
	}
	ContiguousRoad road = getTile( inCol, inRow )->getContiguousRoad( inArea );
	std::vector< PlacedRoad > toCheckQueue;
	for ( ContiguousRoad::const_iterator it = road.begin(); it != road.end(); ++it )
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
				ContiguousRoad contRoad = getTile( neighbor.col, neighbor.row )->getContiguousRoad( neighbor.area );
				for ( ContiguousRoad::const_iterator contRoadIt = contRoad.begin(); contRoadIt != contRoad.end(); ++contRoadIt )
				{
					toCheckQueue.push_back( PlacedRoad( neighbor.col, neighbor.row, *contRoadIt ) );
				}
			}
		}
		++index;
	}
	return false;
}

bool
Board::isOccupiedCity( unsigned inCol, unsigned inRow, Area::Area inArea ) const
{
	if ( !isTile( inCol, inRow ) )
	{
		return false;
	}
	ContiguousCity city = getTile( inCol, inRow )->getContiguousCity( inArea );
	std::vector< PlacedCity > toCheckQueue;
	for ( ContiguousCity::const_iterator it = city.begin(); it != city.end(); ++it )
	{
		toCheckQueue.push_back( PlacedCity( inCol, inRow, *it ) );
	}
	unsigned index = 0;
	while ( index < toCheckQueue.size() )
	{
		if ( isTile( toCheckQueue[index].col, toCheckQueue[index].row )
			&& getTile( toCheckQueue[index].col, toCheckQueue[index].row )->hasPiece( Area::Area( toCheckQueue[index].area ) ) )
		{
			return true;
		}
		PlacedCity neighbor = getNeighbor( toCheckQueue[index] );
		if ( isTile( neighbor.col, neighbor.row ) )
		{
			if ( std::find( toCheckQueue.begin(), toCheckQueue.end(), neighbor ) == toCheckQueue.end() )
			{
				ContiguousCity contCity = getTile( neighbor.col, neighbor.row )->getContiguousCity( neighbor.area );
				for ( ContiguousCity::const_iterator contCityIt = contCity.begin(); contCityIt != contCity.end(); ++contCityIt )
				{
					toCheckQueue.push_back( PlacedCity( neighbor.col, neighbor.row, *contCityIt ) );
				}
			}
		}
		++index;
	}
	return false;
}

bool
Board::isOccupiedField( PlacedField const & inField ) const
{
	if ( !isTile( inField.col, inField.row ) )
	{
		return false;
	}
	ContiguousField field = getTile( inField.col, inField.row )->getContiguousField( inField.area );
	std::vector< PlacedField > toCheckQueue;
	for ( ContiguousField::const_iterator it = field.begin(); it != field.end(); ++it )
	{
		toCheckQueue.push_back( PlacedField( inField.col, inField.row, *it ) );
	}
	unsigned index = 0;
	while ( index < toCheckQueue.size() )
	{
		PlacedField thisField = toCheckQueue[ index ];
		if ( isTile( thisField.col, thisField.row )
			&& getTile( thisField.col, thisField.row )->hasPiece( thisField.area ) )
		{
			return true;
		}
		PlacedField neighbor = getNeighbor( thisField );
		if ( isTile( neighbor.col, neighbor.row ) )
		{
			if ( std::find( toCheckQueue.begin(), toCheckQueue.end(), neighbor ) == toCheckQueue.end() )
			{
				ContiguousField contField = getTile( neighbor.col, neighbor.row )->getContiguousField( neighbor.area );
				for ( ContiguousField::const_iterator contFieldIt = contField.begin();
					contFieldIt != contField.end();
					++contFieldIt )
				{
					toCheckQueue.push_back( PlacedField( neighbor.col, neighbor.row, *contFieldIt ) );
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
	boost::optional< Model::TileOnBoard > & tile = getTile( inCol, inRow );
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
	std::vector< ContiguousCity > contiguousCities = getTile( inCol, inRow )->getContiguousCities();
	for ( unsigned city = 0; city < contiguousCities.size(); ++city )
	{
		// Start a tempQueue and add all Areas from this ContCity to it
		std::vector< PlacedCity > tempQueue;
		for ( unsigned i = 0; i < contiguousCities[city].size(); ++i )
		{
			tempQueue.push_back
			(
				PlacedCity( inCol, inRow, contiguousCities[city][i] )
			);
		}
		// Go over tempQueue, adding ContinuationAreas to it as we go
		unsigned i = 0;
		bool finished = true;
		while ( i < tempQueue.size() )
		{
			PlacedCity currentCity = tempQueue[i];
			PlacedCity neighbor = getNeighbor( currentCity );
			if ( isTile( neighbor.col, neighbor.row ) )
			{
				// If not already in tempQueue, add continuation and all of its contiguous Areas to tempQueue
				if ( std::find( tempQueue.begin(), tempQueue.end(), neighbor ) == tempQueue.end() )
				{
					ContiguousCity contCity = getTile( neighbor.col, neighbor.row )->getContiguousCity( neighbor.area );
					for ( ContiguousCity::const_iterator it = contCity.begin(); it != contCity.end(); ++it )
					{
						tempQueue.push_back( PlacedCity( neighbor.col, neighbor.row, *it ) );
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
			emit finishedCity( tempQueue );
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
	std::vector< ContiguousRoad > contiguousRoads = getTile( inCol, inRow )->getContiguousRoads();
	for ( unsigned road = 0; road < contiguousRoads.size(); ++road )
	{
		// Start a tempQueue and add all Areas from this ContRoad to it
		std::vector< PlacedRoad > tempQueue;
		for ( unsigned i = 0; i < contiguousRoads[road].size(); ++i )
		{
			tempQueue.push_back
			(
				PlacedRoad( inCol, inRow, contiguousRoads[road][i] )
			);
		}
		// Go over tempQueue, adding ContinuationAreas to it as we go
		unsigned i = 0;
		bool finished = true;
		while ( i < tempQueue.size() )
		{
			PlacedRoad currentRoad = tempQueue[i];
			PlacedRoad neighbor = getNeighbor( currentRoad );
			if ( isTile( neighbor.col, neighbor.row ) )
			{
				// If not already in tempQueue, add continuation and all of its contiguous Areas to tempQueue
				if ( std::find( tempQueue.begin(), tempQueue.end(), neighbor ) == tempQueue.end() )
				{
					ContiguousRoad contRoad = getTile( neighbor.col, neighbor.row )->getContiguousRoad( neighbor.area );
					for ( ContiguousRoad::const_iterator it = contRoad.begin(); it != contRoad.end(); ++it )
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
Board::isFinishedCity( unsigned inCol, unsigned inRow, Area::Area inArea ) const
{
	if ( !isTile( inCol, inRow ) || !getTile( inCol, inRow )->isCity( inArea ) )
	{
		return false;
	}
	ContiguousCity city = getTile( inCol, inRow )->getContiguousCity( inArea );
	// Start a tempQueue and add all Areas from this ContCity to it
	std::vector< PlacedCity > tempQueue;
	for ( ContiguousCity::const_iterator it = city.begin(); it != city.end(); ++it )
	{
		tempQueue.push_back( PlacedCity( inCol, inRow, *it ) );
	}
	// Go over tempQueue, adding ContinuationAreas to it as we go
	unsigned i = 0;
	bool finished = true;
	while ( i < tempQueue.size() )
	{
		PlacedCity currentCity = tempQueue[i];
		PlacedCity neighbor = getNeighbor( currentCity );
		if ( isTile( neighbor.col, neighbor.row ) )
		{
			// If not already in tempQueue, add continuation and all of its contiguous Areas to tempQueue
			if ( std::find( tempQueue.begin(), tempQueue.end(), neighbor ) == tempQueue.end() )
			{
				ContiguousCity contCity = getTile( neighbor.col, neighbor.row )->getContiguousCity( neighbor.area );
				for ( ContiguousCity::const_iterator it = contCity.begin(); it != contCity.end(); ++it )
				{
					tempQueue.push_back( PlacedCity( neighbor.col, neighbor.row, *it ) );
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
	return finished;
}

PlacedCity
Board::getUpperLeftPlacedCity( unsigned inCol, unsigned inRow, Area::Area inArea ) const
{
	if ( !isTile( inCol, inRow ) || !getTile( inCol, inRow )->isCity( inArea ) )
	{
		return PlacedCity( inCol, inRow, inArea );
	}
	ContiguousCity city = getTile( inCol, inRow )->getContiguousCity( inArea );
	// Start a completeCity and add all Areas from this ContCity to it
	std::vector< PlacedCity > completeCity;
	for ( ContiguousCity::const_iterator it = city.begin(); it != city.end(); ++it )
	{
		completeCity.push_back( PlacedCity( inCol, inRow, *it ) );
	}
	// Go over completeCity, adding ContinuationAreas to it as we go
	for ( unsigned i = 0; i < completeCity.size(); ++i )
	{
		PlacedCity currentCity = completeCity[i];
		PlacedCity neighbor = getNeighbor( currentCity );
		if ( isTile( neighbor.col, neighbor.row ) )
		{
			// If not already in completeCity, add continuation and all of its contiguous Areas to completeCity
			if ( std::find( completeCity.begin(), completeCity.end(), neighbor ) == completeCity.end() )
			{
				ContiguousCity contCity = getTile( neighbor.col, neighbor.row )->getContiguousCity( neighbor.area );
				for ( ContiguousCity::const_iterator it = contCity.begin(); it != contCity.end(); ++it )
				{
					completeCity.push_back( PlacedRoad( neighbor.col, neighbor.row, *it ) );
				}
			}
		}
	}
	std::sort( completeCity.begin(), completeCity.end(), &upperLeftIncreasingCompare );
	return *completeCity.begin();
}

bool
Board::isFinishedCloister( unsigned inCol, unsigned inRow ) const
{
	if ( !isTile( inCol, inRow ) )
	{
		return false;
	}
	if ( getTile( inCol, inRow )->getCenter() == Tile::kCenterCloister )
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

unsigned
Board::getPointsForCloister( unsigned inCol, unsigned inRow ) const
{
	unsigned points = 0;
	for ( unsigned row = inRow - 1; row <= inRow + 1; ++row )
	{
		for ( unsigned col = inCol - 1; col <= inCol + 1; ++col )
		{
			if ( isTile( col, row ) )
			{
				++points;
			}
		}
	}
	return points;
}

std::vector< PlacedRoad >
Board::getCompleteRoad( PlacedRoad const & inPlacedRoad ) const
{
	unsigned col = inPlacedRoad.col;
	unsigned row = inPlacedRoad.row;
	if ( !isTile( col, row ) )
	{
		return std::vector< PlacedRoad >();
	}
	ContiguousRoad contiguousRoad = getTile( col, row )->getContiguousRoad( inPlacedRoad.area );
	std::vector< PlacedRoad > completeRoad;
	for ( ContiguousRoad::const_iterator it = contiguousRoad.begin();
		it != contiguousRoad.end();
		++it )
	{
		completeRoad.push_back( PlacedRoad( col, row, *it ) );
	}
	unsigned i = 0;
	while ( i < completeRoad.size() )
	{
		PlacedRoad currentRoad = completeRoad[i];
		PlacedRoad neighbor = getNeighbor( currentRoad );
		if ( isTile( neighbor.col, neighbor.row ) )
		{
			if ( std::find( completeRoad.begin(), completeRoad.end(), neighbor ) == completeRoad.end() )
			{
				ContiguousRoad contRoad = getTile( neighbor.col, neighbor.row )->getContiguousRoad( neighbor.area );
				for ( ContiguousRoad::const_iterator it = contRoad.begin();
					it != contRoad.end();
					++it )
				{
					completeRoad.push_back( PlacedRoad( neighbor.col, neighbor.row, *it ) );
				}
			}
		}
		++i;
	}
	return completeRoad;
}

std::vector< PlacedCity >
Board::getCompleteCity( PlacedCity const & inPlacedCity ) const
{
	unsigned col = inPlacedCity.col;
	unsigned row = inPlacedCity.row;
	if ( !isTile( col, row ) )
	{
		return std::vector< PlacedCity >();
	}
	ContiguousCity contiguousCity = getTile( col, row )->getContiguousCity( inPlacedCity.area );
	std::vector< PlacedCity > completeCity;
	for ( ContiguousCity::const_iterator it = contiguousCity.begin();
		it != contiguousCity.end();
		++it )
	{
		completeCity.push_back( PlacedCity( col, row, *it ) );
	}
	unsigned i = 0;
	while ( i < completeCity.size() )
	{
		PlacedCity currentCity = completeCity[i];
		PlacedCity neighbor = getNeighbor( currentCity );
		if ( isTile( neighbor.col, neighbor.row ) )
		{
			if ( std::find( completeCity.begin(), completeCity.end(), neighbor ) == completeCity.end() )
			{
				ContiguousCity contCity = getTile( neighbor.col, neighbor.row )->getContiguousCity( neighbor.area );
				for ( ContiguousCity::const_iterator it = contCity.begin();
					it != contCity.end();
					++it )
				{
					completeCity.push_back( PlacedCity( neighbor.col, neighbor.row, *it ) );
				}
			}
		}
		++i;
	}
	return completeCity;
}

std::vector< PlacedField >
Board::getCompleteField( PlacedField const & inPlacedField ) const
{
	unsigned col = inPlacedField.col;
	unsigned row = inPlacedField.row;
	if ( !isTile( col, row ) )
	{
		return std::vector< PlacedField >();
	}
	ContiguousField contiguousField = getTile( col, row )->getContiguousField( inPlacedField.area );
	std::vector< PlacedField > completeField;
	for ( ContiguousField::const_iterator it = contiguousField.begin();
		it != contiguousField.end();
		++it )
	{
		completeField.push_back( PlacedField( col, row, *it ) );
	}
	for ( unsigned i = 0; i < completeField.size(); ++i )
	{
		PlacedField currentField = completeField[i];
		PlacedField neighbor = getNeighbor( currentField );
		if ( isTile( neighbor.col, neighbor.row ) )
		{
			if ( std::find( completeField.begin(), completeField.end(), neighbor ) == completeField.end() )
			{
				ContiguousField contField = getTile( neighbor.col, neighbor.row )->getContiguousField( neighbor.area );
				for ( ContiguousField::const_iterator it = contField.begin();
					it != contField.end();
					++it )
				{
					completeField.push_back( PlacedField( neighbor.col, neighbor.row, *it ) );
				}
			}
		}
	}
	return completeField;
}

bool
Board::placeTile( const Model::TileOnBoard &inTile, unsigned inCol, unsigned inRow )
{
	if ( inCol < mNrCols && inRow < mNrRows && !isTile( inCol, inRow ) )
	{
		getTile( inCol, inRow ) = boost::optional< Model::TileOnBoard >( inTile );
		checkForFinishedCloisters( inCol, inRow );
		checkForFinishedCities( inCol, inRow );
		checkForFinishedRoads( inCol, inRow );
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
	std::vector< boost::optional< Model::TileOnBoard > >::iterator it = mBoard.begin();
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
	std::vector< boost::optional< Model::TileOnBoard > >::iterator it = mBoard.begin();
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