#include "src-model/Game.h"

#include "src-model/CreateBaseGameTiles.h"

#include <set>
#include <stdio.h>

namespace
{
	int const kSize = 5;
	unsigned const kPointsForFinishedCloister = 9;
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

Game::Game() :
	mBoard( Board( kSize ) ),
	mStartRow( 0 ),
	mStartCol( 0 ),
	mCurrentPlacedTile( boost::none ),
	mCurrentPlacedRow( kInvalid ),
	mCurrentPlacedCol( kInvalid ),
	mBag(),
	mNextTile(),
	mPlayers(),
	mCurrentPlayer( 0 )
{
	mBag = createBaseGameTiles();
	for ( unsigned int i = 0; i < mBag.size(); ++i )
	{
		std::cout << mBag[i].getID();
	}
	std::cout << std::endl;
	if ( !mBag.empty() )
	{
		mNextTile = mBag.back();
		mBag.pop_back();
	}
	mPlayers.push_back( Player( "Thomas", Color::Green ) );
	mPlayers.push_back( Player( "Gijs", Color::Red ) );
	for ( std::vector< Player >::iterator it = mPlayers.begin(); it != mPlayers.end(); ++it )
	{
		connect
		(
			&(*it), SIGNAL( nrOfFreePiecesChanged( Player, uint ) ),
			this, SIGNAL( playerInfoChanged( Player ) )
		);
	}
	emit currentPlayerChanged( mPlayers.front() );
	connect
	(
		&mBoard, SIGNAL( finishedCloister(uint, uint) ),
		this, SLOT( onFinishedCloister(uint, uint) )
	);
	connect
	(
		&mBoard, SIGNAL( finishedCity(std::vector< std::pair< uint, uint > >) ),
		this, SIGNAL( finishedCity(std::vector< std::pair< uint, uint > >) )
	);
	connect
	(
		&mBoard, SIGNAL( finishedRoad(std::vector< std::pair< uint, uint > >) ),
		this, SIGNAL( finishedRoad(std::vector< std::pair< uint, uint > >) )
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

Game::~Game()
{}

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
	if ( mCurrentPlacedCol == inCol && mCurrentPlacedRow == inRow && mCurrentPlacedTile)
	{
		if ( mPlayers[mCurrentPlayer].getColor() == inColor && mPlayers[mCurrentPlayer].hasFreePieces() )
		{
			if ( inArea == Area::Central && mCurrentPlacedTile->getCenter() == Tile::Cloister )
			{
				PlacedPiece placedPiece
				(
					mPlayers[mCurrentPlayer].getPieceToPlace(),
					Area::Central
				);
				if ( mCurrentPlacedTile->placePiece( placedPiece ) )
				{
					emit piecePlaced( mCurrentPlacedCol, mCurrentPlacedRow, mPlayers[mCurrentPlayer] );
				}
				else
				{
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
	emit currentPlayerChanged( mPlayers[mCurrentPlayer] );
}

void
Game::onTryToPlacePiece()
{
	std::cout << "onTryToPlacePiece" << std::endl;
	if ( mCurrentPlacedRow != kInvalid
		&& mCurrentPlacedCol != kInvalid
		&& mPlayers[mCurrentPlayer].hasFreePieces() )
	{
		if ( mCurrentPlacedTile && mCurrentPlacedTile->getCenter() == Tile::Cloister )
		{
			// Place a meeple on this cloister
			PlacedPiece placedPiece
			(
				mPlayers[mCurrentPlayer].getPieceToPlace(),
				Area::Central
			);
			if ( mCurrentPlacedTile->placePiece( placedPiece ) )
			{
				emit piecePlaced( mCurrentPlacedCol, mCurrentPlacedRow, mPlayers[mCurrentPlayer] );
			}
			else
			{
				mPlayers[mCurrentPlayer].returnPiece( placedPiece.getPiece() );
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
			mBoard.checkForFinishedCloisters( mCurrentPlacedCol, mCurrentPlacedRow );
			mBoard.checkForFinishedCities( mCurrentPlacedCol, mCurrentPlacedRow );
			mBoard.checkForFinishedRoads( mCurrentPlacedCol, mCurrentPlacedRow );
			mCurrentPlacedRow = kInvalid;
			mCurrentPlacedCol = kInvalid;
			mCurrentPlacedTile = boost::none;
			pickNextTile();
			if (mNextTile)
			{
				emit nextTile(mNextTile->getID());
			}
			endTurn();
		}
	}
}

void
Game::onFinishedCloister( unsigned int inCol, unsigned int inRow )
{
	std::cout << "onFinishedCloister" << std::endl;
	// Find winning player for this cloister
	std::vector< PlacedPiece > pieces = mBoard.removePieces( inCol, inRow, Area::Central );
	returnPieces( pieces, inCol, inRow );	
	std::set< Color::Color > winningColors = getWinningColors( pieces );
	awardPoints( winningColors, kPointsForFinishedCloister );
	emit finishedCloister( inCol, inRow );
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
		emit pieceReturned( inCol, inRow, getPlayer( it->getPiece().getColor() ) );
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