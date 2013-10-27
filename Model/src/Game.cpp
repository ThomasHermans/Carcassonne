#include "Model/Game.h"

#include "Model/src/CreateBaseGameTiles.h"

#include <set>
#include <stdio.h>

namespace
{
	int const kSize = 5;
	unsigned const kPointsForFinishedCloister = 9;
	unsigned const kPointsPerTileFinishedRoad = 1;
	unsigned const kPointsPerTileFinishedCity = 2;
	unsigned const kPointsPerTileFinishedShield = 2;
	unsigned const kPointsPerTileUnfinishedRoad = 1;
	unsigned const kPointsPerTileUnfinishedCity = 1;
	unsigned const kPointsPerTileUnfinishedShield = 1;
	unsigned const kInvalid = -1;

	std::set< Color::Color >
	getWinningColors( std::vector< PlacedPiece > const & inPieces )
	{
		typedef std::map< Color::Color, unsigned > Counts;
		Counts counts;
		for ( std::vector< PlacedPiece >::const_iterator it = inPieces.begin();
			it != inPieces.end();
			++it )
		{
			Color::Color color = it->getPiece().getColor();
			Counts::iterator countIt = counts.find( color );
			if ( countIt == counts.end() )
			{
				counts.insert( std::make_pair( color, 1 ) );
			}
			else
			{
				++countIt->second;
			}
		}
		std::set< Color::Color > winningColors;
		unsigned winningAmount = 0;
		for ( Counts::const_iterator it = counts.begin(); it != counts.end(); ++it )
		{
			if ( it->second > winningAmount )
			{
				winningColors.clear();
				winningAmount = it->second;
			}
			if ( it->second == winningAmount )
			{
				winningColors.insert( it->first );
			}
		}
		return winningColors;
	}
}

Game::Game( QObject * inParent )
:
	QObject( inParent ),
	mBoard( Board( kSize ) ),
	mStartRow( 0 ),
	mStartCol( 0 ),
	mCurrentPlacedTile( boost::none ),
	mCurrentPlacedRow( kInvalid ),
	mCurrentPlacedCol( kInvalid ),
	mBag(),
	mNextTile(),
	mPlayers(),
	mCurrentPlayer( 0 ),
	mPiecesPlacedInCurrentTurn( 0 )
{
	// Fill bag of tiles
	mBag = createBaseGameTiles();
	// Print out bag
	for ( unsigned int i = 0; i < mBag.size(); ++i )
	{
		std::cout << mBag[i].getID();
	}
	std::cout << std::endl;
	// Game signals
	connectGameSignals();
	// Initialize first tile
	if ( !mBag.empty() )
	{
		mNextTile = mBag.back();
		mBag.pop_back();
	}
	// Initialize players
	mPlayers.push_back( Player( "Thomas", Color::Blue ) );
	mPlayers.push_back( Player( "Gijs", Color::Red ) );
	connectPlayerSignals();
	emit currentPlayerChanged( mPlayers.front() );
}

Game::Game( std::string const & inTiles, QObject * inParent )
:
	QObject( inParent ),
	mBoard( Board( kSize ) ),
	mStartRow( 0 ),
	mStartCol( 0 ),
	mCurrentPlacedTile( boost::none ),
	mCurrentPlacedRow( kInvalid ),
	mCurrentPlacedCol( kInvalid ),
	mBag(),
	mNextTile(),
	mPlayers(),
	mCurrentPlayer( 0 ),
	mPiecesPlacedInCurrentTurn( 0 )
{
	// Initialize bag
	mBag = createTiles( inTiles );
	// Print out bag
	for ( unsigned int i = 0; i < mBag.size(); ++i )
	{
		std::cout << mBag[i].getID();
	}
	std::cout << std::endl;
	// Game signals
	connectGameSignals();
	// Initialize first tile
	if ( !mBag.empty() )
	{
		mNextTile = mBag.back();
		mBag.pop_back();
	}
	// Initialize players
	mPlayers.push_back( Player( "Thomas", Color::Blue ) );
	mPlayers.push_back( Player( "Gijs", Color::Red ) );
	connectPlayerSignals();
	emit currentPlayerChanged( mPlayers.front() );
}

