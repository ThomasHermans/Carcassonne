#include "createbasegametiles.h"

#include <algorithm>

namespace
{
Tile
createTile_CRRs()
{
    std::vector< Tile::ContiguousField > fields;
    std::vector< FieldArea::FieldArea > firstField;
    firstField.push_back(FieldArea::RightTop);
    firstField.push_back(FieldArea::LeftTop);
    std::vector< FieldArea::FieldArea > secondField;
    secondField.push_back(FieldArea::RightBottom);
    secondField.push_back(FieldArea::BottomRight);
    secondField.push_back(FieldArea::BottomLeft);
    secondField.push_back(FieldArea::LeftBottom);
    fields.push_back(firstField);
    fields.push_back(secondField);

    std::vector< Tile::ContiguousRoadOrCity > roads;
    std::vector< RoadOrCityArea::RoadOrCityArea > firstRoad;
    firstRoad.push_back(RoadOrCityArea::Right);
    firstRoad.push_back(RoadOrCityArea::Left);
    roads.push_back(firstRoad);

    std::vector< Tile::ContiguousRoadOrCity > cities;
    std::vector< RoadOrCityArea::RoadOrCityArea > firstCity;
    firstCity.push_back(RoadOrCityArea::Top);
    cities.push_back(firstCity);

    std::vector< RoadOrCityArea::RoadOrCityArea > shields;

    Tile tile_CCRs = Tile(Tile::City, Tile::Road, Tile::Field, Tile::Road, Tile::Nothing,
                          fields, roads, cities, shields);

    return tile_CCRs;
}

Tile
createTile_CCco()
{
    std::vector< Tile::ContiguousField > fields;
    std::vector< FieldArea::FieldArea > firstField;
    firstField.push_back(FieldArea::BottomRight);
    firstField.push_back(FieldArea::BottomLeft);
    firstField.push_back(FieldArea::LeftBottom);
    firstField.push_back(FieldArea::LeftTop);
    fields.push_back(firstField);

    std::vector< Tile::ContiguousRoadOrCity > roads;

    std::vector< Tile::ContiguousRoadOrCity > cities;
    std::vector< RoadOrCityArea::RoadOrCityArea > firstCity;
    firstCity.push_back(RoadOrCityArea::Top);
    firstCity.push_back(RoadOrCityArea::Right);
    cities.push_back(firstCity);

    std::vector< RoadOrCityArea::RoadOrCityArea > shields;

    Tile tile_CCco = Tile(Tile::City, Tile::City, Tile::Field, Tile::Field, Tile::Nothing,
                          fields, roads, cities, shields);

    return tile_CCco;
}

Tile
createTile_CRRR()
{
    std::vector< Tile::ContiguousRoadOrCity > cities;
    std::vector< RoadOrCityArea::RoadOrCityArea > firstCity;
    firstCity.push_back(RoadOrCityArea::Top);
    cities.push_back(firstCity);

    std::vector< RoadOrCityArea::RoadOrCityArea > shields;

    std::vector< Tile::ContiguousRoadOrCity > roads;
    std::vector< RoadOrCityArea::RoadOrCityArea > firstRoad;
    firstRoad.push_back(RoadOrCityArea::Right);
    std::vector< RoadOrCityArea::RoadOrCityArea > secondRoad;
    secondRoad.push_back(RoadOrCityArea::Bottom);
    std::vector< RoadOrCityArea::RoadOrCityArea > thirdRoad;
    thirdRoad.push_back(RoadOrCityArea::Left);
    roads.push_back(firstRoad);
    roads.push_back(secondRoad);
    roads.push_back(thirdRoad);

    std::vector< Tile::ContiguousField > fields;
    std::vector< FieldArea::FieldArea > firstField;
    firstField.push_back(FieldArea::RightTop);
    firstField.push_back(FieldArea::LeftTop);
    std::vector< FieldArea::FieldArea > secondField;
    secondField.push_back(FieldArea::RightBottom);
    secondField.push_back(FieldArea::BottomRight);
    std::vector< FieldArea::FieldArea > thirdField;
    thirdField.push_back(FieldArea::BottomLeft);
    thirdField.push_back(FieldArea::LeftBottom);
    fields.push_back(firstField);
    fields.push_back(secondField);
    fields.push_back(thirdField);

    Tile tile_CRRR = Tile(Tile::City, Tile::Road, Tile::Road, Tile::Road, Tile::Nothing,
                          fields, roads, cities, shields);

    return tile_CRRR;
}
} // End of nameless namespace

std::vector< Tile >
createBaseGameTiles()
{
    std::vector< Tile > tiles;
    for (int i = 0; i < 4; ++i)
    {
        tiles.push_back(createTile_CRRs());
    }
    for (int i = 0; i < 5; ++i)
    {
        tiles.push_back(createTile_CCco());
    }
    for (int i = 0; i < 3; ++i)
    {
        tiles.push_back(createTile_CRRR());
    }
    std::random_shuffle(tiles.begin(), tiles.end());
    return tiles;
}
