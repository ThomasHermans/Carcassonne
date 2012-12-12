#include "createbasegametiles.h"

#include <algorithm>

namespace
{
Tile
createTile_A()
{
    std::vector< Tile::ContiguousField > fields;
    Tile::ContiguousField firstField;
    firstField.push_back(FieldArea::TopLeft);
    firstField.push_back(FieldArea::TopRight);
    firstField.push_back(FieldArea::RightTop);
    firstField.push_back(FieldArea::RightBottom);
    firstField.push_back(FieldArea::BottomRight);
    firstField.push_back(FieldArea::BottomLeft);
    firstField.push_back(FieldArea::LeftBottom);
    firstField.push_back(FieldArea::LeftTop);
    fields.push_back(firstField);

    std::vector< Tile::ContiguousRoadOrCity > roads;
    Tile::ContiguousRoadOrCity firstRoad;
    firstRoad.push_back(RoadOrCityArea::Bottom);
    roads.push_back(firstRoad);

    std::vector< Tile::ContiguousRoadOrCity > cities;

    std::map< Tile::ContiguousField, std::vector< Tile::ContiguousRoadOrCity > > citiesPerField;
    std::vector< Tile::ContiguousRoadOrCity > firstFieldCities;
    citiesPerField.insert( std::pair< Tile::ContiguousField, std::vector< Tile::ContiguousRoadOrCity > >(firstField, firstFieldCities));

    std::vector< RoadOrCityArea::RoadOrCityArea > shields;

    Tile tile_A = Tile(Tile::Field, Tile::Field, Tile::Road, Tile::Field, Tile::Cloister, "A",
                          fields, roads, cities, citiesPerField, shields);

    return tile_A;
}

Tile
createTile_D()
{
    std::vector< Tile::ContiguousField > fields;
    std::vector< FieldArea::FieldArea > firstField;
    firstField.push_back(FieldArea::TopRight);
    firstField.push_back(FieldArea::BottomRight);
    std::vector< FieldArea::FieldArea > secondField;
    secondField.push_back(FieldArea::BottomLeft);
    secondField.push_back(FieldArea::LeftBottom);
    secondField.push_back(FieldArea::LeftTop);
    secondField.push_back(FieldArea::TopLeft);
    fields.push_back(firstField);
    fields.push_back(secondField);

    std::vector< Tile::ContiguousRoadOrCity > roads;
    std::vector< RoadOrCityArea::RoadOrCityArea > firstRoad;
    firstRoad.push_back(RoadOrCityArea::Top);
    firstRoad.push_back(RoadOrCityArea::Bottom);
    roads.push_back(firstRoad);

    std::vector< Tile::ContiguousRoadOrCity > cities;
    std::vector< RoadOrCityArea::RoadOrCityArea > firstCity;
    firstCity.push_back(RoadOrCityArea::Right);
    cities.push_back(firstCity);

    std::map< Tile::ContiguousField, std::vector< Tile::ContiguousRoadOrCity > > citiesPerField;
    std::vector< Tile::ContiguousRoadOrCity > firstFieldCities;
    firstFieldCities.push_back( firstCity );
    std::vector< Tile::ContiguousRoadOrCity > secondFieldCities;

    citiesPerField.insert( std::pair< Tile::ContiguousField, std::vector< Tile::ContiguousRoadOrCity > >(firstField, firstFieldCities));
    citiesPerField.insert( std::pair< Tile::ContiguousField, std::vector< Tile::ContiguousRoadOrCity > >(secondField, secondFieldCities));

    std::vector< RoadOrCityArea::RoadOrCityArea > shields;

    Tile tile_D = Tile(Tile::City, Tile::Road, Tile::Field, Tile::Road, Tile::Nothing, "D",
                          fields, roads, cities, citiesPerField, shields);

    return tile_D;
}

Tile
createTile_L()
{
    std::vector< Tile::ContiguousRoadOrCity > cities;
    std::vector< RoadOrCityArea::RoadOrCityArea > firstCity;
    firstCity.push_back(RoadOrCityArea::Right);
    cities.push_back(firstCity);

    std::vector< RoadOrCityArea::RoadOrCityArea > shields;

    std::vector< Tile::ContiguousRoadOrCity > roads;
    std::vector< RoadOrCityArea::RoadOrCityArea > firstRoad;
    firstRoad.push_back(RoadOrCityArea::Bottom);
    std::vector< RoadOrCityArea::RoadOrCityArea > secondRoad;
    secondRoad.push_back(RoadOrCityArea::Left);
    std::vector< RoadOrCityArea::RoadOrCityArea > thirdRoad;
    thirdRoad.push_back(RoadOrCityArea::Top);
    roads.push_back(firstRoad);
    roads.push_back(secondRoad);
    roads.push_back(thirdRoad);

    std::vector< Tile::ContiguousField > fields;
    std::vector< FieldArea::FieldArea > firstField;
    firstField.push_back(FieldArea::TopRight);
    firstField.push_back(FieldArea::BottomRight);
    std::vector< FieldArea::FieldArea > secondField;
    secondField.push_back(FieldArea::BottomLeft);
    secondField.push_back(FieldArea::LeftBottom);
    std::vector< FieldArea::FieldArea > thirdField;
    thirdField.push_back(FieldArea::LeftTop);
    thirdField.push_back(FieldArea::TopLeft);
    fields.push_back(firstField);
    fields.push_back(secondField);
    fields.push_back(thirdField);

    std::map< Tile::ContiguousField, std::vector< Tile::ContiguousRoadOrCity > > citiesPerField;
    std::vector< Tile::ContiguousRoadOrCity > firstFieldCities;
    firstFieldCities.push_back( firstCity );
    std::vector< Tile::ContiguousRoadOrCity > secondFieldCities;
    std::vector< Tile::ContiguousRoadOrCity > thirdFieldCities;

    citiesPerField.insert( std::pair< Tile::ContiguousField, std::vector< Tile::ContiguousRoadOrCity > >(firstField, firstFieldCities));
    citiesPerField.insert( std::pair< Tile::ContiguousField, std::vector< Tile::ContiguousRoadOrCity > >(secondField, secondFieldCities));
    citiesPerField.insert( std::pair< Tile::ContiguousField, std::vector< Tile::ContiguousRoadOrCity > >(thirdField, thirdFieldCities));

    Tile tile_L = Tile(Tile::City, Tile::Road, Tile::Road, Tile::Road, Tile::Nothing, "L",
                          fields, roads, cities, citiesPerField, shields);

    return tile_L;
}

Tile
createTile_N()
{
    std::vector< Tile::ContiguousField > fields;
    std::vector< FieldArea::FieldArea > firstField;
    firstField.push_back(FieldArea::RightTop);
    firstField.push_back(FieldArea::RightBottom);
    firstField.push_back(FieldArea::BottomRight);
    firstField.push_back(FieldArea::BottomLeft);
    fields.push_back(firstField);

    std::vector< Tile::ContiguousRoadOrCity > roads;

    std::vector< Tile::ContiguousRoadOrCity > cities;
    std::vector< RoadOrCityArea::RoadOrCityArea > firstCity;
    firstCity.push_back(RoadOrCityArea::Top);
    firstCity.push_back(RoadOrCityArea::Left);
    cities.push_back(firstCity);

    std::map< Tile::ContiguousField, std::vector< Tile::ContiguousRoadOrCity > > citiesPerField;
    std::vector< Tile::ContiguousRoadOrCity > firstFieldCities;
    firstFieldCities.push_back( firstCity );

    citiesPerField.insert( std::pair< Tile::ContiguousField, std::vector< Tile::ContiguousRoadOrCity > >(firstField, firstFieldCities));

    std::vector< RoadOrCityArea::RoadOrCityArea > shields;

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
