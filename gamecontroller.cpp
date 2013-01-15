#include "gamecontroller.h"

GameController::GameController(QObject *parent) :
    QObject(parent),
    mGame(new Game()),
    mWindow(new GameWindow())
{
    connect(mGame, SIGNAL(tilePlaced(unsigned int, unsigned int, std::string, TileOnBoard::Rotation, std::string)),
            this, SLOT(placeTile(unsigned int, unsigned int, std::string, TileOnBoard::Rotation, std::string)));
    connect(mWindow, SIGNAL(clicked(uint,uint)), this, SLOT(onClicked(uint,uint)));
    connect(mGame, SIGNAL(tileRotated(uint,uint,std::string,TileOnBoard::Rotation)),
            this, SLOT(onTileRotated(uint,uint,std::string,TileOnBoard::Rotation)));
    mWindow->show();
    if (mGame->getNextTile())
    {
        mGame->placeStartTileOnBoard(2, 2);
    }
}

void
GameController::placeTile(unsigned int inCol, unsigned int inRow, std::string inId, TileOnBoard::Rotation inRot, std::string inNextId)
{
    mWindow->setTile(inCol, inRow, inId, inRot * 30);
    mWindow->setNextTile(inNextId);
}

void
GameController::onClicked(unsigned int inCol, unsigned int inRow)
{
    std::cout << "GameController sees a click" << std::endl;
    mGame->clickTile(inCol, inRow);
}

void
GameController::onTileRotated(unsigned int inCol, unsigned int inRow, std::string inId, TileOnBoard::Rotation inRot)
{
    std::cout << "GameController sees a rotation" << std::endl;
    mWindow->setTile(inCol, inRow, inId, inRot * 30);
}
