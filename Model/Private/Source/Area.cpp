#include "Area.h"

#include <cassert>

std::string
Area::areaToString( Area inArea )
{
	switch ( inArea )
	{
	case kTopLeft:
		return "AreaTopLeft";
	case kTopRight:
		return "AreaTopRight";
	case kRightTop:
		return "AreaRightTop";
	case kRightBottom:
		return "AreaRightBottom";
	case kBottomRight:
		return "AreaBottomRight";
	case kBottomLeft:
		return "AreaBottomLeft";
	case kLeftBottom:
		return "AreaLeftBottom";
	case kLeftTop:
		return "AreaLeftTop";
	case kCentral:
		return "AreaCentral";
	case kTop:
		return "AreaTop";
	case kRight:
		return "AreaRight";
	case kBottom:
		return "AreaBottom";
	case kLeft:
		return "AreaLeft";
	}
	assert( !"Invalid Area" );
	return "Invalid Area";
}

Area::Area
Area::oppositeSide( Area inArea )
{
	switch ( inArea )
	{
	case kTopLeft:
		return kBottomLeft;
	case kTop:
		return kBottom;
	case kTopRight:
		return kBottomRight;
	case kRightTop:
		return kLeftTop;
	case kRight:
		return kLeft;
	case kRightBottom:
		return kLeftBottom;
	case kBottomRight:
		return kTopRight;
	case kBottom:
		return kTop;
	case kBottomLeft:
		return kTopLeft;
	case kLeftBottom:
		return kRightBottom;
	case kLeft:
		return kRight;
	case kLeftTop:
		return kRightTop;
	case kCentral:
		return kCentral;
	}
	assert( !"Invalid Area" );
	return kTop;
}