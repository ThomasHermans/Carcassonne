#ifndef GAME_H
#define GAME_H

#include "src-model/Board.h"
#include "src-model/Tile.h"

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
    void onSubmitCurrentTile();

signals:
    void tileRotated(unsigned int inCol, unsigned int inRow, std::string inId, TileOnBoard::Rotation inRot);
    void tilePlaced(unsigned int col, unsigned int row, std::string id, TileOnBoard::Rotation rot);
    void tileUnplaced(unsigned int inCol, unsigned int inRow);
    void nextTile(std::string inNextId);
    void tilesLeft(unsigned int inNr);

    void finishedCloister(unsigned int inCol, unsigned int inRow);
    void finishedCity(std::vector< std::pair< unsigned int, unsigned int > > inTiles);
    void finishedRoad(std::vector< std::pair< unsigned int, unsigned int > > inTiles);

    void endOfGame(unsigned int inTilesLeft);

private:
    void pickNextTile();

private:
    Board *mBoard;
    std::vector< Tile > mBag;
    boost::optional< Tile > mNextTile;
    unsigned int mStartRow;
    unsigned int mStartCol;
    unsigned int mCurrentPlacedRow;
    unsigned int mCurrentPlacedCol;
};

#endif // GAME_H
