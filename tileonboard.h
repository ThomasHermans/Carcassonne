#ifndef TILEONBOARD_H
#define TILEONBOARD_H

#include "tile.h"
#include "color.h"

class TileOnBoard
{
public:
    enum Rotation
    {
        cw0,
        cw90,
        cw180,
        cw270
    };

public:
    TileOnBoard();
    TileOnBoard(Tile inTile, Rotation inRotation);

    Tile::Side getTop();
    Tile::Side getRight();
    Tile::Side getBottom();
    Tile::Side getLeft();

    bool matchesAbove(TileOnBoard inTileOnBoard);
    bool matchesRightOf(TileOnBoard inTileOnBoard);
    bool matchesBelow(TileOnBoard inTileOnBoard);
    bool matchesLeftOf(TileOnBoard inTileOnBoard);

private:
    Tile mTile;
    Rotation mRotation;
    std::vector< Tile::ContiguousField > mOccupiedFields;
    std::vector< Tile::ContiguousRoadOrCity > mOccupiedRoads;
    std::vector< Tile::ContiguousRoadOrCity > mOccupiedCities;
};

#endif // TILEONBOARD_H
