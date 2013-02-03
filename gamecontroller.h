#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>

#include "Game.h"
#include "GameWindow.h"

class GameController : public QObject
{
    Q_OBJECT
public:
    explicit GameController(QObject *parent = 0);
    
signals:

public slots:
    void placeTile(unsigned int inCol, unsigned int inRow, std::string inId, TileOnBoard::Rotation inRot, std::string inNextId);
    void onClicked(int x, int y);
    void rotateTile(unsigned int inCol, unsigned int inRow, std::string inId, TileOnBoard::Rotation inRot);
    void onTilesLeft(unsigned int inNr);

private:
    Game *mGame;
    GameWindow *mWindow;
    
};

#endif // GAMECONTROLLER_H
