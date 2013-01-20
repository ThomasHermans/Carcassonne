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
            this, SLOT(rotateTile(uint,uint,std::string,TileOnBoard::Rotation)));
    connect(mGame, SIGNAL(addedColsLeft(uint)), this, SLOT(addColsLeft(uint)));
    connect(mGame, SIGNAL(addedColsRight(uint)), this, SLOT(addColsRight(uint)));
    connect(mGame, SIGNAL(addedRowsBelow(uint)), this, SLOT(addRowsBelow(uint)));
    connect(mGame, SIGNAL(addedRowsOnTop(uint)), this, SLOT(addRowsOnTop(uint)));
    connect(mGame, SIGNAL(tilesLeft(uint)), this, SLOT(onTilesLeft(uint)));
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
GameController::rotateTile(unsigned int inCol, unsigned int inRow, std::string inId, TileOnBoard::Rotation inRot)
{
    std::cout << "GameController sees a rotation" << std::endl;
    mWindow->setTile(inCol, inRow, inId, inRot * 30);
}

void
GameController::addRowsOnTop(unsigned int inNr)
{
    mWindow->addRowsOnTop(inNr);
}

void
GameController::addRowsBelow(unsigned int inNr)
{
    mWindow->addRowsBelow(inNr);
}

void
GameController::addColsLeft(unsigned int inNr)
{
    mWindow->addColsLeft(inNr);
}

void
GameController::addColsRight(unsigned int inNr)
{
    mWindow->addColsRight(inNr);
}

void
GameController::onTilesLeft(unsigned int inNr)
{
    mWindow->displayTilesLeft(inNr);
}
