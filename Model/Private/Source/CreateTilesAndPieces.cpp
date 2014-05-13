#include "CreateTilesAndPieces.h"

#include <algorithm>
#include <cassert>
#include <ctime>

namespace
{
	std::size_t
	getLength( std::string inString, std::size_t inStartPos )
	{
		std::size_t const size = inString.size();
		for ( std::size_t length = 0; inStartPos + length < size; ++length )
		{
			char c = inString[ inStartPos + length ];
			if ( c >= 'A' && c <= 'Z' )
			{
				return length + 1;
			}
		}
		assert( !"No capital found after given start position!" );
		return 0;
	}

	Model::Tile
	createTile( std::string const & inId )
	{
		using namespace Model;
		if ( inId == "A" )
		{
			return createTileA();
		}
		else if ( inId == "B" )
		{
			return createTileB();
		}
		else if ( inId == "C" )
		{
			return createTileC();
		}
		else if ( inId == "D" )
		{
			return createTileD();
		}
		else if ( inId == "E" )
		{
			return createTileE();
		}
		else if ( inId == "F" )
		{
			return createTileF();
		}
		else if ( inId == "G" )
		{
			return createTileG();
		}
		else if ( inId == "H" )
		{
			return createTileH();
		}
		else if ( inId == "I" )
		{
			return createTileI();
		}
		else if ( inId == "J" )
		{
			return createTileJ();
		}
		else if ( inId == "K" )
		{
			return createTileK();
		}
		else if ( inId == "L" )
		{
			return createTileL();
		}
		else if ( inId == "M" )
		{
			return createTileM();
		}
		else if ( inId == "N" )
		{
			return createTileN();
		}
		else if ( inId == "O" )
		{
			return createTileO();
		}
		else if ( inId == "P" )
		{
			return createTileP();
		}
		else if ( inId == "Q" )
		{
			return createTileQ();
		}
		else if ( inId == "R" )
		{
			return createTileR();
		}
		else if ( inId == "S" )
		{
			return createTileS();
		}
		else if ( inId == "T" )
		{
			return createTileT();
		}
		else if ( inId == "U" )
		{
			return createTileU();
		}
		else if ( inId == "V" )
		{
			return createTileV();
		}
		else if ( inId == "W" )
		{
			return createTileW();
		}
		else if ( inId == "X" )
		{
			return createTileX();
		}
		else if ( inId == "eA" )
		{
			return createTileEA();
		}
		else if ( inId == "eB" )
		{
			return createTileEB();
		}
		else if ( inId == "eC" )
		{
			return createTileEC();
		}
		else if ( inId == "eD" )
		{
			return createTileED();
		}
		else if ( inId == "eF" )
		{
			return createTileEF();
		}
		else if ( inId == "eG" )
		{
			return createTileEG();
		}
		else if ( inId == "eH" )
		{
			return createTileEH();
		}
		else if ( inId == "eI" )
		{
			return createTileEI();
		}
		else if ( inId == "eJ" )
		{
			return createTileEJ();
		}
		else if ( inId == "eK" )
		{
			return createTileEK();
		}
		else if ( inId == "eL" )
		{
			return createTileEL();
		}
		else if ( inId == "eM" )
		{
			return createTileEM();
		}
		else if ( inId == "eN" )
		{
			return createTileEN();
		}
		else if ( inId == "eO" )
		{
			return createTileEO();
		}
		else if ( inId == "eP" )
		{
			return createTileEP();
		}
		else if ( inId == "eQ" )
		{
			return createTileEQ();
		}
		assert( !"Wrong id given" );
	}
}

Model::Tile
Model::createTileA()
{
	std::vector< ContiguousField > fields;
	ContiguousField firstField;
	firstField.push_back( Area::kTopLeft );
	firstField.push_back( Area::kTop );
	firstField.push_back( Area::kTopRight );
	firstField.push_back( Area::kRightTop );
	firstField.push_back( Area::kRight );
	firstField.push_back( Area::kRightBottom );
	firstField.push_back( Area::kBottomRight );
	firstField.push_back( Area::kBottomLeft );
	firstField.push_back( Area::kLeftBottom );
	firstField.push_back( Area::kLeft );
	firstField.push_back( Area::kLeftTop );
	fields.push_back( firstField );

	std::vector< ContiguousRoad > roads;
	ContiguousRoad firstRoad;
	firstRoad.push_back( Area::kBottom );
	roads.push_back( firstRoad );

	std::vector< ContiguousCity > cities;

	std::map< ContiguousField, std::vector< ContiguousCity > > citiesPerField;
	std::vector< ContiguousCity > firstFieldCities;
	citiesPerField.insert( std::make_pair( firstField, firstFieldCities ) );

	Tile tileA = Tile( Tile::kCenterCloister, "A", fields, roads, cities, citiesPerField );

	return tileA;
}

Model::Tile
Model::createTileB()
{
	std::vector< ContiguousField > fields;
	ContiguousField firstField;
	firstField.push_back( Area::kTopLeft );
	firstField.push_back( Area::kTop );
	firstField.push_back( Area::kTopRight );
	firstField.push_back( Area::kRightTop );
	firstField.push_back( Area::kRight );
	firstField.push_back( Area::kRightBottom );
	firstField.push_back( Area::kBottomRight );
	firstField.push_back( Area::kBottom );
	firstField.push_back( Area::kBottomLeft );
	firstField.push_back( Area::kLeftBottom );
	firstField.push_back( Area::kLeft );
	firstField.push_back( Area::kLeftTop );
	fields.push_back( firstField );

	std::vector< ContiguousRoad > roads;

	std::vector< ContiguousCity > cities;

	std::map< ContiguousField, std::vector< ContiguousCity > > citiesPerField;
	std::vector< ContiguousCity > firstFieldCities;
	citiesPerField.insert( std::make_pair( firstField, firstFieldCities ) );

	Tile tileB = Tile( Tile::kCenterCloister, "B", fields, roads, cities, citiesPerField );

	return tileB;
}

Model::Tile
Model::createTileC()
{
	std::vector< ContiguousField > fields;

	std::vector< ContiguousRoad > roads;

	std::vector< ContiguousCity > cities;
	ContiguousCity city;
	city.push_back( Area::kTopLeft );
	city.push_back( Area::kTop );
	city.push_back( Area::kTopRight );
	city.push_back( Area::kRightTop );
	city.push_back( Area::kRight );
	city.push_back( Area::kRightBottom );
	city.push_back( Area::kBottomRight );
	city.push_back( Area::kBottom );
	city.push_back( Area::kBottomLeft );
	city.push_back( Area::kLeftTop );
	city.push_back( Area::kLeft );
	city.push_back( Area::kLeftBottom );
	city.push_back( Area::kCentral );
	cities.push_back( city );

	std::map< ContiguousField, std::vector< ContiguousCity > > citiesPerField;

	std::vector< Area::Area > shields;
	shields.push_back( Area::kTop );

	Tile tileC = Tile( Tile::kCenterProject, "C", fields, roads, cities, citiesPerField, shields );

	return tileC;
}

Model::Tile
Model::createTileD()
{
	std::vector< ContiguousField > fields;
	ContiguousField firstField;
	firstField.push_back( Area::kTopRight );
	firstField.push_back( Area::kBottomRight );
	ContiguousField secondField;
	secondField.push_back( Area::kBottomLeft );
	secondField.push_back( Area::kLeftBottom );
	secondField.push_back( Area::kLeft );
	secondField.push_back( Area::kLeftTop );
	secondField.push_back( Area::kTopLeft );
	fields.push_back( firstField );
	fields.push_back( secondField );

	std::vector< ContiguousRoad > roads;
	std::vector< Area::Area > firstRoad;
	firstRoad.push_back( Area::kTop );
	firstRoad.push_back( Area::kCentral );
	firstRoad.push_back( Area::kBottom );
	roads.push_back( firstRoad );

	std::vector< ContiguousCity > cities;
	std::vector< Area::Area > firstCity;
	firstCity.push_back( Area::kRightTop );
	firstCity.push_back( Area::kRight );
	firstCity.push_back( Area::kRightBottom );
	cities.push_back( firstCity );

	std::map< ContiguousField, std::vector< ContiguousCity > > citiesPerField;
	std::vector< ContiguousCity > firstFieldCities;
	firstFieldCities.push_back( firstCity );
	std::vector< ContiguousCity > secondFieldCities;

	citiesPerField.insert( std::make_pair( firstField, firstFieldCities ) );
	citiesPerField.insert( std::make_pair( secondField, secondFieldCities ) );

	Tile tileD = Tile( Tile::kCenterProject, "D", fields, roads, cities, citiesPerField );

	return tileD;
}

