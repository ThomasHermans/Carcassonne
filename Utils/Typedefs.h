#ifndef CARCASSONNE_UTILS_TYPEDEFS
#define CARCASSONNE_UTILS_TYPEDEFS

#include <set>
#include <utility>

namespace Utils
{
	namespace Expansion
	{
		enum Type
		{
			kBaseGame,
			kTheExpansion
		};
	}

	typedef std::pair< int, int > Location; // row, col
	typedef std::set< Location > Locations;
}

#endif