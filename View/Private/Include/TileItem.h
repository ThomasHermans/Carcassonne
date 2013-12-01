#ifndef TILEITEM_H
#define TILEITEM_H

#include "View/Typedefs.h"

#include <QGraphicsPixmapItem>

namespace View
{
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
			Rotation inRotation,
			QGraphicsItem * inParent = 0
		);

		void setTile( std::string const & inId, Rotation inRotation );

	protected:
		void mousePressEvent( QGraphicsSceneMouseEvent * inEvent );
	};
}

#endif