#include <test_o_matic.hpp>

#include "Model/Private/Include//Tile.h"
#include "Model/Private/Include//CreateBaseGameTiles.h"

namespace
{
int
countRoads(Tile inTile, Area::Area inArea)
{
	int count = 0;
	for (unsigned int i = 0; i < inTile.getContiguousRoads().size(); ++i)
	{
		ContiguousRoad contRoad = inTile.getContiguousRoads()[i];
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
countCities(Tile inTile, Area::Area inArea)
{
	int count = 0;
	for (unsigned int i = 0; i < inTile.getContiguousCities().size(); ++i)
	{
		ContiguousCity contCity = inTile.getContiguousCities()[i];
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
countFields( Tile inTile, Area::Area inFieldArea )
{
	int count = 0;
	for (unsigned int i = 0; i < inTile.getContiguousFields().size(); ++i)
	{
		ContiguousField contField = inTile.getContiguousFields()[i];
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
	Tile inTile,
	Area::Area inSide,
	Area::Area inFirst,
	Area::Area inSecond,
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
verifyInstanceCounts(Tile inTile, Tile::Side inSide, int inRoads, int inCities, int inFields)
{
	bool res = true;
	if (inTile.getTop() == inSide)
	{
		res = res && countInstances(inTile, Area::kTop, Area::kTopLeft, Area::kTopRight, inRoads, inCities, inFields);
	}
	if (inTile.getRight() == inSide)
	{
		res = res && countInstances(inTile, Area::kRight, Area::kRightTop, Area::kRightBottom, inRoads, inCities, inFields);
	}
	if (inTile.getBottom() == inSide)
	{
		res = res && countInstances(inTile, Area::kBottom, Area::kBottomRight, Area::kBottomLeft, inRoads, inCities, inFields);
	}
	if (inTile.getLeft() == inSide)
	{
		res = res && countInstances(inTile, Area::kLeft, Area::kLeftBottom, Area::kLeftTop, inRoads, inCities, inFields);
	}
	return res;
}

bool
sidesMatch( Tile const & inTile )
{
	// If an Area is part of a Contiguous Road, then that Side must be a Road
	for ( unsigned i = 0; i < inTile.getContiguousRoads().size(); ++i )
	{
		ContiguousRoad contRoad = inTile.getContiguousRoads()[i];
		for ( unsigned j = 0; j < contRoad.size(); ++j )
		{
			if
			(
				( contRoad[j] == Area::kTop && inTile.getTop() != Tile::kSideRoad )
				||
				( contRoad[j] == Area::kRight && inTile.getRight() != Tile::kSideRoad )
				||
				( contRoad[j] == Area::kBottom && inTile.getBottom() != Tile::kSideRoad )
				||
				( contRoad[j] == Area::kLeft && inTile.getLeft() != Tile::kSideRoad )
			)
			{
				return false;
			}
		}
	}
	// If an Area is part of a Contiguous City, then that Side must be a City
	for ( unsigned i = 0; i < inTile.getContiguousCities().size(); ++i )
	{
		ContiguousCity contCity = inTile.getContiguousCities()[i];
		for ( unsigned j = 0; j < contCity.size(); ++j )
		{
			if
			(
				( contCity[j] == Area::kTop && inTile.getTop() != Tile::kSideCity )
				||
				( contCity[j] == Area::kRight && inTile.getRight() != Tile::kSideCity )
				||
				( contCity[j] == Area::kBottom && inTile.getBottom() != Tile::kSideCity )
				||
				( contCity[j] == Area::kLeft && inTile.getLeft() != Tile::kSideCity )
			)
			{
				return false;
			}
		}
	}
	// If an Area is part of a Contiguous Field, then that Side must be a Field or a Road
	for ( unsigned i = 0; i < inTile.getContiguousFields().size(); ++i )
	{
		ContiguousField contField = inTile.getContiguousFields()[i];
		for ( unsigned j = 0; j < contField.size(); ++j )
		{
			if
			(
				( contField[j] == Area::kTopLeft && !( inTile.getTop() == Tile::kSideField || inTile.getTop() == Tile::kSideRoad ) )
				||
				( contField[j] == Area::kTopRight && !( inTile.getTop() == Tile::kSideField || inTile.getTop() == Tile::kSideRoad ) )
				||
				( contField[j] == Area::kRightTop && !( inTile.getRight() == Tile::kSideField || inTile.getRight() == Tile::kSideRoad ) )
				||
				( contField[j] == Area::kRightBottom && !( inTile.getRight() == Tile::kSideField || inTile.getRight() == Tile::kSideRoad ) )
				||
				( contField[j] == Area::kBottomRight && !( inTile.getBottom() == Tile::kSideField || inTile.getBottom() == Tile::kSideRoad ) )
				||
				( contField[j] == Area::kBottomLeft && !( inTile.getBottom() == Tile::kSideField || inTile.getBottom() == Tile::kSideRoad ) )
				||
				( contField[j] == Area::kLeftBottom && !( inTile.getLeft() == Tile::kSideField || inTile.getLeft() == Tile::kSideRoad ) )
				||
				( contField[j] == Area::kLeftTop && !( inTile.getLeft() == Tile::kSideField || inTile.getLeft() == Tile::kSideRoad ) )
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
	if ( !verifyInstanceCounts( inTile, Tile::kSideRoad, 1, 0, 1 ) )
	{
		return false;
	}
	// If a Side is a City, then
	//  that Area must be in one ContiguousCity,
	//  that Area must not be in any ContiguousRoad,
	//  those two FieldAreas must not be in any ContiguousField
	if ( !verifyInstanceCounts( inTile, Tile::kSideCity, 0, 1, 0 ) )
	{
		return false;
	}
	// If a Side is a Field, then
	//  those two FieldAreas must be in the same ContiguousField,
	//  that Area must not be in any ContiguousRoad,
	//  that Area must not be in any ContiguousCity
	if ( !verifyInstanceCounts( inTile, Tile::kSideField, 0, 0, 1 ) )
	{
		return false;
	}
	return true;
}
}

using namespace cbgt;

TEST("Tile emtpy initializer")
{
	Tile tile;
	CHECK(tile.getID() == "D");
}

TEST("Tile creator via id, check sides and center")
{
	Tile a = createTileA();
	CHECK(a.getID() == "A");
	CHECK(a.getTop() == Tile::kSideField);
	CHECK(a.getLeft() == Tile::kSideField);
	CHECK(a.getRight() == Tile::kSideField);
	CHECK(a.getBottom() == Tile::kSideRoad);
	CHECK(a.getCenter() == Tile::kCenterCloister);
}

TEST("Tile creator via id, check id")
{
	CHECK(createTileA().getID() == "A");
	CHECK(createTileB().getID() == "B");
	CHECK(createTileC().getID() == "C");
	CHECK(createTileD().getID() == "D");
	CHECK(createTileE().getID() == "E");
	CHECK(createTileF().getID() == "F");
	CHECK(createTileG().getID() == "G");
	CHECK(createTileH().getID() == "H");
	CHECK(createTileI().getID() == "I");
	CHECK(createTileJ().getID() == "J");
	CHECK(createTileK().getID() == "K");
	CHECK(createTileL().getID() == "L");
	CHECK(createTileM().getID() == "M");
	CHECK(createTileN().getID() == "N");
	CHECK(createTileO().getID() == "O");
	CHECK(createTileP().getID() == "P");
	CHECK(createTileQ().getID() == "Q");
	CHECK(createTileR().getID() == "R");
	CHECK(createTileS().getID() == "S");
	CHECK(createTileT().getID() == "T");
	CHECK(createTileU().getID() == "U");
	CHECK(createTileV().getID() == "V");
	CHECK(createTileW().getID() == "W");
	CHECK(createTileX().getID() == "X");
}

TEST("Tile creator via id, check no double assigned sides")
{
	CHECK(sidesMatch(createTileA()));
	CHECK(sidesMatch(createTileB()));
	CHECK(sidesMatch(createTileC()));
	CHECK(sidesMatch(createTileD()));
	CHECK(sidesMatch(createTileE()));
	CHECK(sidesMatch(createTileF()));
	CHECK(sidesMatch(createTileG()));
	CHECK(sidesMatch(createTileH()));
	CHECK(sidesMatch(createTileI()));
	CHECK(sidesMatch(createTileJ()));
	CHECK(sidesMatch(createTileK()));
	CHECK(sidesMatch(createTileL()));
	CHECK(sidesMatch(createTileM()));
	CHECK(sidesMatch(createTileN()));
	CHECK(sidesMatch(createTileO()));
	CHECK(sidesMatch(createTileP()));
	CHECK(sidesMatch(createTileQ()));
	CHECK(sidesMatch(createTileR()));
	CHECK(sidesMatch(createTileS()));
	CHECK(sidesMatch(createTileT()));
	CHECK(sidesMatch(createTileU()));
	CHECK(sidesMatch(createTileV()));
	CHECK(sidesMatch(createTileW()));
	CHECK(sidesMatch(createTileX()));
}
