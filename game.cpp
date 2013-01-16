#include "game.h"

#include <stdio.h>

#include "createbasegametiles.h"

Game::Game() :
    mBoard(new Board())
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
    connect(mBoard, SIGNAL(tileRotated(uint,uint,std::string,TileOnBoard::Rotation)),
            this, SLOT(onTileRotated(uint,uint,std::string,TileOnBoard::Rotation)));
}

void
Game::clickTile(unsigned int inCol, unsigned int inRow)
{
    if (mBoard->isEmptySpot(inCol, inRow))
    {
        std::cout << "Empty spot -> placeTileOnBoard" << std::endl;
        placeTileOnBoard(inCol, inRow);
    }
    else
    {
        std::cout << "Non-empty spot -> rotateTileOnBoard" << std::endl;
        rotateTileOnBoard(inCol, inRow);
    }
    std::cout << mBoard->toString() << std::flush;
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
            toBePlacedTile = TileOnBoard(mNextTile.get(), rotation);
            if (mBoard->isValidTilePlacement(toBePlacedTile, inCol, inRow))
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
            if (mBoard->placeValidTile(toBePlacedTile, inCol, inRow))
            {
                mLastPlacedCol = inCol;
                mLastPlacedRow = inRow;
                pickNextTile();
                if (mNextTile)
                {
                    emit tilePlaced(inCol, inRow, toBePlacedTile.getID(), toBePlacedTile.getRotation(), mNextTile->getID());
                }
                else
                {
                    emit tilePlaced(inCol, inRow, toBePlacedTile.getID(), toBePlacedTile.getRotation(), "");
                }
            }
        }
    }
}

void
Game::placeStartTileOnBoard(unsigned int inCol, unsigned int inRow)
{
    TileOnBoard::Rotation rotation = TileOnBoard::cw0;
    if (mNextTile)
    {
        TileOnBoard toBePlacedTile = TileOnBoard(mNextTile.get(), rotation);
        mBoard->placeValidTile(toBePlacedTile, inCol, inRow);
        pickNextTile();
        if (mNextTile)
        {
            emit tilePlaced(inCol, inRow, toBePlacedTile.getID(), toBePlacedTile.getRotation(), mNextTile->getID());
        }
        else
        {
            emit tilePlaced(inCol, inRow, toBePlacedTile.getID(), toBePlacedTile.getRotation(), "");
        }
    }
}

void
Game::rotateTileOnBoard(unsigned int inCol, unsigned int inRow)
{
    if (inCol == mLastPlacedCol && inRow == mLastPlacedRow)
    {
        std::cout << "Rotating " << inCol << ", " << inRow << std::endl;
        mBoard->rotateTileOnBoard(inCol, inRow);
    }
    else
    {
        std::cout << "Not rotating" << std::endl;
    }
}

boost::optional< Tile >
Game::getNextTile() const
{
    return mNextTile;
}

void
Game::onTileRotated(unsigned int inCol, unsigned int inRow, std::string inId, TileOnBoard::Rotation inRot)
{
    std::cout << "Game sees a rotation" << std::endl;
    emit tileRotated(inCol, inRow, inId, inRot);
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
