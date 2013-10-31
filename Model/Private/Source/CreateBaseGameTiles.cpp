#include "CreateBaseGameTiles.h"

#include <algorithm>
#include <ctime>

Tile
cbgt::createTileA()
{
	std::vector< Tile::ContiguousField > fields;
	Tile::ContiguousField firstField;
	firstField.push_back(FRCArea::TopLeft);
	firstField.push_back(FRCArea::TopRight);
	firstField.push_back(FRCArea::RightTop);
	firstField.push_back(FRCArea::RightBottom);
	firstField.push_back(FRCArea::BottomRight);
	firstField.push_back(FRCArea::BottomLeft);
	firstField.push_back(FRCArea::LeftBottom);
	firstField.push_back(FRCArea::LeftTop);
	fields.push_back(firstField);

	std::vector< Tile::ContiguousRoad > roads;
	Tile::ContiguousRoad firstRoad;
	firstRoad.push_back(FRCArea::Bottom);
	roads.push_back(firstRoad);

	std::vector< Tile::ContiguousCity > cities;

	std::map< Tile::ContiguousField, std::vector< Tile::ContiguousCity > > citiesPerField;
	std::vector< Tile::ContiguousCity > firstFieldCities;
	citiesPerField.insert( std::pair< Tile::ContiguousField, std::vector< Tile::ContiguousCity > >(firstField, firstFieldCities));

	Tile tileA = Tile(Tile::Cloister, "A", fields, roads, cities, citiesPerField);

	return tileA;
}

Tile
cbgt::createTileB()
{
	std::vector< Tile::ContiguousField > fields;
	Tile::ContiguousField firstField;
	firstField.push_back(FRCArea::TopLeft);
	firstField.push_back(FRCArea::TopRight);
	firstField.push_back(FRCArea::RightTop);
	firstField.push_back(FRCArea::RightBottom);
	firstField.push_back(FRCArea::BottomRight);
	firstField.push_back(FRCArea::BottomLeft);
	firstField.push_back(FRCArea::LeftBottom);
	firstField.push_back(FRCArea::LeftTop);
	fields.push_back(firstField);

	std::vector< Tile::ContiguousRoad > roads;

	std::vector< Tile::ContiguousCity > cities;

	std::map< Tile::ContiguousField, std::vector< Tile::ContiguousCity > > citiesPerField;
	std::vector< Tile::ContiguousCity > firstFieldCities;
	citiesPerField.insert( std::pair< Tile::ContiguousField, std::vector< Tile::ContiguousCity > >(firstField, firstFieldCities));

	Tile tileB = Tile(Tile::Cloister, "B", fields, roads, cities, citiesPerField);

	return tileB;
}

Tile
cbgt::createTileC()
{
	std::vector< Tile::ContiguousField > fields;

	std::vector< Tile::ContiguousRoad > roads;

	std::vector< Tile::ContiguousCity > cities;
	Tile::ContiguousCity city;
	city.push_back(FRCArea::Top);
	city.push_back(FRCArea::Right);
	city.push_back(FRCArea::Bottom);
	city.push_back(FRCArea::Left);
	cities.push_back(city);

	std::map< Tile::ContiguousField, std::vector< Tile::ContiguousCity > > citiesPerField;

	std::vector< FRCArea::CityArea > shields;
	shields.push_back(FRCArea::Top);

	Tile tileC = Tile(Tile::Nothing, "C", fields, roads, cities, citiesPerField, shields);

	return tileC;
}

Tile
cbgt::createTileD()
{
	std::vector< Tile::ContiguousField > fields;
	std::vector< FRCArea::FieldArea > firstField;
	firstField.push_back(FRCArea::TopRight);
	firstField.push_back(FRCArea::BottomRight);
	std::vector< FRCArea::FieldArea > secondField;
	secondField.push_back(FRCArea::BottomLeft);
	secondField.push_back(FRCArea::LeftBottom);
	secondField.push_back(FRCArea::LeftTop);
	secondField.push_back(FRCArea::TopLeft);
	fields.push_back(firstField);
	fields.push_back(secondField);

	std::vector< Tile::ContiguousRoad > roads;
	std::vector< FRCArea::RoadArea > firstRoad;
	firstRoad.push_back(FRCArea::Top);
	firstRoad.push_back(FRCArea::Bottom);
	roads.push_back(firstRoad);

	std::vector< Tile::ContiguousCity > cities;
	std::vector< FRCArea::CityArea > firstCity;
	firstCity.push_back(FRCArea::Right);
	cities.push_back(firstCity);

	std::map< Tile::ContiguousField, std::vector< Tile::ContiguousCity > > citiesPerField;
	std::vector< Tile::ContiguousCity > firstFieldCities;
	firstFieldCities.push_back( firstCity );
	std::vector< Tile::ContiguousCity > secondFieldCities;

	citiesPerField.insert( std::pair< Tile::ContiguousField, std::vector< Tile::ContiguousCity > >(firstField, firstFieldCities));
	citiesPerField.insert( std::pair< Tile::ContiguousField, std::vector< Tile::ContiguousCity > >(secondField, secondFieldCities));

	Tile tileD = Tile(Tile::Nothing, "D", fields, roads, cities, citiesPerField);

	return tileD;
}

