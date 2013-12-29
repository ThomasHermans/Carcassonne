#include "Model/Piece.h"

#include "PlacedPiece.h"

#include <cassert>

namespace
{
	std::string
	convertToString( Model::Piece::PieceType inType )
	{
		switch ( inType )
		{
		case Model::Piece::kFollower:
			return "Follower";
		case Model::Piece::kLargeFollower:
			return "Large Follower";
		case Model::Piece::kBuilder:
			return "Builder";
		case Model::Piece::kPig:
			return "Pig";
		}
		assert( !"Invalid PieceType to translate!" );
		return "Follower";
	}
}

Model::Piece::Piece()
:
	mType( kFollower ),
	mColor( Color::kRed )
{
}

Model::Piece::Piece( const Piece & inPiece )
:
	mType( inPiece.getType() ),
	mColor( inPiece.getColor() )
{
}

Model::Piece::Piece( const PlacedPiece & inPlacedPiece )
:
	mType( inPlacedPiece.getType() ),
	mColor( inPlacedPiece.getColor() )
{
}

Model::Piece::Piece( PieceType inType, Color::Color inColor )
:
	mType( inType ),
	mColor( inColor )
{
}

Model::Piece &
Model::Piece::operator = ( const Piece & inPiece )
{
	if ( this != &inPiece )
	{
		this->mType = inPiece.getType();
		this->mColor = inPiece.getColor();
	}
	return *this;
}

Model::Piece::PieceType
Model::Piece::getType() const
{
	return mType;
}

Model::Color::Color
Model::Piece::getColor() const
{
	return mColor;
}

int
Model::Piece::getPoints() const
{
	switch ( mType )
	{
	case kFollower:
		return 1;
	case kLargeFollower:
		return 2;
	case kBuilder:
	case kPig:
	default:
		return 0;
	}
}

std::string
Model::Piece::toString() const
{
	std::string res = colorToString( mColor );
	res.append( " " );
	res.append( convertToString( mType ) );
	return res;
}
