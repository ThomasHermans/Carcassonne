#ifndef VIEW_MEEPLEUTILS_THHR_20131123
#define VIEW_MEEPLEUTILS_THHR_20131123

#include "View/Typedefs.h"

#include <QPainterPath>
#include <QPixmap>

namespace View
{
	class Meeple;
	
	QPainterPath
	getMeeplePath( int inX, int inY, int inWidth, int inHeight );

	QPixmap
	getMeeplePixmap( Meeple const & inMeeple );
}

#endif