#ifndef TILEITEM_H
#define TILEITEM_H

#include <QGraphicsPixmapItem>

class TileItem : public QGraphicsPixmapItem
{
public:
    explicit TileItem(QGraphicsItem *parent = 0);
    explicit TileItem(std::string inId, unsigned int inRotation, QGraphicsItem *parent = 0);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // TILEITEM_H
