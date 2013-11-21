#ifndef TILEITEM_H
#define TILEITEM_H

#include <QGraphicsPixmapItem>

class TileItem : public QGraphicsPixmapItem
{
public:
	explicit TileItem
	(
		QGraphicsItem * inParent = 0
	);
	
	explicit TileItem
	(
		std::string const & inId,
		int inRotation,
		QGraphicsItem * inParent = 0
	);

	void setTile( std::string const & inId, int inRotation );

protected:
	void mousePressEvent( QGraphicsSceneMouseEvent * inEvent );
};

#endif