Model::Tile
Model::createTileE()
{
	std::vector< ContiguousField > fields;
	ContiguousField firstField;
	firstField.push_back( Area::kRightTop );
	firstField.push_back( Area::kRight );
	firstField.push_back( Area::kRightBottom );
	firstField.push_back( Area::kBottomRight );
	firstField.push_back( Area::kBottom );
	firstField.push_back( Area::kBottomLeft );
	firstField.push_back( Area::kLeftBottom );
	firstField.push_back( Area::kLeft );
	firstField.push_back( Area::kLeftTop );
	firstField.push_back( Area::kCentral );
	fields.push_back( firstField );

	std::vector< ContiguousRoad > roads;

	std::vector< ContiguousCity > cities;
	std::vector< Area::Area > firstCity;
	firstCity.push_back( Area::kTopLeft );
	firstCity.push_back( Area::kTop );
	firstCity.push_back( Area::kTopRight );
	cities.push_back( firstCity );

	std::map< ContiguousField, std::vector< ContiguousCity > > citiesPerField;
	std::vector< ContiguousCity > firstFieldCities;
	firstFieldCities.push_back( firstCity );

	citiesPerField.insert( std::make_pair( firstField, firstFieldCities ) );
	
	Tile tileE = Tile( Tile::kCenterProject, "E", fields, roads, cities, citiesPerField );

	return tileE;
}

Model::Tile
Model::createTileF()
{
	std::vector< ContiguousField > fields;

	ContiguousField firstField;
	firstField.push_back( Area::kTopLeft );
	firstField.push_back( Area::kTop );
	firstField.push_back( Area::kTopRight );

	ContiguousField secondField;
	secondField.push_back( Area::kBottomRight );
	secondField.push_back( Area::kBottom );
	secondField.push_back( Area::kBottomLeft );

	fields.push_back( firstField );
	fields.push_back( secondField );

	std::vector< ContiguousRoad > roads;

	std::vector<ContiguousCity> cities;
	std::vector<Area::Area> firstCity;
	firstCity.push_back( Area::kRightTop );
	firstCity.push_back( Area::kRight );
	firstCity.push_back( Area::kRightBottom );
	firstCity.push_back( Area::kLeftBottom );
	firstCity.push_back( Area::kLeft );
	firstCity.push_back( Area::kLeftTop );
	firstCity.push_back( Area::kCentral );
	cities.push_back( firstCity );

	std::map< ContiguousField, std::vector< ContiguousCity > > citiesPerField;
	std::vector< ContiguousCity > firstFieldCities;
	firstFieldCities.push_back( firstCity );
	std::vector< ContiguousCity > secondFieldCities;
	secondFieldCities.push_back( firstCity );

	citiesPerField.insert( std::make_pair( firstField, firstFieldCities ) );
	citiesPerField.insert( std::make_pair( secondField, secondFieldCities ) );

	std::vector< Area::Area > shields;
	shields.push_back( Area::kRight );

	Tile tileF = Tile( Tile::kCenterProject,"F", fields, roads, cities, citiesPerField, shields );
	return tileF;
}

Model::Tile
Model::createTileG()
{
	std::vector< ContiguousField > fields;
	ContiguousField firstField;
	firstField.push_back( Area::kRightTop );
	firstField.push_back( Area::kRight );
	firstField.push_back( Area::kRightBottom );
	ContiguousField secondField;
	secondField.push_back( Area::kLeftBottom );
	secondField.push_back( Area::kLeft );
	secondField.push_back( Area::kLeftTop );
	fields.push_back( firstField );
	fields.push_back( secondField );

	std::vector< ContiguousCity > cities;
	ContiguousCity firstCity;
	firstCity.push_back( Area::kTopLeft );
	firstCity.push_back( Area::kTop );
	firstCity.push_back( Area::kTopRight );
	firstCity.push_back( Area::kBottomRight );
	firstCity.push_back( Area::kBottom );
	firstCity.push_back( Area::kBottomLeft );
	firstCity.push_back( Area::kCentral );
	cities.push_back( firstCity );

	std::vector< ContiguousRoad > roads;

	std::map< ContiguousField, std::vector< ContiguousCity > > citiesPerField;
	std::vector< ContiguousCity > firstFieldCities;
	firstFieldCities.push_back( firstCity );
	std::vector< ContiguousCity > secondFieldCities;
	secondFieldCities.push_back( firstCity );

	citiesPerField.insert( std::make_pair( firstField, firstFieldCities ) );
	citiesPerField.insert( std::make_pair( secondField, secondFieldCities ) );

	Tile tileG = Tile( Tile::kCenterProject, "G", fields, roads, cities, citiesPerField );

	return tileG;
}

Model::Tile
Model::createTileH()
{
	std::vector< ContiguousField > fields;
	ContiguousField firstField;
	firstField.push_back( Area::kTopLeft );
	firstField.push_back( Area::kTop );
	firstField.push_back( Area::kTopRight );
	firstField.push_back( Area::kBottomRight );
	firstField.push_back( Area::kBottom );
	firstField.push_back( Area::kBottomLeft );
	firstField.push_back( Area::kCentral );
	fields.push_back( firstField );

	std::vector< ContiguousCity > cities;
	ContiguousCity firstCity;
	firstCity.push_back( Area::kRightTop );
	firstCity.push_back( Area::kRight );
	firstCity.push_back( Area::kRightBottom );
	cities.push_back( firstCity );
	ContiguousCity secondCity;
	secondCity.push_back( Area::kLeftBottom );
	secondCity.push_back( Area::kLeft );
	secondCity.push_back( Area::kLeftTop );
	cities.push_back( secondCity );

	std::vector< ContiguousRoad > roads;

	std::map< ContiguousField, std::vector< ContiguousCity > > citiesPerField;
	std::vector< ContiguousCity > firstFieldCities;
	firstFieldCities.push_back( firstCity );
	firstFieldCities.push_back( secondCity );

	citiesPerField.insert( std::make_pair( firstField, firstFieldCities ) );

	Tile tileH = Tile( Tile::kCenterProject, "H", fields, roads, cities, citiesPerField );

	return tileH;
}

Model::Tile
Model::createTileI()
{
	std::vector< ContiguousField > fields;
	ContiguousField firstField;
	firstField.push_back( Area::kTopLeft );
	firstField.push_back( Area::kTop );
	firstField.push_back( Area::kTopRight );
	firstField.push_back( Area::kLeftBottom );
	firstField.push_back( Area::kLeft );
	firstField.push_back( Area::kLeftTop );
	firstField.push_back( Area::kCentral );
	fields.push_back( firstField );

	std::vector< ContiguousCity > cities;
	ContiguousCity firstCity;
	firstCity.push_back( Area::kRightTop );
	firstCity.push_back( Area::kRight );
	firstCity.push_back( Area::kRightBottom );
	cities.push_back( firstCity );
	ContiguousCity secondCity;
	secondCity.push_back( Area::kBottomRight );
	secondCity.push_back( Area::kBottom );
	secondCity.push_back( Area::kBottomLeft );
	cities.push_back( secondCity );

	std::vector< ContiguousRoad > roads;

	std::map< ContiguousField, std::vector< ContiguousCity > > citiesPerField;
	std::vector< ContiguousCity > firstFieldCities;
	firstFieldCities.push_back( firstCity );
	firstFieldCities.push_back( secondCity );

	citiesPerField.insert( std::make_pair( firstField, firstFieldCities ) );

	Tile tileI = Tile( Tile::kCenterProject, "I", fields, roads, cities, citiesPerField );

	return tileI;
}

Model::Tile
Model::createTileJ()
{
	std::vector< ContiguousField > fields;
	ContiguousField firstField;
	firstField.push_back( Area::kRightTop );
	firstField.push_back( Area::kBottomLeft );
	firstField.push_back( Area::kLeftBottom );
	firstField.push_back( Area::kLeft );
	firstField.push_back( Area::kLeftTop );
	fields.push_back( firstField );
	ContiguousField secondField;
	secondField.push_back( Area::kRightBottom );
	secondField.push_back( Area::kBottomRight );
	fields.push_back( secondField );

	std::vector< ContiguousCity > cities;
	ContiguousCity firstCity;
	firstCity.push_back( Area::kTopLeft );
	firstCity.push_back( Area::kTop );
	firstCity.push_back( Area::kTopRight );
	cities.push_back( firstCity );

	std::vector< ContiguousRoad> roads;
	ContiguousRoad firstRoad;
	firstRoad.push_back( Area::kRight );
	firstRoad.push_back( Area::kBottom );
	firstRoad.push_back( Area::kCentral );
	roads.push_back( firstRoad );

	std::map< ContiguousField, std::vector< ContiguousCity > > citiesPerField;
	std::vector< ContiguousCity > firstFieldCities;
	firstFieldCities.push_back( firstCity );
	std::vector< ContiguousCity > secondFieldCities;

	citiesPerField.insert( std::make_pair( firstField, firstFieldCities ) );
	citiesPerField.insert( std::make_pair( secondField, secondFieldCities ) );

	Tile tileJ = Tile( Tile::kCenterProject, "J", fields, roads, cities, citiesPerField );
	return tileJ;
}

