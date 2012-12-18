#include "board.h"

Board::Board()
{
    mNrCols = 5;
    mNrRows = 5;
    mBoard = std::vector< boost::optional< TileOnBoard > >(mNrCols * mNrRows, boost::optional< TileOnBoard >());
}

void
Board::AddRowsOnTop(int inNrOfRows)
{
    mBoard.insert(mBoard.begin(), mNrCols * inNrOfRows, boost::optional< TileOnBoard >());
    mNrRows += inNrOfRows;
}

void
Board::AddRowsBelow(int inNrOfRows)
{
    mBoard.insert(mBoard.end(), mNrCols * inNrOfRows, boost::optional< TileOnBoard >());
    mNrRows += inNrOfRows;
}

void
Board::AddColsLeft(int inNrOfCols)
{
    std::vector< boost::optional< TileOnBoard > >::iterator it = mBoard.begin();
    for(int i = 0; i < mNrRows; ++i)
    {
        it = mBoard.insert(it, inNrOfCols, boost::optional< TileOnBoard >());
        ++it;
        for (int j = 0; j < mNrCols; ++j)
        {
            ++it;
        }
    }
    mNrCols += inNrOfCols;
}

void
Board::AddColsRight(int inNrOfCols)
{
    std::vector< boost::optional< TileOnBoard > >::iterator it = mBoard.begin();
    for(int i = 0; i < mNrRows; ++i)
    {
        for (int j = 0; j < mNrCols; ++j)
        {
            ++it;
        }
        it = mBoard.insert(it, inNrOfCols, boost::optional< TileOnBoard >());
        ++it;

    }
    mNrCols += inNrOfCols;
}

bool
Board::isValidTilePlacement(const TileOnBoard &inTile, int inCol, int inRow) const
{
    bool valid = true;
    // Check if col and row are within bounds
    if (inCol < 0 || inCol >= mNrCols || inRow < 0 || inRow >= mNrRows)
    {
        valid = false;
    }
    // Check if no tile placed there yet
    if (mBoard[inRow * mNrCols + inCol])
    {
        valid = false;
    }
    // Check if one of four neighbors is placed
    int nrOfNeighbors = 0;
    if ((inRow > 0) && mBoard[(inRow - 1) * mNrCols + inCol])
    {
        ++nrOfNeighbors;
    }
    if ((inRow + 1 < mNrCols) && mBoard[(inRow + 1) * mNrCols + inCol])
    {
        ++nrOfNeighbors;
    }
    if ((inCol > 0) && mBoard[inRow * mNrCols + inCol - 1])
    {
        ++nrOfNeighbors;
    }
    if ((inCol + 1 < mNrCols) && mBoard[inRow * mNrCols + inCol + 1])
    {
        ++nrOfNeighbors;
    }
    if (nrOfNeighbors == 0)
    {
        valid = false;
    }
    // Check if sides match with four neighbors
    if ((inRow > 0)
            && mBoard[(inRow - 1) * mNrCols + inCol]
            && !inTile.matchesBelow(mBoard[(inRow - 1) * mNrCols + inCol]))
    {
        valid = false;
    }
    if ((inRow + 1 < mNrCols)
            && mBoard[(inRow + 1) * mNrCols + inCol]
            && !inTile.matchesAbove(mBoard[(inRow + 1) * mNrCols + inCol]))
    {
        valid = false;
    }
    if ((inCol > 0)
            && mBoard[inRow * mNrCols + inCol - 1]
            && !inTile.matchesRightOf(mBoard[inRow * mNrCols + inCol - 1]))
    {
        valid = false;
    }
    if ((inCol + 1 < mNrCols)
            && mBoard[inRow * mNrCols + inCol + 1]
            && !inTile.matchesLeftOf(mBoard[inRow * mNrCols + inCol + 1]))
    {
        valid = false;
    }
    return valid;
}

bool
Board::placeValidTile(const TileOnBoard &inTile, int inCol, int inRow)
{
    if (isValidTilePlacement(inTile, inCol, inRow))
    {
        return placeTile(inTile, inCol, inRow);
    }
    else
    {
        return false;
    }
}

bool
Board::placeTile(const TileOnBoard &inTile, int inCol, int inRow)
{
    bool placed = false;
    if ((inCol >= 0) && (inCol < mNrCols) && (inRow >= 0) && (inRow < mNrRows))
    {
        mBoard[inRow * mNrCols + inCol] = boost::optional< TileOnBoard >( inTile );
        placed = true;
    }
    return placed;
}
