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