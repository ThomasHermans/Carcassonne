#include "createbasegametiles.h"

#include <algorithm>

namespace
{
Tile
createTile_A()
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

    Tile tile_A = Tile(Tile::Field, Tile::Field, Tile::Road, Tile::Field, Tile::Cloister, "A",
                          fields, roads, cities, citiesPerField, shields);

    return tile_A;
}

Tile
createTile_B()
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

    Tile tile_B = Tile(Tile::Field, Tile::Field, Tile::Field, Tile::Field, Tile::Cloister, "B",
                          fields, roads, cities, citiesPerField, shields);

    return tile_B;
}

Tile
createTile_C()
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

    Tile tile_C = Tile(Tile::City, Tile::City, Tile::City, Tile::City, Tile::Nothing, "C",
                          fields, roads, cities, citiesPerField, shields);

    return tile_C;
}

Tile
createTile_D()
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

    Tile tile_D = Tile(Tile::City, Tile::Road, Tile::Field, Tile::Road, Tile::Nothing, "D",
                          fields, roads, cities, citiesPerField, shields);

    return tile_D;
}

Tile
createTile_L()
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

    Tile tile_L = Tile(Tile::City, Tile::Road, Tile::Road, Tile::Road, Tile::Nothing, "L",
                          fields, roads, cities, citiesPerField, shields);

    return tile_L;
}

Tile
createTile_N()
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

    Tile tile_N = Tile(Tile::City, Tile::City, Tile::Field, Tile::Field, Tile::Nothing, "N",
                          fields, roads, cities, citiesPerField, shields);

    return tile_N;
}
} // End of nameless namespace

std::vector< Tile >
createBaseGameTiles()
{
    std::vector< Tile > tiles;
    for (int i = 0; i < 2; ++i)
    {
        tiles.push_back(createTile_A());
    }
    for (int i = 0; i < 3; ++i)
    {
        tiles.push_back(createTile_D());
    }
    for (int i = 0; i < 3; ++i)
    {
        tiles.push_back(createTile_L());
    }
    for (int i = 0; i < 3; ++i)
    {
        tiles.push_back(createTile_N());
    }
    std::random_shuffle(tiles.begin(), tiles.end());
    Tile startTile = createTile_D();
    tiles.insert(tiles.begin(), startTile);
    return tiles;
}