Tile
cbgt::createTileE()
{
	//create fieldarray
	std::vector< Tile::ContiguousField > fields;
	std::vector< FRCArea::FieldArea > firstField;
	//define first contiguous field
	firstField.push_back(FRCArea::RightTop);
	firstField.push_back(FRCArea::RightBottom);
	firstField.push_back(FRCArea::BottomRight);
	firstField.push_back(FRCArea::BottomLeft);
	firstField.push_back(FRCArea::LeftBottom);
	firstField.push_back(FRCArea::LeftTop);
	fields.push_back(firstField);

	//Define city
	std::vector< Tile::ContiguousCity > cities;
	std::vector< FRCArea::CityArea > firstCity;
	firstCity.push_back(FRCArea::Top);
	cities.push_back(firstCity);
	std::map< Tile::ContiguousField, std::vector< Tile::ContiguousCity > > citiesPerField;
	std::vector< Tile::ContiguousCity > firstFieldCities;
	firstFieldCities.push_back( firstCity );
	citiesPerField.insert( std::pair< Tile::ContiguousField, std::vector< Tile::ContiguousCity > >(firstField, firstFieldCities));
	//Define empty road
	std::vector< Tile::ContiguousRoad> roads;
	//Tile constructor, Top Right Bottom Left Center mId fields roads cities citiesPerField shields
	Tile tileE = Tile(Tile::Nothing, "E", fields, roads, cities, citiesPerField);

	return tileE;
}

Tile
cbgt::createTileF()
{
	//Field
	std::vector<Tile::ContiguousField> fields;

	std::vector<FRCArea::FieldArea> firstField;
	firstField.push_back(FRCArea::TopLeft);
	firstField.push_back(FRCArea::TopRight);

	std::vector<FRCArea::FieldArea> secondField;
	secondField.push_back(FRCArea::BottomRight);
	secondField.push_back(FRCArea::BottomLeft);

	fields.push_back(firstField);
	fields.push_back(secondField);

	//City
	std::vector<Tile::ContiguousCity> cities;
	std::vector<FRCArea::CityArea> firstCity;
	firstCity.push_back(FRCArea::Right);
	firstCity.push_back(FRCArea::Left);
	cities.push_back(firstCity);

	//Cities per field
	std::map< Tile::ContiguousField, std::vector< Tile::ContiguousCity > > citiesPerField;
	std::vector< Tile::ContiguousCity > firstFieldCities;
	firstFieldCities.push_back(firstCity);
	std::vector< Tile::ContiguousCity > secondFieldCities;
	secondFieldCities.push_back(firstCity);

	citiesPerField.insert( std::pair< Tile::ContiguousField, std::vector< Tile::ContiguousCity > >(firstField, firstFieldCities));
	citiesPerField.insert( std::pair< Tile::ContiguousField, std::vector< Tile::ContiguousCity > >(secondField, secondFieldCities));

	//Shield
	std::vector< FRCArea::CityArea > shields;
	shields.push_back(FRCArea::Right);

	//Empty road
	std::vector< Tile::ContiguousRoad> roads;

	Tile tileF = Tile(Tile::Nothing,"F", fields, roads, cities, citiesPerField, shields);
	return tileF;
}

Tile
cbgt::createTileG()
{
	std::vector< Tile::ContiguousField > fields;
	Tile::ContiguousField firstField;
	firstField.push_back(FRCArea::RightTop);
	firstField.push_back(FRCArea::RightBottom);
	Tile::ContiguousField secondField;
	secondField.push_back(FRCArea::LeftBottom);
	secondField.push_back(FRCArea::LeftTop);
	fields.push_back(firstField);
	fields.push_back(secondField);

	std::vector< Tile::ContiguousCity > cities;
	Tile::ContiguousCity firstCity;
	firstCity.push_back(FRCArea::Top);
	firstCity.push_back(FRCArea::Bottom);
	cities.push_back(firstCity);

	std::vector< Tile::ContiguousRoad > roads;

	std::map< Tile::ContiguousField, std::vector< Tile::ContiguousCity > > citiesPerField;
	std::vector< Tile::ContiguousCity > firstFieldCities;
	firstFieldCities.push_back(firstCity);
	std::vector< Tile::ContiguousCity > secondFieldCities;
	secondFieldCities.push_back(firstCity);

	citiesPerField.insert( std::pair< Tile::ContiguousField, std::vector< Tile::ContiguousCity > >(firstField, firstFieldCities));
	citiesPerField.insert( std::pair< Tile::ContiguousField, std::vector< Tile::ContiguousCity > >(secondField, secondFieldCities));

	Tile tileG = Tile(Tile::Nothing, "G", fields, roads, cities, citiesPerField);

	return tileG;
}

Tile
cbgt::createTileH()
{
	std::vector< Tile::ContiguousField > fields;
	Tile::ContiguousField firstField;
	firstField.push_back(FRCArea::TopLeft);
	firstField.push_back(FRCArea::TopRight);
	firstField.push_back(FRCArea::BottomRight);
	firstField.push_back(FRCArea::BottomLeft);
	fields.push_back(firstField);

	std::vector< Tile::ContiguousCity > cities;
	Tile::ContiguousCity firstCity;
	firstCity.push_back(FRCArea::Right);
	cities.push_back(firstCity);
	Tile::ContiguousCity secondCity;
	secondCity.push_back(FRCArea::Left);
	cities.push_back(secondCity);

	std::vector< Tile::ContiguousRoad > roads;

	std::map< Tile::ContiguousField, std::vector< Tile::ContiguousCity > > citiesPerField;
	std::vector< Tile::ContiguousCity > firstFieldCities;
	firstFieldCities.push_back(firstCity);
	firstFieldCities.push_back(secondCity);

	citiesPerField.insert( std::pair< Tile::ContiguousField, std::vector< Tile::ContiguousCity > >(firstField, firstFieldCities));

	Tile tileH = Tile(Tile::Nothing, "H", fields, roads, cities, citiesPerField);

	return tileH;
}

