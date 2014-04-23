#include "PlacedPiece.h"

#include <cassert>

namespace
{
	bool
	isValid( Model::Piece inPiece, Model::Area::Area inArea )
	{
        switch ( inPiece.getType() )
		{
		case Model::Piece::kFollower:
		case Model::Piece::kLargeFollower:
			return true;
		case Model::Piece::kBuilder:
			return (
				( inArea == Model::Area::kTop )
				|| (inArea == Model::Area::kRight )
				|| (inArea == Model::Area::kBottom )
				|| (inArea == Model::Area::kLeft ) );
		case Model::Piece::kPig:
			return (
				( inArea == Model::Area::kTopLeft )
				|| ( inArea == Model::Area::kTopRight )
				|| ( inArea == Model::Area::kRightTop )
				|| ( inArea == Model::Area::kRightBottom )
				|| ( inArea == Model::Area::kBottomRight )
				|| ( inArea == Model::Area::kBottomLeft )
				|| ( inArea == Model::Area::kLeftBottom )
				|| ( inArea == Model::Area::kLeftTop )
				|| ( inArea == Model::Area::kCentral ) );
		}
		assert( !"Invalid PieceType" );
		return false;
	}
}

Model::PlacedPiece::PlacedPiece( const Piece & inPiece, Area::Area inArea )
:
	mPiece( inPiece ),
	mArea( inArea )
{
	if ( !isValid( inPiece, inArea ) )
	{
		throw std::exception();
	}
}

Model::Piece
Model::PlacedPiece::getPiece() const
{
	return mPiece;
}

Model::Piece::PieceType
Model::PlacedPiece::getType() const
{
	return mPiece.getType();
}

Model::Color::Color
Model::PlacedPiece::getColor() const
{
	return mPiece.getColor();
}

Model::Area::Area
Model::PlacedPiece::getArea() const
{
	return mArea;
}
