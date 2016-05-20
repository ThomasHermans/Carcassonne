#ifndef CARCASSONNE_UTILS_PRINTUTILS_20160406
#define CARCASSONNE_UTILS_PRINTUTILS_20160406

#include <string>
#include <vector>

namespace Model
{
	class Board;
	class Tile;
}

namespace Utils
{
	std::string
	printBoard( Model::Board const & inBoard );

	std::string
	printTiles( std::vector< Model::Tile > const & inTiles );
}

#endif