Model::Tile
Model::createTileK()
{
	std::vector< ContiguousField > fields;
	ContiguousField firstField;
	firstField.push_back( Area::kTopRight );
	firstField.push_back( Area::kBottomRight );
	firstField.push_back( Area::kBottom );
	firstField.push_back( Area::kBottomLeft );
	firstField.push_back( Area::kLeftBottom );
	ContiguousField secondField;
	secondField.push_back( Area::kLeftTop );
	secondField.push_back( Area::kTopLeft );
	fields.push_back( firstField );
	fields.push_back( secondField );

	std::vector< ContiguousCity > cities;
	std::vector< Area::Area > firstCity;
	firstCity.push_back( Area::kRightTop );
	firstCity.push_back( Area::kRight );
	firstCity.push_back( Area::kRightBottom );
	cities.push_back( firstCity );

	std::vector< ContiguousRoad > roads;
	std::vector< Area::Area > firstRoad;
	firstRoad.push_back( Area::kTop );
	firstRoad.push_back( Area::kLeft );
	firstRoad.push_back( Area::kCentral );
	roads.push_back( firstRoad );

	std::map< ContiguousField, std::vector< ContiguousCity > > citiesPerField;
	std::vector< ContiguousCity > firstFieldCities;
	firstFieldCities.push_back( firstCity );
	std::vector< ContiguousCity > secondFieldCities;

	citiesPerField.insert( std::make_pair( firstField, firstFieldCities ) );
	citiesPerField.insert( std::make_pair( secondField, secondFieldCities ) );

	Tile tileK = Tile( Tile::kCenterProject, "K", fields, roads, cities, citiesPerField );

	return tileK;
}

Model::Tile
Model::createTileL()
{
	std::vector< ContiguousCity > cities;
	std::vector< Area::Area > firstCity;
	firstCity.push_back( Area::kRightTop );
	firstCity.push_back( Area::kRight );
	firstCity.push_back( Area::kRightBottom );
	cities.push_back( firstCity );

	std::vector< ContiguousRoad > roads;
	std::vector< Area::Area > firstRoad;
	firstRoad.push_back( Area::kBottom );
	std::vector< Area::Area > secondRoad;
	secondRoad.push_back( Area::kLeft );
	std::vector< Area::Area > thirdRoad;
	thirdRoad.push_back( Area::kTop );
	roads.push_back( firstRoad );
	roads.push_back( secondRoad );
	roads.push_back( thirdRoad );

	std::vector< ContiguousField > fields;
	ContiguousField firstField;
	firstField.push_back( Area::kTopRight );
	firstField.push_back( Area::kBottomRight );
	ContiguousField secondField;
	secondField.push_back( Area::kBottomLeft );
	secondField.push_back( Area::kLeftBottom );
	ContiguousField thirdField;
	thirdField.push_back( Area::kLeftTop );
	thirdField.push_back( Area::kTopLeft );
	fields.push_back( firstField );
	fields.push_back( secondField );
	fields.push_back( thirdField );

	std::map< ContiguousField, std::vector< ContiguousCity > > citiesPerField;
	std::vector< ContiguousCity > firstFieldCities;
	firstFieldCities.push_back( firstCity );
	std::vector< ContiguousCity > secondFieldCities;
	std::vector< ContiguousCity > thirdFieldCities;

	citiesPerField.insert( std::make_pair( firstField, firstFieldCities ) );
	citiesPerField.insert( std::make_pair( secondField, secondFieldCities ) );
	citiesPerField.insert( std::make_pair( thirdField, thirdFieldCities ) );

	Tile tileL = Tile( Tile::kCenterNoProject, "L", fields, roads, cities, citiesPerField );

	return tileL;
}

Model::Tile
Model::createTileM()
{
	std::vector< ContiguousField > fields;
	ContiguousField firstField;
	firstField.push_back( Area::kRightTop );
	firstField.push_back( Area::kRight );
	firstField.push_back( Area::kRightBottom );
	firstField.push_back( Area::kBottomRight );
	firstField.push_back( Area::kBottom );
	firstField.push_back( Area::kBottomLeft );
	firstField.push_back( Area::kCentral );
	fields.push_back( firstField );

	std::vector< ContiguousRoad > roads;

	std::vector< ContiguousCity > cities;
	std::vector< Area::Area > firstCity;
	firstCity.push_back( Area::kTopLeft );
	firstCity.push_back( Area::kTop );
	firstCity.push_back( Area::kTopRight );
	firstCity.push_back( Area::kLeftBottom );
	firstCity.push_back( Area::kLeft );
	firstCity.push_back( Area::kLeftTop );
	cities.push_back( firstCity );

	std::map< ContiguousField, std::vector< ContiguousCity > > citiesPerField;
	std::vector< ContiguousCity > firstFieldCities;
	firstFieldCities.push_back( firstCity );

	citiesPerField.insert( std::make_pair( firstField, firstFieldCities ) );

	std::vector< Area::Area > shields;
	shields.push_back( Area::kTop );

	Tile tileM = Tile( Tile::kCenterProject, "M", fields, roads, cities, citiesPerField, shields );

	return tileM;
}

Model::Tile
Model::createTileN()
{
	std::vector< ContiguousField > fields;
	ContiguousField firstField;
	firstField.push_back( Area::kRightTop );
	firstField.push_back( Area::kRight );
	firstField.push_back( Area::kRightBottom );
	firstField.push_back( Area::kBottomRight );
	firstField.push_back( Area::kBottom );
	firstField.push_back( Area::kBottomLeft );
	firstField.push_back( Area::kCentral );
	fields.push_back( firstField );

	std::vector< ContiguousRoad > roads;

	std::vector< ContiguousCity > cities;
	std::vector< Area::Area > firstCity;
	firstCity.push_back( Area::kTopLeft );
	firstCity.push_back( Area::kTop );
	firstCity.push_back( Area::kTopRight );
	firstCity.push_back( Area::kLeftBottom );
	firstCity.push_back( Area::kLeft );
	firstCity.push_back( Area::kLeftTop );
	cities.push_back( firstCity );

	std::map< ContiguousField, std::vector< ContiguousCity > > citiesPerField;
	std::vector< ContiguousCity > firstFieldCities;
	firstFieldCities.push_back( firstCity );

	citiesPerField.insert( std::make_pair( firstField, firstFieldCities ) );

	Tile tileN = Tile( Tile::kCenterProject, "N", fields, roads, cities, citiesPerField );

	return tileN;
}

Model::Tile
Model::createTileO()
{
	std::vector< ContiguousField > fields;
	ContiguousField firstField;
	firstField.push_back( Area::kRightTop );
	firstField.push_back( Area::kBottomLeft );
	fields.push_back( firstField );
	ContiguousField secondField;
	secondField.push_back( Area::kRightBottom );
	secondField.push_back( Area::kBottomRight );
	fields.push_back( secondField );


	std::vector< ContiguousRoad > roads;
	ContiguousRoad firstRoad;
	firstRoad.push_back( Area::kRight );
	firstRoad.push_back( Area::kBottom );
	firstRoad.push_back( Area::kCentral );
	roads.push_back( firstRoad );

	std::vector< ContiguousCity > cities;
	std::vector< Area::Area > firstCity;
	firstCity.push_back( Area::kTopLeft );
	firstCity.push_back( Area::kTop );
	firstCity.push_back( Area::kTopRight );
	firstCity.push_back( Area::kLeftBottom );
	firstCity.push_back( Area::kLeft );
	firstCity.push_back( Area::kLeftTop );
	cities.push_back( firstCity );

	std::map< ContiguousField, std::vector< ContiguousCity > > citiesPerField;
	std::vector< ContiguousCity > firstFieldCities;
	firstFieldCities.push_back( firstCity );
	std::vector< ContiguousCity > secondFieldCities;

	citiesPerField.insert( std::make_pair( firstField, firstFieldCities ) );
	citiesPerField.insert( std::make_pair( secondField, secondFieldCities ) );

	std::vector< Area::Area > shields;
	shields.push_back( Area::kTop );

	Tile tileO = Tile( Tile::kCenterProject, "O", fields, roads, cities, citiesPerField, shields );

	return tileO;
}

Model::Tile
Model::createTileP()
{
	std::vector< ContiguousField > fields;
	ContiguousField firstField;
	firstField.push_back( Area::kRightTop );
	firstField.push_back( Area::kBottomLeft );
	fields.push_back( firstField );
	ContiguousField secondField;
	secondField.push_back( Area::kRightBottom );
	secondField.push_back( Area::kBottomRight );
	fields.push_back( secondField );


	std::vector< ContiguousRoad > roads;
	ContiguousRoad firstRoad;
	firstRoad.push_back( Area::kRight );
	firstRoad.push_back( Area::kBottom );
	firstRoad.push_back( Area::kCentral );
	roads.push_back( firstRoad );

	std::vector< ContiguousCity > cities;
	std::vector< Area::Area > firstCity;
	firstCity.push_back( Area::kTopLeft );
	firstCity.push_back( Area::kTop );
	firstCity.push_back( Area::kTopRight );
	firstCity.push_back( Area::kLeftBottom );
	firstCity.push_back( Area::kLeft );
	firstCity.push_back( Area::kLeftTop );
	cities.push_back( firstCity );

	std::map< ContiguousField, std::vector< ContiguousCity > > citiesPerField;
	std::vector< ContiguousCity > firstFieldCities;
	firstFieldCities.push_back( firstCity );
	std::vector< ContiguousCity > secondFieldCities;

	citiesPerField.insert( std::make_pair( firstField, firstFieldCities ) );
	citiesPerField.insert( std::make_pair( secondField, secondFieldCities ) );

	Tile tileP = Tile( Tile::kCenterProject, "P", fields, roads, cities, citiesPerField );

	return tileP;
}

