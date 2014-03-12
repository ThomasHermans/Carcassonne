#include <test_o_matic.hpp>

#include "PlacedProject.h"

using namespace Model;

TEST( "PlacedProject: constructor and getters" )
{
	PlacedProject const project( 3, 2, Area::kBottomRight );

	CHECK( project.col == 3 );
	CHECK( project.row == 2 );
	CHECK( project.area == Area::kBottomRight );
}

TEST( "PlacedProject: getNeighbor" )
{
	PlacedRoad const road( 3, 2, Area::kBottomRight );
	PlacedRoad const neighborRoad = getNeighbor( road );
	CHECK( neighborRoad.col == 3 );
	CHECK( neighborRoad.row == 3 );
	CHECK( neighborRoad.area == Area::kTopRight );
	CHECK( road == getNeighbor( neighborRoad ) );

	PlacedCity const city( 4, 6, Area::kRight );
	PlacedCity const neighborCity = getNeighbor( city );
	CHECK( neighborCity.col == 5 );
	CHECK( neighborCity.row == 6 );
	CHECK( neighborCity.area == Area::kLeft );
	CHECK( city == getNeighbor( neighborCity ) );

	PlacedField const field( 2, 2, Area::kTopLeft );
	PlacedField const neighborField = getNeighbor( field );
	CHECK( neighborField.col == 2 );
	CHECK( neighborField.row == 1 );
	CHECK( neighborField.area == Area::kBottomLeft );
	CHECK( field == getNeighbor( neighborField ) );
}