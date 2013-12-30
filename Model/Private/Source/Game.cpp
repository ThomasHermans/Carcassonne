#include "Model/Game.h"

#include "CreateBaseGameTiles.h"

#include <set>
#include <stdio.h>

namespace
{
	int const kSize = 5;
	unsigned const kMonkPointsForFinishedCloister = 9;
	unsigned const kBrigandPointsPerTileFinishedRoad = 1;
	unsigned const kBrigandPointsPerTileUnfinishedRoad = 1;
	unsigned const kKnightPointsPerTileFinishedCity = 2;
	unsigned const kKnightPointsPerTileFinishedShield = 2;
	unsigned const kKnightPointsPerTileUnfinishedCity = 1;
	unsigned const kKnightPointsPerTileUnfinishedShield = 1;
	unsigned const kFarmerPointsPerFinishedCity = 3;
	unsigned const kInvalid = -1;

	std::set< Model::Color::Color >
	getWinningColors( std::vector< Model::PlacedPiece > const & inPieces )
	{
		typedef std::map< Model::Color::Color, unsigned > Counts;
		Counts counts;
		for ( std::vector< Model::PlacedPiece >::const_iterator it = inPieces.begin();
			it != inPieces.end();
			++it )
		{
			Model::Color::Color color = it->getPiece().getColor();
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
		std::set< Model::Color::Color > winningColors;
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

Model::Game::Game( QObject * inParent )
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
	mPlayers.push_back( Player( "Thomas", Color::kBlue ) );
	mPlayers.push_back( Player( "Gijs", Color::kRed ) );
	connectPlayerSignals();
	emit currentPlayerChanged( mPlayers.front() );
}

Model::Game::Game( std::string const & inTiles, QObject * inParent )
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
	mPlayers.push_back( Player( "Thomas", Color::kBlue ) );
	mPlayers.push_back( Player( "Gijs", Color::kRed ) );
	connectPlayerSignals();
	emit currentPlayerChanged( mPlayers.front() );
}

Model::Game::Game( std::vector< Player > const & inPlayers, QObject * inParent )
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

Model::Game::~Game()
{}

std::vector< Model::Player > const &
Model::Game::getPlayers()
{
	return mPlayers;
}

unsigned int
Model::Game::getNrOfRows() const
{
	return mBoard.getNrOfRows();
}

unsigned int
Model::Game::getNrOfCols() const
{
	return mBoard.getNrOfCols();
}

unsigned int
Model::Game::getStartRow() const
{
	return mStartRow;
}

unsigned int
Model::Game::getStartCol() const
{
	return mStartCol;
}

Model::Player const &
Model::Game::getCurrentPlayer() const
{
	return mPlayers[mCurrentPlayer];
}

void
Model::Game::rotateTile( unsigned inCol, unsigned inRow )
{
	if ( inCol >= mBoard.getNrOfCols() || inRow >= mBoard.getNrOfRows() )
	{
		return;
	}
	if ( isCurrentSpot( inCol, inRow ) )
	{
		Rotation currentRotation = mCurrentPlacedTile->getRotation();
		Tile tile = mCurrentPlacedTile->getTile();
		Rotation newRotation = currentRotation;
		TileOnBoard rotated = TileOnBoard( tile, newRotation );
		for ( int i = 0; i < 4; ++i )
		{
			newRotation = Rotation( (newRotation + kCw90) % (kCw90 * 4) );
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

void
Model::Game::dropTile( unsigned inCol, unsigned inRow, std::string const & inTileId, Model::Rotation inRotation )
{
	if ( inCol >= mBoard.getNrOfCols() || inRow >= mBoard.getNrOfRows() )
	{
		return;
	}
	if ( mNextTile && mNextTile->getID() == inTileId && isEmptySpot( inCol, inRow ) )
	{
		placeTileOnBoard( inCol, inRow, inRotation );
	}
}

void
Model::Game::placeTileOnBoard( unsigned inCol, unsigned inRow, Model::Rotation inRotation )
{
	if ( mNextTile )
	{
		// Delete tile that was placed earlier in current turn
		boost::optional< Model::TileOnBoard > earlierTile = mCurrentPlacedTile;
		if ( earlierTile )
		{
			emit tileUnplaced( mCurrentPlacedCol, mCurrentPlacedRow );
		}
		// Try the newly clicked position (try all rotations until we found a good one)
		Model::Rotation rotation = inRotation;
		Model::TileOnBoard toBePlacedTile = Model::TileOnBoard(mNextTile.get(), rotation);
		bool found = false;
		for ( unsigned i = 0; i < 4; ++i )
		{
			toBePlacedTile = Model::TileOnBoard( mNextTile.get(), rotation );
			if ( mBoard.isValidTilePlacement( toBePlacedTile, inCol, inRow ) )
			{
				found = true;
				break;
			}
			else
			{
				rotation = Model::Rotation( rotation + Model::kCw90 );
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
Model::Game::placeStartTileOnBoard()
{
	if (mNextTile)
	{
		Model::Rotation rotation = Model::kCw0; // TODO: get a random Rotation each time
		Model::TileOnBoard toBePlacedTile = Model::TileOnBoard(mNextTile.get(), rotation);
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

boost::optional< Model::Tile >
Model::Game::getNextTile() const
{
	return mNextTile;
}

void
Model::Game::tryToPlacePiece
(
	Color::Color inColor,
	Piece::PieceType inType,
	unsigned inCol,
	unsigned inRow,
	Area::Area inArea
)
{
	// You can only place a piece on the current placed tile
	if ( isCurrentSpot( inCol, inRow ) )
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
Model::Game::endTurn()
{
	mCurrentPlayer = (mCurrentPlayer + 1) % mPlayers.size();
	mPiecesPlacedInCurrentTurn = 0;
	emit currentPlayerChanged( mPlayers[mCurrentPlayer] );
}

void
Model::Game::calculateEndPoints()
{
	for ( unsigned row = 0; row < mBoard.getNrOfRows(); ++row )
	{
		for ( unsigned col = 0; col < mBoard.getNrOfCols(); ++col )
		{
			if ( mBoard.isTile( col, row ) )
			{
				boost::optional< Model::TileOnBoard > const tile = mBoard.getTile( col, row );
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
						PlacedRoad roadPart( col, row, it->getArea() );
						std::vector< PlacedRoad > const road = mBoard.getCompleteRoad( roadPart );
						// Remove all pieces
						std::vector< PlacedPiece > allPieces;
						std::set< std::pair< unsigned, unsigned > > usedTiles;
						for ( std::vector< PlacedRoad >::const_iterator rIt = road.begin();
							rIt != road.end();
							++rIt )
						{
							usedTiles.insert( std::make_pair( rIt->col, rIt->row ) );
							std::vector< PlacedPiece > pieces = mBoard.removePieces( rIt->col, rIt->row, rIt->area );
							returnPieces( pieces, rIt->col, rIt->row );
							allPieces.insert( allPieces.end(), pieces.begin(), pieces.end() );
						}
						// Calculate winner of unfinished road
						std::set< Color::Color > winningColors = getWinningColors( allPieces );
						// Calculate points
						unsigned points = usedTiles.size() * kBrigandPointsPerTileUnfinishedRoad;
						// Award points
						awardPoints( winningColors, points );
					}
					else if ( tile->isCity( it->getArea() ) )
					{
						PlacedCity cityPart( col, row, it->getArea() );
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
							std::vector< Area::Area > shields = mBoard.getTile( cIt->col, cIt->row )->getShields();
                            if ( std::find( shields.begin(), shields.end(), cIt->area ) != shields.end() )
							{
                                allShields.insert( *cIt );
							}
							// Remove all pieces from this PlacedCity
							std::vector< PlacedPiece > pieces = mBoard.removePieces( cIt->col, cIt->row, cIt->area );
							// Return removed pieces to corresponding Players
							returnPieces( pieces, cIt->col, cIt->row );
							// Insert in allPieces to calculate winning colors
							allPieces.insert( allPieces.end(), pieces.begin(), pieces.end() );
						}
						// Calculate winner of unfinished city
						std::set< Color::Color > winningColors = getWinningColors( allPieces );
						// Calculate points
						unsigned points = usedTiles.size() * kKnightPointsPerTileUnfinishedCity + allShields.size() * kKnightPointsPerTileUnfinishedShield;
						// Award points
						awardPoints( winningColors, points );
					}
					else if ( tile->isField( it->getArea() ) )
					{
						// Calculate winner of field
						std::cout << "Calculate winner of a field." << std::endl;
						PlacedField fieldPart( col, row, it->getArea() );
						std::vector< PlacedField > const field = mBoard.getCompleteField( fieldPart );
						std::vector< PlacedPiece > allPieces;
						std::set< PlacedCity > finishedCities;
						for ( std::vector< PlacedField >::const_iterator fIt = field.begin();
							fIt != field.end();
							++fIt )
						{
							// Get all cities for fIt
							std::vector< ContiguousCity > const cities = mBoard.getTile( fIt->col, fIt->row )->getCitiesPerField( fIt->area );
							// Per city
							// If city is finished
							//	Calculate upperleft PlacedCity
							//	Add to finishedCities
							for ( std::vector< ContiguousCity >::const_iterator cIt = cities.begin();
								cIt != cities.end();
								++cIt )
							{
								if ( mBoard.isFinishedCity( fIt->col, fIt->row, *cIt->begin() ) )
								{
									PlacedCity const upperLeft = mBoard.getUpperLeftPlacedCity( fIt->col, fIt->row, *cIt->begin() );
									finishedCities.insert( upperLeft );
									std::cout << "Found a finished city." << std::endl;
								}
								else
								{
									std::cout << "Found an unfinished city." << std::endl;
								}
							}
							// Remove all pieces from this PlacedField
							std::vector< PlacedPiece > pieces = mBoard.removePieces( fIt->col, fIt->row, fIt->area );
							// Return removed pieces to corresponding Players
							returnPieces( pieces, fIt->col, fIt->row );
							// Insert in allPieces to calculate winning colors
							allPieces.insert( allPieces.end(), pieces.begin(), pieces.end() );
						}
						std::set< Color::Color > winningColors = getWinningColors( allPieces );
						// Calculate points
						unsigned points = finishedCities.size() * kFarmerPointsPerFinishedCity;
						std::cout << "Awarding " << points << " points for a field." << std::endl;
						// Award points
						awardPoints( winningColors, points );
					}
				}
			}
		}
	}
}

void
Model::Game::onEndCurrentTurn()
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
Model::Game::addColsLeft( unsigned inNrOfCols )
{
	mStartCol += inNrOfCols;
}

void
Model::Game::addRowsTop( unsigned inNrOfRows )
{
	mStartRow += inNrOfRows;
}

void
Model::Game::onFinishedCloister( unsigned int inCol, unsigned int inRow )
{
	std::cout << "onFinishedCloister" << std::endl;
	// Remove all pieces
	std::vector< PlacedPiece > pieces = mBoard.removePieces( inCol, inRow, Area::kCentral );
	returnPieces( pieces, inCol, inRow );	
	// Find winning player for this cloister
	std::set< Color::Color > winningColors = getWinningColors( pieces );
	// Award points
	awardPoints( winningColors, kMonkPointsForFinishedCloister );
	emit finishedCloister( inCol, inRow );
}

void
Model::Game::onFinishedRoad( std::vector< PlacedRoad > const & inRoad )
{
	std::vector< PlacedPiece > allPieces;
	std::set< std::pair< unsigned, unsigned > > usedTiles;
	for ( std::vector< PlacedRoad >::const_iterator it = inRoad.begin(); it != inRoad.end(); ++it )
	{
		// Add to usedTiles (will not add doubles)
		usedTiles.insert( std::make_pair( it->col, it->row ) );
		// Remove all pieces from this PlacedRoad
		std::vector< PlacedPiece > pieces = mBoard.getTile( it->col, it->row )->removePieces( it->area );
		// Return removed pieces to corresponding Players
		returnPieces( pieces, it->col, it->row );
		// Insert in allPieces to calculate winning colors
		allPieces.insert( allPieces.end(), pieces.begin(), pieces.end() );
	}
	std::set< Color::Color > winningColors = getWinningColors( allPieces );
	awardPoints( winningColors, usedTiles.size() * kBrigandPointsPerTileFinishedRoad );
}

void
Model::Game::onFinishedCity( std::vector< PlacedCity > const & inCity )
{
	std::vector< PlacedPiece > allPieces;
	std::set< std::pair< unsigned, unsigned > > usedTiles;
	std::set< PlacedCity > allShields;
	for ( std::vector< PlacedCity >::const_iterator it = inCity.begin(); it != inCity.end(); ++it )
	{
		// Add to usedTiles (will not add doubles)
		usedTiles.insert( std::make_pair( it->col, it->row ) );
		// Add shield if there was one
		std::vector< Area::Area > shields = mBoard.getTile( it->col, it->row )->getShields();
		if ( std::find( shields.begin(), shields.end(), it->area ) != shields.end() )
		{
			allShields.insert( *it );
		}
		// Remove all pieces from this PlacedCity
		std::vector< PlacedPiece > pieces = mBoard.getTile( it->col, it->row )->removePieces( it->area );
		// Return removed pieces to corresponding Players
		returnPieces( pieces, it->col, it->row );
		// Insert in allPieces to calculate winning colors
		allPieces.insert( allPieces.end(), pieces.begin(), pieces.end() );
	}
	std::set< Color::Color > winningColors = getWinningColors( allPieces );
	awardPoints( winningColors, usedTiles.size() * kKnightPointsPerTileFinishedCity + allShields.size() * kKnightPointsPerTileFinishedShield );
}

void
Model::Game::pickNextTile()
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

Model::Player &
Model::Game::getPlayer( Color::Color inColor )
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
Model::Game::returnPieces
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
Model::Game::awardPoints( std::set< Color::Color > const & inColors, unsigned inPoints )
{
	for ( std::set< Color::Color >::const_iterator it = inColors.begin();
		it != inColors.end();
		++it )
	{
		getPlayer( *it ).awardPoints( inPoints );
	}
}

bool
Model::Game::isEmptySpot( unsigned inCol, unsigned inRow ) const
{
	return ( !isCurrentSpot( inCol, inRow ) && mBoard.isEmptySpot( inCol, inRow ) );
}

bool
Model::Game::isCurrentSpot( unsigned inCol, unsigned inRow ) const
{
	return ( mCurrentPlacedTile && inCol == mCurrentPlacedCol && inRow == mCurrentPlacedRow );
}

bool
Model::Game::isOccupied( Area::Area inArea ) const
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
		ContiguousRoad road = mCurrentPlacedTile->getContiguousRoad( inArea );
		std::vector< PlacedRoad > roadsToCheck;
		for ( ContiguousRoad::const_iterator it = road.begin(); it != road.end(); ++it )
		{
			if ( mCurrentPlacedTile->hasPiece( *it ) )
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
		ContiguousCity city = mCurrentPlacedTile->getContiguousCity( inArea );
		std::vector< PlacedCity > citiesToCheck;
		for ( ContiguousCity::const_iterator it = city.begin(); it != city.end(); ++it )
		{
			if ( mCurrentPlacedTile->hasPiece( *it ) )
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
	// Field
	if ( mCurrentPlacedTile->isField( inArea ) )
	{
		// A field is occupied if there is a piece somewhere on this city
		ContiguousField field = mCurrentPlacedTile->getContiguousField( inArea );
		std::vector< PlacedField > fieldsToCheck;
		for ( ContiguousField::const_iterator it = field.begin(); it != field.end(); ++it )
		{
			if ( mCurrentPlacedTile->hasPiece( *it ) )
			{
				return true;
			}
			fieldsToCheck.push_back( getNeighbor( PlacedField( mCurrentPlacedCol, mCurrentPlacedRow, *it ) ) );
		}
		for ( std::vector< PlacedField >::iterator it = fieldsToCheck.begin();
			it != fieldsToCheck.end();
			++it )
		{
			if ( mBoard.isOccupiedField( *it ) )
			{
				return true;
			}
		}
		return false;
	}
	return true;
}

void
Model::Game::connectPlayerSignals()
{
	for ( std::vector< Player >::iterator it = mPlayers.begin(); it != mPlayers.end(); ++it )
	{
		connect
		(
			&(*it), SIGNAL( nrOfFreePiecesChanged( Model::Player, uint ) ),
			this, SIGNAL( playerInfoChanged( Model::Player ) )
		);
		connect
		(
			&(*it), SIGNAL( scoreChanged( Model::Player, uint ) ),
			this, SIGNAL( playerInfoChanged( Model::Player ) )
		);
	}
}

void
Model::Game::connectGameSignals()
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