Tile
cbgt::createTileI()
{
	std::vector< Tile::ContiguousField > fields;
	Tile::ContiguousField firstField;
	firstField.push_back(FRCArea::TopLeft);
	firstField.push_back(FRCArea::TopRight);
	firstField.push_back(FRCArea::LeftBottom);
	firstField.push_back(FRCArea::LeftTop);
	fields.push_back(firstField);

	std::vector< Tile::ContiguousCity > cities;
	Tile::ContiguousCity firstCity;
	firstCity.push_back(FRCArea::Right);
	cities.push_back(firstCity);
	Tile::ContiguousCity secondCity;
	secondCity.push_back(FRCArea::Bottom);
	cities.push_back(secondCity);

	std::vector< Tile::ContiguousRoad > roads;

	std::map< Tile::ContiguousField, std::vector< Tile::ContiguousCity > > citiesPerField;
	std::vector< Tile::ContiguousCity > firstFieldCities;
	firstFieldCities.push_back(firstCity);
	firstFieldCities.push_back(secondCity);

	citiesPerField.insert( std::pair< Tile::ContiguousField, std::vector< Tile::ContiguousCity > >(firstField, firstFieldCities));

	Tile tileI = Tile(Tile::Nothing, "I", fields, roads, cities, citiesPerField);

	return tileI;
}

Tile
cbgt::createTileJ()
{
	std::vector< Tile::ContiguousField > fields;
	Tile::ContiguousField firstField;
	firstField.push_back(FRCArea::RightTop);
	firstField.push_back(FRCArea::BottomLeft);
	firstField.push_back(FRCArea::LeftBottom);
	firstField.push_back(FRCArea::LeftTop);
	fields.push_back(firstField);
	Tile::ContiguousField secondField;
	secondField.push_back(FRCArea::RightBottom);
	secondField.push_back(FRCArea::BottomRight);
	fields.push_back(secondField);

	std::vector< Tile::ContiguousCity > cities;
	Tile::ContiguousCity firstCity;
	firstCity.push_back(FRCArea::Top);
	cities.push_back(firstCity);

	std::vector< Tile::ContiguousRoad> roads;
	Tile::ContiguousRoad firstRoad;
	firstRoad.push_back(FRCArea::Right);
	firstRoad.push_back(FRCArea::Bottom);
	roads.push_back(firstRoad);

	std::map< Tile::ContiguousField, std::vector< Tile::ContiguousCity > > citiesPerField;
	std::vector< Tile::ContiguousCity > firstFieldCities;
	firstFieldCities.push_back( firstCity );
	std::vector< Tile::ContiguousCity > secondFieldCities;

	citiesPerField.insert( std::pair< Tile::ContiguousField, std::vector< Tile::ContiguousCity > >(firstField, firstFieldCities));
	citiesPerField.insert( std::pair< Tile::ContiguousField, std::vector< Tile::ContiguousCity > >(secondField, secondFieldCities));

	Tile tileJ = Tile(Tile::Nothing, "J", fields, roads, cities, citiesPerField);
	return tileJ;
}

Tile
cbgt::createTileK()
{
	std::vector< Tile::ContiguousCity > cities;
	std::vector< FRCArea::CityArea > firstCity;
	firstCity.push_back(FRCArea::Right);
	cities.push_back(firstCity);

	std::vector< Tile::ContiguousRoad > roads;
	std::vector< FRCArea::RoadArea > firstRoad;
	firstRoad.push_back(FRCArea::Top);
	firstRoad.push_back(FRCArea::Left);
	roads.push_back(firstRoad);

	std::vector< Tile::ContiguousField > fields;
	std::vector< FRCArea::FieldArea > firstField;
	firstField.push_back(FRCArea::TopRight);
	firstField.push_back(FRCArea::BottomRight);
	firstField.push_back(FRCArea::BottomLeft);
	firstField.push_back(FRCArea::LeftBottom);
	std::vector< FRCArea::FieldArea > secondField;
	secondField.push_back(FRCArea::LeftTop);
	secondField.push_back(FRCArea::TopLeft);
	fields.push_back(firstField);
	fields.push_back(secondField);

	std::map< Tile::ContiguousField, std::vector< Tile::ContiguousCity > > citiesPerField;
	std::vector< Tile::ContiguousCity > firstFieldCities;
	firstFieldCities.push_back( firstCity );
	std::vector< Tile::ContiguousCity > secondFieldCities;

	citiesPerField.insert( std::pair< Tile::ContiguousField, std::vector< Tile::ContiguousCity > >(firstField, firstFieldCities));
	citiesPerField.insert( std::pair< Tile::ContiguousField, std::vector< Tile::ContiguousCity > >(secondField, secondFieldCities));

	Tile tileK = Tile(Tile::Nothing, "K", fields, roads, cities, citiesPerField);

	return tileK;
}

