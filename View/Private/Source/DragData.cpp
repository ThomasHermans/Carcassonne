#include "View/DragData.h"

Dragging::TileData::TileData( std::string const & inTile, Rotation inRotation )
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

Dragging::Rotation
Dragging::TileData::getRotation() const
{
	return mRotation;
}


Dragging::PieceData::PieceData( Piece inPiece, Color inColor )
:
	QMimeData(),
	mPiece( inPiece ),
	mColor( inColor )
{}

Dragging::Piece
Dragging::PieceData::getPiece() const
{
	return mPiece;
}

Dragging::Color
Dragging::PieceData::getColor() const
{
	return mColor;
}
