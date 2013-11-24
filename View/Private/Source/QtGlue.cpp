#include "QtGlue.h"

#include <cassert>

QColor
View::toQColor( Color inColor )
{
	switch ( inColor )
	{
	case kRed:
		return Qt::red;
	case kGreen:
		return Qt::green;
	case kBlue:
		return Qt::blue;
	case kYellow:
		return Qt::yellow;
	case kBlack:
		return Qt::black;
	case kGray:
		return Qt::gray;
	}
	assert( !"Invalid View::Color" );
	return Qt::red;
}