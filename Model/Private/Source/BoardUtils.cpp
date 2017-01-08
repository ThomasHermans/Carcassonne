#include "Model/BoardUtils.h"

#include "Model/Board.h"
#include "Model/PlacedPiece.h"
#include "Model/PlacedProject.h"
#include "Model/Rotation.h"

#include "Utils/Location.h"

std::vector< Model::PlacedProject >
Model::getCompleteProject
(
	Board const & inBoard,
	Utils::Location const & inLocation,
	Area::Area inArea
)
{
	if ( inBoard.isRoad( inLocation, inArea ) )
	{
		return inBoard.getCompleteRoad( inLocation, inArea );
	}
	else if ( inBoard.isCity( inLocation, inArea ) )
	{
		return inBoard.getCompleteCity( inLocation, inArea );
	}
	else if ( inBoard.isField( inLocation, inArea ) )
	{
		return inBoard.getCompleteField( inLocation, inArea );
	}
	else
	{
		return { { inLocation, inArea } };
	}
}

std::vector< Utils::Location >
Model::getTilesToFillCity
(
	Board const & inBoard,
	Utils::Location const & inLocation,
	Area::Area inArea
)
{
	assert( inBoard.isCity( inLocation, inArea ) );
	std::vector< PlacedCity > uncheckedCityParts;
	for ( Area::Area const area : inBoard.getTile( inLocation )->getContiguousCity( inArea ) )
	{
		uncheckedCityParts.emplace_back( inLocation.row, inLocation.col, area );
	}
	std::vector< Utils::Location > missingTiles;
	for ( std::size_t i = 0; i < uncheckedCityParts.size(); ++i )
	{
		PlacedCity const neighbor = getNeighbor( uncheckedCityParts[i] );
		if ( inBoard.isTile( neighbor.row, neighbor.col ) )
		{
			if ( std::find( uncheckedCityParts.begin(), uncheckedCityParts.end(), neighbor ) == uncheckedCityParts.end() )
			{
				for ( Area::Area const area : inBoard.getTile( neighbor.row, neighbor.col )->getContiguousCity( neighbor.area ) )
				{
					uncheckedCityParts.emplace_back( neighbor.row, neighbor.col, area );
				}
			}
		}
		else
		{
			Utils::Location const neighborLocation( neighbor.row, neighbor.col );
			if ( std::find( missingTiles.begin(), missingTiles.end(), neighborLocation ) == missingTiles.end() )
			{
				missingTiles.emplace_back( neighborLocation );
			}
		}
	}
	return missingTiles;
}

std::vector< Utils::Location >
Model::getTilesToFillRoad
(
	Board const & inBoard,
	Utils::Location const & inLocation,
	Area::Area inArea
)
{
	assert( inBoard.isRoad( inLocation, inArea ) );
	std::vector< PlacedRoad > uncheckedRoadParts;
	for ( Area::Area const area : inBoard.getTile( inLocation )->getContiguousRoad( inArea ) )
	{
		uncheckedRoadParts.emplace_back( inLocation.row, inLocation.col, area );
	}
	std::vector< Utils::Location > missingTiles;
	for ( std::size_t i = 0; i < uncheckedRoadParts.size(); ++i )
	{
		PlacedRoad const neighbor = getNeighbor( uncheckedRoadParts[i] );
		if ( inBoard.isTile( neighbor.row, neighbor.col ) )
		{
			if ( std::find( uncheckedRoadParts.begin(), uncheckedRoadParts.end(), neighbor ) == uncheckedRoadParts.end() )
			{
				for ( Area::Area const area : inBoard.getTile( neighbor.row, neighbor.col )->getContiguousRoad( neighbor.area ) )
				{
					uncheckedRoadParts.emplace_back( neighbor.row, neighbor.col, area );
				}
			}
		}
		else
		{
			Utils::Location const neighborLocation( neighbor.row, neighbor.col );
			if ( std::find( missingTiles.begin(), missingTiles.end(), neighborLocation ) == missingTiles.end() )
			{
				missingTiles.emplace_back( neighborLocation );
			}
		}
	}
	return missingTiles;
}

std::vector< Utils::Location >
Model::getTilesToFillCloister
(
	Board const & inBoard,
	Utils::Location const & inLocation
)
{
	std::vector< Utils::Location > missingTiles;
	for ( int row = inLocation.row - 1; row <= inLocation.row + 1; ++row )
	{
		for ( int col = inLocation.col - 1; col <= inLocation.col + 1; ++col )
		{
			if ( !inBoard.isTile( row, col ) )
			{
				missingTiles.emplace_back( Utils::Location( row, col ) );
			}
		}
	}
	return missingTiles;
}

std::vector< Utils::Location >
Model::getTilesToFillProject
(
	Board const & inBoard,
	Utils::Location const & inLocation,
	Area::Area inArea
)
{
	if ( inBoard.isCloister( inLocation, inArea ) )
	{
		return getTilesToFillCloister( inBoard, inLocation );
	}
	else if ( inBoard.isCity( inLocation, inArea ) )
	{
		return getTilesToFillCity( inBoard, inLocation, inArea );
	}
	else if ( inBoard.isRoad( inLocation, inArea ) )
	{
		return getTilesToFillRoad( inBoard, inLocation, inArea );
	}
	return {};
}

bool
Model::isFillable
(
	Board const & inBoard,
	Utils::Location const & inLocationToFill,
	std::vector< Tile > const & inTiles
)
{
	for ( auto const & tile : inTiles )
	{
		for ( auto rotation : getAllRotations() )
		{
			if ( inBoard.isValidTilePlacement( TileOnBoard( tile, rotation ), inLocationToFill ) )
			{
				return true;
			}
		}
	}
	return false;
}

std::vector< std::pair< Utils::Location, Model::PlacedPiece > >
Model::getPieces( Board const & inBoard, Color::Color inColor )
{
	std::vector< std::pair< Utils::Location, PlacedPiece > > allPieces;
	for ( int row = inBoard.getTopRow(); row <= inBoard.getBottomRow(); ++row )
	{
		for ( int col = inBoard.getLeftCol(); col <= inBoard.getRightCol(); ++col )
		{
			if ( inBoard.isTile( row, col ) )
			{
				for ( Area::Area area : getAllAreas() )
				{
					Utils::Location const location( row, col );
					std::vector< PlacedPiece > const pieces = inBoard.getPieces( { location, area } );
					for ( PlacedPiece const & piece : pieces )
					{
						if ( piece.getColor() == inColor )
						{
							allPieces.emplace_back( location, piece );
						}
					}
				}
			}
		}
	}
	return allPieces;
}