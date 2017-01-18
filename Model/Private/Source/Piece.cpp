#include "Model/Piece.h"

#include "Model/PlacedPiece.h"

#include <cassert>

Model::Piece::Piece()
:
	mType( kFollower ),
	mColor( Color::kRed )
{
}

Model::Piece::Piece( PlacedPiece const & inPlacedPiece )
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

std::size_t
Model::Piece::getWeight() const
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

bool
Model::operator == ( Piece const & inLeft, Piece const & inRight )
{
	return inLeft.getType() == inRight.getType() && inLeft.getColor() == inRight.getColor();
}