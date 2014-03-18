#ifndef PLACEDPROJECT_H
#define PLACEDPROJECT_H

#include "Model/Area.h"

namespace Model
{
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

	struct NewPlacedProject
	{
		int row;
		int col;
		Area::Area area;

		NewPlacedProject( int inRow, int inCol, Area::Area inArea );
	};

	bool
	operator == ( NewPlacedProject const & inLeft, NewPlacedProject const & inRight );

	bool
	operator != ( NewPlacedProject const & inLeft, NewPlacedProject const & inRight );

	bool
	operator < ( NewPlacedProject const & inLeft, NewPlacedProject const & inRight );

	NewPlacedProject
	getNeighbor( NewPlacedProject const & inPlacedRoad );

	typedef NewPlacedProject NewPlacedRoad;
	typedef NewPlacedProject NewPlacedField;
	typedef NewPlacedProject NewPlacedCity;
}

#endif // PLACEDPROJECT_H
