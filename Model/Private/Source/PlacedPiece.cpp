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
				( inArea == Area::Top )
				|| (inArea == Area::Right )
				|| (inArea == Area::Bottom )
				|| (inArea == Area::Left ) );
		case Piece::kPig:
			return (
				( inArea == Area::TopLeft )
				|| ( inArea == Area::TopRight )
				|| ( inArea == Area::RightTop )
				|| ( inArea == Area::RightBottom )
				|| ( inArea == Area::BottomRight )
				|| ( inArea == Area::BottomLeft )
				|| ( inArea == Area::LeftBottom )
				|| ( inArea == Area::LeftTop )
				|| ( inArea == Area::Central ) );
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
