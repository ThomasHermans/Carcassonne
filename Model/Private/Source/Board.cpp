#include "Model/Board.h"

#include "Model/PlacedPiece.h"

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
	mNrCols( 1 ),
	mTilePlacedSignal( std::make_shared< boost::signals2::signal< void ( Utils::Location, TileOnBoard ) > >() ),
	mPiecePlacedSignal( std::make_shared< boost::signals2::signal< void ( Utils::Location, PlacedPiece ) > >() ),
	mPieceRemovedSignal( std::make_shared< boost::signals2::signal< void ( Utils::Location, PlacedPiece ) > >() )
{
}

Model::Board::Board( Board const & inBoard )
:
	mTiles( inBoard.mTiles ),
	mStartRow( inBoard.mStartRow ),
	mStartCol( inBoard.mStartCol ),
	mNrRows( inBoard.mNrRows ),
	mNrCols( inBoard.mNrCols ),
	mTilePlacedSignal( std::make_shared< boost::signals2::signal< void ( Utils::Location, TileOnBoard ) > >() ),
	mPiecePlacedSignal( std::make_shared< boost::signals2::signal< void ( Utils::Location, PlacedPiece ) > >() ),
	mPieceRemovedSignal( std::make_shared< boost::signals2::signal< void ( Utils::Location, PlacedPiece ) > >() )
{
}


boost::signals2::signal< void ( Utils::Location, Model::TileOnBoard ) > &
Model::Board::getTilePlacedSignal()
{
	assert( mTilePlacedSignal );
	return *mTilePlacedSignal;
}

boost::signals2::signal< void ( Utils::Location, Model::PlacedPiece ) > &
Model::Board::getPiecePlacedSignal()
{
	assert( mPiecePlacedSignal );
	return *mPiecePlacedSignal;
}

boost::signals2::signal< void ( Utils::Location, Model::PlacedPiece ) > &
Model::Board::getPieceRemovedSignal()
{
	assert( mPieceRemovedSignal );
	return *mPieceRemovedSignal;
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
	return !!getTile( inRow, inCol );
}

