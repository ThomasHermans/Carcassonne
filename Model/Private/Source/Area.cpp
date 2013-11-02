#include "Area.h"

#include <cassert>

std::string
Area::areaToString(Area inArea)
{
	switch (inArea)
	{
	case TopLeft:
		return "AreaTopLeft";
	case TopRight:
		return "AreaTopRight";
	case RightTop:
		return "AreaRightTop";
	case RightBottom:
		return "AreaRightBottom";
	case BottomRight:
		return "AreaBottomRight";
	case BottomLeft:
		return "AreaBottomLeft";
	case LeftBottom:
		return "AreaLeftBottom";
	case LeftTop:
		return "AreaLeftTop";
	case Central:
		return "AreaCentral";
	case Top:
		return "AreaTop";
	case Right:
		return "AreaRight";
	case Bottom:
		return "AreaBottom";
	case Left:
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
	case TopLeft:
		return BottomLeft;
	case Top:
		return Bottom;
	case TopRight:
		return BottomRight;
	case RightTop:
		return LeftTop;
	case Right:
		return Left;
	case RightBottom:
		return LeftBottom;
	case BottomRight:
		return TopRight;
	case Bottom:
		return Top;
	case BottomLeft:
		return TopLeft;
	case LeftBottom:
		return RightBottom;
	case Left:
		return Right;
	case LeftTop:
		return RightTop;
	case Central:
		return Central;
	}
	assert( !"Invalid Area" );
	return Top;
}