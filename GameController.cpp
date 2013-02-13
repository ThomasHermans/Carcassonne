#include "GameController.h"

#include "src-view/GuiConstants.h"

GameController::GameController(QObject *parent) :
    QObject(parent),
    mGame(new Game()),
    mWindow(new GameWindow())
{
    connect( mGame, SIGNAL( tilePlaced(unsigned int, unsigned int, std::string, TileOnBoard::Rotation) ),
            this, SLOT( onTilePlaced(unsigned int, unsigned int, std::string, TileOnBoard::Rotation) ) );
    connect( mGame, SIGNAL( tileUnplaced(unsigned int, unsigned int) ),
            this, SLOT( onTileUnplaced(unsigned int, unsigned int) ) );
    connect( mGame, SIGNAL( tileRotated(uint,uint,std::string,TileOnBoard::Rotation) ),
            this, SLOT( onTileRotated(uint,uint,std::string,TileOnBoard::Rotation) ) );
    connect( mGame, SIGNAL( nextTile(std::string) ), this, SLOT( onNextTile(std::string) ) );
    connect( mGame, SIGNAL( tilesLeft(uint) ), this, SLOT( onTilesLeft(uint) ) );
    connect( mWindow, SIGNAL( clicked(int,int) ), this, SLOT( onClicked(int,int) ) );
    connect( mWindow, SIGNAL( submitCurrentTile() ), this, SLOT( onSubmitCurrentTile() ) );
    mWindow->show();
    if (mGame->getNextTile())
    {
        mGame->placeStartTileOnBoard();
    }
}

void
GameController::onTilePlaced(unsigned int inCol, unsigned int inRow, std::string inId, TileOnBoard::Rotation inRot)
{
    int x = ((int)inCol - (int)mGame->getStartCol()) * GuiConstants::tileWidth;
    int y = ((int)inRow - (int)mGame->getStartRow()) * GuiConstants::tileHeight;
    mWindow->setTile(x, y, inId, inRot * 30);
    mWindow->fadeNextTile();
}

void
GameController::onTileUnplaced(unsigned int inCol, unsigned int inRow)
{
    int x = ((int)inCol - (int)mGame->getStartCol()) * GuiConstants::tileWidth;
    int y = ((int)inRow - (int)mGame->getStartRow()) * GuiConstants::tileHeight;
    mWindow->clearTile(x, y);
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
GameController::onTileRotated(unsigned int inCol, unsigned int inRow, std::string inId, TileOnBoard::Rotation inRot)
{
    int x = ((int)inCol - (int)mGame->getStartCol()) * GuiConstants::tileWidth;
    int y = ((int)inRow - (int)mGame->getStartRow()) * GuiConstants::tileHeight;
    mWindow->rotateTile(x, y, inId, inRot * 30);
}

void
GameController::onNextTile( std::string inNextId )
{
    mWindow->setNextTile(inNextId);
}

void
GameController::onSubmitCurrentTile()
{
    mGame->submitCurrentTile();
}

void
GameController::onTilesLeft(unsigned int inNr)
{
    mWindow->displayTilesLeft(inNr);
}
