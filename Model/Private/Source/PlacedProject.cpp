#include "Model/PlacedProject.h"

#include <cassert>

Model::PlacedProject::PlacedProject
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
Model::operator == ( PlacedProject const & inLeft, PlacedProject const & inRight )
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
Model::operator != ( PlacedProject const & inLeft, PlacedProject const & inRight )
{
	return !( inLeft == inRight );
}

bool
Model::operator < ( PlacedProject const & inLeft, PlacedProject const & inRight )
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

Model::PlacedProject
Model::getNeighbor( PlacedProject const & inProject )
{
	switch ( inProject.area )
	{
		case Area::kTopLeft:
		case Area::kTop:
		case Area::kTopRight:
		{
			return PlacedProject
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
			return PlacedProject
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
			return PlacedProject
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
			return PlacedProject
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