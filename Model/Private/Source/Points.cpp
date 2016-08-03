#include "Model/Points.h"

#include "Model/Board.h"

#include "Utils/Location.h"

#include <cassert>

namespace
{
	std::size_t
	getPointsPerRoadTile( bool inHasInn, bool inIsFinished )
	{
		return inHasInn ? ( inIsFinished ? 2 : 0 ) : 1;
	}

	std::size_t
	getPointsPerCityTile( bool inHasCathedral, bool inIsFinished )
	{
		return inHasCathedral ? ( inIsFinished ? 3 : 0 ) : ( inIsFinished ? 2 : 1 );
	}

	std::size_t
	getPointsPerCityPennant( bool inHasCathedral, bool inIsFinished )
	{
		return inHasCathedral ? ( inIsFinished ? 3 : 0 ) : ( inIsFinished ? 2 : 1 );
	}

	std::size_t
	getPointsPerCloisterTile()
	{
		return 1;
	}

	std::size_t
	getPointsPerFieldCity()
	{
		return 3;
	}
}

std::size_t
Model::getPointsForRoad
(
	Board const & inBoard,
	Utils::Location const & inLocation,
	Model::Area::Area inArea
)
{
	assert( inBoard.isRoad( inLocation, inArea ) );
	std::set< Utils::Location > usedTiles;
	bool hasInn = false;
	for ( auto const & roadPiece : inBoard.getCompleteRoad( inLocation, inArea ) )
	{
		// Add this tile to the used tiles.
		usedTiles.insert( Utils::Location( roadPiece.row, roadPiece.col ) );
		hasInn = hasInn || inBoard.hasInn( roadPiece );
	}
	bool const isFinished = inBoard.isFinishedRoad( inLocation, inArea );
	return usedTiles.size() * getPointsPerRoadTile( hasInn, isFinished );
}

std::size_t
Model::getPointsForRoadAssumingFinished
(
	Board const & inBoard,
	Utils::Location const & inLocation,
	Model::Area::Area inArea
)
{
	assert( inBoard.isRoad( inLocation, inArea ) );
	std::set< Utils::Location > usedTiles;
	bool hasInn = false;
	for ( auto const & roadPiece : inBoard.getCompleteRoad( inLocation, inArea ) )
	{
		// Add this tile to the used tiles.
		usedTiles.insert( Utils::Location( roadPiece.row, roadPiece.col ) );
		hasInn = hasInn || inBoard.hasInn( roadPiece );
		// Add its neighboring tile, even if it is not (yet) placed.
		PlacedRoad const neighbor = getNeighbor( roadPiece );
		usedTiles.insert( Utils::Location( neighbor.row, neighbor.col ) );
	}
	return usedTiles.size() * getPointsPerRoadTile( hasInn, true );
}

std::size_t
Model::getPointsForCity
(
	Board const & inBoard,
	Utils::Location const & inLocation,
	Model::Area::Area inArea
)
{
	assert( inBoard.isCity( inLocation, inArea ) );
	std::set< Utils::Location > usedTiles;
	std::set< Model::PlacedCity > pennants;
	bool hasCathedral = false;
	for ( auto const & cityPiece : inBoard.getCompleteCity( inLocation, inArea ) )
	{
		// Add this tile to the used tiles.
		usedTiles.insert( Utils::Location( cityPiece.row, cityPiece.col ) );
		hasCathedral = hasCathedral || inBoard.hasCathedral( cityPiece );
		if ( inBoard.hasPennant( cityPiece ) )
		{
			pennants.insert( cityPiece );
		}
	}
	bool const isFinished = inBoard.isFinishedCity( inLocation, inArea );
	return usedTiles.size() * getPointsPerCityTile( hasCathedral, isFinished )
		+ pennants.size() * getPointsPerCityPennant( hasCathedral, isFinished );
}

