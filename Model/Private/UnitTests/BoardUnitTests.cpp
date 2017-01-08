#include <test_o_matic.hpp>

#include "Model/CreateTilesAndPieces.h"
#include "Model/Board.h"
#include "Model/Tile.h"
#include "Model/TileOnBoard.h"

#include "Utils/Typedefs.h"

using namespace Model;

TEST( "Board: constructor creates an empty board" )
{
	Board const board;
	CHECK( board.getNrOfTiles() == 0 );
	CHECK( !board.isTile( 0, 0 ) );
	CHECK( !board.isTile( 0, 1 ) );
	CHECK( !board.isTile( 1, 0 ) );
	CHECK( !board.isTile( -1, 0 ) );
	CHECK( !board.isTile( 0, -1 ) );
}

TEST( "Board: place a start tile" )
{
	Board board;
	CHECK( board.placeStartTile( TileOnBoard( createTileD(), kCw0 ) ) );
	CHECK( board.getNrOfTiles() == 1 );
	CHECK( board.isTile( 0, 0 ) );
	CHECK( !board.placeStartTile( TileOnBoard( createTileF(), kCw0 ) ) );
}

TEST( "Board: check for valid tile placements" )
{
	Board board;
	TileOnBoard const startTile( createTileD(), kCw0 );
	CHECK( !board.isValidTilePlacement( startTile, 1, 3 ) );
	board.placeStartTile( startTile );
	CHECK( board.isValidTilePlacement( startTile, -1, 0 ) );
	CHECK( board.isValidTilePlacement( startTile, 1, 0 ) );
	CHECK( !board.isValidTilePlacement( startTile, 0, 1 ) );
	CHECK( !board.isValidTilePlacement( startTile, 0, -1 ) );
}

TEST( "Board: check for possible tile" )
{
	Board board;
	TileOnBoard const startTile( createTileD(), kCw0 );
	board.placeStartTile( startTile );
	CHECK( board.isPossibleTile( createTileE() ) );
}

namespace
{
	struct BoardFixture
	{
		Board board;
		TileOnBoard startTile;
		Piece piece;

		BoardFixture()
		:
			board(),
			startTile( createTileD(), kCw0 ),
			piece( Piece::kFollower, Color::kRed )
		{
			board.placeStartTile( startTile );
		}
	};
}

TESTFIX( "Board: place extra tiles", BoardFixture )
{
	CHECK( board.placeValidTile( TileOnBoard( createTileI(), kCw180 ), 0, 1 ) );
	CHECK( board.placeValidTile( TileOnBoard( createTileV(), kCw270 ), -1, 0 ) );
	CHECK( !board.placeValidTile( TileOnBoard( createTileB(), kCw0 ), -1, 1 ) );
	CHECK( board.placeValidTile( TileOnBoard( createTileJ(), kCw180 ), -1, 1 ) );
	CHECK( board.getNrOfTiles() == 4 );
	CHECK( board.getTopRow() == -1 );
	CHECK( board.getBottomRow() == 0 );
	CHECK( board.getLeftCol() == 0 );
	CHECK( board.getRightCol() == 1 );
}

TESTFIX( "Board: check for occupied roads", BoardFixture )
{
	CHECK( !board.isOccupiedRoad( PlacedRoad( 0, 0, Area::kTop ) ) );
	CHECK( !board.isOccupiedRoad( PlacedRoad( 0, 0, Area::kLeft ) ) );
	CHECK( !board.isOccupiedRoad( PlacedRoad( 3, 0, Area::kBottomLeft ) ) );

	TileOnBoard tileV( createTileV(), kCw270 );
	tileV.placePiece( PlacedPiece( piece, Area::kRight ) );
	REQUIRE( board.placeValidTile( tileV, -1, 0 ) );
	CHECK( board.isOccupiedRoad( PlacedRoad( -1, 0, Area::kRight ) ) );
	CHECK( board.isOccupiedRoad( PlacedRoad( 0, 0, Area::kBottom ) ) );

	TileOnBoard tileW( createTileW(), kCw180 );
	tileW.placePiece( PlacedPiece( piece, Area::kRight ) );
	REQUIRE( board.placeValidTile( tileW, 1, 0 ) );
	CHECK( board.isOccupiedRoad( PlacedRoad( 1, 0, Area::kRight ) ) );
	CHECK( !board.isOccupiedRoad( PlacedRoad( 1, 0, Area::kLeft ) ) );
	CHECK( board.isOccupiedRoad( PlacedRoad( 1, 0, Area::kTop ) ) );
}

