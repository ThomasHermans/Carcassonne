#include "src-view/TileItem.h"

#include "src-view/GuiConstants.h"

#include <QGraphicsSceneMouseEvent>

#include <iostream>
#include <sstream>

TileItem::TileItem(QGraphicsItem *parent) :
    QGraphicsPixmapItem(parent)
{
}

TileItem::TileItem(QPixmap inPixmap, QGraphicsItem *parent) :
    QGraphicsPixmapItem( parent )
{
    setPixmap( inPixmap );
}

void
TileItem::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
    double x = event->pos().x();
    double y = event->pos().y();
    std::cout << "TileItem clicked at [" << x << ", " << y << "]" << std::endl;
    double scenex = scenePos().x();
    double sceney = scenePos().y();
    std::cout << "TileItem clicked at [" << scenex + x << ", " << sceney + y << "] in the scene." << std::endl;
    // TODO: get position of click.
}
