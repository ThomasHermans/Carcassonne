#include "src-model/Player.h"

#include "src-model/CreateBaseGameTiles.h"
#include "src-model/Tile.h"

#include <cassert>

Player::Player( std::string inName, Color::Color inColor )
:
mName( inName ),
mColor( inColor ),
mScore( 0 ),
mFreePieces( createBaseGamePieces( inColor ) )
{
}

Player::Player( Player const & inPlayer )
:
mName( inPlayer.mName ),
mColor( inPlayer.mColor ),
mScore( inPlayer.mScore ),
mFreePieces( inPlayer.mFreePieces )
{
}

Player::~Player()
{
}

Player &
Player::operator = ( Player const & inPlayer )
{
	if ( this != &inPlayer )
	{
		mName = inPlayer.mName;
		mColor = inPlayer.mColor;
		mScore = inPlayer.mScore;
		mFreePieces = inPlayer.mFreePieces;
	}
	return *this;
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

Piece
Player::getPieceToPlace()
{
	assert( hasFreePieces() );
	Piece result = mFreePieces.back();
	mFreePieces.pop_back();
	emit nrOfFreePiecesChanged( *this, mFreePieces.size() );
	return result;
}

void
Player::returnPiece( Piece inPiece )
{
	mFreePieces.push_back( inPiece );
	emit nrOfFreePiecesChanged( *this, mFreePieces.size() );
}

void
Player::awardPoints( unsigned inPoints )
{
	mScore += inPoints;
	emit scoreChanged( mScore );
}
