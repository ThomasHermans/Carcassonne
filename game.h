#ifndef GAME_H
#define GAME_H

#include <QObject>

#include <vector>
#include <boost/optional/optional.hpp>

#include "board.h"
#include "tile.h"

class Game : public QObject
{
    Q_OBJECT
public:
    Game();

    void clickTile(unsigned int inCol, unsigned int inRow);
    void placeTileOnBoard(unsigned int inCol, unsigned int inRow);
    void placeStartTileOnBoard(unsigned int inCol, unsigned int inRow);
    void rotateTileOnBoard(unsigned int inCol, unsigned int inRow);

    boost::optional< Tile > getNextTile() const;

public slots:
    void onTileRotated(unsigned int inCol, unsigned int inRow, std::string inId, TileOnBoard::Rotation inRot);

signals:
    void tileRotated(unsigned int inCol, unsigned int inRow, std::string inId, TileOnBoard::Rotation inRot);
    void tilePlaced(unsigned int col, unsigned int row, std::string id, TileOnBoard::Rotation rot, std::string nextId);

private:
    void pickNextTile();

private:
    Board *mBoard;
    std::vector< Tile > mBag;
    boost::optional< Tile > mNextTile;
    unsigned int mLastPlacedCol;
    unsigned int mLastPlacedRow;
};

#endif // GAME_H
