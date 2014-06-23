#include "Model/Player.h"

#include "Model/CreateTilesAndPieces.h"
#include "Model/Tile.h"

#include <cassert>

Model::Player::Player
(
	std::string const & inName,
	Color::Color inColor,
	std::set< Utils::Expansion::Type > const & inExpansions
)
:
	mName( inName ),
	mColor( inColor ),
	mScore( 0 ),
	mPieces( createPieces( inExpansions ) ),
	mInfoChanged( new boost::signals2::signal< void () >() )
{
}

Model::Player::Player
(
	std::string const & inName,
	Color::Color inColor,
	std::size_t inScore,
	std::map< Piece::PieceType, std::size_t > const & inPieces
)
:
	mName( inName ),
	mColor( inColor ),
	mScore( inScore ),
	mPieces( inPieces ),
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
Model::Player::getNumberOfFreePieces( Piece::PieceType inType ) const
{
	std::map< Piece::PieceType, std::size_t >::const_iterator const it = mPieces.find( inType );
	if ( it != mPieces.end() )
	{
		return it->second;
	}
	return 0;
}

bool
Model::Player::hasPieceToPlace( Piece::PieceType inType ) const
{
	return getNumberOfFreePieces( inType ) > 0;
}

boost::optional< Model::Piece >
Model::Player::getPieceToPlace( Piece::PieceType inType )
{
	std::map< Piece::PieceType, std::size_t >::iterator const it = mPieces.find( inType );
	if ( it != mPieces.end() && it->second > 0 )
	{
		it->second--;
		Piece const result( inType, mColor );
		GetInfoChangedSignal()();
		return result;
	}
	return boost::none;
}

void
Model::Player::returnPiece( Piece const & inPiece )
{
	std::map< Piece::PieceType, std::size_t >::iterator const it = mPieces.find( inPiece.getType() );
	if ( it != mPieces.end() )
	{
		it->second++;
	}
	else
	{
		mPieces.insert( std::make_pair( inPiece.getType(), 1 ) );
	}
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

std::map< Model::Piece::PieceType, std::size_t > const &
Model::Player::getPieces() const
{
	return mPieces;
}