#include <test_o_matic.hpp>

#include "Model/CreateTilesAndPieces.h"
#include "Model/Board.h"
#include "Model/BoardUtils.h"
#include "Model/Tile.h"
#include "Model/TileOnBoard.h"

#include "Utils/Typedefs.h"

#include <utility>

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

TESTFIX( "BoardUtils: isFillable returns the correct result.", BoardFixture )
{
	std::vector< Model::Tile > cloistersAndRoads = { Model::createTileB(), Model::createTileV(), Model::createTileA() };
	CHECK( !isFillable( board, { -1, 1 }, cloistersAndRoads ) );
	CHECK( isFillable( board, { 1, 0 }, cloistersAndRoads ) );
	std::vector< Model::Tile > tiles = { Model::createTileA(), Model::createTileC(), Model::createTileI() };
	CHECK( isFillable( board, { -1, 1 }, tiles ) );
	CHECK( isFillable( board, { 1, 0 }, tiles ) );
}

TESTFIX( "BoardUtils: getPieces returns a list of all the pieces of a color on the board.", BoardFixture )
{
	Model::PlacedPiece const redPieceOne{ Model::Piece{ Model::Piece::kFollower, Model::Color::kRed }, Model::Area::kLeft };
	Utils::Location const locationRPO{ 0, 1 };
	board.placeValidPiece( redPieceOne, locationRPO );
	board.placeValidTile( Model::TileOnBoard{ Model::createTileV(), Model::kCw90 }, { 1, 0 } );
	Model::PlacedPiece const redPieceTwo{ Model::Piece{ Model::Piece::kFollower, Model::Color::kRed }, Model::Area::kTop };
	Utils::Location const locationRPT{ 1, 0 };
	board.placeValidPiece( redPieceTwo, locationRPT );
	board.placeValidTile( Model::TileOnBoard{ Model::createTileB(), Model::kCw0 }, { 1, 1 } );
	Model::PlacedPiece const greenPieceOne{ Model::Piece{ Model::Piece::kFollower, Model::Color::kGreen }, Model::Area::kCentral };
	Utils::Location const locationGPO{ 1, 1 };
	board.placeValidPiece( greenPieceOne, locationGPO );

	auto const redPieces = getPieces( board, Model::Color::kRed );
	CHECK( redPieces.size() == 2 );
	CHECK( std::find( redPieces.cbegin(), redPieces.cend(), std::make_pair( locationRPO, redPieceOne ) ) != redPieces.cend() );
	CHECK( std::find( redPieces.cbegin(), redPieces.cend(), std::make_pair( locationRPT, redPieceTwo ) ) != redPieces.cend() );

	auto const greenPieces = getPieces( board, Model::Color::kGreen );
	CHECK( greenPieces.size() == 1 );
	CHECK( std::find( greenPieces.cbegin(), greenPieces.cend(), std::make_pair( locationGPO, greenPieceOne ) ) != greenPieces.cend() );
}