Game::Game( std::vector< Player > const & inPlayers, QObject * inParent )
:
	QObject( inParent ),
	mBoard( Board( kSize ) ),
	mStartRow( 0 ),
	mStartCol( 0 ),
	mCurrentPlacedTile( boost::none ),
	mCurrentPlacedRow( kInvalid ),
	mCurrentPlacedCol( kInvalid ),
	mBag(),
	mNextTile(),
	mPlayers( inPlayers ),
	mCurrentPlayer( 0 ),
	mPiecesPlacedInCurrentTurn( 0 )
{
	// Initialize bag
	mBag = createBaseGameTiles();
	// Print out bag
	for ( unsigned int i = 0; i < mBag.size(); ++i )
	{
		std::cout << mBag[i].getID();
	}
	std::cout << std::endl;
	// Game signals
	connectGameSignals();
	// Initialize first tile
	if ( !mBag.empty() )
	{
		mNextTile = mBag.back();
		mBag.pop_back();
	}
	// Player signals
	connectPlayerSignals();
	emit currentPlayerChanged( mPlayers.front() );
}

Game::~Game()
{}

std::vector< Player > const &
Game::getPlayers()
{
	return mPlayers;
}

unsigned int
Game::getNrOfRows() const
{
	return mBoard.getNrOfRows();
}

unsigned int
Game::getNrOfCols() const
{
	return mBoard.getNrOfCols();
}

unsigned int
Game::getStartRow() const
{
	return mStartRow;
}

unsigned int
Game::getStartCol() const
{
	return mStartCol;
}

Player const &
Game::getCurrentPlayer() const
{
	return mPlayers[mCurrentPlayer];
}

void
Game::clickTile( unsigned int inCol, unsigned int inRow )
{
	if ( isEmptySpot( inCol, inRow ) )
	{
		std::cout << "Empty spot -> placeTileOnBoard" << std::endl;
		placeTileOnBoard( inCol, inRow );
	}
	else if ( isCurrentSpot( inCol, inRow ) )
	{
		std::cout << "Non-empty spot -> rotateCurrentTile" << std::endl;
		rotateCurrentTile();
	}
}

void
Game::placeTileOnBoard( unsigned int inCol, unsigned int inRow )
{
	if ( mNextTile )
	{
		// Delete tile that was placed earlier in current turn
		boost::optional< TileOnBoard > earlierTile = mCurrentPlacedTile;
		if ( earlierTile )
		{
			emit tileUnplaced( mCurrentPlacedCol, mCurrentPlacedRow );
		}
		// Try the newly clicked position (try all rotations until we found a good one)
		TileOnBoard::Rotation rotation = TileOnBoard::cw0;
		TileOnBoard toBePlacedTile = TileOnBoard(mNextTile.get(), rotation);
		bool found = false;
		for ( int i = 0; i < 4; ++i )
		{
			toBePlacedTile = TileOnBoard( mNextTile.get(), rotation );
			if ( mBoard.isValidTilePlacement( toBePlacedTile, inCol, inRow ) )
			{
				found = true;
				break;
			}
			else
			{
				rotation = TileOnBoard::Rotation( rotation + TileOnBoard::cw90 );
			}
		}
		std::cout << "Found: " << found << std::endl;
		if ( found )
		{
			if ( earlierTile && earlierTile->hasPieces() )
			{
				mPiecesPlacedInCurrentTurn = 0;
				returnPieces( earlierTile->removeAllPieces(), mCurrentPlacedCol, mCurrentPlacedRow );
			}
			mCurrentPlacedCol = inCol;
			mCurrentPlacedRow = inRow;
			mCurrentPlacedTile = toBePlacedTile;
			emit tilePlaced( mCurrentPlacedCol, mCurrentPlacedRow, toBePlacedTile.getID(), toBePlacedTile.getRotation() );
		}
		else
		{
			// If no valid placement is found at the newly clicked position, restore earlier tile
			if ( earlierTile )
			{
				mCurrentPlacedTile = earlierTile;
				emit tilePlaced( mCurrentPlacedCol, mCurrentPlacedRow, earlierTile->getID(), earlierTile->getRotation() );
			}
		}
	}
}

void
Game::placeStartTileOnBoard()
{
	if (mNextTile)
	{
		TileOnBoard::Rotation rotation = TileOnBoard::cw0; // TODO: get a random Rotation each time
		TileOnBoard toBePlacedTile = TileOnBoard(mNextTile.get(), rotation);
		unsigned int pos = mBoard.placeStartTile(toBePlacedTile);
		mStartCol = pos % mBoard.getNrOfCols();
		mStartRow = pos / mBoard.getNrOfCols();
		pickNextTile();
		emit tilePlaced(mStartCol, mStartRow, toBePlacedTile.getID(), toBePlacedTile.getRotation());
		if (mNextTile)
		{
			emit nextTile(mNextTile->getID());
		}
	}
}

