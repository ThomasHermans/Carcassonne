#include "src-view/DragData.h"

DragData::DragData( Dragging::Piece inPiece, Dragging::Color inColor )
:
	QMimeData(),
	mPiece( inPiece ),
	mColor( inColor )
{}

DragData::~DragData()
{}

Dragging::Piece
DragData::getPiece() const
{
	return mPiece;
}

Dragging::Color
DragData::getColor() const
{
	return mColor;
}