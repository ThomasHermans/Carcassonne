#include "tileonboard.h"

TileOnBoard::TileOnBoard()
{
}

TileOnBoard::TileOnBoard(Tile inTile, Rotation inRotation):
    mTile(inTile),
    mRotation(inRotation)
{
}

Tile::Side
TileOnBoard::GetTop()
{
    switch (mRotation)
    {
    case cw0:
        return mTile.GetTop();
    case cw90:
        return mTile.GetLeft();
    case cw180:
        return mTile.GetBottom();
    case cw270:
        return mTile.GetRight();
    }
}

Tile::Side
TileOnBoard::GetRight()
{
    switch (mRotation)
    {
    case cw0:
        return mTile.GetRight();
    case cw90:
        return mTile.GetTop();
    case cw180:
        return mTile.GetLeft();
    case cw270:
        return mTile.GetBottom();
    }
}

Tile::Side
TileOnBoard::GetBottom()
{
    switch (mRotation)
    {
    case cw0:
        return mTile.GetBottom();
    case cw90:
        return mTile.GetRight();
    case cw180:
        return mTile.GetTop();
    case cw270:
        return mTile.GetLeft();
    }
}

Tile::Side
TileOnBoard::GetLeft()
{
    switch (mRotation)
    {
    case cw0:
        return mTile.GetLeft();
    case cw90:
        return mTile.GetBottom();
    case cw180:
        return mTile.GetRight();
    case cw270:
        return mTile.GetRight();
    }
}

bool
TileOnBoard::MatchesAbove(TileOnBoard inTileOnBoard)
{
    return (this->GetBottom() == inTileOnBoard.GetTop());
}

bool
TileOnBoard::MatchesRightOf(TileOnBoard inTileOnBoard)
{
    return (this->GetLeft() == inTileOnBoard.GetRight());
}

bool
TileOnBoard::MatchesBelow(TileOnBoard inTileOnBoard)
{
    return (this->GetTop() == inTileOnBoard.GetBottom());
}

bool
TileOnBoard::MatchesLeftOf(TileOnBoard inTileOnBoard)
{
    return (this->GetRight() == inTileOnBoard.GetLeft());
}
