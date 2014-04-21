#include <test_o_matic.hpp>

#include "Tile.h"
#include "CreateTilesAndPieces.h"

namespace
{
	int
	countRoads( Model::Tile inTile, Model::Area::Area inArea )
	{
		int count = 0;
		for (unsigned int i = 0; i < inTile.getContiguousRoads().size(); ++i)
		{
			Model::ContiguousRoad contRoad = inTile.getContiguousRoads()[i];
			for (unsigned int j = 0; j < contRoad.size(); ++j)
			{
				if (contRoad[j] == inArea)
				{
					++count;
				}
			}
		}
		return count;
	}

	int
	countCities( Model::Tile inTile, Model::Area::Area inArea )
	{
		int count = 0;
		for (unsigned int i = 0; i < inTile.getContiguousCities().size(); ++i)
		{
			Model::ContiguousCity contCity = inTile.getContiguousCities()[i];
			for (unsigned int j = 0; j < contCity.size(); ++j)
			{
				if (contCity[j] == inArea)
				{
					++count;
				}
			}
		}
		return count;
	}

	int
	countFields( Model::Tile inTile, Model::Area::Area inFieldArea )
	{
		int count = 0;
		for (unsigned int i = 0; i < inTile.getContiguousFields().size(); ++i)
		{
			Model::ContiguousField contField = inTile.getContiguousFields()[i];
			for (unsigned int j = 0; j < contField.size(); ++j)
			{
				if (contField[j] == inFieldArea)
				{
					++count;
				}
			}
		}
		return count;
	}

	bool
	countInstances
	(
		Model::Tile inTile,
		Model::Area::Area inSide,
		Model::Area::Area inFirst,
		Model::Area::Area inSecond,
		int inRoads,
		int inCities,
		int inFields
	)
	{
		bool res = true;
		if (countRoads(inTile, inSide) != inRoads)
		{
			res = false;
		}
		if (countCities(inTile, inSide) != inCities)
		{
			res = false;
		}
		if (countFields(inTile, inFirst) != inFields)
		{
			res = false;
		}
		if (countFields(inTile, inSecond) != inFields)
		{
			res = false;
		}
		return res;
	}

	bool
	verifyInstanceCounts
	(
		Model::Tile inTile,
		Model::Tile::Side inSide,
		int inRoads,
		int inCities,
		int inFields
	)
	{
		using namespace Model::Area;
		bool res = true;
		if (inTile.getTop() == inSide)
		{
			res = res && countInstances( inTile, kTop, kTopLeft, kTopRight, inRoads, inCities, inFields);
		}
		if (inTile.getRight() == inSide)
		{
			res = res && countInstances( inTile, kRight, kRightTop, kRightBottom, inRoads, inCities, inFields);
		}
		if (inTile.getBottom() == inSide)
		{
			res = res && countInstances( inTile, kBottom, kBottomRight, kBottomLeft, inRoads, inCities, inFields);
		}
		if (inTile.getLeft() == inSide)
		{
			res = res && countInstances( inTile, kLeft, kLeftBottom, kLeftTop, inRoads, inCities, inFields);
		}
		return res;
	}

