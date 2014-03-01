#include <test_o_matic.hpp>

#include "CreateBaseGameTiles.h"
#include "Board.h"
#include "Tile.h"
#include "TileOnBoard.h"

using namespace Model;

TEST( "Board constructor and dimension checkers" )
{
	Board const board( 7 );
	Board const other( 8 );

	CHECK( board.getNrOfRows() == 7 );
	CHECK( board.getNrOfCols() == 7 );
	CHECK( other.getNrOfRows() == 9 );
	CHECK( other.getNrOfCols() == 9 );
}

TEST( "Board constructor gives empty board" )
{
	Board const board( 7 );

	CHECK( board.getNrOfTiles() == 0 );
}

TEST( "Board place start tile places one tile" )
{
	Board board( 5 );

	unsigned const place = board.placeStartTile( TileOnBoard( createTileD(), kCw0 ) );

	CHECK( place == 12 );
	CHECK( board.getNrOfTiles() == 1 );
}