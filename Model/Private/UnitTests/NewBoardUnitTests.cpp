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

	class SignalCounter
	{
	public:
		SignalCounter( Model::NewBoard & inBoard )
		:
			mFinishedCityCount( 0 ),
			mFinishedRoadCount( 0 )
		{
			inBoard.finishedCity.connect
			(
				boost::bind( &SignalCounter::IncrementFinishedCity, this )
			);
			inBoard.finishedRoad.connect
			(
				boost::bind( &SignalCounter::IncrementFinishedRoad, this )
			);
		}
		
		void IncrementFinishedCity()
		{
			++mFinishedCityCount;
		}
		
		void IncrementFinishedRoad()
		{
			++mFinishedRoadCount;
		}

		std::size_t GetFinishedCityCount() const
		{
			return mFinishedCityCount;
		}

		std::size_t GetFinishedRoadCount() const
		{
			return mFinishedRoadCount;
		}
	private:
		std::size_t mFinishedCityCount;
		std::size_t mFinishedRoadCount;
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
	CHECK( !board.isOccupiedRoad( NewPlacedRoad( 0, 0, Area::kTop ) ) );
	CHECK( !board.isOccupiedRoad( NewPlacedRoad( 0, 0, Area::kLeft ) ) );
	CHECK( !board.isOccupiedRoad( NewPlacedRoad( 3, 0, Area::kBottomLeft ) ) );

	TileOnBoard tileV( createTileV(), kCw270 );
	tileV.placePiece( PlacedPiece( piece, Area::kRight ) );
	REQUIRE( board.placeValidTile( tileV, -1, 0 ) );
	CHECK( board.isOccupiedRoad( NewPlacedRoad( -1, 0, Area::kRight ) ) );
	CHECK( board.isOccupiedRoad( NewPlacedRoad( 0, 0, Area::kBottom ) ) );

	TileOnBoard tileW( createTileW(), kCw180 );
	tileW.placePiece( PlacedPiece( piece, Area::kRight ) );
	REQUIRE( board.placeValidTile( tileW, 1, 0 ) );
	CHECK( board.isOccupiedRoad( NewPlacedRoad( 1, 0, Area::kRight ) ) );
	CHECK( !board.isOccupiedRoad( NewPlacedRoad( 1, 0, Area::kLeft ) ) );
	CHECK( board.isOccupiedRoad( NewPlacedRoad( 1, 0, Area::kTop ) ) );
}

TESTFIX( "NewBoard: check for occupied cities", BoardFixture )
{
	CHECK( !board.isOccupiedCity( NewPlacedCity( 0, 0, Area::kLeft ) ) );
	CHECK( !board.isOccupiedCity( NewPlacedCity( 0, 0, Area::kCentral ) ) );
	CHECK( !board.isOccupiedCity( NewPlacedCity( 3, 7, Area::kBottomLeft ) ) );

	TileOnBoard tileN( createTileN(), kCw0 );
	tileN.placePiece( PlacedPiece( piece, Area::kLeft ) );
	REQUIRE( board.placeValidTile( tileN, 0, 1 ) );
	CHECK( board.isOccupiedCity( NewPlacedCity( 0, 1, Area::kTop ) ) );
	CHECK( board.isOccupiedCity( NewPlacedCity( 0, 0, Area::kRightBottom ) ) );

	TileOnBoard const secondN( createTileN(), kCw180 );
	TileOnBoard const thirdN( createTileN(), kCw0 );
	REQUIRE( board.placeValidTile( secondN, -1, 1 ) );
	REQUIRE( board.placeValidTile( thirdN, -1, 2 ) );
	CHECK( board.isOccupiedCity( NewPlacedCity( -1, 2, Area::kTopRight ) ) );
}

TESTFIX( "NewBoard: check for occupied fields", BoardFixture )
{
	CHECK( !board.isOccupiedField( NewPlacedField( 0, 0, Area::kLeftTop ) ) );
	CHECK( !board.isOccupiedField( NewPlacedField( 0, 0, Area::kCentral ) ) );
	CHECK( !board.isOccupiedField( NewPlacedField( 3, 7, Area::kBottomLeft ) ) );

	TileOnBoard tileU( createTileU(), kCw0 );
	tileU.placePiece( PlacedPiece( piece, Area::kLeft ) );
	REQUIRE( board.placeValidTile( tileU, 1, 0 ) );
	CHECK( board.isOccupiedField( NewPlacedField( 0, 0, Area::kLeftTop ) ) );
	CHECK( board.isOccupiedField( NewPlacedField( 1, 0, Area::kBottomLeft ) ) );
	CHECK( !board.isOccupiedField( NewPlacedField( 0, 0, Area::kTopRight ) ) );

	TileOnBoard const tileA( createTileA(), kCw180 );
	REQUIRE( board.placeValidTile( tileA, 2, 0 ) );
	CHECK( board.isOccupiedField( NewPlacedField( 0, 0, Area::kTopRight ) ) );
	CHECK( board.isOccupiedField( NewPlacedField( 2, 0, Area::kBottom ) ) );
}

TESTFIX( "NewBoard: remove pieces from a specified tile & area", BoardFixture )
{
	TileOnBoard tileU( createTileU(), kCw0 );
	tileU.placePiece( PlacedPiece( piece, Area::kLeft ) );
	board.placeValidTile( tileU, 1, 0 );
	CHECK( board.isOccupiedField( NewPlacedField( 1, 0, Area::kBottomLeft ) ) );
	std::vector< PlacedPiece > const pieces = board.removePieces( NewPlacedProject( 1, 0, Area::kLeft ) );
	CHECK( pieces.size() == 1 );
	CHECK( !board.isOccupiedField( NewPlacedField( 1, 0, Area::kBottomLeft ) ) );
}

TESTFIX( "NewBoard: signal finishedCity is sent when needed", BoardFixture )
{
	SignalCounter counter( board );
	CHECK( counter.GetFinishedCityCount() == 0 );

	TileOnBoard const tileE( createTileE(), kCw270 );
	board.placeValidTile( tileE, 0, 1 );
	CHECK( counter.GetFinishedCityCount() == 1 );

	TileOnBoard const tileNTopLeft( createTileN(), kCw180 );
	TileOnBoard const tileNTopRight( createTileN(), kCw270 );
	TileOnBoard const tileNBottomRight( createTileN(), kCw0 );
	TileOnBoard const tileNBottomLeft( createTileN(), kCw90 );
	board.placeValidTile( tileNTopLeft, 0, 2 );
	board.placeValidTile( tileNTopRight, 0, 3 );
	board.placeValidTile( tileNBottomRight, 1, 3 );
	CHECK( counter.GetFinishedCityCount() == 1 );
	board.placeValidTile( tileNBottomLeft, 1, 2 );
	CHECK( counter.GetFinishedCityCount() == 2 );
}

TESTFIX( "NewBoard: signal finishedRoad is sent when needed", BoardFixture )
{
	SignalCounter counter( board );
	CHECK( counter.GetFinishedRoadCount() == 0 );

	TileOnBoard const tileA( createTileA(), kCw0 );
	board.placeValidTile( tileA, -1, 0 );
	TileOnBoard const tileV( createTileV(), kCw180 );
	board.placeValidTile( tileV, 1, 0 );
	TileOnBoard const tileS( createTileS(), kCw0 );
	board.placeValidTile( tileS, 0, 1 );
	CHECK( counter.GetFinishedRoadCount() == 0 );

	TileOnBoard const otherV( createTileV(), kCw90 );
	board.placeValidTile( otherV, 1, 1 );
	CHECK( counter.GetFinishedRoadCount() == 1 );
}