	bool
	sidesMatch( Model::Tile const & inTile )
	{
		// If an Area is part of a Contiguous Road, then that Side must be a Road
		for ( unsigned i = 0; i < inTile.getContiguousRoads().size(); ++i )
		{
			Model::ContiguousRoad contRoad = inTile.getContiguousRoads()[i];
			for ( unsigned j = 0; j < contRoad.size(); ++j )
			{
				if
				(
					( contRoad[j] == Model::Area::kTop && inTile.getTop() != Model::Tile::kSideRoad )
					||
					( contRoad[j] == Model::Area::kRight && inTile.getRight() != Model::Tile::kSideRoad )
					||
					( contRoad[j] == Model::Area::kBottom && inTile.getBottom() != Model::Tile::kSideRoad )
					||
					( contRoad[j] == Model::Area::kLeft && inTile.getLeft() != Model::Tile::kSideRoad )
				)
				{
					return false;
				}
			}
		}
		// If an Area is part of a Contiguous City, then that Side must be a City
		for ( unsigned i = 0; i < inTile.getContiguousCities().size(); ++i )
		{
			Model::ContiguousCity contCity = inTile.getContiguousCities()[i];
			for ( unsigned j = 0; j < contCity.size(); ++j )
			{
				if
				(
					( contCity[j] == Model::Area::kTop && inTile.getTop() != Model::Tile::kSideCity )
					||
					( contCity[j] == Model::Area::kRight && inTile.getRight() != Model::Tile::kSideCity )
					||
					( contCity[j] == Model::Area::kBottom && inTile.getBottom() != Model::Tile::kSideCity )
					||
					( contCity[j] == Model::Area::kLeft && inTile.getLeft() != Model::Tile::kSideCity )
				)
				{
					return false;
				}
			}
		}
		// If an Area is part of a Contiguous Field, then that Side must be a Field or a Road
		for ( unsigned i = 0; i < inTile.getContiguousFields().size(); ++i )
		{
			Model::ContiguousField contField = inTile.getContiguousFields()[i];
			for ( unsigned j = 0; j < contField.size(); ++j )
			{
				if
				(
					( contField[j] == Model::Area::kTopLeft && !( inTile.getTop() == Model::Tile::kSideField || inTile.getTop() == Model::Tile::kSideRoad ) )
					||
					( contField[j] == Model::Area::kTopRight && !( inTile.getTop() == Model::Tile::kSideField || inTile.getTop() == Model::Tile::kSideRoad ) )
					||
					( contField[j] == Model::Area::kRightTop && !( inTile.getRight() == Model::Tile::kSideField || inTile.getRight() == Model::Tile::kSideRoad ) )
					||
					( contField[j] == Model::Area::kRightBottom && !( inTile.getRight() == Model::Tile::kSideField || inTile.getRight() == Model::Tile::kSideRoad ) )
					||
					( contField[j] == Model::Area::kBottomRight && !( inTile.getBottom() == Model::Tile::kSideField || inTile.getBottom() == Model::Tile::kSideRoad ) )
					||
					( contField[j] == Model::Area::kBottomLeft && !( inTile.getBottom() == Model::Tile::kSideField || inTile.getBottom() == Model::Tile::kSideRoad ) )
					||
					( contField[j] == Model::Area::kLeftBottom && !( inTile.getLeft() == Model::Tile::kSideField || inTile.getLeft() == Model::Tile::kSideRoad ) )
					||
					( contField[j] == Model::Area::kLeftTop && !( inTile.getLeft() == Model::Tile::kSideField || inTile.getLeft() == Model::Tile::kSideRoad ) )
				)
				{
					return false;
				}
			}
		}
		// If a Side is a Road, then
		//  that Area must be in one ContiguousRoad,
		//  those two FieldAreas must each be in one ContiguousField (maybe the same, but maybe not),
		//  that Area must not be in any ContiguousCity
		if ( !verifyInstanceCounts( inTile, Model::Tile::kSideRoad, 1, 0, 1 ) )
		{
			return false;
		}
		// If a Side is a City, then
		//  that Area must be in one ContiguousCity,
		//  that Area must not be in any ContiguousRoad,
		//  those two FieldAreas must not be in any ContiguousField
		if ( !verifyInstanceCounts( inTile, Model::Tile::kSideCity, 0, 1, 0 ) )
		{
			return false;
		}
		// If a Side is a Field, then
		//  those two FieldAreas must be in the same ContiguousField,
		//  that Area must not be in any ContiguousRoad,
		//  that Area must not be in any ContiguousCity
		if ( !verifyInstanceCounts( inTile, Model::Tile::kSideField, 0, 0, 1 ) )
		{
			return false;
		}
		return true;
	}
}

using namespace Model;

TEST( "Tile: emtpy initializer" )
{
	Tile const tile;
	CHECK( tile.getID() == "D" );
}

