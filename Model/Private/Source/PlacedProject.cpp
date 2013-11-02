#include "PlacedProject.h"

#include <cassert>

PlacedProject::PlacedProject( unsigned inCol, unsigned inRow, Area::Area inArea )
:
	col( inCol ),
	row( inRow ),
	area( inArea )
{}

bool
operator == ( PlacedProject const & inLeft, PlacedProject const & inRight )
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
operator != ( PlacedProject const & inLeft, PlacedProject const & inRight )
{
	return !( inLeft == inRight );
}

bool
operator < ( PlacedProject const & inLeft, PlacedProject const & inRight )
{
	return
	(
		inLeft.row < inRight.row
		||
		(
			inLeft.row == inRight.row
			&&
			(
				inLeft.col < inRight.col
				||
				(
					inLeft.col == inRight.col
					&&
					inLeft.area < inRight.area
				)
			)
		)
	);
}

PlacedProject
getNeighbor( PlacedProject const & inProject )
{
	switch ( inProject.area )
	{
		case Area::TopLeft:
		case Area::Top:
		case Area::TopRight:
		{
			return PlacedProject
			(
				inProject.col,
				inProject.row - 1,
				oppositeSide( inProject.area )
			);
		}
		case Area::RightTop:
		case Area::Right:
		case Area::RightBottom:
		{
			return PlacedProject
			(
				inProject.col + 1,
				inProject.row,
				oppositeSide( inProject.area )
			);
		}
		case Area::BottomRight:
		case Area::Bottom:
		case Area::BottomLeft:
		{
			return PlacedProject
			(
				inProject.col,
				inProject.row + 1,
				oppositeSide( inProject.area )
			);
		}
		case Area::LeftBottom:
		case Area::Left:
		case Area::LeftTop:
		{
			return PlacedProject
			(
				inProject.col - 1,
				inProject.row,
				oppositeSide( inProject.area )
			);
		}
		case Area::Central:
		{
			return inProject;
		}
	}
	assert( !"Invalid Area to get neighbor of" );
	return inProject;
}