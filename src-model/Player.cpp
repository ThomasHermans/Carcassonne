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

bool
Player::hasFreePieces() const
{
	return !mFreePieces.empty();
}

bool
Player::placePiece( unsigned int inLocation, Area::Area inArea )
{
	if ( hasFreePieces() )
	{
		PlacedPiece placedPiece( mFreePieces.back(), inLocation, inArea );
        if ( placedPiece.getArea() != Area::Invalid )
		{
			mPlacedPieces.push_back( placedPiece );
			mFreePieces.pop_back();
			return true;
		}
	}
	return false;
}
