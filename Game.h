#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include "Tile.h"

#include <QObject>

#include <vector>
#include <boost/optional/optional.hpp>

class Game : public QObject
{
    Q_OBJECT
public:
    Game();

    unsigned int getNrOfRows() const;
    unsigned int getNrOfCols() const;
    unsigned int getStartRow() const;
    unsigned int getStartCol() const;

    void clickTile(unsigned int inCol, unsigned int inRow);
    void placeTileOnBoard(unsigned int inCol, unsigned int inRow);
    void placeStartTileOnBoard();
    void rotateTileOnBoard(unsigned int inCol, unsigned int inRow);

    boost::optional< Tile > getNextTile() const;

public slots:
    void onTileRotated(unsigned int inCol, unsigned int inRow, std::string inId, TileOnBoard::Rotation inRot);

signals:
    void tileRotated(unsigned int inCol, unsigned int inRow, std::string inId, TileOnBoard::Rotation inRot);
    void tilePlaced(unsigned int col, unsigned int row, std::string id, TileOnBoard::Rotation rot, std::string nextId);
    void tilesLeft(unsigned int inNr);

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