boost::optional< Tile >
Game::getNextTile() const
{
	return mNextTile;
}

void
Game::tryToPlacePiece
(
	Color::Color inColor,
	Piece::PieceType inType,
	unsigned inCol,
	unsigned inRow,
	Area::Area inArea
)
{
	// You can only place a piece on the current placed tile
	if ( isCurrentSpot( inCol, inRow ) && mCurrentPlacedTile )
	{
		// Only the active player can place a piece
		// The active player needs a free piece to place
		// The active player cannot place more than one piece
		if ( mPlayers[mCurrentPlayer].getColor() == inColor
			&& mPlayers[mCurrentPlayer].hasFreePieces()
			&& mPiecesPlacedInCurrentTurn == 0 )
		{
			// You can only place a piece in an unoccupied area
			if ( !isOccupied( inArea ) )
			{
				// Construct a piece to place
				PlacedPiece placedPiece
				(
					mPlayers[mCurrentPlayer].getPieceToPlace(),
					inArea
				);
				// Place the piece
				if ( mCurrentPlacedTile->placePiece( placedPiece ) )
				{
					++mPiecesPlacedInCurrentTurn;
					emit piecePlaced( mCurrentPlacedCol, mCurrentPlacedRow, inArea, mPlayers[mCurrentPlayer] );
				}
				else
				{
					// Return the piece if the placement failed
					mPlayers[mCurrentPlayer].returnPiece( placedPiece.getPiece() );
				}
			}
		}
	}
}

void
Game::endTurn()
{
	mCurrentPlayer = (mCurrentPlayer + 1) % mPlayers.size();
	mPiecesPlacedInCurrentTurn = 0;
	emit currentPlayerChanged( mPlayers[mCurrentPlayer] );
}

void
Game::calculateEndPoints()
{
	for ( unsigned row = 0; row < mBoard.getNrOfRows(); ++row )
	{
		for ( unsigned col = 0; col < mBoard.getNrOfCols(); ++col )
		{
			if ( mBoard.isTile( col, row ) )
			{
				boost::optional< TileOnBoard > const tile = mBoard.getTile( col, row );
				std::vector< PlacedPiece > const pieces = tile->getPlacedPieces();
				for ( std::vector< PlacedPiece >::const_iterator it = pieces.begin();
					it != pieces.end();
					++it )
				{
					if ( tile->isCloister( it->getArea() ) )
					{
						// Remove all pieces
						std::vector< PlacedPiece > pieces = mBoard.removePieces( col, row, it->getArea() );
						returnPieces( pieces, col, row );
						// Calculate winner of unfinished cloister
						std::set< Color::Color > winningColors = getWinningColors( pieces );
						// Calculate points
						unsigned points = mBoard.getPointsForCloister( col, row );
						// Award points
						awardPoints( winningColors, points );
					}
					else if ( tile->isRoad( it->getArea() ) )
					{
						PlacedRoad roadPart( col, row, FRCArea::RoadArea( it->getArea() ) );
						std::vector< PlacedRoad > const road = mBoard.getCompleteRoad( roadPart );
						// Remove all pieces
						std::vector< PlacedPiece > allPieces;
						std::set< std::pair< unsigned, unsigned > > usedTiles;
						for ( std::vector< PlacedRoad >::const_iterator rIt = road.begin();
							rIt != road.end();
							++rIt )
						{
							usedTiles.insert( std::make_pair( rIt->col, rIt->row ) );
							std::vector< PlacedPiece > pieces = mBoard.getTile( rIt->col, rIt->row )->removePieces( Area::Area( rIt->area ) );
							returnPieces( pieces, rIt->col, rIt->row );
							allPieces.insert( allPieces.end(), pieces.begin(), pieces.end() );
						}
						// Calculate winner of unfinished road
						std::set< Color::Color > winningColors = getWinningColors( allPieces );
						// Calculate points
						unsigned points = usedTiles.size() * kPointsPerTileUnfinishedRoad;
						// Award points
						awardPoints( winningColors, points );
					}
					else if ( tile->isCity( it->getArea() ) )
					{
						PlacedCity cityPart( col, row, FRCArea::CityArea( it->getArea() ) );
						std::vector< PlacedCity > const city = mBoard.getCompleteCity( cityPart );
						// Calculate winner of unfinished city
						std::vector< PlacedPiece > allPieces;
						std::set< std::pair< unsigned, unsigned > > usedTiles;
						std::set< PlacedCity > allShields;
						for ( std::vector< PlacedCity >::const_iterator cIt = city.begin();
							cIt != city.end();
							++cIt )
						{
							// Add to usedTiles
							usedTiles.insert( std::make_pair( cIt->col, cIt->row ) );
							// Add shield if there was one
							std::vector< FRCArea::CityArea > shields = mBoard.getTile( cIt->col, cIt->row )->getShields();
                            if ( std::find( shields.begin(), shields.end(), cIt->area ) != shields.end() )
							{
                                allShields.insert( *cIt );
							}
							// Remove all pieces from this PlacedCity
							std::vector< PlacedPiece > pieces = mBoard.getTile( cIt->col, cIt->row )->removePieces( Area::Area( cIt->area ) );
							// Return removed pieces to corresponding Players
							returnPieces( pieces, cIt->col, cIt->row );
							// Insert in allPieces to calculate winning colors
							allPieces.insert( allPieces.end(), pieces.begin(), pieces.end() );
						}
						// Calculate winner of unfinished city
						std::set< Color::Color > winningColors = getWinningColors( allPieces );
						// Calculate points
						unsigned points = usedTiles.size() * kPointsPerTileUnfinishedCity + allShields.size() * kPointsPerTileUnfinishedShield;
						// Award points
						awardPoints( winningColors, points );
					}
				}
			}
		}
	}
}

