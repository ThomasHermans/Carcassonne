#include "Board.h"

#include <algorithm>
#include <cassert>
#include <sstream>

namespace
{
	int const kExtraRowsAndCols = 1;
	unsigned const kInvalid = -1;

	bool
	upperLeftIncreasingCompare
	(
		Model::PlacedCity const & inFirst,
		Model::PlacedCity const & inSecond
	)
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

Model::Board::Board( unsigned inSize )
:
	mNrRows( makeUneven( inSize ) ),
	mNrCols( makeUneven( inSize ) ),
	mBoard( mNrCols * mNrRows, boost::none )
{}

unsigned
Model::Board::getNrOfRows() const
{
	return mNrRows;
}

unsigned
Model::Board::getNrOfCols() const
{
	return mNrCols;
}

unsigned
Model::Board::getNrOfTiles() const
{
	unsigned nrOfTiles = 0;
	for ( unsigned row = 0; row < mNrRows; ++row )
	{
		for ( unsigned col = 0; col < mNrCols; ++col )
		{
			if ( isTile( col, row ) )
			{
				++nrOfTiles;
			}
		}
	}
	return nrOfTiles;
}

bool
Model::Board::isTile( unsigned inCol, unsigned inRow ) const
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
Model::Board::getTile( unsigned inCol, unsigned inRow ) const
{
	assert( inCol < mNrCols );
	assert( inRow < mNrRows );
	return mBoard[inRow * mNrCols + inCol];
}

boost::optional< Model::TileOnBoard > &
Model::Board::getTile( unsigned inCol, unsigned inRow )
{
	assert( inCol < mNrCols );
	assert( inRow < mNrRows );
	return mBoard[inRow * mNrCols + inCol];
}

bool
Model::Board::isPossibleTile( Tile const & inTile )
{
	for ( unsigned row = 0; row < mNrRows; ++row )
	{
		for ( unsigned col = 0; col < mNrCols; ++col )
		{
			for ( unsigned rot = 0; rot < 4; ++rot )
			{
				TileOnBoard const tileOnBoard( inTile, Rotation(rot * 3) );
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
Model::Board::isEmptySpot( unsigned inCol, unsigned inRow ) const
{
	return !isTile( inCol, inRow );
}

bool
Model::Board::isValidTilePlacement( Model::TileOnBoard const & inTile, unsigned inCol, unsigned inRow ) const
{
	if ( isEmptySpot( inCol, inRow ) )
	{
		return isValidAlternateTilePlacement( inTile, inCol, inRow );
	}
	return false;
}

bool
Model::Board::isValidAlternateTilePlacement( Model::TileOnBoard const & inTile, unsigned inCol, unsigned inRow ) const
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
Model::Board::placeValidTile( Model::TileOnBoard const & inTile, unsigned inCol, unsigned inRow )
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
Model::Board::placeStartTile( Model::TileOnBoard const & inTile )
{
	if ( getNrOfTiles() > 0 )
	{
		return kInvalid;
	}
	unsigned const col = mNrCols / 2;
	unsigned const row = mNrRows / 2;
	placeTile( inTile, col, row );
	return row * mNrCols + col;
}

bool
Model::Board::isOccupiedRoad( unsigned inCol, unsigned inRow, Area::Area inArea ) const
{
	if ( !isTile( inCol, inRow ) )
	{
		return false;
	}
	ContiguousRoad const road = getTile( inCol, inRow )->getContiguousRoad( inArea );
	std::vector< PlacedRoad > toCheckQueue;
	for ( ContiguousRoad::const_iterator it = road.begin(); it != road.end(); ++it )
	{
		toCheckQueue.push_back( PlacedRoad( inCol, inRow, *it ) );
	}
	unsigned index = 0;
	while ( index < toCheckQueue.size() )
	{
		PlacedRoad const thisRoad = toCheckQueue[index];
		if ( getTile( thisRoad.col, thisRoad.row )->hasPiece( Area::Area( thisRoad.area ) ) )
		{
			return true;
		}
		PlacedRoad const neighbor = getNeighbor( thisRoad );
		if ( isTile( neighbor.col, neighbor.row ) )
		{
			if ( std::find( toCheckQueue.begin(), toCheckQueue.end(), neighbor ) == toCheckQueue.end() )
			{
				ContiguousRoad const neighborRoad = getTile( neighbor.col, neighbor.row )->getContiguousRoad( neighbor.area );
				for ( ContiguousRoad::const_iterator it = neighborRoad.begin(); it != neighborRoad.end(); ++it )
				{
					toCheckQueue.push_back( PlacedRoad( neighbor.col, neighbor.row, *it ) );
				}
			}
		}
		++index;
	}
	return false;
}

bool
Model::Board::isOccupiedCity( unsigned inCol, unsigned inRow, Area::Area inArea ) const
{
	if ( !isTile( inCol, inRow ) )
	{
		return false;
	}
	ContiguousCity const city = getTile( inCol, inRow )->getContiguousCity( inArea );
	std::vector< PlacedCity > toCheckQueue;
	for ( ContiguousCity::const_iterator it = city.begin(); it != city.end(); ++it )
	{
		toCheckQueue.push_back( PlacedCity( inCol, inRow, *it ) );
	}
	unsigned index = 0;
	while ( index < toCheckQueue.size() )
	{
		PlacedCity const thisCity = toCheckQueue[index];
		if ( getTile( thisCity.col, thisCity.row )->hasPiece( Area::Area( thisCity.area ) ) )
		{
			return true;
		}
		PlacedCity const neighbor = getNeighbor( thisCity );
		if ( isTile( neighbor.col, neighbor.row ) )
		{
			if ( std::find( toCheckQueue.begin(), toCheckQueue.end(), neighbor ) == toCheckQueue.end() )
			{
				ContiguousCity const neighborCity = getTile( neighbor.col, neighbor.row )->getContiguousCity( neighbor.area );
				for ( ContiguousCity::const_iterator contCityIt = neighborCity.begin(); contCityIt != neighborCity.end(); ++contCityIt )
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
Model::Board::isOccupiedField( PlacedField const & inField ) const
{
	if ( !isTile( inField.col, inField.row ) )
	{
		return false;
	}
	ContiguousField const field = getTile( inField.col, inField.row )->getContiguousField( inField.area );
	std::vector< PlacedField > toCheckQueue;
	for ( ContiguousField::const_iterator it = field.begin(); it != field.end(); ++it )
	{
		toCheckQueue.push_back( PlacedField( inField.col, inField.row, *it ) );
	}
	unsigned index = 0;
	while ( index < toCheckQueue.size() )
	{
		PlacedField const thisField = toCheckQueue[ index ];
		if ( getTile( thisField.col, thisField.row )->hasPiece( thisField.area ) )
		{
			return true;
		}
		PlacedField const neighbor = getNeighbor( thisField );
		if ( isTile( neighbor.col, neighbor.row ) )
		{
			if ( std::find( toCheckQueue.begin(), toCheckQueue.end(), neighbor ) == toCheckQueue.end() )
			{
				ContiguousField const neighborField = getTile( neighbor.col, neighbor.row )->getContiguousField( neighbor.area );
				for ( ContiguousField::const_iterator contFieldIt = neighborField.begin();
					contFieldIt != neighborField.end();
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

std::vector< Model::PlacedPiece >
Model::Board::removePieces( unsigned inCol, unsigned inRow, Area::Area inArea )
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
Model::Board::checkForFinishedCities( unsigned inCol, unsigned inRow )
{
	if ( !isTile( inCol, inRow ) )
	{
		return;
	}
	std::vector< ContiguousCity > const contiguousCities = getTile( inCol, inRow )->getContiguousCities();
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
			PlacedCity const currentCity = tempQueue[i];
			PlacedCity const neighbor = getNeighbor( currentCity );
			if ( isTile( neighbor.col, neighbor.row ) )
			{
				// If not already in tempQueue, add continuation and all of its contiguous Areas to tempQueue
				if ( std::find( tempQueue.begin(), tempQueue.end(), neighbor ) == tempQueue.end() )
				{
					ContiguousCity const contCity = getTile( neighbor.col, neighbor.row )->getContiguousCity( neighbor.area );
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
Model::Board::checkForFinishedRoads( unsigned inCol, unsigned inRow )
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
			PlacedRoad const currentRoad = tempQueue[i];
			PlacedRoad const neighbor = getNeighbor( currentRoad );
			if ( isTile( neighbor.col, neighbor.row ) )
			{
				// If not already in tempQueue, add continuation and all of its contiguous Areas to tempQueue
				if ( std::find( tempQueue.begin(), tempQueue.end(), neighbor ) == tempQueue.end() )
				{
					ContiguousRoad const contRoad = getTile( neighbor.col, neighbor.row )->getContiguousRoad( neighbor.area );
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
Model::Board::checkForFinishedCloisters( unsigned inCol, unsigned inRow )
{
	unsigned const leftCol = std::max< unsigned >( inCol, 1 ) - 1;
	unsigned const rightCol = std::min( inCol + 1, mNrCols );
	unsigned const topRow = std::max< unsigned >( inRow, 1 ) - 1;
	unsigned const bottomRow = std::min( inRow + 1, mNrRows );
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
Model::Board::isFinishedCity( unsigned inCol, unsigned inRow, Area::Area inArea ) const
{
	if ( !isTile( inCol, inRow ) || !getTile( inCol, inRow )->isCity( inArea ) )
	{
		return false;
	}
	ContiguousCity const city = getTile( inCol, inRow )->getContiguousCity( inArea );
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
		PlacedCity const currentCity = tempQueue[i];
		PlacedCity const neighbor = getNeighbor( currentCity );
		if ( isTile( neighbor.col, neighbor.row ) )
		{
			// If not already in tempQueue, add continuation and all of its contiguous Areas to tempQueue
			if ( std::find( tempQueue.begin(), tempQueue.end(), neighbor ) == tempQueue.end() )
			{
				ContiguousCity const contCity = getTile( neighbor.col, neighbor.row )->getContiguousCity( neighbor.area );
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

Model::PlacedCity
Model::Board::getUpperLeftPlacedCity( unsigned inCol, unsigned inRow, Area::Area inArea ) const
{
	if ( !isTile( inCol, inRow ) || !getTile( inCol, inRow )->isCity( inArea ) )
	{
		return PlacedCity( inCol, inRow, inArea );
	}
	ContiguousCity const city = getTile( inCol, inRow )->getContiguousCity( inArea );
	// Start a completeCity and add all Areas from this ContCity to it
	std::vector< PlacedCity > completeCity;
	for ( ContiguousCity::const_iterator it = city.begin(); it != city.end(); ++it )
	{
		completeCity.push_back( PlacedCity( inCol, inRow, *it ) );
	}
	// Go over completeCity, adding ContinuationAreas to it as we go
	for ( unsigned i = 0; i < completeCity.size(); ++i )
	{
		PlacedCity const currentCity = completeCity[i];
		PlacedCity const neighbor = getNeighbor( currentCity );
		if ( isTile( neighbor.col, neighbor.row ) )
		{
			// If not already in completeCity, add continuation and all of its contiguous Areas to completeCity
			if ( std::find( completeCity.begin(), completeCity.end(), neighbor ) == completeCity.end() )
			{
				ContiguousCity const contCity = getTile( neighbor.col, neighbor.row )->getContiguousCity( neighbor.area );
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
Model::Board::isFinishedCloister( unsigned inCol, unsigned inRow ) const
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
Model::Board::isFullySurrounded( unsigned inCol, unsigned inRow ) const
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
Model::Board::toString() const
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
Model::Board::shortPrint( unsigned inCol, unsigned inRow ) const
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
Model::Board::getPointsForCloister( unsigned inCol, unsigned inRow ) const
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

std::vector< Model::PlacedRoad >
Model::Board::getCompleteRoad( PlacedRoad const & inPlacedRoad ) const
{
	unsigned const col = inPlacedRoad.col;
	unsigned const row = inPlacedRoad.row;
	if ( !isTile( col, row ) )
	{
		return std::vector< PlacedRoad >();
	}
	ContiguousRoad const contiguousRoad = getTile( col, row )->getContiguousRoad( inPlacedRoad.area );
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
		PlacedRoad const currentRoad = completeRoad[i];
		PlacedRoad const neighbor = getNeighbor( currentRoad );
		if ( isTile( neighbor.col, neighbor.row ) )
		{
			if ( std::find( completeRoad.begin(), completeRoad.end(), neighbor ) == completeRoad.end() )
			{
				ContiguousRoad const contRoad = getTile( neighbor.col, neighbor.row )->getContiguousRoad( neighbor.area );
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

std::vector< Model::PlacedCity >
Model::Board::getCompleteCity( PlacedCity const & inPlacedCity ) const
{
	unsigned const col = inPlacedCity.col;
	unsigned const row = inPlacedCity.row;
	if ( !isTile( col, row ) )
	{
		return std::vector< PlacedCity >();
	}
	ContiguousCity const contiguousCity = getTile( col, row )->getContiguousCity( inPlacedCity.area );
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
		PlacedCity const currentCity = completeCity[i];
		PlacedCity const neighbor = getNeighbor( currentCity );
		if ( isTile( neighbor.col, neighbor.row ) )
		{
			if ( std::find( completeCity.begin(), completeCity.end(), neighbor ) == completeCity.end() )
			{
				ContiguousCity const contCity = getTile( neighbor.col, neighbor.row )->getContiguousCity( neighbor.area );
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

std::vector< Model::PlacedField >
Model::Board::getCompleteField( PlacedField const & inPlacedField ) const
{
	unsigned const col = inPlacedField.col;
	unsigned const row = inPlacedField.row;
	if ( !isTile( col, row ) )
	{
		return std::vector< PlacedField >();
	}
	ContiguousField const contiguousField = getTile( col, row )->getContiguousField( inPlacedField.area );
	std::vector< PlacedField > completeField;
	for ( ContiguousField::const_iterator it = contiguousField.begin();
		it != contiguousField.end();
		++it )
	{
		completeField.push_back( PlacedField( col, row, *it ) );
	}
	for ( unsigned i = 0; i < completeField.size(); ++i )
	{
		PlacedField const currentField = completeField[i];
		PlacedField const neighbor = getNeighbor( currentField );
		if ( isTile( neighbor.col, neighbor.row ) )
		{
			if ( std::find( completeField.begin(), completeField.end(), neighbor ) == completeField.end() )
			{
				ContiguousField const contField = getTile( neighbor.col, neighbor.row )->getContiguousField( neighbor.area );
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
Model::Board::placeTile( const Model::TileOnBoard &inTile, unsigned inCol, unsigned inRow )
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
Model::Board::removeTile( unsigned inCol, unsigned inRow )
{
	if ( isTile( inCol, inRow ) )
	{
		getTile( inCol, inRow ) = boost::none;
		return true;
	}
	return false;
}

void
Model::Board::widenUp( unsigned inCol, unsigned inRow )
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
Model::Board::addRowsOnTop( unsigned inNrOfRows )
{
	mBoard.insert( mBoard.begin(), mNrCols * inNrOfRows, boost::none );
	mNrRows += inNrOfRows;
	emit dimensionsChanged();
}

void
Model::Board::addRowsBelow( unsigned inNrOfRows )
{
	mBoard.insert( mBoard.end(), mNrCols * inNrOfRows, boost::none );
	mNrRows += inNrOfRows;
	emit dimensionsChanged();
}

void
Model::Board::addColsLeft( unsigned inNrOfCols )
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
	emit dimensionsChanged();
}

void
Model::Board::addColsRight( unsigned inNrOfCols )
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
	emit dimensionsChanged();
}