Model::Tile
Model::createTileQ()
{
	std::vector< ContiguousField > fields;
	ContiguousField firstField;
	firstField.push_back( Area::kBottomRight );
	firstField.push_back( Area::kBottom );
	firstField.push_back( Area::kBottomLeft );
	fields.push_back( firstField );

	std::vector< ContiguousRoad > roads;

	std::vector< ContiguousCity > cities;
	ContiguousCity city;
	city.push_back( Area::kTopLeft );
	city.push_back( Area::kTop );
	city.push_back( Area::kTopRight );
	city.push_back( Area::kRightTop );
	city.push_back( Area::kRight );
	city.push_back( Area::kRightBottom );
	city.push_back( Area::kLeftBottom );
	city.push_back( Area::kLeft );
	city.push_back( Area::kLeftTop );
	city.push_back( Area::kCentral );
	cities.push_back( city );

	std::map< ContiguousField, std::vector< ContiguousCity > > citiesPerField;
	std::vector< ContiguousCity > firstFieldCities;
	firstFieldCities.push_back( city );
	citiesPerField.insert( std::make_pair( firstField, firstFieldCities ) );

	std::vector< Area::Area > shields;
	shields.push_back( Area::kTop );

	Tile tileQ = Tile( Tile::kCenterProject, "Q", fields, roads, cities, citiesPerField, shields );

	return tileQ;
}

Model::Tile
Model::createTileR()
{
	std::vector< ContiguousField > fields;
	ContiguousField firstField;
	firstField.push_back( Area::kBottomRight );
	firstField.push_back( Area::kBottom );
	firstField.push_back( Area::kBottomLeft );
	fields.push_back( firstField );

	std::vector< ContiguousRoad > roads;

	std::vector< ContiguousCity > cities;
	ContiguousCity city;
	city.push_back( Area::kTopLeft );
	city.push_back( Area::kTop );
	city.push_back( Area::kTopRight );
	city.push_back( Area::kRightTop );
	city.push_back( Area::kRight );
	city.push_back( Area::kRightBottom );
	city.push_back( Area::kLeftBottom );
	city.push_back( Area::kLeft );
	city.push_back( Area::kLeftTop );
	city.push_back( Area::kCentral );
	cities.push_back( city );

	std::map< ContiguousField, std::vector< ContiguousCity > > citiesPerField;
	std::vector< ContiguousCity > firstFieldCities;
	firstFieldCities.push_back( city );
	citiesPerField.insert( std::make_pair( firstField, firstFieldCities ) );

	Tile tileR = Tile( Tile::kCenterProject, "R", fields, roads, cities, citiesPerField );

	return tileR;
}

Model::Tile
Model::createTileS()
{
	std::vector< ContiguousField > fields;
	ContiguousField firstField;
	firstField.push_back( Area::kBottomRight );
	fields.push_back( firstField );
	ContiguousField secondField;
	secondField.push_back( Area::kBottomLeft );
	fields.push_back( secondField );

	std::vector< ContiguousRoad > roads;
	ContiguousRoad road;
	road.push_back( Area::kBottom );
	roads.push_back( road );

	std::vector< ContiguousCity > cities;
	ContiguousCity city;
	city.push_back( Area::kTopLeft );
	city.push_back( Area::kTop );
	city.push_back( Area::kTopRight );
	city.push_back( Area::kRightTop );
	city.push_back( Area::kRight );
	city.push_back( Area::kRightBottom );
	city.push_back( Area::kLeftBottom );
	city.push_back( Area::kLeft );
	city.push_back( Area::kLeftTop );
	city.push_back( Area::kCentral );
	cities.push_back( city );

	std::map< ContiguousField, std::vector< ContiguousCity > > citiesPerField;
	std::vector< ContiguousCity > firstFieldCities;
	firstFieldCities.push_back( city );
	std::vector< ContiguousCity > secondFieldCities;
	secondFieldCities.push_back( city );
	citiesPerField.insert( std::make_pair( firstField, firstFieldCities ) );
	citiesPerField.insert( std::make_pair( secondField, secondFieldCities ) );

	std::vector< Area::Area > shields;
	shields.push_back( Area::kTop );

	Tile tileS = Tile( Tile::kCenterProject, "S", fields, roads, cities, citiesPerField, shields );

	return tileS;
}

Model::Tile
Model::createTileT()
{
	std::vector< ContiguousField > fields;
	ContiguousField firstField;
	firstField.push_back( Area::kBottomRight );
	fields.push_back( firstField );
	ContiguousField secondField;
	secondField.push_back( Area::kBottomLeft );
	fields.push_back( secondField );

	std::vector< ContiguousRoad > roads;
	ContiguousRoad road;
	road.push_back( Area::kBottom );
	roads.push_back( road );

	std::vector< ContiguousCity > cities;
	ContiguousCity city;
	city.push_back( Area::kTopLeft );
	city.push_back( Area::kTop );
	city.push_back( Area::kTopRight );
	city.push_back( Area::kRightTop );
	city.push_back( Area::kRight );
	city.push_back( Area::kRightBottom );
	city.push_back( Area::kLeftBottom );
	city.push_back( Area::kLeft );
	city.push_back( Area::kLeftTop );
	city.push_back( Area::kCentral );
	cities.push_back( city );

	std::map< ContiguousField, std::vector< ContiguousCity > > citiesPerField;
	std::vector< ContiguousCity > firstFieldCities;
	firstFieldCities.push_back( city );
	std::vector< ContiguousCity > secondFieldCities;
	secondFieldCities.push_back( city );
	citiesPerField.insert( std::make_pair( firstField, firstFieldCities ) );
	citiesPerField.insert( std::make_pair( secondField, secondFieldCities ) );

	Tile tileT = Tile( Tile::kCenterProject, "T",     fields, roads, cities, citiesPerField );

	return tileT;
}

Model::Tile
Model::createTileU()
{
	std::vector< ContiguousField > fields;
	ContiguousField firstField;
	firstField.push_back( Area::kTopLeft );
	firstField.push_back( Area::kBottomLeft );
	firstField.push_back( Area::kLeftBottom );
	firstField.push_back( Area::kLeft );
	firstField.push_back( Area::kLeftTop );
	fields.push_back( firstField );
	ContiguousField secondField;
	secondField.push_back( Area::kTopRight );
	secondField.push_back( Area::kRightTop );
	secondField.push_back( Area::kRight );
	secondField.push_back( Area::kRightBottom );
	secondField.push_back( Area::kBottomRight );
	fields.push_back( secondField );

	std::vector< ContiguousRoad > roads;
	ContiguousRoad road;
	road.push_back( Area::kTop );
	road.push_back( Area::kBottom );
	road.push_back( Area::kCentral );
	roads.push_back( road );

	std::vector< ContiguousCity> cities;

	std::map< ContiguousField, std::vector< ContiguousCity > > citiesPerField;
	citiesPerField.insert( std::make_pair( firstField, std::vector< ContiguousCity >() ) );
	citiesPerField.insert( std::make_pair( secondField, std::vector< ContiguousCity >() ) );

	Tile tileU = Tile( Tile::kCenterProject, "U", fields, roads, cities, citiesPerField );

	return tileU;
}

Model::Tile
Model::createTileV()
{
	std::vector< ContiguousField > fields;
	ContiguousField firstField;
	firstField.push_back( Area::kTopLeft );
	firstField.push_back( Area::kTop );
	firstField.push_back( Area::kTopRight );
	firstField.push_back( Area::kRightTop );
	firstField.push_back( Area::kRight );
	firstField.push_back( Area::kRightBottom );
	firstField.push_back( Area::kBottomRight );
	firstField.push_back( Area::kLeftTop );
	fields.push_back( firstField );
	ContiguousField secondField;
	secondField.push_back( Area::kBottomLeft );
	secondField.push_back( Area::kLeftBottom );
	fields.push_back( secondField );

	std::vector< ContiguousRoad > roads;
	ContiguousRoad road;
	road.push_back( Area::kBottom );
	road.push_back( Area::kLeft );
	road.push_back( Area::kCentral );
	roads.push_back( road );

	std::vector< ContiguousCity> cities;

	std::map< ContiguousField, std::vector< ContiguousCity > > citiesPerField;
	citiesPerField.insert( std::make_pair( firstField, std::vector< ContiguousCity >() ) );
	citiesPerField.insert( std::make_pair( secondField, std::vector< ContiguousCity >() ) );

	Tile tileV = Tile( Tile::kCenterProject, "V", fields, roads, cities, citiesPerField );

	return tileV;
}