void
Game::onEndCurrentTurn()
{
	std::cout << "onEndCurrentTurn" << std::endl;
	if ( mCurrentPlacedRow != kInvalid && mCurrentPlacedCol != kInvalid && mCurrentPlacedTile )
	{
		if ( mBoard.placeValidTile( *mCurrentPlacedTile, mCurrentPlacedCol, mCurrentPlacedRow ) )
		{
			mCurrentPlacedRow = kInvalid;
			mCurrentPlacedCol = kInvalid;
			mCurrentPlacedTile = boost::none;
			pickNextTile();
			if (mNextTile)
			{
				emit nextTile( mNextTile->getID() );
			}
			endTurn();
		}
	}
}

void
Game::addColsLeft( unsigned inNrOfCols )
{
	mStartCol += inNrOfCols;
}

void
Game::addRowsTop( unsigned inNrOfRows )
{
	mStartRow += inNrOfRows;
}

void
Game::onFinishedCloister( unsigned int inCol, unsigned int inRow )
{
	std::cout << "onFinishedCloister" << std::endl;
	// Remove all pieces
	std::vector< PlacedPiece > pieces = mBoard.removePieces( inCol, inRow, Area::Central );
	returnPieces( pieces, inCol, inRow );	
	// Find winning player for this cloister
	std::set< Color::Color > winningColors = getWinningColors( pieces );
	// Award points
	awardPoints( winningColors, kPointsForFinishedCloister );
	emit finishedCloister( inCol, inRow );
}

void
Game::onFinishedRoad( std::vector< PlacedRoad > const & inRoad )
{
	std::vector< PlacedPiece > allPieces;
	std::set< std::pair< unsigned, unsigned > > usedTiles;
	for ( std::vector< PlacedRoad >::const_iterator it = inRoad.begin(); it != inRoad.end(); ++it )
	{
		// Add to usedTiles (will not add doubles)
		usedTiles.insert( std::make_pair( it->col, it->row ) );
		// Remove all pieces from this PlacedRoad
		std::vector< PlacedPiece > pieces = mBoard.getTile( it->col, it->row )->removePieces( Area::Area( it->area ) );
		// Return removed pieces to corresponding Players
		returnPieces( pieces, it->col, it->row );
		// Insert in allPieces to calculate winning colors
		allPieces.insert( allPieces.end(), pieces.begin(), pieces.end() );
	}
	std::set< Color::Color > winningColors = getWinningColors( allPieces );
	awardPoints( winningColors, usedTiles.size() * kPointsPerTileFinishedRoad );
}

