#include "GameController.h"

#include "GuiConstants.h"

GameController::GameController(QObject *parent) :
    QObject(parent),
    mGame(new Game()),
    mWindow(new GameWindow())
{
    connect(mGame, SIGNAL(tilePlaced(unsigned int, unsigned int, std::string, TileOnBoard::Rotation, std::string)),
            this, SLOT(placeTile(unsigned int, unsigned int, std::string, TileOnBoard::Rotation, std::string)));
    connect(mWindow, SIGNAL(clicked(int,int)), this, SLOT(onClicked(int,int)));
    connect(mGame, SIGNAL(tileRotated(uint,uint,std::string,TileOnBoard::Rotation)),
            this, SLOT(rotateTile(uint,uint,std::string,TileOnBoard::Rotation)));
    connect(mGame, SIGNAL(tilesLeft(uint)), this, SLOT(onTilesLeft(uint)));
    mWindow->show();
    if (mGame->getNextTile())
    {
        mGame->placeStartTileOnBoard();
    }
}

void
GameController::placeTile(unsigned int inCol, unsigned int inRow, std::string inId, TileOnBoard::Rotation inRot, std::string inNextId)
{
    int x = ((int)inCol - (int)mGame->getStartCol()) * GuiConstants::tileWidth;
    int y = ((int)inRow - (int)mGame->getStartRow()) * GuiConstants::tileHeight;
    mWindow->setTile(x, y, inId, inRot * 30);
    mWindow->setNextTile(inNextId);
}

void
GameController::onClicked(int x, int y)
{
    unsigned int col = x / GuiConstants::tileWidth + mGame->getStartCol() - (x < 0 ? 1 : 0);
    unsigned int row = y / GuiConstants::tileHeight + mGame->getStartRow() - (y < 0 ? 1 : 0);
    std::cout << "GameController sees a click at x, y: " << x << ", " << y << ", which is col, row: " << col << ", " << row << std::endl;
    if (col >= 0 && col < mGame->getNrOfCols() && row >= 0 && row < mGame->getNrOfRows())
    {
        mGame->clickTile(col, row);
    }
}

void
GameController::rotateTile(unsigned int inCol, unsigned int inRow, std::string inId, TileOnBoard::Rotation inRot)
{
    int x = ((int)inCol - (int)mGame->getStartCol()) * GuiConstants::tileWidth;
    int y = ((int)inRow - (int)mGame->getStartRow()) * GuiConstants::tileHeight;
    std::cout << "GameController sees a rotation" << std::endl;
    mWindow->setTile(x, y, inId, inRot * 30);
}

void
GameController::onTilesLeft(unsigned int inNr)
{
    mWindow->displayTilesLeft(inNr);
}
