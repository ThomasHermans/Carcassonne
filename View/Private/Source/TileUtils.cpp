#include "TileUtils.h"

#include "View/Typedefs.h"

#include <QString>
#include <QTransform>

#include <cassert>
#include <cmath>
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

	QPixmap
	getPixmap( std::string const & inTileId )
	{
		std::stringstream stream;
		stream << ":/tiles/" << inTileId << ".png";
		QPixmap const pixmap( QString::fromStdString( stream.str() ) );
		QTransform scaling;
		double const widthFactor = 1.0 * Gui::kTileWidth / pixmap.width();
		double const heightFactor = 1.0 * Gui::kTileHeight / pixmap.height();
		scaling.scale( widthFactor, heightFactor );
		return pixmap.transformed( scaling );
	}
}

QPixmap
View::getPixmapForTile( std::string const & inTileId, View::Rotation inRotation )
{
	QPixmap const pixmap = getPixmap( inTileId );
	QTransform rotation = QTransform();
	rotation.rotate( getAngle( inRotation ) );
	return pixmap.transformed( rotation );
}

QPainterPath
View::getLastPlacedTilePath()
{
	QPainterPath path;
	static double sInnerBorder = Gui::kTileHeight / 2;
	static double sAddition = .1 * sInnerBorder;
	static double sOuterBorder = sInnerBorder + sAddition;
	path.addRect( 0, 0, 2 * sInnerBorder, 2 * sInnerBorder );
	path.addRoundedRect( sInnerBorder - sOuterBorder, sInnerBorder - sOuterBorder, 2 * sOuterBorder, 2 * sOuterBorder, sAddition, sAddition );
	return path;
}