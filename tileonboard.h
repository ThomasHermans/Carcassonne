#ifndef TILEONBOARD_H
#define TILEONBOARD_H

#include "color.h"
#include "placedpiece.h"
#include "tile.h"

class TileOnBoard
{
public:
    enum Rotation
    {
        cw0 = 0,
        cw90 = 3,
        cw180 = 6,
        cw270 = 9
    };

public:
    TileOnBoard();
    TileOnBoard(Tile inTile, Rotation inRotation);

    bool placePiece(PlacedPiece inPlacedPiece);

    Tile::Side getTop();
    Tile::Side getRight();
    Tile::Side getBottom();
    Tile::Side getLeft();
    Tile::Center getCenter();

    bool matchesAbove(TileOnBoard inTileOnBoard);
    bool matchesRightOf(TileOnBoard inTileOnBoard);
    bool matchesBelow(TileOnBoard inTileOnBoard);
    bool matchesLeftOf(TileOnBoard inTileOnBoard);

    std::vector< Tile::ContiguousField > getContiguousFields();
    std::vector< Tile::ContiguousRoadOrCity > getContiguousRoads();
    std::vector< Tile::ContiguousRoadOrCity > getContiguousCities();

    std::vector< Tile::ContiguousRoadOrCity > getFinishedRoads();
    std::vector< Tile::ContiguousRoadOrCity > getFinishedCities();

    std::vector< RoadOrCityArea::RoadOrCityArea > getShields();
    std::vector< RoadOrCityArea::RoadOrCityArea > getInns();

    bool isFieldOccupied(FieldArea::FieldArea inFieldArea);
    bool isRoadOccupied(RoadOrCityArea::RoadOrCityArea inRoadArea);
    bool isCityOccupied(RoadOrCityArea::RoadOrCityArea inCityArea);

    bool isRoadFinished(RoadOrCityArea::RoadOrCityArea inRoadArea);
    bool isCityFinished(RoadOrCityArea::RoadOrCityArea inCityArea);

    void occupyField(FieldArea::FieldArea inFieldArea);
    void occupyRoad(RoadOrCityArea::RoadOrCityArea inRoadArea);
    void occupyCity(RoadOrCityArea::RoadOrCityArea inCityArea);

    void finishRoad(RoadOrCityArea::RoadOrCityArea inRoadArea);
    void finishCity(RoadOrCityArea::RoadOrCityArea inCityArea);

    std::string toString();

private:
    Tile mTile;
    Rotation mRotation;
    std::vector< Tile::ContiguousField > mOccupiedFields;
    std::vector< Tile::ContiguousRoadOrCity > mOccupiedRoads;
    std::vector< Tile::ContiguousRoadOrCity > mFinishedRoads;
    std::vector< Tile::ContiguousRoadOrCity > mOccupiedCities;
    std::vector< Tile::ContiguousRoadOrCity > mFinishedCities;
    std::vector< PlacedPiece > mPlacedPieces;
};

#endif // TILEONBOARD_H
