#include <test_o_matic.hpp>

#include "Model/CreateTilesAndPieces.h"
#include "Model/Board.h"
#include "Model/BoardUtils.h"
#include "Model/Tile.h"
#include "Model/TileOnBoard.h"

#include "Utils/Typedefs.h"

namespace
{
	struct BoardFixture
	{
		Model::Board board;

		BoardFixture()
		:
			board()
		{
			board.placeStartTile( Model::TileOnBoard( Model::createTileD(), Model::kCw0 ) );
			board.placeValidTile( Model::TileOnBoard( Model::createTileN(), Model::kCw0 ), { 0, 1 } );
		}
	};
}

TESTFIX( "BoardUtils: getCompleteProject returns the complete project.", BoardFixture )
{
	CHECK( board.getNrOfTiles() == 2 );
	auto const completeProject = getCompleteProject( board, { 0, 0 }, Model::Area::kRightTop );
	CHECK( completeProject.size() == 9 );
}

TESTFIX( "BoardUtils: getTilesToFillCity returns the correct result.", BoardFixture )
{
	auto const missingTiles = getTilesToFillCity( board, { 0, 0 }, Model::Area::kRightTop );
	REQUIRE( missingTiles.size() == 1 );
	CHECK( missingTiles[0] == Utils::Location{ -1, 1 } );
}

TESTFIX( "BoardUtils: getTilesToFillCity returns the correct result. 2", BoardFixture )
{
	board.placeValidTile( Model::TileOnBoard( Model::createTileS(), Model::kCw180 ), { -1, 1 } );
	auto const missingTiles = getTilesToFillCity( board, { 0, 0 }, Model::Area::kRightTop );
	REQUIRE( missingTiles.size() == 2 );
	CHECK( std::find( missingTiles.cbegin(), missingTiles.cend(), Utils::Location{ -1, 0 } ) != missingTiles.cend() );
	CHECK( std::find( missingTiles.cbegin(), missingTiles.cend(), Utils::Location{ -1, 2 } ) != missingTiles.cend() );
}

TESTFIX( "BoardUtils: getTilesToFillRoad returns the correct result.", BoardFixture )
{
	auto const missingTiles = getTilesToFillRoad( board, { 0, 0 }, Model::Area::kTop );
	REQUIRE( missingTiles.size() == 2 );
	CHECK( std::find( missingTiles.cbegin(), missingTiles.cend(), Utils::Location{ -1, 0 } ) != missingTiles.cend() );
	CHECK( std::find( missingTiles.cbegin(), missingTiles.cend(), Utils::Location{ 1, 0 } ) != missingTiles.cend() );
}

TESTFIX( "BoardUtils: getTilesToFillCloister returns the correct result.", BoardFixture )
{
	board.placeValidTile( Model::TileOnBoard( Model::createTileB(), Model::kCw0 ), { 0, -1 } );
	auto const missingTiles = getTilesToFillCloister( board, { 0, -1 } );
	REQUIRE( missingTiles.size() == 7 );
	std::vector< Utils::Location > const locations = { { -1, -2 }, { -1, -1 }, { -1, 0 }, { 0, -2 }, { 1, -2 }, { 1, -1 }, { 1, 0 } };
	for ( Utils::Location location : locations )
	{
		CHECK( std::find( missingTiles.cbegin(), missingTiles.cend(), location ) != missingTiles.cend() );
	}
}