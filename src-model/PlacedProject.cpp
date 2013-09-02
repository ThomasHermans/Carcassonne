#include "src-model/PlacedProject.h"

#include <cassert>

namespace
{
	FRCArea::CityArea
	oppositeSide( FRCArea::CityArea inCityArea )
	{
		switch ( inCityArea )
		{
		case FRCArea::Top:
			return FRCArea::Bottom;
		case FRCArea::Right:
			return FRCArea::Left;
		case FRCArea::Bottom:
			return FRCArea::Top;
		case FRCArea::Left:
			return FRCArea::Right;
		default:
			return FRCArea::Top;
		}
	}
}

PlacedRoad::PlacedRoad( unsigned inCol, unsigned inRow, FRCArea::RoadArea inRoadArea )
:
	col( inCol ),
	row( inRow ),
	area( inRoadArea )
{}

bool
operator == ( PlacedRoad const & inLeft, PlacedRoad const & inRight )
{
	return
	(
		inLeft.col == inRight.col
		&&
		inLeft.row == inRight.row
		&&
		inLeft.area == inRight.area
	);
}

bool
operator != ( PlacedRoad const & inLeft, PlacedRoad const & inRight )
{
	return !( inLeft == inRight );
}

PlacedRoad
getNeighbor( PlacedRoad const & inPlacedRoad )
{
	switch ( inPlacedRoad.area )
	{
		case FRCArea::Top:
		{
			return PlacedRoad
			(
				inPlacedRoad.col,
				inPlacedRoad.row - 1,
				oppositeSide( inPlacedRoad.area )
			);
		}
		case FRCArea::Right:
		{
			return PlacedRoad
			(
				inPlacedRoad.col + 1,
				inPlacedRoad.row,
				oppositeSide( inPlacedRoad.area )
			);
		}
		case FRCArea::Bottom:
		{
			return PlacedRoad
			(
				inPlacedRoad.col,
				inPlacedRoad.row + 1,
				oppositeSide( inPlacedRoad.area )
			);
		}
		case FRCArea::Left:
		{
			return PlacedRoad
			(
				inPlacedRoad.col - 1,
				inPlacedRoad.row,
				oppositeSide( inPlacedRoad.area )
			);
		}
		default:
		{
			assert( !"Invalid inPlacedRoad given to getNeighbor" );
		}
	}
}