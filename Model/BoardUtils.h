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
	class Tile;
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
	 *	Get the tiles that need to be filled to complete the
	 *	specified city.
	 */
	std::vector< Utils::Location >
	getTilesToFillCity
	(
		Board const & inBoard,
		Utils::Location const & inLocation,
		Area::Area inArea
	);

	/**
	 *	Get the tiles that need to be filled to complete the
	 *	specified road.
	 */
	std::vector< Utils::Location >
	getTilesToFillRoad
	(
		Board const & inBoard,
		Utils::Location const & inLocation,
		Area::Area inArea
	);

	/**
	 *	Get the tiles that need to be filled to complete the
	 *	specified cloister.
	 */
	std::vector< Utils::Location >
	getTilesToFillCloister
	(
		Board const & inBoard,
		Utils::Location const & inLocation
	);

	/**
	 *	Get the tiles that need to be filled to complete the
	 *	specified project.
	 */
	std::vector< Utils::Location >
	getTilesToFillProject
	(
		Board const & inBoard,
		Utils::Location const & inLocation,
		Area::Area inArea
	);

	/**
	 *	Does the provided list of tiles contain a tile that
	 *	fits in the specified position?
	 */
	bool
	isFillable
	(
		Board const & inBoard,
		Utils::Location const & inLocationToFill,
		std::vector< Tile > const & inTiles
	);

	/**
	 *	Get all the pieces belonging to the specified color.
	 *	The pieces remain on the board.
	 */
	std::vector< std::pair< Utils::Location, PlacedPiece > >
	getPieces( Board const & inBoard, Color::Color inColor );
}

#endif