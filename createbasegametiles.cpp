#include "createbasegametiles.h"

#include <algorithm>

namespace
{
Tile
createTileA()
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

    std::vector< FRCArea::CityArea > shields;

    Tile tileA = Tile(Tile::Field, Tile::Field, Tile::Road, Tile::Field, Tile::Cloister, "A",
                          fields, roads, cities, citiesPerField, shields);

    return tileA;
}

Tile
createTileB()
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

    std::vector< FRCArea::CityArea > shields;

    Tile tileB = Tile(Tile::Field, Tile::Field, Tile::Field, Tile::Field, Tile::Cloister, "B",
                          fields, roads, cities, citiesPerField, shields);

    return tileB;
}

Tile
createTileC()
{
    std::vector< Tile::ContiguousField > fields;

    std::vector< Tile::ContiguousRoad > roads;

    std::vector< Tile::ContiguousCity > cities;
    Tile::ContiguousCity city;
    city.push_back(FRCArea::Top);
    city.push_back(FRCArea::Right);
    city.push_back(FRCArea::Bottom);
    city.push_back(FRCArea::Left);

    std::map< Tile::ContiguousField, std::vector< Tile::ContiguousCity > > citiesPerField;

    std::vector< FRCArea::CityArea > shields;
    shields.push_back(FRCArea::Top);

    Tile tileC = Tile(Tile::City, Tile::City, Tile::City, Tile::City, Tile::Nothing, "C",
                          fields, roads, cities, citiesPerField, shields);

    return tileC;
}

Tile
createTileD()
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

    std::vector< FRCArea::CityArea > shields;

    Tile tileD = Tile(Tile::City, Tile::Road, Tile::Field, Tile::Road, Tile::Nothing, "D",
                          fields, roads, cities, citiesPerField, shields);

    return tileD;
}

Tile
createTileE()
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
    //Empty row for shield
    std::vector< FRCArea::CityArea > shields;
    //Define empty road
    std::vector< Tile::ContiguousRoad> roads;
    //Tile constructor, Top Right Bottom Left Center mId fields roads cities citiesPerField shields
    Tile tileE = Tile(Tile::City, Tile::Field, Tile::Field, Tile::Field, Tile::Nothing, "E",
                          fields, roads, cities, citiesPerField, shields);

    return tileE;
}

Tile
createTileF()
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

    Tile tileF = Tile(Tile::Field,Tile::City,Tile::Field,Tile::City,Tile::Nothing,"F",
                       fields,roads,cities,citiesPerField,shields);
    return tileF;
}

Tile
createTileG()
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

    std::vector< FRCArea::CityArea > shields;

    std::map< Tile::ContiguousField, std::vector< Tile::ContiguousCity > > citiesPerField;
    std::vector< Tile::ContiguousCity > firstFieldCities;
    firstFieldCities.push_back(firstCity);
    std::vector< Tile::ContiguousCity > secondFieldCities;
    secondFieldCities.push_back(firstCity);

    citiesPerField.insert( std::pair< Tile::ContiguousField, std::vector< Tile::ContiguousCity > >(firstField, firstFieldCities));
    citiesPerField.insert( std::pair< Tile::ContiguousField, std::vector< Tile::ContiguousCity > >(secondField, secondFieldCities));

    Tile tileG = Tile(Tile::City, Tile::Field, Tile::City, Tile::Field, Tile::Nothing, "G",
                      fields, roads, cities, citiesPerField, shields);

    return tileG;
}

Tile
createTileL()
{
    std::vector< Tile::ContiguousCity > cities;
    std::vector< FRCArea::CityArea > firstCity;
    firstCity.push_back(FRCArea::Right);
    cities.push_back(firstCity);

    std::vector< FRCArea::CityArea > shields;

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

    Tile tileL = Tile(Tile::City, Tile::Road, Tile::Road, Tile::Road, Tile::Nothing, "L",
                          fields, roads, cities, citiesPerField, shields);

    return tileL;
}

Tile
createTileN()
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

    Tile tileN = Tile(Tile::City, Tile::City, Tile::Field, Tile::Field, Tile::Nothing, "N",
                          fields, roads, cities, citiesPerField, shields);

    return tileN;
}
} // End of nameless namespace

std::vector< Tile >
createBaseGameTiles()
{
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
    //...
    for (int i = 0; i < 3; ++i)
    {
        tiles.push_back(createTileL());
    }
    //...
    for (int i = 0; i < 3; ++i)
    {
        tiles.push_back(createTileN());
    }

    std::random_shuffle(tiles.begin(), tiles.end());
    //add starting tile
    tiles.push_back(createTileD());

    return tiles;
}
