#include "GameController.h"

#include "src-view/GuiConstants.h"

namespace
{
    unsigned int
    fromXToCol(int inX, unsigned int inStartCol)
    {
        return (inX / GuiConstants::tileWidth + inStartCol - (inX < 0 ? 1 : 0));
    }

    unsigned int
    fromYToRow(int inY, unsigned int inStartRow)
    {
        return (inY / GuiConstants::tileHeight + inStartRow - (inY < 0 ? 1 : 0));
    }

    int
    fromColToX(unsigned int inCol, unsigned int inStartCol)
    {
        return ( ((int)inCol - (int)inStartCol) * GuiConstants::tileWidth );
    }

    int
    fromRowToY(unsigned int inRow, unsigned int inStartRow)
    {
        return ( ((int)inRow - (int)inStartRow) * GuiConstants::tileHeight );
    }
}

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
    connect( mGame, SIGNAL( finishedCloister(uint, uint)), this, SLOT( onFinishedCloister(uint, uint)));
    connect( mWindow, SIGNAL( clicked(int,int) ), this, SLOT( onClicked(int,int) ) );
    connect( mWindow, SIGNAL( submitCurrentTile() ), mGame, SLOT( onSubmitCurrentTile() ) );
    mWindow->show();
    if (mGame->getNextTile())
    {
        mGame->placeStartTileOnBoard();
    }
}

void
GameController::onTilePlaced(unsigned int inCol, unsigned int inRow, std::string inId, TileOnBoard::Rotation inRot)
{
    int x = fromColToX( inCol, mGame->getStartCol() );
    int y = fromRowToY( inRow, mGame->getStartRow() );
    mWindow->setTile(x, y, inId, inRot * 30);
    mWindow->fadeNextTile();
}

void
GameController::onTileUnplaced(unsigned int inCol, unsigned int inRow)
{
    int x = fromColToX( inCol, mGame->getStartCol() );
    int y = fromRowToY( inRow, mGame->getStartRow() );
    mWindow->clearTile(x, y);
}

void
GameController::onClicked(int x, int y)
{
    unsigned int col = fromXToCol( x, mGame->getStartCol() );
    unsigned int row = fromYToRow( y, mGame->getStartRow() );
    std::cout << "GameController sees a click at x, y: " << x << ", " << y << ", which is col, row: " << col << ", " << row << std::endl;
    if (col >= 0 && col < mGame->getNrOfCols() && row >= 0 && row < mGame->getNrOfRows())
    {
        mGame->clickTile(col, row);
    }
}

void
GameController::onTileRotated(unsigned int inCol, unsigned int inRow, std::string inId, TileOnBoard::Rotation inRot)
{
    int x = fromColToX( inCol, mGame->getStartCol() );
    int y = fromRowToY( inRow, mGame->getStartRow() );
    mWindow->rotateTile(x, y, inId, inRot * 30);
}

void
GameController::onNextTile( std::string inNextId )
{
    mWindow->setNextTile(inNextId);
}

void
GameController::onTilesLeft(unsigned int inNr)
{
    mWindow->displayTilesLeft(inNr);
}

void
GameController::onFinishedCloister(unsigned int inCol, unsigned int inRow)
{
    std::cout << "Finished cloister on tile " << inCol << ", " << inRow << std::endl;
    int x = fromColToX( inCol, mGame->getStartCol() );
    int y = fromRowToY( inRow, mGame->getStartRow() );
    mWindow->finishCloister( x, y );
}