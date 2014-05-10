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

View::Color
View::fromQColor( QColor const & inColor )
{
	if ( inColor == Qt::red )
	{
		return kRed;
	}
	else if ( inColor == Qt::green )
	{
		return kGreen;
	}
	else if ( inColor == Qt::blue )
	{
		return kBlue;
	}
	else if ( inColor == Qt::yellow )
	{
		return kYellow;
	}
	else if ( inColor == Qt::black )
	{
		return kBlack;
	}
	else if ( inColor == Qt::gray )
	{
		return kGray;
	}
	assert( !"Invalid View::Color" );
	return kRed;
}

QPixmap
View::getEmptyPixmap()
{
	QPixmap emptyPixmap( 1, 1 );
	emptyPixmap.fill( Qt::transparent );
	return emptyPixmap;
}