bool
Model::Board::isTile( Utils::Location const & inLocation ) const
{
	return isTile( inLocation.row, inLocation.col );
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

boost::optional< Model::TileOnBoard >
Model::Board::getTile( Utils::Location const & inLocation ) const
{
	return getTile( inLocation.row, inLocation.col );
}

boost::optional< Model::TileOnBoard > &
Model::Board::getTile( int inRow, int inCol )
{
	ensureTile( inRow, inCol );
	int const index = getIndex( inRow, inCol );
	return mTiles[ index ];
}

boost::optional< Model::TileOnBoard > &
Model::Board::getTile( Utils::Location const & inLocation )
{
	return getTile( inLocation.row, inLocation.col );
}

bool
Model::Board::isCity( PlacedCity const & inCity ) const
{
	return isTile( inCity.row, inCity.col ) && getTile( inCity.row, inCity.col )->isCity( inCity.area );
}

bool
Model::Board::isCity( Utils::Location const & inLocation, Area::Area inArea ) const
{
	return isTile( inLocation ) && getTile( inLocation )->isCity( inArea );
}

bool
Model::Board::isRoad( PlacedRoad const & inRoad ) const
{
	return isTile( inRoad.row, inRoad.col ) && getTile( inRoad.row, inRoad.col )->isRoad( inRoad.area );
}

bool
Model::Board::isRoad( Utils::Location const & inLocation, Area::Area inArea ) const
{
	return isTile( inLocation ) && getTile( inLocation )->isRoad( inArea );
}

bool
Model::Board::isField( PlacedField const & inField ) const
{
	return isTile( inField.row, inField.col ) && getTile( inField.row, inField.col )->isField( inField.area );
}

bool
Model::Board::isField( Utils::Location const & inLocation, Area::Area inArea ) const
{
	return isTile( inLocation ) && getTile( inLocation )->isField( inArea );
}

bool
Model::Board::isCloister( Utils::Location const & inLocation, Area::Area inArea ) const
{
	return isTile( inLocation ) && getTile( inLocation )->isCloister( inArea );
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
Model::Board::isValidTilePlacement( TileOnBoard const & inTile, Utils::Location const & inLocation ) const
{
	return isValidTilePlacement( inTile, inLocation.row, inLocation.col );
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
Model::Board::placeValidTile( TileOnBoard const & inTile, Utils::Location const & inLocation )
{
	return placeValidTile( inTile, inLocation.row, inLocation.col );
}

bool
Model::Board::placeValidPiece( PlacedPiece const & inPiece, Utils::Location const & inLocation )
{
	if ( isValidPiecePlacement( inLocation, inPiece ) )
	{
		bool const placed = getTile( inLocation )->placePiece( inPiece );
		if ( placed )
		{
			getPiecePlacedSignal()( inLocation, inPiece );
			return true;
		}
	}
	return false;
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

Utils::Locations
Model::Board::getPossibleLocations( Tile const & inTile ) const
{
	int const topRow = getTopRow() - 1;
	int const bottomRow = getBottomRow() + 1;
	int const leftCol = getLeftCol() - 1;
	int const rightCol = getRightCol() + 1;
	Utils::Locations locations;
	for ( int row = topRow; row <= bottomRow; ++row )
	{
		for ( int col = leftCol; col <= rightCol; ++col )
		{
			Rotation rotation = kCw0;
			for ( std::size_t i = 0; i < 4; ++i )
			{
				TileOnBoard const tile( inTile, rotation );
				if ( isValidTilePlacement( tile, row, col ) )
				{
					locations.insert( Utils::Location( row, col ) );
					break;
				}
				rotation = rotateCW( rotation );
			}
		}
	}
	return locations;
}

bool
Model::Board::isValidPiecePlacement
(
	Utils::Location const & inLocation,
	PlacedPiece const & inPiece
) const
{
	PlacedProject const project( inLocation.row, inLocation.col, inPiece.getArea() );
	return
	(
		isTile( inLocation )
		&&
		!( isOccupiedRoad( project ) || isOccupiedCity( project ) || isOccupiedField( project ) )
	);
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
Model::Board::isOccupiedRoad( Utils::Location const & inLocation, Area::Area inArea ) const
{
	return isOccupiedRoad( PlacedRoad( inLocation.row, inLocation.col, inArea ) );
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
Model::Board::isOccupiedCity( Utils::Location const & inLocation, Area::Area inArea ) const
{
	return isOccupiedCity( PlacedCity( inLocation.row, inLocation.col, inArea ) );
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

bool
Model::Board::isOccupiedField( Utils::Location const & inLocation, Area::Area inArea ) const
{
	return isOccupiedField( PlacedField( inLocation.row, inLocation.col, inArea ) );
}

std::vector< Model::PlacedPiece >
Model::Board::getPieces( PlacedProject inArea ) const
{
	if ( isTile( inArea.row, inArea.col ) )
	{
		return getTile( inArea.row, inArea.col )->getPieces( inArea.area );
	}
	else
	{
		return std::vector< PlacedPiece >();
	}
}

std::vector< Model::PlacedPiece >
Model::Board::removePieces( PlacedProject inArea )
{
	if ( isTile( inArea.row, inArea.col ) )
	{
		auto const pieces = getTile( inArea.row, inArea.col )->removePieces( inArea.area );
		Utils::Location const location( inArea.row, inArea.col );
		for ( PlacedPiece const & piece : pieces )
		{
			getPieceRemovedSignal()( location, piece );
		}
		return pieces;
	}
	else
	{
		return std::vector< PlacedPiece >();
	}
}

bool
Model::Board::isFinishedCity( PlacedCity const & inCity ) const
{
	return isFinishedCity( Utils::Location( inCity.row, inCity.col ), inCity.area );
}

bool
Model::Board::isFinishedCity( Utils::Location const & inLocation, Model::Area::Area inArea ) const
{
	if ( !isCity( inLocation, inArea ) )
	{
		return false;
	}
	ContiguousCity const city = getTile( inLocation )->getContiguousCity( inArea );
	// Check if this city is unfinished
	// Create a queue and add all areas from this city to it
	std::vector< PlacedCity > queue;
	BOOST_FOREACH( Area::Area const area, city )
	{
		queue.push_back( PlacedCity( inLocation.row, inLocation.col, area ) );
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

bool
Model::Board::isFinishedRoad( PlacedRoad const & inRoad ) const
{
	return isFinishedRoad( Utils::Location( inRoad.row, inRoad.col ), inRoad.area );
}

bool
Model::Board::isFinishedRoad( Utils::Location const & inLocation, Model::Area::Area inArea ) const
{
	if ( !isRoad( inLocation, inArea ) )
	{
		return false;
	}
	ContiguousRoad const road = getTile( inLocation )->getContiguousRoad( inArea );
	// Check if this road is unfinished
	// Create a queue and add all areas from this road to it
	std::vector< PlacedRoad > queue;
	for ( Area::Area const area : road )
	{
		queue.push_back( PlacedRoad( inLocation.row, inLocation.col, area ) );
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
				for ( Area::Area const area : neighborRoad )
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
	return finished;
}

bool
Model::Board::isFinishedCloister( Utils::Location const & inLocation ) const
{
	return isFinishedCloister( inLocation.row, inLocation.col );
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

std::size_t
Model::Board::getNrOfSurroundingTiles( Utils::Location const & inLocation ) const
{
	return getNrOfSurroundingTiles( inLocation.row, inLocation.col );
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
Model::Board::hasPennant( PlacedCity const & inCity ) const
{
	if ( !isTile( inCity.row, inCity.col ) )
	{
		return false;
	}
	return getTile( inCity.row, inCity.col )->hasPennant( inCity.area );
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

bool
Model::Board::hasCathedral( PlacedCity const & inCity ) const
{
	if ( !isTile( inCity.row, inCity.col ) )
	{
		return false;
	}
	return getTile( inCity.row, inCity.col )->hasCathedral( inCity.area );
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
		getTilePlacedSignal()( Utils::Location( inRow, inCol ), inTile );
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