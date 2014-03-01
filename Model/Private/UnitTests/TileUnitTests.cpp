#include <test_o_matic.hpp>

#include "Tile.h"
#include "CreateBaseGameTiles.h"

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

TEST("Tile emtpy initializer")
{
	Model::Tile tile;
	CHECK(tile.getID() == "D");
}

TEST("Tile creator via id, check sides and center")
{
	Model::Tile a = Model::createTileA();
	CHECK( a.getID() == "A" );
	CHECK( a.getTop() == Model::Tile::kSideField );
	CHECK( a.getLeft() == Model::Tile::kSideField );
	CHECK( a.getRight() == Model::Tile::kSideField );
	CHECK( a.getBottom() == Model::Tile::kSideRoad );
	CHECK( a.getCenter() == Model::Tile::kCenterCloister );
}

TEST("Tile creator via id, check id")
{
	CHECK( Model::createTileA().getID() == "A" );
	CHECK( Model::createTileB().getID() == "B" );
	CHECK( Model::createTileC().getID() == "C" );
	CHECK( Model::createTileD().getID() == "D" );
	CHECK( Model::createTileE().getID() == "E" );
	CHECK( Model::createTileF().getID() == "F" );
	CHECK( Model::createTileG().getID() == "G" );
	CHECK( Model::createTileH().getID() == "H" );
	CHECK( Model::createTileI().getID() == "I" );
	CHECK( Model::createTileJ().getID() == "J" );
	CHECK( Model::createTileK().getID() == "K" );
	CHECK( Model::createTileL().getID() == "L" );
	CHECK( Model::createTileM().getID() == "M" );
	CHECK( Model::createTileN().getID() == "N" );
	CHECK( Model::createTileO().getID() == "O" );
	CHECK( Model::createTileP().getID() == "P" );
	CHECK( Model::createTileQ().getID() == "Q" );
	CHECK( Model::createTileR().getID() == "R" );
	CHECK( Model::createTileS().getID() == "S" );
	CHECK( Model::createTileT().getID() == "T" );
	CHECK( Model::createTileU().getID() == "U" );
	CHECK( Model::createTileV().getID() == "V" );
	CHECK( Model::createTileW().getID() == "W" );
	CHECK( Model::createTileX().getID() == "X" );
}

TEST("Tile creator via id, check no double assigned sides")
{
	CHECK( sidesMatch( Model::createTileA() ) );
	CHECK( sidesMatch( Model::createTileB() ) );
	CHECK( sidesMatch( Model::createTileC() ) );
	CHECK( sidesMatch( Model::createTileD() ) );
	CHECK( sidesMatch( Model::createTileE() ) );
	CHECK( sidesMatch( Model::createTileF() ) );
	CHECK( sidesMatch( Model::createTileG() ) );
	CHECK( sidesMatch( Model::createTileH() ) );
	CHECK( sidesMatch( Model::createTileI() ) );
	CHECK( sidesMatch( Model::createTileJ() ) );
	CHECK( sidesMatch( Model::createTileK() ) );
	CHECK( sidesMatch( Model::createTileL() ) );
	CHECK( sidesMatch( Model::createTileM() ) );
	CHECK( sidesMatch( Model::createTileN() ) );
	CHECK( sidesMatch( Model::createTileO() ) );
	CHECK( sidesMatch( Model::createTileP() ) );
	CHECK( sidesMatch( Model::createTileQ() ) );
	CHECK( sidesMatch( Model::createTileR() ) );
	CHECK( sidesMatch( Model::createTileS() ) );
	CHECK( sidesMatch( Model::createTileT() ) );
	CHECK( sidesMatch( Model::createTileU() ) );
	CHECK( sidesMatch( Model::createTileV() ) );
	CHECK( sidesMatch( Model::createTileW() ) );
	CHECK( sidesMatch( Model::createTileX() ) );
}
