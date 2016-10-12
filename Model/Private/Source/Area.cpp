#include "Model/Area.h"

#include <cassert>

Model::Area::Area
Model::oppositeSide( Area::Area inArea )
{
	switch ( inArea )
	{
	case Area::kTopLeft:
		return Area::kBottomLeft;
	case Area::kTop:
		return Area::kBottom;
	case Area::kTopRight:
		return Area::kBottomRight;
	case Area::kRightTop:
		return Area::kLeftTop;
	case Area::kRight:
		return Area::kLeft;
	case Area::kRightBottom:
		return Area::kLeftBottom;
	case Area::kBottomRight:
		return Area::kTopRight;
	case Area::kBottom:
		return Area::kTop;
	case Area::kBottomLeft:
		return Area::kTopLeft;
	case Area::kLeftBottom:
		return Area::kRightBottom;
	case Area::kLeft:
		return Area::kRight;
	case Area::kLeftTop:
		return Area::kRightTop;
	case Area::kCentral:
		return Area::kCentral;
	}
	assert( !"Invalid Area" );
	return Area::kTop;
}

std::array< Model::Area::Area, 13 >
Model::getAllAreas()
{
	return
	{
		{
			Area::kTopLeft,
			Area::kTop,
			Area::kTopRight,
			Area::kRightTop,
			Area::kRight,
			Area::kRightBottom,
			Area::kBottomRight,
			Area::kBottom,
			Area::kBottomLeft,
			Area::kLeftBottom,
			Area::kLeft,
			Area::kLeftTop,
			Area::kCentral
		}
	};
}