TEST("Tile: creator via id, check no double assigned sides")
{
	CHECK( sidesMatch( createTileA() ) );
	CHECK( sidesMatch( createTileB() ) );
	CHECK( sidesMatch( createTileC() ) );
	CHECK( sidesMatch( createTileD() ) );
	CHECK( sidesMatch( createTileE() ) );
	CHECK( sidesMatch( createTileF() ) );
	CHECK( sidesMatch( createTileG() ) );
	CHECK( sidesMatch( createTileH() ) );
	CHECK( sidesMatch( createTileI() ) );
	CHECK( sidesMatch( createTileJ() ) );
	CHECK( sidesMatch( createTileK() ) );
	CHECK( sidesMatch( createTileL() ) );
	CHECK( sidesMatch( createTileM() ) );
	CHECK( sidesMatch( createTileN() ) );
	CHECK( sidesMatch( createTileO() ) );
	CHECK( sidesMatch( createTileP() ) );
	CHECK( sidesMatch( createTileQ() ) );
	CHECK( sidesMatch( createTileR() ) );
	CHECK( sidesMatch( createTileS() ) );
	CHECK( sidesMatch( createTileT() ) );
	CHECK( sidesMatch( createTileU() ) );
	CHECK( sidesMatch( createTileV() ) );
	CHECK( sidesMatch( createTileW() ) );
	CHECK( sidesMatch( createTileX() ) );

	CHECK( sidesMatch( createTileEA() ) );
	CHECK( sidesMatch( createTileEB() ) );
	CHECK( sidesMatch( createTileEC() ) );
	CHECK( sidesMatch( createTileED() ) );
	CHECK( sidesMatch( createTileEE() ) );
	CHECK( sidesMatch( createTileEF() ) );
	CHECK( sidesMatch( createTileEG() ) );
	CHECK( sidesMatch( createTileEH() ) );
	CHECK( sidesMatch( createTileEI() ) );
	CHECK( sidesMatch( createTileEJ() ) );
	CHECK( sidesMatch( createTileEK() ) );
	CHECK( sidesMatch( createTileEL() ) );
	CHECK( sidesMatch( createTileEM() ) );
	CHECK( sidesMatch( createTileEN() ) );
	CHECK( sidesMatch( createTileEO() ) );
	CHECK( sidesMatch( createTileEP() ) );
	CHECK( sidesMatch( createTileEQ() ) );
}

TEST( "Tile: check hasInn for several tiles" )
{
	Tile const tileEA = createTileEA();

	CHECK( !tileEA.hasInn( Area::kTop ) );
	CHECK( tileEA.hasInn( Area::kLeft ) );
	CHECK( tileEA.hasInn( Area::kCentral ) );
	CHECK( tileEA.hasInn( Area::kBottom ) );

	Tile const tileEB = createTileEB();

	CHECK( !tileEB.hasInn( Area::kLeft ) );
	CHECK( tileEB.hasInn( Area::kTop ) );
	CHECK( tileEB.hasInn( Area::kCentral ) );
	CHECK( tileEB.hasInn( Area::kBottom ) );

	Tile const tileEC = createTileEC();

	CHECK( !tileEC.hasInn( Area::kLeft ) );
	CHECK( !tileEC.hasInn( Area::kTop ) );
	CHECK( !tileEC.hasInn( Area::kCentral ) );
	CHECK( !tileEC.hasInn( Area::kBottom ) );
	CHECK( tileEC.hasInn( Area::kRight ) );

	Tile const tileEL = createTileEL();

	CHECK( !tileEL.hasInn( Area::kTopLeft ) );
	CHECK( tileEL.hasInn( Area::kRight ) );
	CHECK( tileEL.hasInn( Area::kCentral ) );
	CHECK( tileEL.hasInn( Area::kBottom ) );

	Tile const tileEM = createTileEM();

	CHECK( !tileEM.hasInn( Area::kTopLeft ) );
	CHECK( !tileEM.hasInn( Area::kRight ) );
	CHECK( tileEM.hasInn( Area::kCentral ) );
	CHECK( tileEM.hasInn( Area::kBottom ) );

	Tile const tileEN = createTileEN();

	CHECK( !tileEN.hasInn( Area::kTopLeft ) );
	CHECK( !tileEN.hasInn( Area::kRight ) );
	CHECK( tileEN.hasInn( Area::kCentral ) );
	CHECK( tileEN.hasInn( Area::kBottom ) );
}