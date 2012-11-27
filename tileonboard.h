#ifndef TILEONBOARD_H
#define TILEONBOARD_H

#include "tile.h"

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

    Tile::Side GetTop();
    Tile::Side GetRight();
    Tile::Side GetBottom();
    Tile::Side GetLeft();

    bool MatchesAbove(TileOnBoard inTileOnBoard);
    bool MatchesRightOf(TileOnBoard inTileOnBoard);
    bool MatchesBelow(TileOnBoard inTileOnBoard);
    bool MatchesLeftOf(TileOnBoard inTileOnBoard);

private:
    Tile mTile;
    Rotation mRotation;
    std::vector< Tile::ContiguousMeadow > mOccupiedMeadows;
    std::vector< Tile::ContiguousRoadOrCastle > mOccupiedRoads;
    std::vector< Tile::ContiguousRoadOrCastle > mOccupiedCastles;
};

#endif // TILEONBOARD_H
