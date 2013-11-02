#ifndef PLACEDPROJECT_H
#define PLACEDPROJECT_H

#include "Area.h"

struct PlacedProject
{
	unsigned col;
	unsigned row;
	Area::Area area;

	PlacedProject( unsigned inCol, unsigned inRow, Area::Area inArea );
};

bool
operator == ( PlacedProject const & inLeft, PlacedProject const & inRight );

bool
operator != ( PlacedProject const & inLeft, PlacedProject const & inRight );

bool
operator < ( PlacedProject const & inLeft, PlacedProject const & inRight );

PlacedProject
getNeighbor( PlacedProject const & inPlacedRoad );

typedef PlacedProject PlacedRoad;
typedef PlacedProject PlacedField;
typedef PlacedProject PlacedCity;

#endif // PLACEDPROJECT_H