Model::Tile
Model::createTileW()
{
	std::vector< ContiguousField > fields;
	ContiguousField firstField;
	firstField.push_back( Area::kTopLeft );
	firstField.push_back( Area::kTop );
	firstField.push_back( Area::kTopRight );
	firstField.push_back( Area::kRightTop );
	firstField.push_back( Area::kLeftTop );
	fields.push_back( firstField );
	ContiguousField secondField;
	secondField.push_back( Area::kRightBottom );
	secondField.push_back( Area::kBottomRight );
	fields.push_back( secondField );
	ContiguousField thirdField;
	thirdField.push_back( Area::kBottomLeft );
	thirdField.push_back( Area::kLeftBottom );
	fields.push_back( thirdField );

	std::vector< ContiguousRoad > roads;
	ContiguousRoad firstRoad;
	firstRoad.push_back( Area::kRight );
	roads.push_back( firstRoad );
	ContiguousRoad secondRoad;
	secondRoad.push_back( Area::kBottom );
	roads.push_back( secondRoad );
	ContiguousRoad thirdRoad;
	thirdRoad.push_back( Area::kLeft );
	roads.push_back( thirdRoad );

	std::vector< ContiguousCity> cities;

	std::map< ContiguousField, std::vector< ContiguousCity > > citiesPerField;
	citiesPerField.insert( std::make_pair( firstField, std::vector< ContiguousCity >() ) );
	citiesPerField.insert( std::make_pair( secondField, std::vector< ContiguousCity >() ) );
	citiesPerField.insert( std::make_pair( thirdField, std::vector< ContiguousCity >() ) );

	Tile const tileW( Tile::kCenterNoProject, "W", fields, roads, cities, citiesPerField );

	return tileW;
}

Model::Tile
Model::createTileX()
{
	std::vector< ContiguousField > fields;
	ContiguousField firstField;
	firstField.push_back( Area::kTopLeft );
	firstField.push_back( Area::kLeftTop );
	fields.push_back( firstField );
	ContiguousField secondField;
	secondField.push_back( Area::kTopRight );
	secondField.push_back( Area::kRightTop );
	fields.push_back( secondField );
	ContiguousField thirdField;
	thirdField.push_back( Area::kRightBottom );
	thirdField.push_back( Area::kBottomRight );
	fields.push_back( thirdField );
	ContiguousField fourthField;
	fourthField.push_back( Area::kBottomLeft );
	fourthField.push_back( Area::kLeftBottom );
	fields.push_back( fourthField );

	std::vector< ContiguousRoad > roads;
	ContiguousRoad firstRoad;
	firstRoad.push_back( Area::kTop );
	roads.push_back( firstRoad );
	ContiguousRoad secondRoad;
	secondRoad.push_back( Area::kRight );
	roads.push_back( secondRoad );
	ContiguousRoad thirdRoad;
	thirdRoad.push_back( Area::kBottom );
	roads.push_back( thirdRoad );
	ContiguousRoad fourthRoad;
	fourthRoad.push_back( Area::kLeft );
	roads.push_back( fourthRoad );

	std::vector< ContiguousCity> cities;

	std::map< ContiguousField, std::vector< ContiguousCity > > citiesPerField;
	std::vector< ContiguousCity > firstFieldCities;
	std::vector< ContiguousCity > secondFieldCities;
	std::vector< ContiguousCity > thirdFieldCities;
	std::vector< ContiguousCity > fourthFieldCities;
	citiesPerField.insert( std::make_pair( firstField, firstFieldCities ) );
	citiesPerField.insert( std::make_pair( secondField, secondFieldCities ) );
	citiesPerField.insert( std::make_pair( thirdField, thirdFieldCities ) );
	citiesPerField.insert( std::make_pair( fourthField, fourthFieldCities ) );

	Tile tileX = Tile( Tile::kCenterNoProject, "X", fields, roads, cities, citiesPerField );

	return tileX;
}

Model::Tile
Model::createTileEA()
{
	std::vector< ContiguousField > fields;
	ContiguousField firstField;
	firstField.push_back( Area::kTopLeft );
	firstField.push_back( Area::kTop );
	firstField.push_back( Area::kTopRight );
	firstField.push_back( Area::kRightTop );
	firstField.push_back( Area::kRight );
	firstField.push_back( Area::kRightBottom );
	firstField.push_back( Area::kBottomRight );
	firstField.push_back( Area::kLeftTop );
	fields.push_back( firstField );
	ContiguousField secondField;
	secondField.push_back( Area::kBottomLeft );
	secondField.push_back( Area::kLeftBottom );
	fields.push_back( secondField );

	std::vector< ContiguousRoad > roads;
	ContiguousRoad road;
	road.push_back( Area::kBottom );
	road.push_back( Area::kLeft );
	road.push_back( Area::kCentral );
	roads.push_back( road );

	std::vector< ContiguousCity > cities;

	std::map< ContiguousField, std::vector< ContiguousCity > > citiesPerField;
	citiesPerField.insert( std::make_pair( firstField, std::vector< ContiguousCity >() ) );
	citiesPerField.insert( std::make_pair( secondField, std::vector< ContiguousCity >() ) );

	std::vector< Area::Area > inns;
	inns.push_back( Area::kLeft );

	Tile const tileEA
	(
		Tile::kCenterProject,
		"eA",
		fields,
		roads,
		cities,
		citiesPerField,
		std::vector< Area::Area >(),
		inns
	);

	return tileEA;
}

Model::Tile
Model::createTileEB()
{
	std::vector< ContiguousField > fields;
	ContiguousField firstField;
	firstField.push_back( Area::kTopLeft );
	firstField.push_back( Area::kBottomLeft );
	firstField.push_back( Area::kLeftBottom );
	firstField.push_back( Area::kLeft );
	firstField.push_back( Area::kLeftTop );
	fields.push_back( firstField );
	ContiguousField secondField;
	secondField.push_back( Area::kTopRight );
	secondField.push_back( Area::kRightTop );
	secondField.push_back( Area::kRight );
	secondField.push_back( Area::kRightBottom );
	secondField.push_back( Area::kBottomRight );
	fields.push_back( secondField );

	std::vector< ContiguousRoad > roads;
	ContiguousRoad road;
	road.push_back( Area::kTop );
	road.push_back( Area::kBottom );
	road.push_back( Area::kCentral );
	roads.push_back( road );

	std::vector< ContiguousCity> cities;

	std::map< ContiguousField, std::vector< ContiguousCity > > citiesPerField;
	citiesPerField.insert( std::make_pair( firstField, std::vector< ContiguousCity >() ) );
	citiesPerField.insert( std::make_pair( secondField, std::vector< ContiguousCity >() ) );

	std::vector< Area::Area > inns;
	inns.push_back( Area::kTop );

	Tile const tileEB
	(
		Tile::kCenterProject,
		"eB",
		fields,
		roads,
		cities,
		citiesPerField,
		std::vector< Area::Area >(),
		inns
	);

	return tileEB;
}

Model::Tile
Model::createTileEC()
{
	std::vector< ContiguousField > fields;
	ContiguousField firstField;
	firstField.push_back( Area::kTopLeft );
	firstField.push_back( Area::kTop );
	firstField.push_back( Area::kTopRight );
	firstField.push_back( Area::kRightTop );
	firstField.push_back( Area::kLeftTop );
	fields.push_back( firstField );
	ContiguousField secondField;
	secondField.push_back( Area::kRightBottom );
	secondField.push_back( Area::kBottomRight );
	fields.push_back( secondField );
	ContiguousField thirdField;
	thirdField.push_back( Area::kBottomLeft );
	thirdField.push_back( Area::kLeftBottom );
	fields.push_back( thirdField );

	std::vector< ContiguousRoad > roads;
	roads.push_back( ContiguousRoad( 1, Area::kRight ) );
	roads.push_back( ContiguousRoad( 1, Area::kBottom ) );
	roads.push_back( ContiguousRoad( 1, Area::kLeft ) );

	std::vector< ContiguousCity> cities;

	std::map< ContiguousField, std::vector< ContiguousCity > > citiesPerField;
	citiesPerField.insert( std::make_pair( firstField, std::vector< ContiguousCity >() ) );
	citiesPerField.insert( std::make_pair( secondField, std::vector< ContiguousCity >() ) );
	citiesPerField.insert( std::make_pair( thirdField, std::vector< ContiguousCity >() ) );

	std::vector< Area::Area > inns;
	inns.push_back( Area::kRight );

	Tile const tileEC
	(
		Tile::kCenterNoProject,
		"eC",
		fields,
		roads,
		cities,
		citiesPerField,
		std::vector< Area::Area >(),
		inns
	);

	return tileEC;
}

