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
    void onSubmitCurrentTile();
    void onTilesLeft(unsigned int inNr);

private:
    Game *mGame;
    GameWindow *mWindow;
    
};

#endif // GAMECONTROLLER_H