void
Game::onFinishedCity( std::vector< PlacedCity > const & inCity )
{
	std::vector< PlacedPiece > allPieces;
	std::set< std::pair< unsigned, unsigned > > usedTiles;
	std::set< PlacedCity > allShields;
	for ( std::vector< PlacedCity >::const_iterator it = inCity.begin(); it != inCity.end(); ++it )
	{
		// Add to usedTiles (will not add doubles)
		usedTiles.insert( std::make_pair( it->col, it->row ) );
		// Add shield if there was one
		std::vector< FRCArea::CityArea > shields = mBoard.getTile( it->col, it->row )->getShields();
		if ( std::find( shields.begin(), shields.end(), it->area ) != shields.end() )
		{
			allShields.insert( *it );
		}
		// Remove all pieces from this PlacedCity
		std::vector< PlacedPiece > pieces = mBoard.getTile( it->col, it->row )->removePieces( Area::Area( it->area ) );
		// Return removed pieces to corresponding Players
		returnPieces( pieces, it->col, it->row );
		// Insert in allPieces to calculate winning colors
		allPieces.insert( allPieces.end(), pieces.begin(), pieces.end() );
	}
	std::set< Color::Color > winningColors = getWinningColors( allPieces );
	awardPoints( winningColors, usedTiles.size() * kPointsPerTileFinishedCity + allShields.size() * kPointsPerTileFinishedShield );
}

void
Game::pickNextTile()
{
	std::cout << "pickNextTile" << std::endl;
	if (!mBag.empty())
	{
		std::vector< Tile >::iterator it = mBag.end();
		bool foundPossible = false;
		while (!foundPossible)
		{
			if (it != mBag.begin())
			{
				--it;
				Tile maybeNextTile = *it;
				if (mBoard.isPossibleTile(maybeNextTile))
				{
					mNextTile = maybeNextTile;
					mBag.erase(it);
					foundPossible = true;
				}
			}
			else
			{
				// No more valid tiles to place, this should give End Of Game
				mNextTile = boost::optional< Tile >();
				emit endOfGame( mBag.size() );
			}
		}
		emit tilesLeft(mBag.size());
	}
	else
	{
		mNextTile = boost::optional< Tile >();
		emit endOfGame( 0 );
	}
}

void
Game::rotateCurrentTile()
{
	if ( mCurrentPlacedTile )
	{
		TileOnBoard::Rotation currentRotation = mCurrentPlacedTile->getRotation();
		Tile tile = mCurrentPlacedTile->getTile();
		TileOnBoard::Rotation newRotation = currentRotation;
		TileOnBoard rotated = TileOnBoard( tile, newRotation );
		for ( int i = 0; i < 4; ++i )
		{
			newRotation = TileOnBoard::Rotation( (newRotation + TileOnBoard::cw90) % (TileOnBoard::cw90 * 4) );
			rotated = TileOnBoard( tile, newRotation );
			if ( mBoard.isValidTilePlacement( rotated, mCurrentPlacedCol, mCurrentPlacedRow ) )
			{
				break;
			}
		}
		if ( mBoard.isValidTilePlacement( rotated, mCurrentPlacedCol, mCurrentPlacedRow ) )
		{
			if ( mCurrentPlacedTile->hasPieces() )
			{
				mPiecesPlacedInCurrentTurn = 0;
				returnPieces( mCurrentPlacedTile->removeAllPieces(), mCurrentPlacedCol, mCurrentPlacedRow );
			}
			mCurrentPlacedTile = rotated;
			emit tileRotated( mCurrentPlacedCol, mCurrentPlacedRow, mCurrentPlacedTile->getID(), mCurrentPlacedTile->getRotation() );
		}
	}
}

Player &
Game::getPlayer( Color::Color inColor )
{
	for ( std::vector< Player >::iterator it = mPlayers.begin(); it != mPlayers.end(); ++it )
	{
		if ( it->getColor() == inColor )
		{
			return *it;
		}
	}
	assert( !"getPlayer found no player with given Color" );
}

void
Game::returnPieces
(
	std::vector< PlacedPiece > const & inPieces,
	unsigned inCol,
	unsigned inRow
)
{
	for ( std::vector< PlacedPiece >::const_iterator it = inPieces.begin();
		it != inPieces.end();
		++it )
	{
		getPlayer( it->getPiece().getColor() ).returnPiece( it->getPiece() );
		emit pieceReturned( inCol, inRow, it->getArea(), getPlayer( it->getPiece().getColor() ) );
	}
}

