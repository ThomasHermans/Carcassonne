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
Player::placePiece( unsigned int inCol, unsigned int inRow, Area::Area inArea )
{
	if ( hasFreePieces() )
	{
		PlacedPiece placedPiece( mFreePieces.back(), inCol, inRow, inArea );
        if ( placedPiece.getArea() != Area::Invalid )
		{
			mPlacedPieces.push_back( placedPiece );
			mFreePieces.pop_back();
			return true;
		}
	}
	return false;
}

bool
Player::returnPiece( unsigned int inCol, unsigned int inRow, Area::Area inArea )
{
	std::vector< PlacedPiece >::iterator it = mPlacedPieces.begin();
	while ( it != mPlacedPieces.end() )
	{
		if ( it->getCol() == inCol && it->getRow() == inRow && it->getArea() == inArea )
		{
			mFreePieces.push_back( Piece( *it ) );
			it = mPlacedPieces.erase( it );
			return true;
		}
		else
		{
			++it;
		}
	}
	return false;
}

bool
Player::hasPiece( unsigned int inCol, unsigned int inRow, Area::Area inArea )
{
	std::vector< PlacedPiece >::iterator it = mPlacedPieces.begin();
	while ( it != mPlacedPieces.end() )
	{
		if ( it->getCol() == inCol && it->getRow() == inRow && it->getArea() == inArea )
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