std::size_t
Model::getPointsForCityAssumingFinished
(
	Board const & inBoard,
	Utils::Location const & inLocation,
	Model::Area::Area inArea
)
{
	assert( inBoard.isCity( inLocation, inArea ) );
	std::set< Utils::Location > usedTiles;
	std::set< Model::PlacedCity > pennants;
	bool hasCathedral = false;
	for ( auto const & cityPiece : inBoard.getCompleteCity( inLocation, inArea ) )
	{
		// Add this tile to the used tiles.
		usedTiles.insert( Utils::Location( cityPiece.row, cityPiece.col ) );
		hasCathedral = hasCathedral || inBoard.hasCathedral( cityPiece );
		// Add its neighboring tile, even if it is not (yet) placed.
		PlacedRoad const neighbor = getNeighbor( cityPiece );
		usedTiles.insert( Utils::Location( neighbor.row, neighbor.col ) );
	}
	return usedTiles.size() * getPointsPerCityTile( hasCathedral, true )
		+ pennants.size() * getPointsPerCityPennant( hasCathedral, true );
}

std::size_t
Model::getPointsForCloister
(
	Board const & inBoard,
	Utils::Location const & inLocation,
	Model::Area::Area inArea
)
{
	assert( inBoard.isCloister( inLocation, inArea ) );
	std::set< Utils::Location > neighbors;
	for ( int row = inLocation.row - 1; row <= inLocation.row + 1; ++row )
	{
		for ( int col = inLocation.col - 1; col <= inLocation.col + 1; ++col )
		{
			if ( inBoard.isTile( row, col ) )
			{
				neighbors.insert( Utils::Location( row, col ) );
			}
		}
	}
	return neighbors.size() * getPointsPerCloisterTile();
}

std::size_t
Model::getPointsForCloisterAssumingFinished
(
	Board const & inBoard,
	Utils::Location const & inLocation,
	Model::Area::Area inArea
)
{
	assert( inBoard.isCloister( inLocation, inArea ) );
	return 9;
}

std::size_t
Model::getPointsForField
(
	Board const & inBoard,
	Utils::Location const & inLocation,
	Model::Area::Area inArea
)
{
	assert( inBoard.isField( inLocation, inArea ) );
	std::vector< Model::PlacedField > const completeField = inBoard.getCompleteField( inLocation, inArea );
	std::set< Model::PlacedCity > finishedCities;
	for ( Model::PlacedField const & field : completeField )
	{
		std::vector< Model::ContiguousCity > const cities = inBoard.getTile( field.row, field.col )->getCitiesPerField( field.area );
		for ( Model::ContiguousCity const & city : cities )
		{
			Model::PlacedCity const cityPart( field.row, field.col, city.front() );
			if ( inBoard.isFinishedCity( cityPart ) )
			{
				finishedCities.insert( inBoard.getIdentifierCity( cityPart ) );
			}
		}
	}
	return finishedCities.size() * getPointsPerFieldCity();
}

std::size_t
Model::getPointsForFieldAssumingFinished
(
	Board const & inBoard,
	Utils::Location const & inLocation,
	Model::Area::Area inArea
)
{
	return getPointsForField( inBoard, inLocation, inArea );
}

std::size_t
Model::getPoints
(
	Board const & inBoard,
	Utils::Location const & inLocation,
	Model::Area::Area inArea
)
{
	if ( inBoard.isRoad( inLocation, inArea ) )
	{
		return getPointsForRoad( inBoard, inLocation, inArea );
	}
	else if ( inBoard.isCity( inLocation, inArea ) )
	{
		return getPointsForCity( inBoard, inLocation, inArea );
	}
	else if ( inBoard.isCloister( inLocation, inArea ) )
	{
		return getPointsForCloister( inBoard, inLocation, inArea );
	}
	else if ( inBoard.isField( inLocation, inArea ) )
	{
		return getPointsForField( inBoard, inLocation, inArea );
	}
	assert( !"Specified location is nothing" );
	return 0;
}

std::size_t
Model::getPointsAssumingFinished
(
	Board const & inBoard,
	Utils::Location const & inLocation,
	Model::Area::Area inArea
)
{
	if ( inBoard.isRoad( inLocation, inArea ) )
	{
		return getPointsForRoadAssumingFinished( inBoard, inLocation, inArea );
	}
	else if ( inBoard.isCity( inLocation, inArea ) )
	{
		return getPointsForCityAssumingFinished( inBoard, inLocation, inArea );
	}
	else if ( inBoard.isCloister( inLocation, inArea ) )
	{
		return getPointsForCloisterAssumingFinished( inBoard, inLocation, inArea );
	}
	else if ( inBoard.isField( inLocation, inArea ) )
	{
		return getPointsForFieldAssumingFinished( inBoard, inLocation, inArea );
	}
	assert( !"Specified location is nothing" );
	return 0;
}