Tile
cbgt::createTileL()
{
	std::vector< Tile::ContiguousCity > cities;
	std::vector< FRCArea::CityArea > firstCity;
	firstCity.push_back(FRCArea::Right);
	cities.push_back(firstCity);

	std::vector< Tile::ContiguousRoad > roads;
	std::vector< FRCArea::RoadArea > firstRoad;
	firstRoad.push_back(FRCArea::Bottom);
	std::vector< FRCArea::RoadArea > secondRoad;
	secondRoad.push_back(FRCArea::Left);
	std::vector< FRCArea::RoadArea > thirdRoad;
	thirdRoad.push_back(FRCArea::Top);
	roads.push_back(firstRoad);
	roads.push_back(secondRoad);
	roads.push_back(thirdRoad);

	std::vector< Tile::ContiguousField > fields;
	std::vector< FRCArea::FieldArea > firstField;
	firstField.push_back(FRCArea::TopRight);
	firstField.push_back(FRCArea::BottomRight);
	std::vector< FRCArea::FieldArea > secondField;
	secondField.push_back(FRCArea::BottomLeft);
	secondField.push_back(FRCArea::LeftBottom);
	std::vector< FRCArea::FieldArea > thirdField;
	thirdField.push_back(FRCArea::LeftTop);
	thirdField.push_back(FRCArea::TopLeft);
	fields.push_back(firstField);
	fields.push_back(secondField);
	fields.push_back(thirdField);

	std::map< Tile::ContiguousField, std::vector< Tile::ContiguousCity > > citiesPerField;
	std::vector< Tile::ContiguousCity > firstFieldCities;
	firstFieldCities.push_back( firstCity );
	std::vector< Tile::ContiguousCity > secondFieldCities;
	std::vector< Tile::ContiguousCity > thirdFieldCities;

	citiesPerField.insert( std::pair< Tile::ContiguousField, std::vector< Tile::ContiguousCity > >(firstField, firstFieldCities));
	citiesPerField.insert( std::pair< Tile::ContiguousField, std::vector< Tile::ContiguousCity > >(secondField, secondFieldCities));
	citiesPerField.insert( std::pair< Tile::ContiguousField, std::vector< Tile::ContiguousCity > >(thirdField, thirdFieldCities));

	Tile tileL = Tile(Tile::Nothing, "L", fields, roads, cities, citiesPerField);

	return tileL;
}

Tile
cbgt::createTileM()
{
	std::vector< Tile::ContiguousField > fields;
	std::vector< FRCArea::FieldArea > firstField;
	firstField.push_back(FRCArea::RightTop);
	firstField.push_back(FRCArea::RightBottom);
	firstField.push_back(FRCArea::BottomRight);
	firstField.push_back(FRCArea::BottomLeft);
	fields.push_back(firstField);

	std::vector< Tile::ContiguousRoad > roads;

	std::vector< Tile::ContiguousCity > cities;
	std::vector< FRCArea::CityArea > firstCity;
	firstCity.push_back(FRCArea::Top);
	firstCity.push_back(FRCArea::Left);
	cities.push_back(firstCity);

	std::map< Tile::ContiguousField, std::vector< Tile::ContiguousCity > > citiesPerField;
	std::vector< Tile::ContiguousCity > firstFieldCities;
	firstFieldCities.push_back( firstCity );

	citiesPerField.insert( std::pair< Tile::ContiguousField, std::vector< Tile::ContiguousCity > >(firstField, firstFieldCities));

	std::vector< FRCArea::CityArea > shields;
	shields.push_back( FRCArea::Top );

	Tile tileM = Tile(Tile::Nothing, "M", fields, roads, cities, citiesPerField, shields);

	return tileM;
}

Tile
cbgt::createTileN()
{
	std::vector< Tile::ContiguousField > fields;
	std::vector< FRCArea::FieldArea > firstField;
	firstField.push_back(FRCArea::RightTop);
	firstField.push_back(FRCArea::RightBottom);
	firstField.push_back(FRCArea::BottomRight);
	firstField.push_back(FRCArea::BottomLeft);
	fields.push_back(firstField);

	std::vector< Tile::ContiguousRoad > roads;

	std::vector< Tile::ContiguousCity > cities;
	std::vector< FRCArea::CityArea > firstCity;
	firstCity.push_back(FRCArea::Top);
	firstCity.push_back(FRCArea::Left);
	cities.push_back(firstCity);

	std::map< Tile::ContiguousField, std::vector< Tile::ContiguousCity > > citiesPerField;
	std::vector< Tile::ContiguousCity > firstFieldCities;
	firstFieldCities.push_back( firstCity );

	citiesPerField.insert( std::pair< Tile::ContiguousField, std::vector< Tile::ContiguousCity > >(firstField, firstFieldCities));

	Tile tileN = Tile(Tile::Nothing, "N", fields, roads, cities, citiesPerField);

	return tileN;
}

