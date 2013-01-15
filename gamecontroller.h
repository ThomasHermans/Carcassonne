#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>

#include "game.h"
#include "gamewindow.h"

class GameController : public QObject
{
    Q_OBJECT
public:
    explicit GameController(QObject *parent = 0);
    
signals:

public slots:
    void placeTile(unsigned int inCol, unsigned int inRow, std::string inId, TileOnBoard::Rotation inRot, std::string inNextId);
    void onClicked(unsigned int inCol, unsigned int inRow);
    void onTileRotated(unsigned int inCol, unsigned int inRow, std::string inId, TileOnBoard::Rotation inRot);

private:
    Game *mGame;
    GameWindow *mWindow;
    
};

#endif // GAMECONTROLLER_H
