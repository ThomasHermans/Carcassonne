#include "src-model/Player.h"

#include "src-model/CreateBaseGameTiles.h"
#include "src-model/Tile.h"

Player::Player( std::string inName, Color::Color inColor )
:
mName( inName ),
mColor( inColor ),
mScore( 0 ),
mFreePieces( createBaseGamePieces( inColor ) ),
mPlacedPieces()
{
}

Player::~Player()
{
}

std::string
Player::getName() const
{
	return mName;
}

Color::Color
Player::getColor() const
{
	return mColor;
}

int
Player::getScore() const
{
	return mScore;
}

int
Player::getNumberOfFreePieces() const
{
	return mFreePieces.size();
}

bool
Player::hasFreePieces() const
{
	return !mFreePieces.empty();
}

bool
Player::placePiece( int inRelCol, int inRelRow, Area::Area inArea )
{
	if ( hasFreePieces() )
	{
		PlacedPiece placedPiece( mFreePieces.back(), inRelCol, inRelRow, inArea );
        if ( placedPiece.getArea() != Area::Invalid )
		{
			mPlacedPieces.push_back( placedPiece );
			mFreePieces.pop_back();
			emit nrOfFreePiecesChanged( mFreePieces.size() );
			return true;
		}
	}
	return false;
}

bool
Player::returnPiece( int inRelCol, int inRelRow, Area::Area inArea )
{
	std::vector< PlacedPiece >::iterator it = mPlacedPieces.begin();
	bool removed = false;
	while ( it != mPlacedPieces.end() )
	{
		if ( it->getCol() == inRelCol && it->getRow() == inRelRow && it->getArea() == inArea )
		{
			mFreePieces.push_back( Piece( *it ) );
			it = mPlacedPieces.erase( it );
			removed = true;
		}
		else
		{
			++it;
		}
	}
	emit nrOfFreePiecesChanged( mFreePieces.size() );
	return removed;
}

bool
Player::hasPiece( int inRelCol, int inRelRow, Area::Area inArea )
{
	std::vector< PlacedPiece >::iterator it = mPlacedPieces.begin();
	while ( it != mPlacedPieces.end() )
	{
		if ( it->getCol() == inRelCol && it->getRow() == inRelRow && it->getArea() == inArea )
		{
			return true;
		}
		else
		{
			++it;
		}
	}
	return false;
}

unsigned
Player::getNrOfPieces( int inRelCol, int inRelRow, Area::Area inArea )
{
	unsigned nrOfPieces = 0;
	for ( std::vector< PlacedPiece >::iterator it = mPlacedPieces.begin(); it != mPlacedPieces.end(); ++it )
	{
		if ( it->getCol() == inRelCol && it->getRow() == inRelRow && it->getArea() == inArea )
		{
            ++nrOfPieces;
		}
	}
	return nrOfPieces;
}

void
Player::awardPoints( unsigned inPoints )
{
	mScore += inPoints;
	emit scoreChanged( mScore );
}
