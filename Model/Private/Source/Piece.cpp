#include "Piece.h"

#include "PlacedPiece.h"

#include <cassert>

namespace
{
	std::string
	convertToString( Piece::PieceType inType )
	{
		switch ( inType )
		{
		case Piece::kFollower:
			return "Follower";
		case Piece::kLargeFollower:
			return "Large Follower";
		case Piece::kBuilder:
			return "Builder";
		case Piece::kPig:
			return "Pig";
		}
		assert( !"Invalid PieceType to translate!" );
		return "Follower";
	}
}

Piece::Piece():
	mType( Piece::kFollower ),
	mColor( Color::kRed )
{
}

Piece::Piece( const Piece & inPiece ):
	mType( inPiece.getType() ),
	mColor( inPiece.getColor() )
{
}

Piece::Piece( const PlacedPiece & inPlacedPiece ) :
	mType( inPlacedPiece.getType() ),
	mColor( inPlacedPiece.getColor() )
{
}

Piece::Piece( PieceType inType, Color::Color inColor ):
	mType( inType ),
	mColor( inColor )
{
}

Piece&
Piece::operator = ( const Piece & inPiece )
{
	if ( this != &inPiece )
	{
		this->mType = inPiece.getType();
		this->mColor = inPiece.getColor();
	}
	return *this;
}

Piece::PieceType
Piece::getType() const
{
	return mType;
}

Color::Color
Piece::getColor() const
{
	return mColor;
}

int
Piece::getPoints() const
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
Piece::toString() const
{
	std::string res = Color::colorToString( mColor );
	res.append( " " );
	res.append( convertToString( mType ) );
	return res;
}