Tile
cbgt::createTileO()
{
	std::vector< Tile::ContiguousField > fields;
	std::vector< FRCArea::FieldArea > firstField;
	firstField.push_back(FRCArea::RightTop);
	firstField.push_back(FRCArea::BottomLeft);
	fields.push_back(firstField);
	std::vector< FRCArea::FieldArea > secondField;
	secondField.push_back(FRCArea::RightBottom);
	secondField.push_back(FRCArea::BottomRight);
	fields.push_back(secondField);


	std::vector< Tile::ContiguousRoad > roads;
	Tile::ContiguousRoad road;
	road.push_back(FRCArea::Right);
	road.push_back(FRCArea::Bottom);
	roads.push_back(road);

	std::vector< Tile::ContiguousCity > cities;
	std::vector< FRCArea::CityArea > firstCity;
	firstCity.push_back(FRCArea::Top);
	firstCity.push_back(FRCArea::Left);
	cities.push_back(firstCity);

	std::map< Tile::ContiguousField, std::vector< Tile::ContiguousCity > > citiesPerField;
	std::vector< Tile::ContiguousCity > firstFieldCities;
	firstFieldCities.push_back( firstCity );
	std::vector< Tile::ContiguousCity > secondFieldCities;

	citiesPerField.insert( std::pair< Tile::ContiguousField, std::vector< Tile::ContiguousCity > >(firstField, firstFieldCities) );
	citiesPerField.insert( std::pair< Tile::ContiguousField, std::vector< Tile::ContiguousCity > >(secondField, secondFieldCities) );

	std::vector< FRCArea::CityArea > shields;
	shields.push_back( FRCArea::Top );

	Tile tileO = Tile(Tile::Nothing, "O", fields, roads, cities, citiesPerField, shields);

	return tileO;
}

Tile
cbgt::createTileP()
{
	std::vector< Tile::ContiguousField > fields;
	std::vector< FRCArea::FieldArea > firstField;
	firstField.push_back(FRCArea::RightTop);
	firstField.push_back(FRCArea::BottomLeft);
	fields.push_back(firstField);
	std::vector< FRCArea::FieldArea > secondField;
	secondField.push_back(FRCArea::RightBottom);
	secondField.push_back(FRCArea::BottomRight);
	fields.push_back(secondField);


	std::vector< Tile::ContiguousRoad > roads;
	Tile::ContiguousRoad road;
	road.push_back(FRCArea::Right);
	road.push_back(FRCArea::Bottom);
	roads.push_back(road);

	std::vector< Tile::ContiguousCity > cities;
	std::vector< FRCArea::CityArea > firstCity;
	firstCity.push_back(FRCArea::Top);
	firstCity.push_back(FRCArea::Left);
	cities.push_back(firstCity);

	std::map< Tile::ContiguousField, std::vector< Tile::ContiguousCity > > citiesPerField;
	std::vector< Tile::ContiguousCity > firstFieldCities;
	firstFieldCities.push_back( firstCity );
	std::vector< Tile::ContiguousCity > secondFieldCities;

	citiesPerField.insert( std::pair< Tile::ContiguousField, std::vector< Tile::ContiguousCity > >(firstField, firstFieldCities) );
	citiesPerField.insert( std::pair< Tile::ContiguousField, std::vector< Tile::ContiguousCity > >(secondField, secondFieldCities) );

	Tile tileP = Tile(Tile::Nothing, "P", fields, roads, cities, citiesPerField);

	return tileP;
}

Tile
cbgt::createTileQ()
{
	std::vector< Tile::ContiguousField > fields;
	Tile::ContiguousField firstField;
	firstField.push_back(FRCArea::BottomRight);
	firstField.push_back(FRCArea::BottomLeft);
	fields.push_back(firstField);

	std::vector< Tile::ContiguousRoad > roads;

	std::vector< Tile::ContiguousCity > cities;
	Tile::ContiguousCity city;
	city.push_back(FRCArea::Top);
	city.push_back(FRCArea::Right);
	city.push_back(FRCArea::Left);
	cities.push_back(city);

	std::map< Tile::ContiguousField, std::vector< Tile::ContiguousCity > > citiesPerField;
	std::vector< Tile::ContiguousCity > firstFieldCities;
	firstFieldCities.push_back( city );
	citiesPerField.insert( std::pair< Tile::ContiguousField, std::vector< Tile::ContiguousCity > >( firstField, firstFieldCities ) );

	std::vector< FRCArea::CityArea > shields;
	shields.push_back(FRCArea::Top);

	Tile tileQ = Tile(Tile::Nothing, "Q", fields, roads, cities, citiesPerField, shields);

	return tileQ;
}

Tile
cbgt::createTileR()
{
	std::vector< Tile::ContiguousField > fields;
	Tile::ContiguousField firstField;
	firstField.push_back(FRCArea::BottomRight);
	firstField.push_back(FRCArea::BottomLeft);
	fields.push_back(firstField);

	std::vector< Tile::ContiguousRoad > roads;

	std::vector< Tile::ContiguousCity > cities;
	Tile::ContiguousCity city;
	city.push_back(FRCArea::Top);
	city.push_back(FRCArea::Right);
	city.push_back(FRCArea::Left);
	cities.push_back(city);

	std::map< Tile::ContiguousField, std::vector< Tile::ContiguousCity > > citiesPerField;
	std::vector< Tile::ContiguousCity > firstFieldCities;
	firstFieldCities.push_back( city );
	citiesPerField.insert( std::pair< Tile::ContiguousField, std::vector< Tile::ContiguousCity > >( firstField, firstFieldCities ) );

	Tile tileR = Tile(Tile::Nothing, "R", fields, roads, cities, citiesPerField);

	return tileR;
}

