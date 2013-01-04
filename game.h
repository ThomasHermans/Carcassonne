#ifndef GAME_H
#define GAME_H

#include <vector>
#include <boost/optional/optional.hpp>

#include "board.h"
#include "tile.h"

class Game
{
public:
    Game();

    void clickTile(unsigned int inCol, unsigned int inRow);
    void placeTileOnBoard(unsigned int inCol, unsigned int inRow);
    void rotateTileOnBoard(unsigned int inCol, unsigned int inRow);

    Board getBoard() const;
    boost::optional< Tile > getNextTile() const;

private:
    void pickNextTile();

private:
    Board mBoard;
    std::vector< Tile > mBag;
    boost::optional< Tile > mNextTile;
    unsigned int mLastPlacedCol;
    unsigned int mLastPlacedRow;
};

#endif // GAME_H
