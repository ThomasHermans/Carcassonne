#include "TileUtils.h"

#include <QString>
#include <QTransform>

#include <cassert>
#include <sstream>

namespace
{
	double
	getAngle( View::Rotation inRotation )
	{
		switch ( inRotation )
		{
		case View::kCw0:
			return 0;
		case View::kCw90:
			return 90;
		case View::kCw180:
			return 180;
		case View::kCw270:
			return 270;
		}
		assert( !"Invalid View::Rotation" );
		return 0;
	}
}

QPixmap
View::getPixmapForTile( std::string const & inTileId )
{
	std::stringstream stream;
	stream << ":/tiles/" << inTileId << ".png";
	return QPixmap( QString::fromStdString( stream.str() ) );
}

QPixmap
View::getPixmapForTile( std::string const & inTileId, View::Rotation inRotation )
{
	QPixmap pixmap = getPixmapForTile( inTileId );
	QTransform rotation = QTransform();
	rotation.rotate( getAngle( inRotation ) );
	return pixmap.transformed( rotation );
}