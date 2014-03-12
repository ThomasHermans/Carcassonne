#include <test_o_matic.hpp>

#include "Model/Area.h"

using namespace Model;

TEST( "Area: calculate the opposite side" )
{
	CHECK( Area::kTopLeft		== oppositeSide( Area::kBottomLeft ) );
	CHECK( Area::kTop			== oppositeSide( Area::kBottom ) );
	CHECK( Area::kTopRight		== oppositeSide( Area::kBottomRight ) );
	CHECK( Area::kRightTop		== oppositeSide( Area::kLeftTop ) );
	CHECK( Area::kRight			== oppositeSide( Area::kLeft ) );
	CHECK( Area::kRightBottom	== oppositeSide( Area::kLeftBottom ) );
	CHECK( Area::kBottomRight	== oppositeSide( Area::kTopRight ) );
	CHECK( Area::kBottom		== oppositeSide( Area::kTop ) );
	CHECK( Area::kBottomLeft	== oppositeSide( Area::kTopLeft ) );
	CHECK( Area::kLeftBottom	== oppositeSide( Area::kRightBottom ) );
	CHECK( Area::kLeft			== oppositeSide( Area::kRight ) );
	CHECK( Area::kLeftTop		== oppositeSide( Area::kRightTop ) );
	CHECK( Area::kCentral		== oppositeSide( Area::kCentral ) );
}