Tile
cbgt::createTileS()
{
	std::vector< Tile::ContiguousField > fields;
	Tile::ContiguousField firstField;
	firstField.push_back(FRCArea::BottomRight);
	fields.push_back(firstField);
	Tile::ContiguousField secondField;
	secondField.push_back(FRCArea::BottomLeft);
	fields.push_back(secondField);

	std::vector< Tile::ContiguousRoad > roads;
	Tile::ContiguousRoad road;
	road.push_back(FRCArea::Bottom);
	roads.push_back(road);

	std::vector< Tile::ContiguousCity > cities;
	Tile::ContiguousCity city;
	city.push_back(FRCArea::Top);
	city.push_back(FRCArea::Right);
	city.push_back(FRCArea::Left);
	cities.push_back(city);

	std::map< Tile::ContiguousField, std::vector< Tile::ContiguousCity > > citiesPerField;
	std::vector< Tile::ContiguousCity > firstFieldCities;
	firstFieldCities.push_back( city );
	std::vector< Tile::ContiguousCity > secondFieldCities;
	secondFieldCities.push_back( city );
	citiesPerField.insert( std::pair< Tile::ContiguousField, std::vector< Tile::ContiguousCity > >( firstField, firstFieldCities ) );
	citiesPerField.insert( std::pair< Tile::ContiguousField, std::vector< Tile::ContiguousCity > >( secondField, secondFieldCities ) );

	std::vector< FRCArea::CityArea > shields;
	shields.push_back(FRCArea::Top);

	Tile tileS = Tile(Tile::Nothing, "S", fields, roads, cities, citiesPerField, shields);

	return tileS;
}

Tile
cbgt::createTileT()
{
	std::vector< Tile::ContiguousField > fields;
	Tile::ContiguousField firstField;
	firstField.push_back(FRCArea::BottomRight);
	fields.push_back(firstField);
	Tile::ContiguousField secondField;
	secondField.push_back(FRCArea::BottomLeft);
	fields.push_back(secondField);

	std::vector< Tile::ContiguousRoad > roads;
	Tile::ContiguousRoad road;
	road.push_back(FRCArea::Bottom);
	roads.push_back(road);

	std::vector< Tile::ContiguousCity > cities;
	Tile::ContiguousCity city;
	city.push_back(FRCArea::Top);
	city.push_back(FRCArea::Right);
	city.push_back(FRCArea::Left);
	cities.push_back(city);

	std::map< Tile::ContiguousField, std::vector< Tile::ContiguousCity > > citiesPerField;
	std::vector< Tile::ContiguousCity > firstFieldCities;
	firstFieldCities.push_back( city );
	std::vector< Tile::ContiguousCity > secondFieldCities;
	secondFieldCities.push_back( city );
	citiesPerField.insert( std::pair< Tile::ContiguousField, std::vector< Tile::ContiguousCity > >( firstField, firstFieldCities ) );
	citiesPerField.insert( std::pair< Tile::ContiguousField, std::vector< Tile::ContiguousCity > >( secondField, secondFieldCities ) );

	Tile tileT = Tile(Tile::Nothing, "T",     fields, roads, cities, citiesPerField);

	return tileT;
}

Tile
cbgt::createTileU()
{
	std::vector< Tile::ContiguousField > fields;
	Tile::ContiguousField firstField;
	firstField.push_back(FRCArea::TopLeft);
	firstField.push_back(FRCArea::BottomLeft);
	firstField.push_back(FRCArea::LeftBottom);
	firstField.push_back(FRCArea::LeftTop);
	fields.push_back(firstField);
	Tile::ContiguousField secondField;
	secondField.push_back(FRCArea::TopRight);
	secondField.push_back(FRCArea::RightTop);
	secondField.push_back(FRCArea::RightBottom);
	secondField.push_back(FRCArea::BottomRight);
	fields.push_back(secondField);

	std::vector< Tile::ContiguousRoad > roads;
	Tile::ContiguousRoad road;
	road.push_back(FRCArea::Top);
	road.push_back(FRCArea::Bottom);
	roads.push_back(road);

	std::vector< Tile::ContiguousCity> cities;

	std::map< Tile::ContiguousField, std::vector< Tile::ContiguousCity > > citiesPerField;
	std::vector< Tile::ContiguousCity > firstFieldCities;
	std::vector< Tile::ContiguousCity > secondFieldCities;
	citiesPerField.insert( std::pair< Tile::ContiguousField, std::vector< Tile::ContiguousCity > >( firstField, firstFieldCities ) );
	citiesPerField.insert( std::pair< Tile::ContiguousField, std::vector< Tile::ContiguousCity > >( secondField, secondFieldCities ) );

	Tile tileU = Tile(Tile::Nothing, "U", fields, roads, cities, citiesPerField);

	return tileU;
}

