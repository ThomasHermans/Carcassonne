#include "PlacedPiece.h"

#include <cassert>

namespace
{
	bool
	isValid( Piece inPiece, Area::Area inArea )
	{
        switch ( inPiece.getType() )
		{
		case Piece::kFollower:
		case Piece::kLargeFollower:
			return true;
		case Piece::kBuilder:
			return (
				( inArea == Area::kTop )
				|| (inArea == Area::kRight )
				|| (inArea == Area::kBottom )
				|| (inArea == Area::kLeft ) );
		case Piece::kPig:
			return (
				( inArea == Area::kTopLeft )
				|| ( inArea == Area::kTopRight )
				|| ( inArea == Area::kRightTop )
				|| ( inArea == Area::kRightBottom )
				|| ( inArea == Area::kBottomRight )
				|| ( inArea == Area::kBottomLeft )
				|| ( inArea == Area::kLeftBottom )
				|| ( inArea == Area::kLeftTop )
				|| ( inArea == Area::kCentral ) );
		}
		assert( !"Invalid PieceType" );
		return false;
	}
}

PlacedPiece::PlacedPiece( const Piece & inPiece, Area::Area inArea )
:
	mPiece( inPiece ),
	mArea( inArea )
{
	if ( !isValid( inPiece, inArea ) )
	{
		throw std::exception();
	}
}

Piece
PlacedPiece::getPiece() const
{
	return mPiece;
}

Piece::PieceType
PlacedPiece::getType() const
{
	return mPiece.getType();
}

Color::Color
PlacedPiece::getColor() const
{
	return mPiece.getColor();
}

Area::Area
PlacedPiece::getArea() const
{
	return mArea;
}

std::string
PlacedPiece::toString() const
{
	std::string res = mPiece.toString();
	res.append(" on ");
	res.append(Area::areaToString(mArea));
	return res;
}
