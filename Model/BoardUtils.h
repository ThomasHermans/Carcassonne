#ifndef CARCASSONNE_MODEL_BOARDUTILS_20170108
#define CARCASSONNE_MODEL_BOARDUTILS_20170108

#include "Model/Area.h"
#include "Model/Color.h"

#include <utility>
#include <vector>

namespace Utils
{
	struct Location;
}

namespace Model
{
	class Board;
	class PlacedPiece;
	struct PlacedProject;

	/**
	 *	Get the complete project that contains { inLocation, inArea }.
	 */
	std::vector< PlacedProject >
	getCompleteProject
	(
		Board const & inBoard,
		Utils::Location const & inLocation,
		Area::Area inArea
	);

	/**
	 *	Get all the pieces belonging to the specified color.
	 *	The pieces remain on the board.
	 */
	std::vector< std::pair< Utils::Location, PlacedPiece > >
	getPieces( Board const & inBoard, Color::Color inColor );
}

#endif