Tile
cbgt::createTileV()
{
	std::vector< Tile::ContiguousField > fields;
	Tile::ContiguousField firstField;
	firstField.push_back(FRCArea::TopLeft);
	firstField.push_back(FRCArea::TopRight);
	firstField.push_back(FRCArea::RightTop);
	firstField.push_back(FRCArea::RightBottom);
	firstField.push_back(FRCArea::BottomRight);
	firstField.push_back(FRCArea::LeftTop);
	fields.push_back(firstField);
	Tile::ContiguousField secondField;
	secondField.push_back(FRCArea::BottomLeft);
	secondField.push_back(FRCArea::LeftBottom);
	fields.push_back(secondField);

	std::vector< Tile::ContiguousRoad > roads;
	Tile::ContiguousRoad road;
	road.push_back(FRCArea::Bottom);
	road.push_back(FRCArea::Left);
	roads.push_back(road);

	std::vector< Tile::ContiguousCity> cities;

	std::map< Tile::ContiguousField, std::vector< Tile::ContiguousCity > > citiesPerField;
	std::vector< Tile::ContiguousCity > firstFieldCities;
	std::vector< Tile::ContiguousCity > secondFieldCities;
	citiesPerField.insert( std::pair< Tile::ContiguousField, std::vector< Tile::ContiguousCity > >( firstField, firstFieldCities ) );
	citiesPerField.insert( std::pair< Tile::ContiguousField, std::vector< Tile::ContiguousCity > >( secondField, secondFieldCities ) );

	Tile tileV = Tile(Tile::Nothing, "V", fields, roads, cities, citiesPerField);

	return tileV;
}

Tile
cbgt::createTileW()
{
	std::vector< Tile::ContiguousField > fields;
	Tile::ContiguousField firstField;
	firstField.push_back(FRCArea::TopLeft);
	firstField.push_back(FRCArea::TopRight);
	firstField.push_back(FRCArea::RightTop);
	firstField.push_back(FRCArea::LeftTop);
	fields.push_back(firstField);
	Tile::ContiguousField secondField;
	secondField.push_back(FRCArea::RightBottom);
	secondField.push_back(FRCArea::BottomRight);
	fields.push_back(secondField);
	Tile::ContiguousField thirdField;
	thirdField.push_back(FRCArea::BottomLeft);
	thirdField.push_back(FRCArea::LeftBottom);
	fields.push_back(thirdField);

	std::vector< Tile::ContiguousRoad > roads;
	Tile::ContiguousRoad firstRoad;
	firstRoad.push_back(FRCArea::Right);
	roads.push_back(firstRoad);
	Tile::ContiguousRoad secondRoad;
	secondRoad.push_back(FRCArea::Bottom);
	roads.push_back(secondRoad);
	Tile::ContiguousRoad thirdRoad;
	thirdRoad.push_back(FRCArea::Left);
	roads.push_back(thirdRoad);

	std::vector< Tile::ContiguousCity> cities;

	std::map< Tile::ContiguousField, std::vector< Tile::ContiguousCity > > citiesPerField;
	std::vector< Tile::ContiguousCity > firstFieldCities;
	std::vector< Tile::ContiguousCity > secondFieldCities;
	std::vector< Tile::ContiguousCity > thirdFieldCities;
	citiesPerField.insert( std::pair< Tile::ContiguousField, std::vector< Tile::ContiguousCity > >( firstField, firstFieldCities ) );
	citiesPerField.insert( std::pair< Tile::ContiguousField, std::vector< Tile::ContiguousCity > >( secondField, secondFieldCities ) );
	citiesPerField.insert( std::pair< Tile::ContiguousField, std::vector< Tile::ContiguousCity > >( thirdField, thirdFieldCities ) );

	Tile tileW = Tile(Tile::Nothing, "W", fields, roads, cities, citiesPerField);

	return tileW;
}

Tile
cbgt::createTileX()
{
	std::vector< Tile::ContiguousField > fields;
	Tile::ContiguousField firstField;
	firstField.push_back(FRCArea::TopLeft);
	firstField.push_back(FRCArea::LeftTop);
	fields.push_back(firstField);
	Tile::ContiguousField secondField;
	secondField.push_back(FRCArea::TopRight);
	secondField.push_back(FRCArea::RightTop);
	fields.push_back(secondField);
	Tile::ContiguousField thirdField;
	thirdField.push_back(FRCArea::RightBottom);
	thirdField.push_back(FRCArea::BottomRight);
	fields.push_back(thirdField);
	Tile::ContiguousField fourthField;
	fourthField.push_back(FRCArea::BottomLeft);
	fourthField.push_back(FRCArea::LeftBottom);
	fields.push_back(fourthField);

	std::vector< Tile::ContiguousRoad > roads;
	Tile::ContiguousRoad firstRoad;
	firstRoad.push_back(FRCArea::Top);
	roads.push_back(firstRoad);
	Tile::ContiguousRoad secondRoad;
	secondRoad.push_back(FRCArea::Right);
	roads.push_back(secondRoad);
	Tile::ContiguousRoad thirdRoad;
	thirdRoad.push_back(FRCArea::Bottom);
	roads.push_back(thirdRoad);
	Tile::ContiguousRoad fourthRoad;
	fourthRoad.push_back(FRCArea::Left);
	roads.push_back(fourthRoad);

	std::vector< Tile::ContiguousCity> cities;

	std::map< Tile::ContiguousField, std::vector< Tile::ContiguousCity > > citiesPerField;
	std::vector< Tile::ContiguousCity > firstFieldCities;
	std::vector< Tile::ContiguousCity > secondFieldCities;
	std::vector< Tile::ContiguousCity > thirdFieldCities;
	std::vector< Tile::ContiguousCity > fourthFieldCities;
	citiesPerField.insert( std::pair< Tile::ContiguousField, std::vector< Tile::ContiguousCity > >( firstField, firstFieldCities ) );
	citiesPerField.insert( std::pair< Tile::ContiguousField, std::vector< Tile::ContiguousCity > >( secondField, secondFieldCities ) );
	citiesPerField.insert( std::pair< Tile::ContiguousField, std::vector< Tile::ContiguousCity > >( thirdField, thirdFieldCities ) );
	citiesPerField.insert( std::pair< Tile::ContiguousField, std::vector< Tile::ContiguousCity > >( fourthField, fourthFieldCities ) );

	Tile tileX = Tile(Tile::Nothing, "X", fields, roads, cities, citiesPerField);

	return tileX;
}

