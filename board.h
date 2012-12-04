#ifndef BOARD_H
#define BOARD_H

#include "tileonboard.h"
#include "boost/optional/optional.hpp"

class Board
{
public:
    Board();

    bool isValidTilePlacement(TileOnBoard inTile, int col, int row);
    bool placeValidTile(TileOnBoard inTile, int col, int row);

private:
    bool placeTile(TileOnBoard inTile, int col, int row);

private:
    std::vector< boost::optional< TileOnBoard > > mBoard; // one vector of size cols * rows might be easier to use than nested vectors
    int mNrRows;
    int mNrCols;
};

#endif // BOARD_H
