#include "game.h"

#include <stdio.h>

#include "createbasegametiles.h"

Game::Game() :
    mBoard(Board())
{
    mBag = createBaseGameTiles();
    for(unsigned int i = 0; i < mBag.size(); ++i)
    {
        std::cout << mBag[i].getID();
    }
    std::cout << std::endl;
    mNextTile = boost::optional< Tile >();
    mLastPlacedCol = (unsigned int)-1;
    mLastPlacedCol = (unsigned int)-1;
    pickNextTile();
}

void
Game::clickTile(unsigned int inCol, unsigned int inRow)
{
    if (mBoard.isEmptySpot(inCol, inRow))
    {
        placeTileOnBoard(inCol, inRow);
    }
    else
    {
        rotateTileOnBoard(inCol, inRow);
    }
}

void
Game::placeTileOnBoard(unsigned int inCol, unsigned int inRow)
{
    TileOnBoard::Rotation rotation = TileOnBoard::cw0;
    if (mNextTile)
    {
        TileOnBoard toBePlacedTile = TileOnBoard(mNextTile.get(), rotation);
        bool found = false;
        for (int i = 0; i < 4; ++i)
        {
            if (mBoard.isValidTilePlacement(toBePlacedTile, inCol, inRow))
            {
                found = true;
                break;
            }
            else
            {
                rotation = TileOnBoard::Rotation(rotation + TileOnBoard::cw90);
            }
        }
        if (found)
        {
            mBoard.placeValidTile(toBePlacedTile, inCol, inRow);
            pickNextTile();
        }
    }
}

void
Game::rotateTileOnBoard(unsigned int inCol, unsigned int inRow)
{
    if (inCol == mLastPlacedCol && inRow == mLastPlacedRow)
    {
        mBoard.rotateTileOnBoard(inCol, inRow);
    }
}

Board
Game::getBoard() const
{
    return mBoard;
}

boost::optional< Tile >
Game::getNextTile() const
{
    return mNextTile;
}

void
Game::pickNextTile()
{
    if (!mBag.empty())
    {
        mNextTile = mBag.back();
        mBag.pop_back();
    }
    else
    {
        mNextTile = boost::optional< Tile >();
    }
}