TESTFIX( "Board: check for occupied cities", BoardFixture )
{
	CHECK( !board.isOccupiedCity( PlacedCity( 0, 0, Area::kLeft ) ) );
	CHECK( !board.isOccupiedCity( PlacedCity( 0, 0, Area::kCentral ) ) );
	CHECK( !board.isOccupiedCity( PlacedCity( 3, 7, Area::kBottomLeft ) ) );

	TileOnBoard tileN( createTileN(), kCw0 );
	tileN.placePiece( PlacedPiece( piece, Area::kLeft ) );
	REQUIRE( board.placeValidTile( tileN, 0, 1 ) );
	CHECK( board.isOccupiedCity( PlacedCity( 0, 1, Area::kTop ) ) );
	CHECK( board.isOccupiedCity( PlacedCity( 0, 0, Area::kRightBottom ) ) );

	TileOnBoard const secondN( createTileN(), kCw180 );
	TileOnBoard const thirdN( createTileN(), kCw0 );
	REQUIRE( board.placeValidTile( secondN, -1, 1 ) );
	REQUIRE( board.placeValidTile( thirdN, -1, 2 ) );
	CHECK( board.isOccupiedCity( PlacedCity( -1, 2, Area::kTopRight ) ) );
}

TESTFIX( "Board: check for occupied fields", BoardFixture )
{
	CHECK( !board.isOccupiedField( PlacedField( 0, 0, Area::kLeftTop ) ) );
	CHECK( !board.isOccupiedField( PlacedField( 0, 0, Area::kCentral ) ) );
	CHECK( !board.isOccupiedField( PlacedField( 3, 7, Area::kBottomLeft ) ) );

	TileOnBoard tileU( createTileU(), kCw0 );
	tileU.placePiece( PlacedPiece( piece, Area::kLeft ) );
	REQUIRE( board.placeValidTile( tileU, 1, 0 ) );
	CHECK( board.isOccupiedField( PlacedField( 0, 0, Area::kLeftTop ) ) );
	CHECK( board.isOccupiedField( PlacedField( 1, 0, Area::kBottomLeft ) ) );
	CHECK( !board.isOccupiedField( PlacedField( 0, 0, Area::kTopRight ) ) );

	TileOnBoard const tileA( createTileA(), kCw180 );
	REQUIRE( board.placeValidTile( tileA, 2, 0 ) );
	CHECK( board.isOccupiedField( PlacedField( 0, 0, Area::kTopRight ) ) );
	CHECK( board.isOccupiedField( PlacedField( 2, 0, Area::kBottom ) ) );
}

TESTFIX( "Board: remove pieces from a specified tile & area", BoardFixture )
{
	TileOnBoard tileU( createTileU(), kCw0 );
	tileU.placePiece( PlacedPiece( piece, Area::kLeft ) );
	board.placeValidTile( tileU, 1, 0 );
	CHECK( board.isOccupiedField( PlacedField( 1, 0, Area::kBottomLeft ) ) );
	std::vector< PlacedPiece > const pieces = board.removePieces( PlacedProject( 1, 0, Area::kLeft ) );
	CHECK( pieces.size() == 1 );
	CHECK( !board.isOccupiedField( PlacedField( 1, 0, Area::kBottomLeft ) ) );
}

TESTFIX( "Board: simple isFinishedCity", BoardFixture )
{
	CHECK( !board.isFinishedCity( PlacedCity( 0, 0, Area::kTop ) ) );
	CHECK( !board.isFinishedCity( PlacedCity( 0, 0, Area::kLeft ) ) );
	CHECK( !board.isFinishedCity( PlacedCity( 0, 0, Area::kRight ) ) );

	TileOnBoard const tileE( createTileE(), kCw270 );
	board.placeValidTile( tileE, 0, 1 );
	CHECK( board.isFinishedCity( PlacedCity( 0, 0, Area::kRight ) ) );
	CHECK( board.isFinishedCity( PlacedCity( 0, 1, Area::kLeftTop ) ) );
}

TESTFIX( "Board: extended isFinishedCity", BoardFixture )
{
	TileOnBoard const tileN( createTileN(), kCw0 );
	board.placeValidTile( tileN, 0, 1 );
	TileOnBoard const otherN( createTileN(), kCw270 );
	board.placeValidTile( otherN, -1, 1 );
	TileOnBoard const tileT( createTileT(), kCw0 );
	board.placeValidTile( tileT, -1, 0 );
	TileOnBoard const tileH( createTileH(), kCw0 );
	board.placeValidTile( tileH, -1, -1 );
	TileOnBoard const tileG( createTileG(), kCw0 );
	board.placeValidTile( tileG, -2, 0 );
	CHECK( !board.isFinishedCity( PlacedCity( -2, 0, Area::kCentral ) ) );
	TileOnBoard const tileE( createTileE(), kCw180 );
	board.placeValidTile( tileE, -3, 0 );
	CHECK( board.isFinishedCity( PlacedCity( 0, 0, Area::kRight ) ) );
	CHECK( board.isFinishedCity( PlacedCity( -2, 0, Area::kCentral ) ) );
}

