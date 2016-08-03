#ifndef PLACEDPROJECT_H
#define PLACEDPROJECT_H

#include "Model/Area.h"

#include "Utils/Location.h"

namespace Model
{
	struct PlacedProject
	{
		int row;
		int col;
		Area::Area area;

		PlacedProject( int inRow, int inCol, Area::Area inArea );
		PlacedProject( Utils::Location const & inLocation, Area::Area inArea );
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
}

#endif // PLACEDPROJECT_H
