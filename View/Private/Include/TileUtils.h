#ifndef VIEW_TILEUTILS_THHR_20131121
#define VIEW_TILEUTILS_THHR_20131121

#include "View/Typedefs.h"

#include <QPixmap>

#include <string>

namespace View
{
	QPixmap
	getPixmapForTile( std::string const & inTileId );

	QPixmap
	getPixmapForTile( std::string const & inTileId, Dragging::Rotation inRotation );

	QPixmap
	getRotatedPixmapForTile( std::string const & inTileId, double inCwAngle );
}

#endif