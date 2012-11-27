#include "tileonboard.h"
#include "assert.h"

TileOnBoard::TileOnBoard()
{
}

TileOnBoard::TileOnBoard(Tile inTile, Rotation inRotation):
    mTile(inTile),
    mRotation(inRotation)
{
}

Tile::Side
TileOnBoard::getTop()
{
    switch (mRotation)
    {
    case cw90:
        return mTile.getLeft();
    case cw180:
        return mTile.getBottom();
    case cw270:
        return mTile.getRight();
    case cw0:
    default:
        return mTile.getTop();
    }
}

Tile::Side
TileOnBoard::getRight()
{
    switch (mRotation)
    {
    case cw90:
        return mTile.getTop();
    case cw180:
        return mTile.getLeft();
    case cw270:
        return mTile.getBottom();
    case cw0:
    default:
        return mTile.getRight();
    }
}

Tile::Side
TileOnBoard::getBottom()
{
    switch (mRotation)
    {
    case cw90:
        return mTile.getRight();
    case cw180:
        return mTile.getTop();
    case cw270:
        return mTile.getLeft();
    case cw0:
    default:
        return mTile.getBottom();
    }
}

Tile::Side
TileOnBoard::getLeft()
{
    switch (mRotation)
    {
    case cw90:
        return mTile.getBottom();
    case cw180:
        return mTile.getRight();
    case cw270:
        return mTile.getRight();
    case cw0:
    default:
        return mTile.getLeft();
    }
}

bool
TileOnBoard::matchesAbove(TileOnBoard inTileOnBoard)
{
    return (this->getBottom() == inTileOnBoard.getTop());
}

bool
TileOnBoard::matchesRightOf(TileOnBoard inTileOnBoard)
{
    return (this->getLeft() == inTileOnBoard.getRight());
}

bool
TileOnBoard::matchesBelow(TileOnBoard inTileOnBoard)
{
    return (this->getTop() == inTileOnBoard.getBottom());
}

bool
TileOnBoard::matchesLeftOf(TileOnBoard inTileOnBoard)
{
    return (this->getRight() == inTileOnBoard.getLeft());
}
