#include "Utils/Location.h"

Utils::Location::Location()
:
	row( 0 ),
	col( 0 )
{}

Utils::Location::Location( int inRow, int inCol )
:
	row( inRow ),
	col( inCol )
{}

bool
Utils::operator == ( Location const & inFirst, Location const & inSecond )
{
	return inFirst.row == inSecond.row && inFirst.col == inSecond.col;
}

bool
Utils::operator < ( Location const & inFirst, Location const & inSecond )
{
	if ( inFirst.row != inSecond.row )
	{
		return inFirst.row < inSecond.row;
	}
	return inFirst.col < inSecond.col;
}

std::ostream &
Utils::operator << ( std::ostream & ioStream, Location const & inLocation )
{
	ioStream << "( " << inLocation.row << ", " << inLocation.col << " )";
	return ioStream;
}