Model::Tile
Model::createTileED()
{
	std::vector< ContiguousField > fields;
	ContiguousField firstField;
	firstField.push_back( Area::kTopLeft );
	firstField.push_back( Area::kTop );
	firstField.push_back( Area::kTopRight );
	firstField.push_back( Area::kRightTop );
	firstField.push_back( Area::kLeftTop );
	fields.push_back( firstField );
	ContiguousField secondField;
	secondField.push_back( Area::kRightBottom );
	secondField.push_back( Area::kBottomRight );
	secondField.push_back( Area::kBottom );
	secondField.push_back( Area::kBottomLeft );
	secondField.push_back( Area::kLeftBottom );
	fields.push_back( secondField );

	std::vector< ContiguousRoad > roads;
	roads.push_back( ContiguousRoad( 1, Area::kRight ) );
	roads.push_back( ContiguousRoad( 1, Area::kLeft ) );

	std::vector< ContiguousCity > cities;

	std::map< ContiguousField, std::vector< ContiguousCity > > citiesPerField;
	citiesPerField.insert( std::make_pair( firstField, std::vector< ContiguousCity >() ) );
	citiesPerField.insert( std::make_pair( secondField, std::vector< ContiguousCity >() ) );

	Tile const tileED
	(
		Tile::kCenterCloister,
		"eD",
		fields,
		roads,
		cities,
		citiesPerField
	);

	return tileED;
}

Model::Tile
Model::createTileEE()
{
	std::vector< ContiguousField > fields;
	ContiguousField firstField;
	firstField.push_back( Area::kTopLeft );
	firstField.push_back( Area::kLeftTop );
	fields.push_back( firstField );
	ContiguousField secondField;
	secondField.push_back( Area::kTopRight );
	secondField.push_back( Area::kRightTop );
	secondField.push_back( Area::kBottomLeft );
	secondField.push_back( Area::kLeftBottom );
	secondField.push_back( Area::kCentral );
	fields.push_back( secondField );
	ContiguousField thirdField;
	thirdField.push_back( Area::kRightBottom );
	thirdField.push_back( Area::kBottomRight );
	fields.push_back( thirdField );

	std::vector< ContiguousRoad > roads;
	ContiguousRoad firstRoad;
	firstRoad.push_back( Area::kTop );
	firstRoad.push_back( Area::kLeft );
	roads.push_back( firstRoad );
	ContiguousRoad secondRoad;
	secondRoad.push_back( Area::kRight );
	secondRoad.push_back( Area::kBottom );
	roads.push_back( secondRoad );

	std::vector< ContiguousCity > cities;

	std::map< ContiguousField, std::vector< ContiguousCity > > citiesPerField;
	citiesPerField.insert( std::make_pair( firstField, std::vector< ContiguousCity >() ) );
	citiesPerField.insert( std::make_pair( secondField, std::vector< ContiguousCity >() ) );
	citiesPerField.insert( std::make_pair( thirdField, std::vector< ContiguousCity >() ) );

	Tile const tileEE
	(
		Tile::kCenterProject,
		"eE",
		fields,
		roads,
		cities,
		citiesPerField
	);

	return tileEE;
}

Model::Tile
Model::createTileEF()
{
	std::vector< ContiguousField > fields;
	ContiguousField firstField;
	firstField.push_back( Area::kBottomRight );
	fields.push_back( firstField );
	ContiguousField secondField;
	secondField.push_back( Area::kBottomLeft );
	secondField.push_back( Area::kLeftBottom );
	secondField.push_back( Area::kLeft );
	secondField.push_back( Area::kLeftTop );
	fields.push_back( secondField );

	std::vector< ContiguousRoad > roads;
	ContiguousRoad firstRoad;
	firstRoad.push_back( Area::kBottom );
	firstRoad.push_back( Area::kCentral );
	roads.push_back( firstRoad );

	std::vector< ContiguousCity > cities;
	ContiguousCity firstCity;
	firstCity.push_back( Area::kTopLeft );
	firstCity.push_back( Area::kTop );
	firstCity.push_back( Area::kTopRight );
	firstCity.push_back( Area::kRightTop );
	firstCity.push_back( Area::kRight );
	firstCity.push_back( Area::kRightBottom );
	cities.push_back( firstCity );

	std::map< ContiguousField, std::vector< ContiguousCity > > citiesPerField;
	citiesPerField[ firstField ] = std::vector< ContiguousCity >( 1, firstCity );
	citiesPerField[ secondField ] = std::vector< ContiguousCity >( 1, firstCity );

	Tile const tileEF
	(
		Tile::kCenterProject,
		"eF",
		fields,
		roads,
		cities,
		citiesPerField
	);

	return tileEF;
}

Model::Tile
Model::createTileEG()
{
	std::vector< ContiguousField > fields;
	ContiguousField firstField;
	firstField.push_back( Area::kTopLeft );
	firstField.push_back( Area::kTop );
	firstField.push_back( Area::kTopRight );
	fields.push_back( firstField );
	ContiguousField secondField;
	secondField.push_back( Area::kRightTop );
	secondField.push_back( Area::kRight );
	secondField.push_back( Area::kRightBottom );
	secondField.push_back( Area::kBottomRight );
	secondField.push_back( Area::kBottom );
	secondField.push_back( Area::kBottomLeft );
	fields.push_back( secondField );

	std::vector< ContiguousRoad > roads;

	std::vector< ContiguousCity > cities;
	ContiguousCity firstCity;
	firstCity.push_back( Area::kLeftBottom );
	firstCity.push_back( Area::kLeft );
	firstCity.push_back( Area::kLeftTop );
	firstCity.push_back( Area::kCentral );
	cities.push_back( firstCity );

	std::map< ContiguousField, std::vector< ContiguousCity > > citiesPerField;
	citiesPerField[ firstField ] = std::vector< ContiguousCity >( 1, firstCity );
	citiesPerField[ secondField ] = std::vector< ContiguousCity >( 1, firstCity );

	Tile const tileEG
	(
		Tile::kCenterProject,
		"eG",
		fields,
		roads,
		cities,
		citiesPerField
	);

	return tileEG;
}

Model::Tile
Model::createTileEH()
{
	std::vector< ContiguousField > fields;
	fields.push_back( ContiguousField( 1, Area::kCentral ) );

	std::vector< ContiguousRoad > roads;

	std::vector< ContiguousCity > cities;
	ContiguousCity firstCity;
	firstCity.push_back( Area::kTopLeft );
	firstCity.push_back( Area::kTop );
	firstCity.push_back( Area::kTopRight );
	cities.push_back( firstCity );
	ContiguousCity secondCity;
	secondCity.push_back( Area::kRightTop );
	secondCity.push_back( Area::kRight );
	secondCity.push_back( Area::kRightBottom );
	cities.push_back( secondCity );
	ContiguousCity thirdCity;
	thirdCity.push_back( Area::kBottomRight );
	thirdCity.push_back( Area::kBottom );
	thirdCity.push_back( Area::kBottomLeft );
	cities.push_back( thirdCity );
	ContiguousCity fourthCity;
	fourthCity.push_back( Area::kLeftBottom );
	fourthCity.push_back( Area::kLeft );
	fourthCity.push_back( Area::kLeftTop );
	cities.push_back( fourthCity );

	std::map< ContiguousField, std::vector< ContiguousCity > > citiesPerField;
	citiesPerField[ ContiguousField( 1, Area::kCentral ) ] = cities;

	Tile const tileEH
	(
		Tile::kCenterProject,
		"eH",
		fields,
		roads,
		cities,
		citiesPerField
	);

	return tileEH;
}

Model::Tile
Model::createTileEI()
{
	std::vector< ContiguousField > fields;
	fields.push_back( ContiguousField( 1, Area::kTopLeft ) );
	fields.push_back( ContiguousField( 1, Area::kTopRight ) );
	fields.push_back( ContiguousField( 1, Area::kBottomRight ) );
	fields.push_back( ContiguousField( 1, Area::kBottomLeft ) );

	std::vector< ContiguousRoad > roads;
	roads.push_back( ContiguousRoad( 1, Area::kTop ) );
	roads.push_back( ContiguousRoad( 1, Area::kBottom ) );

	std::vector< ContiguousCity > cities;
	ContiguousCity firstCity;
	firstCity.push_back( Area::kRightTop );
	firstCity.push_back( Area::kRight );
	firstCity.push_back( Area::kRightBottom );
	cities.push_back( firstCity );
	ContiguousCity secondCity;
	secondCity.push_back( Area::kLeftBottom );
	secondCity.push_back( Area::kLeft );
	secondCity.push_back( Area::kLeftTop );
	cities.push_back( secondCity );

	std::map< ContiguousField, std::vector< ContiguousCity > > citiesPerField;
	citiesPerField[ ContiguousField( 1, Area::kTopLeft ) ] = std::vector< ContiguousCity >( 1, secondCity );
	citiesPerField[ ContiguousField( 1, Area::kTopRight ) ] = std::vector< ContiguousCity >( 1, firstCity );
	citiesPerField[ ContiguousField( 1, Area::kBottomRight ) ] = std::vector< ContiguousCity >( 1, firstCity );
	citiesPerField[ ContiguousField( 1, Area::kBottomLeft ) ] = std::vector< ContiguousCity >( 1, secondCity );

	Tile const tileEI
	(
		Tile::kCenterNoProject,
		"eI",
		fields,
		roads,
		cities,
		citiesPerField
	);

	return tileEI;
}

