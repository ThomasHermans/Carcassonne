#include "Game.h"

#include <stdio.h>

#include "createbasegametiles.h"

namespace
{
int extraRowsAndCols = 1;
int size = 5;
}

Game::Game() :
    mBoard(new Board(size))
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
    if (!mBag.empty())
    {
        mNextTile = mBag.back();
        mBag.pop_back();
    }
    connect(mBoard, SIGNAL(tileRotated(uint,uint,std::string,TileOnBoard::Rotation)),
            this, SLOT(onTileRotated(uint,uint,std::string,TileOnBoard::Rotation)));
//    emit tilePlaced( size / 2, size / 2, mBoard->getTile( size / 2, size / 2 )->getID(), mBoard->getTile( size / 2, size / 2 )->getRotation(), mNextTile->getID());
}

unsigned int
Game::getNrOfRows() const
{
    return mBoard->getNrOfRows();
}

unsigned int
Game::getNrOfCols() const
{
    return mBoard->getNrOfCols();
}

unsigned int
Game::getStartRow() const
{
    return mBoard->getStartRow();
}

unsigned int
Game::getStartCol() const
{
    return mBoard->getStartCol();
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
            unsigned int col = inCol;
            unsigned int row = inRow;
            if (inCol == 0)
            {
                mBoard->addColsLeft( extraRowsAndCols );
                col += extraRowsAndCols;
            }
            else if (inCol == mBoard->getNrOfCols() - 1)
            {
                mBoard->addColsRight( extraRowsAndCols );
            }
            else if (inRow == 0)
            {
                mBoard->addRowsOnTop( extraRowsAndCols );
                row += extraRowsAndCols;
            }
            else if (inRow == mBoard->getNrOfRows() - 1)
            {
                mBoard->addRowsBelow( extraRowsAndCols );
            }
            if (mBoard->placeValidTile(toBePlacedTile, col, row))
            {
                mLastPlacedCol = col;
                mLastPlacedRow = row;
                pickNextTile();
                if (mNextTile)
                {
                    emit tilePlaced(col, row, toBePlacedTile.getID(), toBePlacedTile.getRotation(), mNextTile->getID());
                }
                else
                {
                    emit tilePlaced(col, row, toBePlacedTile.getID(), toBePlacedTile.getRotation(), "");
                }
            }
        }
    }
}

void
Game::placeStartTileOnBoard()
{
    TileOnBoard::Rotation rotation = TileOnBoard::cw0;
    if (mNextTile)
    {
        TileOnBoard toBePlacedTile = TileOnBoard(mNextTile.get(), rotation);
        unsigned int pos = mBoard->placeStartTile(toBePlacedTile);
        unsigned int col = pos % mBoard->getNrOfCols();
        unsigned int row = pos / mBoard->getNrOfCols();
        pickNextTile();
        if (mNextTile)
        {
            emit tilePlaced(col, row, toBePlacedTile.getID(), toBePlacedTile.getRotation(), mNextTile->getID());
        }
        else
        {
            emit tilePlaced(col, row, toBePlacedTile.getID(), toBePlacedTile.getRotation(), "");
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
        std::vector< Tile >::iterator it = mBag.end();
        bool foundPossible = false;
        while (!foundPossible)
        {
            if (it != mBag.begin())
            {
                --it;
                Tile maybeNextTile = *it;
                if (mBoard->isPossibleTile(maybeNextTile))
                {
                    mNextTile = maybeNextTile;
                    mBag.erase(it);
                    foundPossible = true;
                }
            }
            else
            {
                // No more valid tiles to place, this should give End Of Game
                mNextTile = mBag.back();
            }
        }
        emit tilesLeft(mBag.size());
    }
    else
    {
        mNextTile = boost::optional< Tile >();
    }
}
