#include "board.h"

Board::Board()
{
    mNrCols = 5;
    mNrRows = 5;
    mBoard = std::vector< boost::optional< TileOnBoard > >(mNrCols * mNrRows, boost::optional< TileOnBoard >());
}

bool
Board::isValidTilePlacement(const TileOnBoard &inTile, int inCol, int inRow) const
{
    // Check if col and row are within bounds
    // Check if no tile placed there yet
    bool valid = true;
    if (mBoard[inRow * mNrCols + inCol])
    {
        valid = false;
    }
    // Check if one of four neighbors is placed
    // Check if sides match with four neighbors
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
