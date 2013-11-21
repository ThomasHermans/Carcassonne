#include "TileItem.h"

#include "TileUtils.h"

#include "View/Typedefs.h"

#include <QGraphicsSceneMouseEvent>

#include <iostream>
#include <sstream>

TileItem::TileItem
(
	QGraphicsItem *inParent
)
:
	QGraphicsPixmapItem( inParent )
{
}

TileItem::TileItem
(
	std::string const & inId,
	int inRotation,
	QGraphicsItem *inParent
)
:
	QGraphicsPixmapItem( inParent )
{
	setTile( inId, inRotation );
}

void
TileItem::setTile( std::string const & inId, int inRotation )
{
	setPixmap( View::getRotatedPixmapForTile( inId, inRotation ) );
}

void
TileItem::mousePressEvent( QGraphicsSceneMouseEvent * inEvent )
{
	double const x = inEvent->pos().x();
	double const y = inEvent->pos().y();
	std::cout << "TileItem clicked at [" << x << ", " << y << "]" << std::endl;
	double const scenex = scenePos().x();
	double const sceney = scenePos().y();
	std::cout << "TileItem clicked at [" << scenex + x << ", " << sceney + y << "] in the scene." << std::endl;
}