Tile
cbgt::createTile( char inChar )
{
	switch ( inChar )
	{
		case 'A':
		case 'a':
			return createTileA();
		case 'B':
		case 'b':
			return createTileB();
		case 'C':
		case 'c':
			return createTileC();
		case 'D':
		case 'd':
			return createTileD();
		case 'E':
		case 'e':
			return createTileE();
		case 'F':
		case 'f':
			return createTileF();
		case 'G':
		case 'g':
			return createTileG();
		case 'H':
		case 'h':
			return createTileH();
		case 'I':
		case 'i':
			return createTileI();
		case 'J':
		case 'j':
			return createTileJ();
		case 'K':
		case 'k':
			return createTileK();
		case 'L':
		case 'l':
			return createTileL();
		case 'M':
		case 'm':
			return createTileM();
		case 'N':
		case 'n':
			return createTileN();
		case 'O':
		case 'o':
			return createTileO();
		case 'P':
		case 'p':
			return createTileP();
		case 'Q':
		case 'q':
			return createTileQ();
		case 'R':
		case 'r':
			return createTileR();
		case 'S':
		case 's':
			return createTileS();
		case 'T':
		case 't':
			return createTileT();
		case 'U':
		case 'u':
			return createTileU();
		case 'V':
		case 'v':
			return createTileV();
		case 'W':
		case 'w':
			return createTileW();
		case 'X':
		case 'x':
			return createTileX();
		default:
			return createTileD();
	}
}

std::vector< Tile >
createBaseGameTiles()
{
	using namespace cbgt;
	std::vector< Tile > tiles;
	for (int i = 0; i < 2; ++i)
	{
		tiles.push_back(createTileA());
	}
	for (int i = 0; i < 4; ++i)
	{
		tiles.push_back(createTileB());
	}
	tiles.push_back(createTileC());
	//3 instead of 4, will facilitate easy starting tile
	for (int i = 0; i < 3; ++i)
	{
		tiles.push_back(createTileD());
	}
	for (int i = 0; i < 5; ++i)
	{
		tiles.push_back(createTileE());
	}
	for (int i = 0; i < 2; ++i)
	{
		tiles.push_back(createTileF());
	}
	tiles.push_back(createTileG());
	for (int i = 0; i < 3; ++i)
	{
		tiles.push_back(createTileH());
	}
	for (int i = 0; i < 2; ++i)
	{
		tiles.push_back(createTileI());
	}
	for (int i = 0; i < 3; ++i)
	{
		tiles.push_back(createTileJ());
	}
	for (int i = 0; i < 3; ++i)
	{
		tiles.push_back(createTileK());
	}
	for (int i = 0; i < 3; ++i)
	{
		tiles.push_back(createTileL());
	}
	for (int i = 0; i < 2; ++i)
	{
		tiles.push_back(createTileM());
	}
	for (int i = 0; i < 3; ++i)
	{
		tiles.push_back(createTileN());
	}
	for (int i = 0; i < 2; ++i)
	{
		tiles.push_back(createTileO());
	}
	for (int i = 0; i < 3; ++i)
	{
		tiles.push_back(createTileP());
	}
	tiles.push_back(createTileQ());
	for (int i = 0; i < 3; ++i)
	{
		tiles.push_back(createTileR());
	}
	for (int i = 0; i < 2; ++i)
	{
		tiles.push_back(createTileS());
	}
	tiles.push_back(createTileT());
	for (int i = 0; i < 8; ++i)
	{
		tiles.push_back(createTileU());
	}
	for (int i = 0; i < 9; ++i)
	{
		tiles.push_back(createTileV());
	}
	for (int i = 0; i < 4; ++i)
	{
		tiles.push_back(createTileW());
	}
	tiles.push_back(createTileX());

	std::srand(std::time(0));
	std::random_shuffle(tiles.begin(), tiles.end());
	//add starting tile
	tiles.push_back(createTileD());

	return tiles;
}

std::vector< Tile >
createTiles( std::string const & inTiles )
{
	std::vector< Tile > tiles;

	for ( std::string::const_iterator it = inTiles.begin(); it != inTiles.end(); ++it )
	{
		tiles.push_back( cbgt::createTile( *it ) );
	}

	return tiles;
}

std::vector< Piece >
createBaseGamePieces( Color::Color inColor )
{
	std::vector< Piece > pieces;
	for ( unsigned int i = 0; i < 6; ++i)
	{
		pieces.push_back( Piece( Piece::Follower, inColor ) );
	}
	return pieces;
}
