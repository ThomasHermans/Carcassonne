#ifndef BOARD_H
#define BOARD_H

#include "tileonboard.h"
#include "boost/optional/optional.hpp"

class Board
{
public:
    Board();

    void AddRowsOnTop(int inNrOfRows);
    void AddRowsBelow(int inNrOfRows);
    void AddColsLeft(int inNrOfCols);
    void AddColsRight(int inNrOfCols);

    bool isValidTilePlacement(const TileOnBoard & inTile, int inCol, int inRow) const;
    bool placeValidTile(const TileOnBoard & inTile, int inCol, int inRow);

private:
    bool placeTile(const TileOnBoard & inTile, int inCol, int inRow);

private:
    std::vector< boost::optional< TileOnBoard > > mBoard; // one vector of size cols * rows might be easier to use than nested vectors
    int mNrRows;
    int mNrCols;
};

#endif // BOARD_H
