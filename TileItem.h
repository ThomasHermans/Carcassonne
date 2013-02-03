#ifndef TILEITEM_H
#define TILEITEM_H

#include <QGraphicsPixmapItem>
#include <QGraphicsLayoutItem>

class TileItem : public QGraphicsPixmapItem, public QGraphicsLayoutItem
{
public:
    explicit TileItem(QGraphicsItem *parent = 0);
    explicit TileItem(std::string inId, unsigned int inRotation, QGraphicsItem *parent = 0);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    QSizeF sizeHint( Qt::SizeHint which, const QSizeF & constraint = QSizeF() ) const;
};

#endif // TILEITEM_H
