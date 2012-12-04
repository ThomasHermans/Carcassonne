#include "board.h"

Board::Board()
{
    mNrCols = 5;
    mNrRows = 5;
    mBoard = std::vector< boost::optional< TileOnBoard > >(mNrCols * mNrRows, boost::optional< TileOnBoard >());
}

bool
Board::isValidTilePlacement(TileOnBoard inTile, int col, int row)
{
    // Check if col and row are within bounds
    // Check if no tile placed there yet
    bool valid = true;
    if (mBoard[row * mNrCols + col])
    {
        valid = false;
    }
    // Check if one of four neighbors is placed
    // Check if sides match with four neighbors
    return valid;
}

bool
Board::placeValidTile(TileOnBoard inTile, int col, int row)
{
    if (isValidTilePlacement(inTile, col, row))
    {
        return placeTile(inTile, col, row);
    }
    else
    {
        return false;
    }
}

bool
Board::placeTile(TileOnBoard inTile, int col, int row)
{
    bool placed = false;
    if ((col >= 0) && (col < mNrCols) && (row >= 0) && (row < mNrRows))
    {
        mBoard[row * mNrCols + col] = boost::optional< TileOnBoard >( inTile );
        placed = true;
    }
    return placed;
}
