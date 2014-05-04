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

Dragging::PieceData::PieceData( View::Meeple inMeeple )
:
	QMimeData(),
	mMeeple( inMeeple )
{}

View::Meeple const &
Dragging::PieceData::getMeeple() const
{
	return mMeeple;
}