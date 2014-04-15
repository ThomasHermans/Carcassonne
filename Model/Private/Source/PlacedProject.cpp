#include "PlacedProject.h"

#include <cassert>

Model::NewPlacedProject::NewPlacedProject
(
	int inRow,
	int inCol,
	Area::Area inArea
)
:
	row( inRow ),
	col( inCol ),
	area( inArea )
{}

bool
Model::operator == ( NewPlacedProject const & inLeft, NewPlacedProject const & inRight )
{
	return
	(
		inLeft.row == inRight.row
		&&
		inLeft.col == inRight.col
		&&
		inLeft.area == inRight.area
	);
}

bool
Model::operator != ( NewPlacedProject const & inLeft, NewPlacedProject const & inRight )
{
	return !( inLeft == inRight );
}

bool
Model::operator < ( NewPlacedProject const & inLeft, NewPlacedProject const & inRight )
{
	if ( inLeft.row != inRight.row )
	{
		return inLeft.row < inRight.row;
	}
	else if ( inLeft.col != inRight.col )
	{
		return inLeft.col < inRight.col;
	}
	else
	{
		return inLeft.area < inRight.area;
	}
}

Model::NewPlacedProject
Model::getNeighbor( NewPlacedProject const & inProject )
{
	switch ( inProject.area )
	{
		case Area::kTopLeft:
		case Area::kTop:
		case Area::kTopRight:
		{
			return NewPlacedProject
			(
				inProject.row - 1,
				inProject.col,
				oppositeSide( inProject.area )
			);
		}
		case Area::kRightTop:
		case Area::kRight:
		case Area::kRightBottom:
		{
			return NewPlacedProject
			(
				inProject.row,
				inProject.col + 1,
				oppositeSide( inProject.area )
			);
		}
		case Area::kBottomRight:
		case Area::kBottom:
		case Area::kBottomLeft:
		{
			return NewPlacedProject
			(
				inProject.row + 1,
				inProject.col,
				oppositeSide( inProject.area )
			);
		}
		case Area::kLeftBottom:
		case Area::kLeft:
		case Area::kLeftTop:
		{
			return NewPlacedProject
			(
				inProject.row,
				inProject.col - 1,
				oppositeSide( inProject.area )
			);
		}
		case Area::kCentral:
		{
			return inProject;
		}
	}
	assert( !"Invalid Area to get neighbor of" );
	return inProject;
}