void
Game::awardPoints( std::set< Color::Color > const & inColors, unsigned inPoints )
{
	for ( std::set< Color::Color >::const_iterator it = inColors.begin();
		it != inColors.end();
		++it )
	{
		getPlayer( *it ).awardPoints( inPoints );
	}
}

bool
Game::isEmptySpot( unsigned inCol, unsigned inRow ) const
{
	return ( !isCurrentSpot( inCol, inRow ) && mBoard.isEmptySpot( inCol, inRow ) );
}

bool
Game::isCurrentSpot( unsigned inCol, unsigned inRow ) const
{
	return ( inCol == mCurrentPlacedCol && inRow == mCurrentPlacedRow );
}

bool
Game::isOccupied( Area::Area inArea ) const
{
	// Cloister
	if ( mCurrentPlacedTile->isCloister( inArea ) )
	{
		// A cloister is occupied if it has a piece
		return mCurrentPlacedTile->hasPiece( inArea );
	}
	// Road
	if ( mCurrentPlacedTile->isRoad( inArea ) )
	{
		// A road is occupied if there is a piece somewhere on this road
		Tile::ContiguousRoad road = mCurrentPlacedTile->getContiguousRoad( FRCArea::RoadArea( inArea ) );
		std::vector< PlacedRoad > roadsToCheck;
		for ( Tile::ContiguousRoad::const_iterator it = road.begin(); it != road.end(); ++it )
		{
			if ( mCurrentPlacedTile->hasPiece( Area::Area( *it ) ) )
			{
				return true;
			}
            roadsToCheck.push_back( getNeighbor( PlacedRoad( mCurrentPlacedCol, mCurrentPlacedRow, *it ) ) );
		}
		for ( std::vector< PlacedRoad >::iterator it = roadsToCheck.begin();
			it != roadsToCheck.end();
			++it )
		{
			if ( mBoard.isOccupiedRoad( it->col, it->row, it->area ) )
			{
				return true;
			}
		}
		return false;
	}
	// City
	if ( mCurrentPlacedTile->isCity( inArea ) )
	{
		// A city is occupied if there is a piece somewhere on this city
		Tile::ContiguousCity city = mCurrentPlacedTile->getContiguousCity( FRCArea::CityArea( inArea ) );
		std::vector< PlacedCity > citiesToCheck;
		for ( Tile::ContiguousCity::const_iterator it = city.begin(); it != city.end(); ++it )
		{
			if ( mCurrentPlacedTile->hasPiece( Area::Area( *it ) ) )
			{
				return true;
			}
			citiesToCheck.push_back( getNeighbor( PlacedCity( mCurrentPlacedCol, mCurrentPlacedRow, *it ) ) );
		}
		for ( std::vector< PlacedCity >::iterator it = citiesToCheck.begin();
			it != citiesToCheck.end();
			++it )
		{
			if ( mBoard.isOccupiedCity( it->col, it->row, it->area ) )
			{
				return true;
			}
		}
		return false;
	}
	return true;
}

void
Game::connectPlayerSignals()
{
	for ( std::vector< Player >::iterator it = mPlayers.begin(); it != mPlayers.end(); ++it )
	{
		connect
		(
			&(*it), SIGNAL( nrOfFreePiecesChanged( Player, uint ) ),
			this, SIGNAL( playerInfoChanged( Player ) )
		);
		connect
		(
			&(*it), SIGNAL( scoreChanged( Player, uint ) ),
			this, SIGNAL( playerInfoChanged( Player ) )
		);
	}
}

void
Game::connectGameSignals()
{
	connect
	(
		&mBoard, SIGNAL( finishedCloister(uint, uint) ),
		this, SLOT( onFinishedCloister(uint, uint) )
	);
	connect
	(
		&mBoard, SIGNAL( finishedCity( std::vector< PlacedCity > ) ),
		this, SLOT( onFinishedCity( std::vector< PlacedCity > ) )
	);
	connect
	(
		&mBoard, SIGNAL( finishedRoad( std::vector< PlacedRoad > ) ),
		this, SLOT( onFinishedRoad( std::vector< PlacedRoad > ) )
	);
	connect
	(
		&mBoard, SIGNAL( colsAddedLeft( unsigned ) ),
		this, SLOT( addColsLeft( unsigned ) )
	);
	connect
	(
		&mBoard, SIGNAL( rowsAddedTop( unsigned ) ),
		this, SLOT( addRowsTop( unsigned ) )
	);
}