Model::Tile
Model::createTileEJ()
{
	std::vector< ContiguousField > fields;
	ContiguousField firstField;
	firstField.push_back( Area::kRightTop );
	firstField.push_back( Area::kRight );
	firstField.push_back( Area::kRightBottom );
	firstField.push_back( Area::kBottomRight );
	fields.push_back( firstField );
	ContiguousField secondField;
	secondField.push_back( Area::kBottomLeft );
	secondField.push_back( Area::kLeftBottom );
	secondField.push_back( Area::kLeft );
	secondField.push_back( Area::kLeftTop );
	fields.push_back( secondField );

	std::vector< ContiguousRoad > roads;
	ContiguousRoad firstRoad;
	firstRoad.push_back( Area::kBottom );
	firstRoad.push_back( Area::kCentral );
	roads.push_back( firstRoad );

	std::vector< ContiguousCity > cities;
	ContiguousCity firstCity;
	firstCity.push_back( Area::kTopLeft );
	firstCity.push_back( Area::kTop );
	firstCity.push_back( Area::kTopRight );
	cities.push_back( firstCity );

	std::map< ContiguousField, std::vector< ContiguousCity > > citiesPerField;
	citiesPerField[ firstField ] = cities;
	citiesPerField[ secondField ] = cities;

	Tile const tileEJ
	(
		Tile::kCenterProject,
		"eJ",
		fields,
		roads,
		cities,
		citiesPerField
	);

	return tileEJ;
}

Model::Tile
Model::createTileEK()
{
	std::vector< ContiguousField > fields;

	std::vector< ContiguousRoad > roads;

	std::vector< ContiguousCity > cities;
	ContiguousCity firstCity;
	firstCity.push_back( Area::kTopLeft );
	firstCity.push_back( Area::kTop );
	firstCity.push_back( Area::kTopRight );
	firstCity.push_back( Area::kRightTop );
	firstCity.push_back( Area::kRight );
	firstCity.push_back( Area::kRightBottom );
	firstCity.push_back( Area::kBottomRight );
	firstCity.push_back( Area::kBottom );
	firstCity.push_back( Area::kBottomLeft );
	firstCity.push_back( Area::kLeftBottom );
	firstCity.push_back( Area::kLeft );
	firstCity.push_back( Area::kLeftTop );
	firstCity.push_back( Area::kCentral );
	cities.push_back( firstCity );

	std::map< ContiguousField, std::vector< ContiguousCity > > citiesPerField;

	Tile const tileEK
	(
		Tile::kCenterCathedral,
		"eK",
		fields,
		roads,
		cities,
		citiesPerField
	);

	return tileEK;
}

Model::Tile
Model::createTileEL()
{
	std::vector< ContiguousField > fields;
	ContiguousField firstField;
	firstField.push_back( Area::kRightTop );
	firstField.push_back( Area::kBottomLeft );
	fields.push_back( firstField );
	ContiguousField secondField;
	secondField.push_back( Area::kRightBottom );
	secondField.push_back( Area::kBottomRight );
	fields.push_back( secondField );


	std::vector< ContiguousRoad > roads;
	ContiguousRoad firstRoad;
	firstRoad.push_back( Area::kRight );
	firstRoad.push_back( Area::kBottom );
	firstRoad.push_back( Area::kCentral );
	roads.push_back( firstRoad );

	std::vector< ContiguousCity > cities;
	std::vector< Area::Area > firstCity;
	firstCity.push_back( Area::kTopLeft );
	firstCity.push_back( Area::kTop );
	firstCity.push_back( Area::kTopRight );
	firstCity.push_back( Area::kLeftBottom );
	firstCity.push_back( Area::kLeft );
	firstCity.push_back( Area::kLeftTop );
	cities.push_back( firstCity );

	std::map< ContiguousField, std::vector< ContiguousCity > > citiesPerField;
	citiesPerField[ firstField ] = std::vector< ContiguousCity >( 1, firstCity );
	citiesPerField[ secondField ] = std::vector< ContiguousCity >();

	std::vector< Area::Area > shields( 1, Area::kTop );

	std::vector< Area::Area > inns( 1, Area::kRight );

	Tile const tileEL
	(
		Tile::kCenterProject,
		"eL",
		fields,
		roads,
		cities,
		citiesPerField,
		shields,
		inns
	);

	return tileEL;
}

Model::Tile
Model::createTileEM()
{
	std::vector< ContiguousField > fields;
	ContiguousField firstField;
	firstField.push_back( Area::kRightTop );
	firstField.push_back( Area::kRight );
	firstField.push_back( Area::kRightBottom );
	firstField.push_back( Area::kBottomRight );
	firstField.push_back( Area::kLeftTop );
	fields.push_back( firstField );
	ContiguousField secondField;
	secondField.push_back( Area::kBottomLeft );
	secondField.push_back( Area::kLeftBottom );
	fields.push_back( secondField );


	std::vector< ContiguousRoad > roads;
	ContiguousRoad firstRoad;
	firstRoad.push_back( Area::kBottom );
	firstRoad.push_back( Area::kLeft );
	firstRoad.push_back( Area::kCentral );
	roads.push_back( firstRoad );

	std::vector< ContiguousCity > cities;
	std::vector< Area::Area > firstCity;
	firstCity.push_back( Area::kTopLeft );
	firstCity.push_back( Area::kTop );
	firstCity.push_back( Area::kTopRight );
	cities.push_back( firstCity );

	std::map< ContiguousField, std::vector< ContiguousCity > > citiesPerField;
	citiesPerField[ firstField ] = std::vector< ContiguousCity >( 1, firstCity );
	citiesPerField[ secondField ] = std::vector< ContiguousCity >();

	std::vector< Area::Area > shields;

	std::vector< Area::Area > inns( 1, Area::kBottom );

	Tile const tileEM
	(
		Tile::kCenterProject,
		"eM",
		fields,
		roads,
		cities,
		citiesPerField,
		shields,
		inns
	);

	return tileEM;
}


Model::Tile
Model::createTileEN()
{
	std::vector< ContiguousField > fields;
	ContiguousField firstField;
	firstField.push_back( Area::kRightTop );
	firstField.push_back( Area::kRight );
	firstField.push_back( Area::kRightBottom );
	firstField.push_back( Area::kBottomRight );
	fields.push_back( firstField );
	ContiguousField secondField;
	secondField.push_back( Area::kBottomLeft );
	fields.push_back( secondField );


	std::vector< ContiguousRoad > roads;
	ContiguousRoad firstRoad;
	firstRoad.push_back( Area::kBottom );
	firstRoad.push_back( Area::kCentral );
	roads.push_back( firstRoad );

	std::vector< ContiguousCity > cities;
	std::vector< Area::Area > firstCity;
	firstCity.push_back( Area::kTopLeft );
	firstCity.push_back( Area::kTop );
	firstCity.push_back( Area::kTopRight );
	firstCity.push_back( Area::kLeftBottom );
	firstCity.push_back( Area::kLeft );
	firstCity.push_back( Area::kLeftTop );
	cities.push_back( firstCity );

	std::map< ContiguousField, std::vector< ContiguousCity > > citiesPerField;
	citiesPerField[ firstField ] = std::vector< ContiguousCity >( 1, firstCity );
	citiesPerField[ secondField ] = std::vector< ContiguousCity >( 1, firstCity );

	std::vector< Area::Area > shields;

	std::vector< Area::Area > inns( 1, Area::kBottom );

	Tile const tileEN
	(
		Tile::kCenterProject,
		"eN",
		fields,
		roads,
		cities,
		citiesPerField,
		shields,
		inns
	);

	return tileEN;
}

Model::Tile
Model::createTileEO()
{
	std::vector< ContiguousField > fields;
	ContiguousField firstField;
	firstField.push_back( Area::kBottomRight );
	firstField.push_back( Area::kBottom );
	firstField.push_back( Area::kBottomLeft );
	firstField.push_back( Area::kCentral );
	fields.push_back( firstField );

	std::vector< ContiguousRoad > roads;

	std::vector< ContiguousCity > cities;
	ContiguousCity firstCity;
	firstCity.push_back( Area::kTopLeft );
	firstCity.push_back( Area::kTop );
	firstCity.push_back( Area::kTopRight );
	cities.push_back( firstCity );
	ContiguousCity secondCity;
	secondCity.push_back( Area::kRightTop );
	secondCity.push_back( Area::kRight );
	secondCity.push_back( Area::kRightBottom );
	cities.push_back( secondCity );
	ContiguousCity thirdCity;
	thirdCity.push_back( Area::kLeftBottom );
	thirdCity.push_back( Area::kLeft );
	thirdCity.push_back( Area::kLeftTop );
	cities.push_back( thirdCity );

	std::map< ContiguousField, std::vector< ContiguousCity > > citiesPerField;
	citiesPerField[ firstField ] = cities;

	Tile const tileEO
	(
		Tile::kCenterProject,
		"eO",
		fields,
		roads,
		cities,
		citiesPerField
	);

	return tileEO;
}