TESTFIX( "Board: getIdentifierCity", BoardFixture )
{
	PlacedCity const originTop( 0, 0, Area::kRightTop );
	CHECK( board.getIdentifierCity( PlacedCity( 0, 0, Area::kRightBottom ) ) == originTop );
	board.placeValidTile( TileOnBoard( createTileG(), kCw90 ), 0, 1 );
	CHECK( board.getIdentifierCity( PlacedCity( 0, 1, Area::kCentral ) ) == originTop );
	board.placeValidTile( TileOnBoard( createTileN(), kCw0 ), 0, 2 );
	board.placeValidTile( TileOnBoard( createTileG(), kCw0 ), -1, 2 );
	PlacedCity const top( -1, 2, Area::kTopLeft );
	CHECK( board.getIdentifierCity( PlacedCity( 0, 0, Area::kRight ) ) == top );
	CHECK( board.getIdentifierCity( PlacedCity( 0, 2, Area::kLeft ) ) == top );
}

TESTFIX( "Board: getNrOfSurroundingTiles", BoardFixture )
{
	CHECK( board.getNrOfSurroundingTiles( 0, 0 ) == 1 );
	CHECK( board.getNrOfSurroundingTiles( 0, 1 ) == 0 );
	board.placeValidTile( TileOnBoard( createTileG(), kCw90 ), 0, 1 );
	board.placeValidTile( TileOnBoard( createTileT(), kCw0 ), -1, 0 );
	board.placeValidTile( TileOnBoard( createTileH(), kCw0 ), -1, -1 );
	CHECK( board.getNrOfSurroundingTiles( 0, 0 ) == 4 );
}

TESTFIX( "Board: getCompleteCity", BoardFixture )
{
	CHECK( board.getCompleteCity( PlacedCity( 0, 0, Area::kRight ) ).size() == 3 );
	board.placeValidTile( TileOnBoard( createTileT(), kCw0 ), 0, 1 );
	CHECK( board.getCompleteCity( PlacedCity( 0, 0, Area::kRight ) ).size() == 13 );
}

TESTFIX( "Board: getCompleteRoad", BoardFixture )
{
	CHECK( board.getCompleteRoad( PlacedRoad( 0, 0, Area::kCentral ) ).size() == 3 );
	board.placeValidTile( TileOnBoard( createTileK(), kCw0 ), 1, 0 );
	CHECK( board.getCompleteRoad( PlacedRoad( 0, 0, Area::kTop ) ).size() == 6 );
	board.placeValidTile( TileOnBoard( createTileW(), kCw0 ), 1, -1 );
	CHECK( board.getCompleteRoad( PlacedRoad( 0, 0, Area::kTop ) ).size() == 7 );
}

TESTFIX( "Board: getCompleteField", BoardFixture )
{
	CHECK( board.getCompleteField( PlacedField( 0, 0, Area::kLeft ) ).size() == 5 );
	CHECK( board.getCompleteField( PlacedField( 0, 0, Area::kTopRight ) ).size() == 2 );
	board.placeValidTile( TileOnBoard( createTileK(), kCw0 ), 1, 0 );
	CHECK( board.getCompleteField( PlacedField( 0, 0, Area::kTopLeft ) ).size() == 7 );
	CHECK( board.getCompleteField( PlacedField( 0, 0, Area::kTopRight ) ).size() == 7 );
	board.placeValidTile( TileOnBoard( createTileW(), kCw0 ), 1, -1 );
	CHECK( board.getCompleteField( PlacedField( 0, 0, Area::kTopLeft ) ).size() == 12 );
	CHECK( board.getCompleteField( PlacedField( 0, 0, Area::kTopRight ) ).size() == 9 );
}

TESTFIX( "Board: getPossibleLocations", BoardFixture )
{
	Utils::Locations expectedLocations;
	expectedLocations.insert( Utils::Location( -1, 0 ) );
	expectedLocations.insert( Utils::Location( 0, -1 ) );
	expectedLocations.insert( Utils::Location( 1, 0 ) );

	Utils::Locations locations = board.getPossibleLocations( createTileU() );
	CHECK( locations == expectedLocations );

	board.placeValidTile( TileOnBoard( createTileO(), kCw0 ), -1, 0 );
	expectedLocations.clear();
	expectedLocations.insert( Utils::Location( -2, 0 ) );
	expectedLocations.insert( Utils::Location( -1, -1 ) );
	expectedLocations.insert( Utils::Location( 0, 1 ) );

	locations = board.getPossibleLocations( createTileC() );
	CHECK( locations == expectedLocations );

	expectedLocations.clear();
	expectedLocations.insert( Utils::Location( 0, -1 ) );

	locations = board.getPossibleLocations( createTileB() );
	CHECK( locations == expectedLocations );
}