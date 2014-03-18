#include <test_o_matic.hpp>

#include "CreateBaseGameTiles.h"
#include "NewBoard.h"
#include "Tile.h"
#include "TileOnBoard.h"

using namespace Model;

TEST( "NewBoard: constructor creates an empty board" )
{
	NewBoard const board;
	CHECK( board.getNrOfTiles() == 0 );
	CHECK( !board.isTile( 0, 0 ) );
	CHECK( !board.isTile( 0, 1 ) );
	CHECK( !board.isTile( 1, 0 ) );
	CHECK( !board.isTile( -1, 0 ) );
	CHECK( !board.isTile( 0, -1 ) );
}

TEST( "NewBoard: place a start tile" )
{
	NewBoard board;
	CHECK( board.placeStartTile( TileOnBoard( createTileD(), kCw0 ) ) );
	CHECK( board.getNrOfTiles() == 1 );
	CHECK( board.isTile( 0, 0 ) );
	CHECK( !board.placeStartTile( TileOnBoard( createTileF(), kCw0 ) ) );
}

TEST( "NewBoard: check for valid tile placements" )
{
	NewBoard board;
	TileOnBoard const startTile( createTileD(), kCw0 );
	CHECK( !board.isValidTilePlacement( startTile, 1, 3 ) );
	board.placeStartTile( startTile );
	CHECK( board.isValidTilePlacement( startTile, -1, 0 ) );
	CHECK( board.isValidTilePlacement( startTile, 1, 0 ) );
	CHECK( !board.isValidTilePlacement( startTile, 0, 1 ) );
	CHECK( !board.isValidTilePlacement( startTile, 0, -1 ) );
}

namespace
{
	struct BoardFixture
	{
		NewBoard board;

		BoardFixture()
		: board()
		{
			board.placeStartTile( TileOnBoard( createTileD(), kCw0 ) );
		}
	};
}

TESTFIX( "NewBoard: place extra tiles", BoardFixture )
{
	CHECK( board.placeValidTile( TileOnBoard( createTileI(), kCw180 ), 0, 1 ) );
	CHECK( board.placeValidTile( TileOnBoard( createTileV(), kCw270 ), -1, 0 ) );
	CHECK( !board.placeValidTile( TileOnBoard( createTileB(), kCw0 ), -1, 1 ) );
	CHECK( board.placeValidTile( TileOnBoard( createTileJ(), kCw180 ), -1, 1 ) );
	CHECK( board.getNrOfTiles() == 4 );
}

TESTFIX( "NewBoard: check for occupied roads", BoardFixture )
{
	CHECK( !board.isOccupiedRoad( 0, 0, Area::kTop ) );
	CHECK( !board.isOccupiedRoad( 0, 0, Area::kLeft ) );
	CHECK( !board.isOccupiedRoad( 3, 0, Area::kBottomLeft ) );

	TileOnBoard secondTile( createTileV(), kCw270 );
	secondTile.placePiece( PlacedPiece( Piece( Piece::kFollower, Color::kRed ), Area::kRight ) );
	REQUIRE( board.placeValidTile( secondTile, -1, 0 ) );
	CHECK( board.isOccupiedRoad( -1, 0, Area::kRight ) );
	CHECK( board.isOccupiedRoad( 0, 0, Area::kBottom ) );

	TileOnBoard thirdTile( createTileW(), kCw180 );
	thirdTile.placePiece( PlacedPiece( Piece( Piece::kFollower, Color::kRed ), Area::kRight ) );
	REQUIRE( board.placeValidTile( thirdTile, 1, 0 ) );
	CHECK( board.isOccupiedRoad( 1, 0, Area::kRight ) );
	CHECK( !board.isOccupiedRoad( 1, 0, Area::kLeft ) );
	CHECK( board.isOccupiedRoad( 1, 0, Area::kTop ) );
}