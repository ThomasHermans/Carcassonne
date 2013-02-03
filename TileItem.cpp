#include "TileItem.h"

#include "GuiConstants.h"

#include <QGraphicsSceneMouseEvent>

#include <iostream>
#include <sstream>

namespace
{

QPixmap
getPixmapFromId( std::string inId )
{
    std::stringstream sstr;
    sstr << ":/tiles/" << inId << ".png";
    const QPixmap pm(QString::fromStdString(sstr.str()));
    return pm;
}

} // end of nameless namespace

TileItem::TileItem(QGraphicsItem *parent) :
    QGraphicsPixmapItem(parent)
{
}

TileItem::TileItem(std::string inId, unsigned int inRotation, QGraphicsItem *parent) :
    QGraphicsPixmapItem( parent )
{
    QPixmap pixmap = getPixmapFromId( inId );
    QTransform rotation = QTransform();
    rotation.rotate( inRotation );
    pixmap = pixmap.transformed( rotation );
    setPixmap( pixmap );
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
