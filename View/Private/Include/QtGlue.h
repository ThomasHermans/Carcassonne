#ifndef VIEW_QTGLUE_THHR_20131124
#define VIEW_QTGLUE_THHR_20131124

#include "View/Typedefs.h"

#include <QColor>
#include <QPixmap>

namespace View
{
	QColor
	toQColor( Color inColor );

	Color
	fromQColor( QColor const & inColor );

	QPixmap
	getEmptyPixmap();
}

#endif