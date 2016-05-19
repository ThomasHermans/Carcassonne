#ifndef CARCASSONNE_UTILS_PRINTUTILS_20160406
#define CARCASSONNE_UTILS_PRINTUTILS_20160406

#include <string>

namespace Model
{
	class Board;
}

namespace Utils
{
	std::string
	printBoard( Model::Board const & inBoard );
}

#endif