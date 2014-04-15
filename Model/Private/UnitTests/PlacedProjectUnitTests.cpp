#include <test_o_matic.hpp>

#include "PlacedProject.h"

using namespace Model;

TEST( "NewPlacedProject: constructor and getters" )
{
	NewPlacedProject const project( 3, 2, Area::kBottomRight );

	CHECK( project.row == 3 );
	CHECK( project.col == 2 );
	CHECK( project.area == Area::kBottomRight );
}

TEST( "NewPlacedProject: getNeighbor" )
{
	NewPlacedRoad const road( 3, 2, Area::kBottomRight );
	NewPlacedRoad const neighborRoad = getNeighbor( road );
	CHECK( neighborRoad.row == 4 );
	CHECK( neighborRoad.col == 2 );
	CHECK( neighborRoad.area == Area::kTopRight );
	CHECK( road == getNeighbor( neighborRoad ) );

	NewPlacedCity const city( 4, 6, Area::kRight );
	NewPlacedCity const neighborCity = getNeighbor( city );
	CHECK( neighborCity.row == 4 );
	CHECK( neighborCity.col == 7 );
	CHECK( neighborCity.area == Area::kLeft );
	CHECK( city == getNeighbor( neighborCity ) );

	NewPlacedField const field( 0, 0, Area::kTopLeft );
	NewPlacedField const neighborField = getNeighbor( field );
	CHECK( neighborField.row == -1 );
	CHECK( neighborField.col == 0 );
	CHECK( neighborField.area == Area::kBottomLeft );
	CHECK( field == getNeighbor( neighborField ) );
}