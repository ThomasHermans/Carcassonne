#include "Model/Area.h"

#include <cassert>

std::string
Model::areaToString( Area::Area inArea )
{
	switch ( inArea )
	{
	case Area::kTopLeft:
		return "AreaTopLeft";
	case Area::kTopRight:
		return "AreaTopRight";
	case Area::kRightTop:
		return "AreaRightTop";
	case Area::kRightBottom:
		return "AreaRightBottom";
	case Area::kBottomRight:
		return "AreaBottomRight";
	case Area::kBottomLeft:
		return "AreaBottomLeft";
	case Area::kLeftBottom:
		return "AreaLeftBottom";
	case Area::kLeftTop:
		return "AreaLeftTop";
	case Area::kCentral:
		return "AreaCentral";
	case Area::kTop:
		return "AreaTop";
	case Area::kRight:
		return "AreaRight";
	case Area::kBottom:
		return "AreaBottom";
	case Area::kLeft:
		return "AreaLeft";
	}
	assert( !"Invalid Area" );
	return "Invalid Area";
}

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