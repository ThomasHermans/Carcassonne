#include <test_o_matic.hpp>

#include "CreateBaseGameTiles.h"
#include "Board.h"
#include "Tile.h"
#include "TileOnBoard.h"

using namespace Model;

TEST( "Board: constructor and dimension getters" )
{
	Board const board( 7 );
	Board const other( 8 );

	CHECK( board.getNrOfRows() == 7 );
	CHECK( board.getNrOfCols() == 7 );
	CHECK( other.getNrOfRows() == 9 );
	CHECK( other.getNrOfCols() == 9 );
}

TEST( "Board: constructor gives empty board" )
{
	Board const board( 7 );

	CHECK( board.getNrOfTiles() == 0 );
}

TEST( "Board: place start tile places one tile" )
{
	Board board( 5 );

	unsigned const place = board.placeStartTile( TileOnBoard( createTileD(), kCw0 ) );

	CHECK( place == 12 );
	CHECK( board.getNrOfTiles() == 1 );
}

TEST( "Board: place start tile on non-empty board is impossible" )
{
	Board board( 5 );

	board.placeStartTile( TileOnBoard( createTileD(), kCw0 ) );
	board.placeStartTile( TileOnBoard( createTileE(), kCw0 ) );

	CHECK( board.getNrOfTiles() == 1 );
}

TEST( "Board: place start tile and other tiles" )
{
	Board board( 5 );

	board.placeStartTile( TileOnBoard( createTileD(), kCw0 ) );
	CHECK( board.placeValidTile( TileOnBoard( createTileE(), kCw270 ), 3, 2 ) );
	CHECK( board.getNrOfTiles() == 2 );
	CHECK( board.placeValidTile( TileOnBoard( createTileV(), kCw0 ), 2, 1 ) );
	CHECK( board.getNrOfTiles() == 3 );
	CHECK( board.placeValidTile( TileOnBoard( createTileB(), kCw180 ), 1, 2 ) );
	CHECK( board.getNrOfTiles() == 4 );
	CHECK( !board.placeValidTile( TileOnBoard( createTileB(), kCw180 ), 1, 2 ) );
	CHECK( board.getNrOfTiles() == 4 );
}

TEST( "Board: check for (im)possible tiles" )
{
	Board board ( 7 );
	board.placeStartTile( TileOnBoard( createTileD(), kCw0 ) );
	board.placeValidTile( TileOnBoard( createTileE(), kCw270 ), 4, 3 );

	CHECK( board.isPossibleTile( createTileA() ) );
	CHECK( !board.isPossibleTile( createTileC() ) );
}