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

//-----------------------------------------------------------------------------

Model::NewPlayer::NewPlayer( std::string const & inName, Color::Color inColor )
:
	mName( inName ),
	mColor( inColor ),
	mScore( 0 ),
	mFreePieces( createBaseGamePieces( inColor ) ),
	mInfoChanged( new boost::signals2::signal< void () >() )
{
}

std::string const &
Model::NewPlayer::getName() const
{
	return mName;
}

Model::Color::Color
Model::NewPlayer::getColor() const
{
	return mColor;
}

std::size_t
Model::NewPlayer::getScore() const
{
	return mScore;
}

std::size_t
Model::NewPlayer::getNumberOfFreePieces() const
{
	return mFreePieces.size();
}

bool
Model::NewPlayer::hasFreePieces() const
{
	return !mFreePieces.empty();
}

Model::Piece
Model::NewPlayer::getPieceToPlace()
{
	assert( hasFreePieces() );
	Piece const result = mFreePieces.back();
	mFreePieces.pop_back();
	GetInfoChangedSignal()();
	return result;
}

void
Model::NewPlayer::returnPiece( Piece const & inPiece )
{
	mFreePieces.push_back( inPiece );
	GetInfoChangedSignal()();
}

void
Model::NewPlayer::awardPoints( std::size_t inPoints )
{
	mScore += inPoints;
	GetInfoChangedSignal()();
}

boost::signals2::signal< void () > &
Model::NewPlayer::GetInfoChangedSignal()
{
	return *mInfoChanged;
}