#ifndef PLACEDPROJECT_H
#define PLACEDPROJECT_H

#include "Tile.h"

struct PlacedRoad
{
	unsigned col;
	unsigned row;
	FRCArea::RoadArea area;

	PlacedRoad( unsigned inCol, unsigned inRow, FRCArea::RoadArea inRoadArea );
};

bool
operator == ( PlacedRoad const & inLeft, PlacedRoad const & inRight );

bool
operator != ( PlacedRoad const & inLeft, PlacedRoad const & inRight );

bool
operator < ( PlacedRoad const & inLeft, PlacedRoad const & inRight );

PlacedRoad
getNeighbor( PlacedRoad const & inPlacedRoad );

typedef PlacedRoad PlacedCity;

#endif // PLACEDPROJECT_H
