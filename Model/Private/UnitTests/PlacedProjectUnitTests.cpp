#include <test_o_matic.hpp>

#include "Model/PlacedProject.h"

using namespace Model;

TEST( "PlacedProject: constructor and getters" )
{
	PlacedProject const project( 3, 2, Area::kBottomRight );

	CHECK( project.row == 3 );
	CHECK( project.col == 2 );
	CHECK( project.area == Area::kBottomRight );
}

TEST( "PlacedProject: getNeighbor" )
{
	PlacedRoad const road( 3, 2, Area::kBottomRight );
	PlacedRoad const neighborRoad = getNeighbor( road );
	CHECK( neighborRoad.row == 4 );
	CHECK( neighborRoad.col == 2 );
	CHECK( neighborRoad.area == Area::kTopRight );
	CHECK( road == getNeighbor( neighborRoad ) );

	PlacedCity const city( 4, 6, Area::kRight );
	PlacedCity const neighborCity = getNeighbor( city );
	CHECK( neighborCity.row == 4 );
	CHECK( neighborCity.col == 7 );
	CHECK( neighborCity.area == Area::kLeft );
	CHECK( city == getNeighbor( neighborCity ) );

	PlacedField const field( 0, 0, Area::kTopLeft );
	PlacedField const neighborField = getNeighbor( field );
	CHECK( neighborField.row == -1 );
	CHECK( neighborField.col == 0 );
	CHECK( neighborField.area == Area::kBottomLeft );
	CHECK( field == getNeighbor( neighborField ) );
}