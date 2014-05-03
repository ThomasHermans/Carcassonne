#ifndef VIEW_MEEPLEUTILS_THHR_20131123
#define VIEW_MEEPLEUTILS_THHR_20131123

#include "View/Typedefs.h"

#include <QPainterPath>
#include <QPixmap>

namespace View
{
	QPainterPath
	getMeeplePath( int inX, int inY, int inWidth, int inHeight );

	QPixmap
	getMeeplePixmap( Piece inType, Color inColor );
}

#endif