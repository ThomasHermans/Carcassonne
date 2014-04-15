#include "Model/Player.h"

#include "CreateBaseGameTiles.h"
#include "Tile.h"

#include <cassert>

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