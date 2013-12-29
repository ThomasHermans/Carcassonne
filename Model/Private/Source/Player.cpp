#include "Model/Player.h"

#include "CreateBaseGameTiles.h"
#include "Tile.h"

#include <cassert>

Model::Player::Player( std::string inName, Color::Color inColor )
:
mName( inName ),
mColor( inColor ),
mScore( 0 ),
mFreePieces( createBaseGamePieces( inColor ) )
{
}

Model::Player::Player( Player const & inPlayer )
:
mName( inPlayer.mName ),
mColor( inPlayer.mColor ),
mScore( inPlayer.mScore ),
mFreePieces( inPlayer.mFreePieces )
{
}

Model::Player::~Player()
{
}

Model::Player &
Model::Player::operator = ( Player const & inPlayer )
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
Model::Player::getName() const
{
	return mName;
}

Model::Color::Color
Model::Player::getColor() const
{
	return mColor;
}

unsigned
Model::Player::getScore() const
{
	return mScore;
}

unsigned
Model::Player::getNumberOfFreePieces() const
{
	return mFreePieces.size();
}

bool
Model::Player::hasFreePieces() const
{
	return !mFreePieces.empty();
}

Model::Piece
Model::Player::getPieceToPlace()
{
	assert( hasFreePieces() );
	Piece result = mFreePieces.back();
	mFreePieces.pop_back();
	emit nrOfFreePiecesChanged( *this, mFreePieces.size() );
	return result;
}

void
Model::Player::returnPiece( Piece inPiece )
{
	mFreePieces.push_back( inPiece );
	emit nrOfFreePiecesChanged( *this, mFreePieces.size() );
}

void
Model::Player::awardPoints( unsigned inPoints )
{
	mScore += inPoints;
	emit scoreChanged( *this, mScore );
}
