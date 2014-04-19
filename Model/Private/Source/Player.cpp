#include "Model/Player.h"

#include "CreateTilesAndPieces.h"
#include "Tile.h"

#include <cassert>

Model::Player::Player( std::string const & inName, Color::Color inColor )
:
	mName( inName ),
	mColor( inColor ),
	mScore( 0 ),
	mFreePieces( createBaseGamePieces( inColor ) ),
	mInfoChanged( new boost::signals2::signal< void () >() )
{
}

std::string const &
Model::Player::getName() const
{
	return mName;
}

Model::Color::Color
Model::Player::getColor() const
{
	return mColor;
}

std::size_t
Model::Player::getScore() const
{
	return mScore;
}

std::size_t
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
	Piece const result = mFreePieces.back();
	mFreePieces.pop_back();
	GetInfoChangedSignal()();
	return result;
}

void
Model::Player::returnPiece( Piece const & inPiece )
{
	mFreePieces.push_back( inPiece );
	GetInfoChangedSignal()();
}

void
Model::Player::awardPoints( std::size_t inPoints )
{
	mScore += inPoints;
	GetInfoChangedSignal()();
}

boost::signals2::signal< void () > &
Model::Player::GetInfoChangedSignal()
{
	return *mInfoChanged;
}