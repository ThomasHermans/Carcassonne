#include <test_o_matic.hpp>

#include "Model/TileOnBoard.h"
#include "Model/CreateTilesAndPieces.h"

using namespace Model;

TEST( "TileOnBoard: check hasInn" )
{
	TileOnBoard const tileOnBoard( createTileEC(), kCw90 );

	CHECK( tileOnBoard.hasInn( Area::kBottom ) );
	CHECK( !tileOnBoard.hasInn( Area::kRight ) );
}