Model::Tile
Model::createTileEP()
{
	std::vector< ContiguousField > fields;
	ContiguousField firstField;
	firstField.push_back( Area::kBottomRight );
	firstField.push_back( Area::kBottom );
	firstField.push_back( Area::kBottomLeft );
	firstField.push_back( Area::kCentral );
	fields.push_back( firstField );

	std::vector< ContiguousRoad > roads;

	std::vector< ContiguousCity > cities;
	ContiguousCity firstCity;
	firstCity.push_back( Area::kTopLeft );
	firstCity.push_back( Area::kTop );
	firstCity.push_back( Area::kTopRight );
	firstCity.push_back( Area::kRightTop );
	firstCity.push_back( Area::kRight );
	firstCity.push_back( Area::kRightBottom );
	cities.push_back( firstCity );
	ContiguousCity secondCity;
	secondCity.push_back( Area::kLeftBottom );
	secondCity.push_back( Area::kLeft );
	secondCity.push_back( Area::kLeftTop );
	cities.push_back( secondCity );

	std::map< ContiguousField, std::vector< ContiguousCity > > citiesPerField;
	citiesPerField[ firstField ] = cities;

	std::vector< Area::Area > shields( 1, Area::kTopRight );

	Tile const tileEP
	(
		Tile::kCenterProject,
		"eP",
		fields,
		roads,
		cities,
		citiesPerField,
		shields
	);

	return tileEP;
}

Model::Tile
Model::createTileEQ()
{
	std::vector< ContiguousField > fields;
	ContiguousField const firstField( 1, Area::kTopLeft );
	ContiguousField const secondField( 1, Area::kTopRight );
	ContiguousField const thirdField( 1, Area::kBottomRight );
	ContiguousField const fourthField( 1, Area::kBottomLeft );
	fields.push_back( firstField );
	fields.push_back( secondField );
	fields.push_back( thirdField );
	fields.push_back( fourthField );

	std::vector< ContiguousRoad > roads;
	ContiguousRoad const firstRoad( 1, Area::kTop );
	ContiguousRoad const secondRoad( 1, Area::kBottom );
	roads.push_back( firstRoad );
	roads.push_back( secondRoad );

	std::vector< ContiguousCity > cities;
	ContiguousCity firstCity;
	firstCity.push_back( Area::kRightTop );
	firstCity.push_back( Area::kRight );
	firstCity.push_back( Area::kRightBottom );
	firstCity.push_back( Area::kLeftBottom );
	firstCity.push_back( Area::kLeft );
	firstCity.push_back( Area::kLeftTop );
	firstCity.push_back( Area::kCentral );
	cities.push_back( firstCity );

	std::map< ContiguousField, std::vector< ContiguousCity > > citiesPerField;
	citiesPerField[ firstField ] = cities;
	citiesPerField[ secondField ] = cities;
	citiesPerField[ thirdField ] = cities;
	citiesPerField[ fourthField ] = cities;

	std::vector< Area::Area > shields( 1, Area::kRight );

	Tile const tileEQ
	(
		Tile::kCenterProject,
		"eQ",
		fields,
		roads,
		cities,
		citiesPerField,
		shields
	);

	return tileEQ;
}

std::vector< Model::Tile >
Model::createBaseGameTiles()
{
	std::vector< Tile > tiles;
	for (int i = 0; i < 2; ++i)
	{
		tiles.push_back( createTileA() );
	}
	for (int i = 0; i < 4; ++i)
	{
		tiles.push_back( createTileB() );
	}
	tiles.push_back( createTileC() );
	//3 instead of 4, will facilitate easy starting tile
	for (int i = 0; i < 3; ++i)
	{
		tiles.push_back( createTileD() );
	}
	for (int i = 0; i < 5; ++i)
	{
		tiles.push_back( createTileE() );
	}
	for (int i = 0; i < 2; ++i)
	{
		tiles.push_back( createTileF() );
	}
	tiles.push_back( createTileG() );
	for (int i = 0; i < 3; ++i)
	{
		tiles.push_back( createTileH() );
	}
	for (int i = 0; i < 2; ++i)
	{
		tiles.push_back( createTileI() );
	}
	for (int i = 0; i < 3; ++i)
	{
		tiles.push_back( createTileJ() );
	}
	for (int i = 0; i < 3; ++i)
	{
		tiles.push_back( createTileK() );
	}
	for (int i = 0; i < 3; ++i)
	{
		tiles.push_back( createTileL() );
	}
	for (int i = 0; i < 2; ++i)
	{
		tiles.push_back( createTileM() );
	}
	for (int i = 0; i < 3; ++i)
	{
		tiles.push_back( createTileN() );
	}
	for (int i = 0; i < 2; ++i)
	{
		tiles.push_back( createTileO() );
	}
	for (int i = 0; i < 3; ++i)
	{
		tiles.push_back( createTileP() );
	}
	tiles.push_back( createTileQ() );
	for (int i = 0; i < 3; ++i)
	{
		tiles.push_back( createTileR() );
	}
	for (int i = 0; i < 2; ++i)
	{
		tiles.push_back( createTileS() );
	}
	tiles.push_back( createTileT() );
	for (int i = 0; i < 8; ++i)
	{
		tiles.push_back( createTileU() );
	}
	for (int i = 0; i < 9; ++i)
	{
		tiles.push_back( createTileV() );
	}
	for (int i = 0; i < 4; ++i)
	{
		tiles.push_back( createTileW() );
	}
	tiles.push_back( createTileX() );

	std::srand( std::time( 0 ) );
	std::random_shuffle( tiles.begin(), tiles.end() );
	//add starting tile
	tiles.push_back( createTileD() );

	return tiles;
}

std::vector< Model::Tile >
Model::createTheExpansionTiles()
{
	std::vector< Tile > tiles;
	tiles.push_back( createTileEA() );
	tiles.push_back( createTileEB() );
	tiles.push_back( createTileEC() );
	tiles.push_back( createTileED() );
	tiles.push_back( createTileEE() );
	tiles.push_back( createTileEF() );
	tiles.push_back( createTileEG() );
	tiles.push_back( createTileEH() );
	tiles.push_back( createTileEI() );
	tiles.push_back( createTileEJ() );
	tiles.push_back( createTileEK() );
	tiles.push_back( createTileEK() );
	tiles.push_back( createTileEL() );
	tiles.push_back( createTileEM() );
	tiles.push_back( createTileEN() );
	tiles.push_back( createTileEO() );
	tiles.push_back( createTileEP() );
	tiles.push_back( createTileEQ() );
	return tiles;
}

std::vector< Model::Tile >
Model::createTiles( std::set< Utils::Expansion::Type > const & inExpansions )
{
	std::vector< Tile > tiles;
	// Base game
	std::vector< Tile > const baseGameTiles = createBaseGameTiles();
	tiles.insert( tiles.end(), baseGameTiles.begin(), baseGameTiles.end() );
	Tile const startTile = tiles.back();
	tiles.pop_back();
	// Expansions
	if ( inExpansions.count( Utils::Expansion::kTheExpansion ) == 1 )
	{
		std::vector< Tile > const expansionTiles = createTheExpansionTiles();
		tiles.insert( tiles.end(), expansionTiles.begin(), expansionTiles.end() );
	}
	// Shuffle the tiles
	std::srand( std::time( 0 ) );
	std::random_shuffle( tiles.begin(), tiles.end() );
	tiles.push_back( startTile );

	return tiles;
}

std::vector< Model::Tile >
Model::createTiles( std::string const & inTiles )
{
	std::vector< Tile > tiles;

	std::size_t startPos = 0;
	std::size_t const size = inTiles.size();
	while ( startPos < size )
	{
		std::size_t const length = getLength( inTiles, startPos );
		std::string const id = inTiles.substr( startPos, length );
		tiles.push_back( createTile( id ) );
		startPos += length;
	}

	return tiles;
}

std::map< Model::Piece::PieceType, std::size_t >
Model::createPieces( std::set< Utils::Expansion::Type > const & inExpansions )
{
	std::map< Piece::PieceType, std::size_t > pieces;
	// Base game
	pieces[ Piece::kFollower ] = 6;
	// Expansions
	if ( inExpansions.count( Utils::Expansion::kTheExpansion ) == 1 )
	{
		pieces[ Piece::kLargeFollower ] = 1;
	}
	return pieces;
}