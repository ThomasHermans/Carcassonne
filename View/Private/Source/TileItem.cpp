#include "TileItem.h"

#include "TileUtils.h"

#include "View/Typedefs.h"

#include <QGraphicsSceneMouseEvent>

#include <iostream>
#include <sstream>

View::TileItem::TileItem
(
	QGraphicsItem *inParent
)
:
	QGraphicsPixmapItem( inParent )
{
}

View::TileItem::TileItem
(
	std::string const & inId,
	Rotation inRotation,
	QGraphicsItem *inParent
)
:
	QGraphicsPixmapItem( inParent )
{
	setTile( inId, inRotation );
}

void
View::TileItem::setTile( std::string const & inId, Rotation inRotation )
{
	setPixmap( View::getPixmapForTile( inId, inRotation ) );
}

void
View::TileItem::mousePressEvent( QGraphicsSceneMouseEvent * inEvent )
{
	double const x = inEvent->pos().x();
	double const y = inEvent->pos().y();
	std::cout << "TileItem clicked at [" << x << ", " << y << "]" << std::endl;
	double const scenex = scenePos().x();
	double const sceney = scenePos().y();
	std::cout << "TileItem clicked at [" << scenex + x << ", " << sceney + y << "] in the scene." << std::endl;
}
