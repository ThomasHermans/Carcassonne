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
    std::string getID();

    bool matchesAbove(TileOnBoard inTileOnBoard);
    bool matchesRightOf(TileOnBoard inTileOnBoard);
    bool matchesBelow(TileOnBoard inTileOnBoard);
    bool matchesLeftOf(TileOnBoard inTileOnBoard);

    std::vector< Tile::ContiguousField > getContiguousFields();
    std::vector< Tile::ContiguousRoad > getContiguousRoads();
    std::vector< Tile::ContiguousCity > getContiguousCities();

    std::vector< Tile::ContiguousCity > getCitiesPerField(FRCArea::FieldArea inFieldArea);

    std::vector< Tile::ContiguousRoad > getFinishedRoads();
    std::vector< Tile::ContiguousCity > getFinishedCities();

    std::vector< FRCArea::CityArea > getShields();
    std::vector< FRCArea::RoadArea > getInns();

    bool isFieldOccupied(FRCArea::FieldArea inFieldArea);
    bool isRoadOccupied(FRCArea::RoadArea inRoadArea);
    bool isCityOccupied(FRCArea::CityArea inCityArea);

    bool isRoadFinished(FRCArea::RoadArea inRoadArea);
    bool isCityFinished(FRCArea::CityArea inCityArea);

    void occupyField(FRCArea::FieldArea inFieldArea);
    void occupyRoad(FRCArea::RoadArea inRoadArea);
    void occupyCity(FRCArea::CityArea inCityArea);

    void finishRoad(FRCArea::RoadArea inRoadArea);
    void finishCity(FRCArea::CityArea inCityArea);

    std::string toString();

private:
    Tile mTile;
    Rotation mRotation;
    std::vector< Tile::ContiguousField > mOccupiedFields;
    std::vector< Tile::ContiguousRoad > mOccupiedRoads;
    std::vector< Tile::ContiguousRoad > mFinishedRoads;
    std::vector< Tile::ContiguousCity > mOccupiedCities;
    std::vector< Tile::ContiguousCity > mFinishedCities;
    std::vector< PlacedPiece > mPlacedPieces;
};

#endif // TILEONBOARD_H
