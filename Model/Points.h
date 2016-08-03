#ifndef CARCASSONNE_MODEL_POINTS_20160727
#define CARCASSONNE_MODEL_POINTS_20160727

#include "Model/PlacedProject.h"

namespace Model
{
	class Board;

	// Road
	std::size_t
	getPointsForRoad
	(
		Board const & inBoard,
		Utils::Location const & inLocation,
		Model::Area::Area inArea
	);

	std::size_t
	getPointsForRoadAssumingFinished
	(
		Board const & inBoard,
		Utils::Location const & inLocation,
		Model::Area::Area inArea
	);

	// City
	std::size_t
	getPointsForCity
	(
		Board const & inBoard,
		Utils::Location const & inLocation,
		Model::Area::Area inArea
	);

	std::size_t
	getPointsForCityAssumingFinished
	(
		Board const & inBoard,
		Utils::Location const & inLocation,
		Model::Area::Area inArea
	);

	// Cloister
	std::size_t
	getPointsForCloister
	(
		Board const & inBoard,
		Utils::Location const & inLocation,
		Model::Area::Area inArea
	);

	std::size_t
	getPointsForCloisterAssumingFinished
	(
		Board const & inBoard,
		Utils::Location const & inLocation,
		Model::Area::Area inArea
	);

	// Field
	std::size_t
	getPointsForField
	(
		Board const & inBoard,
		Utils::Location const & inLocation,
		Model::Area::Area inArea
	);

	std::size_t
	getPointsForFieldAssumingFinished
	(
		Board const & inBoard,
		Utils::Location const & inLocation,
		Model::Area::Area inArea
	);

	// Anything
	std::size_t
	getPoints
	(
		Board const & inBoard,
		Utils::Location const & inLocation,
		Model::Area::Area inArea
	);

	std::size_t
	getPointsAssumingFinished
	(
		Board const & inBoard,
		Utils::Location const & inLocation,
		Model::Area::Area inArea
	);
}

#endif