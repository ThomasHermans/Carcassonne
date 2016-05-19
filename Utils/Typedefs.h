#ifndef CARCASSONNE_UTILS_TYPEDEFS
#define CARCASSONNE_UTILS_TYPEDEFS

#include <cstdlib>

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

	typedef std::size_t PlayerID;
}

#endif