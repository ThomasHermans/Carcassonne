#ifndef UTILS_LOCATION_20160404
#define UTILS_LOCATION_20160404

#include <iostream>
#include <set>

namespace Utils
{
	struct Location
	{
		int row;
		int col;

		Location();
		Location( int inRow, int inCol );
	};

	bool operator == ( Location const & inFirst, Location const & inSecond );
	bool operator < ( Location const & inFirst, Location const & inSecond );
	std::ostream & operator << ( std::ostream & ioStream, Location const & inLocation );

	typedef std::set< Location > Locations;
}

#endif