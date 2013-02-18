#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "src-model/Game.h"
#include "src-view/GameWindow.h"

#include <QObject>

class GameController : public QObject
{
    Q_OBJECT
public:
    explicit GameController(QObject *parent = 0);
    
signals:

public slots:
    void onTilePlaced(unsigned int inCol, unsigned int inRow, std::string inId, TileOnBoard::Rotation inRot);
    void onTileUnplaced(unsigned int inCol, unsigned int inRow);
    void onClicked(int x, int y);
    void onTileRotated(unsigned int inCol, unsigned int inRow, std::string inId, TileOnBoard::Rotation inRot);
    void onNextTile(std::string inNextId);
    void onTilesLeft(unsigned int inNr);

    void onCurrentPlayerChanged(unsigned int inCurrentPlayer);

    void onFinishedCloister(unsigned int inCol, unsigned int inRow);
    void onFinishedCity(std::vector< std::pair< unsigned int, unsigned int > > inTiles);
    void onFinishedRoad(std::vector< std::pair< unsigned int, unsigned int > > inTiles);

    void onEndOfGame(unsigned int inTilesLeft);

private:
    Game *mGame;
    GameWindow *mWindow;
    
};

#endif // GAMECONTROLLER_H
