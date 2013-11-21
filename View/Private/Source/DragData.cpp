#include "View/DragData.h"

Dragging::TileData::TileData( std::string const & inTile, View::Rotation inRotation )
:
	QMimeData(),
	mTile( inTile ),
	mRotation( inRotation )
{}

std::string const &
Dragging::TileData::getTile() const
{
	return mTile;
}

View::Rotation
Dragging::TileData::getRotation() const
{
	return mRotation;
}


Dragging::PieceData::PieceData( View::Piece inPiece, View::Color inColor )
:
	QMimeData(),
	mPiece( inPiece ),
	mColor( inColor )
{}

View::Piece
Dragging::PieceData::getPiece() const
{
	return mPiece;
}

View::Color
Dragging::PieceData::getColor() const
{
	return mColor;
}
