#include "src-model/Game.h"

#include "src-model/CreateBaseGameTiles.h"

#include <stdio.h>

namespace
{
int extraRowsAndCols = 1;
int size = 5;
}

Game::Game() :
    mBoard(new Board(size)),
    mBag(),
    mNextTile(),
    mStartRow( 0 ),
    mStartCol( 0 ),
    mCurrentPlacedRow( (unsigned int)-1 ),
    mCurrentPlacedCol( (unsigned int)-1 ),
    mPlayers(),
    mCurrentPlayer( 0 )
{
    mBag = createBaseGameTiles();
    for ( unsigned int i = 0; i < mBag.size(); ++i )
    {
        std::cout << mBag[i].getID();
    }
    std::cout << std::endl;
    if ( !mBag.empty() )
    {
        mNextTile = mBag.back();
        mBag.pop_back();
    }
    mPlayers.push_back( Player( "Thomas", Color::Green ) );
    mPlayers.push_back( Player( "Gijs", Color::Red ) );
    emit currentPlayerChanged( mPlayers.front() );
    connect
    (
        mBoard, SIGNAL( tileRotated(uint,uint,std::string,TileOnBoard::Rotation) ),
        this, SLOT( onTileRotated(uint,uint,std::string,TileOnBoard::Rotation) )
    );
    connect( mBoard, SIGNAL( finishedCloister(uint, uint) ), this, SIGNAL( finishedCloister(uint, uint) ) );
    connect
    (
        mBoard, SIGNAL( finishedCity(std::vector< std::pair< uint, uint > >) ),
        this, SIGNAL( finishedCity(std::vector< std::pair< uint, uint > >) )
    );
    connect
    (
        mBoard, SIGNAL( finishedRoad(std::vector< std::pair< uint, uint > >) ),
        this, SIGNAL( finishedRoad(std::vector< std::pair< uint, uint > >) )
    );
}

unsigned int
Game::getNrOfRows() const
{
    return mBoard->getNrOfRows();
}

unsigned int
Game::getNrOfCols() const
{
    return mBoard->getNrOfCols();
}

unsigned int
Game::getStartRow() const
{
    return mStartRow;
}

unsigned int
Game::getStartCol() const
{
    return mStartCol;
}

Player const &
Game::getCurrentPlayer() const
{
    return mPlayers[mCurrentPlayer];
}

void
Game::clickTile(unsigned int inCol, unsigned int inRow)
{
    if (mBoard->isEmptySpot(inCol, inRow))
    {
        std::cout << "Empty spot -> placeTileOnBoard" << std::endl;
        placeTileOnBoard(inCol, inRow);
    }
    else
    {
        std::cout << "Non-empty spot -> rotateTileOnBoard" << std::endl;
        rotateTileOnBoard(inCol, inRow);
    }
    std::cout << mBoard->toString() << std::flush;
}

void
Game::placeTileOnBoard(unsigned int inCol, unsigned int inRow)
{
    if (mNextTile)
    {
        // Delete tile that was placed earlier in current turn
        boost::optional< TileOnBoard > earlierTile;
        if (mCurrentPlacedCol != (unsigned int)-1 && mCurrentPlacedRow != (unsigned int)-1)
        {
            earlierTile = mBoard->removeTile(mCurrentPlacedCol, mCurrentPlacedRow);
            emit tileUnplaced(mCurrentPlacedCol, mCurrentPlacedRow);
        }
        // Try the newly clicked position (try all rotations until we found a good one)
        TileOnBoard::Rotation rotation = TileOnBoard::cw0;
        TileOnBoard toBePlacedTile = TileOnBoard(mNextTile.get(), rotation);
        bool found = false;
        for (int i = 0; i < 4; ++i)
        {
            toBePlacedTile = TileOnBoard(mNextTile.get(), rotation);
            if (mBoard->isValidTilePlacement(toBePlacedTile, inCol, inRow))
            {
                found = true;
                break;
            }
            else
            {
                rotation = TileOnBoard::Rotation(rotation + TileOnBoard::cw90);
            }
        }
        std::cout << "Found: " << found << std::endl;
        if (found)
        {
            // If a valid placement is found at the newly clicked position, place tile
            if (mBoard->placeValidTile(toBePlacedTile, inCol, inRow))
            {
                unsigned int col = inCol;
                unsigned int row = inRow;
                if (inCol == 0)
                {
                    mBoard->addColsLeft( extraRowsAndCols );
                    col += extraRowsAndCols;
                    mStartCol += extraRowsAndCols;
                }
                else if (inCol == mBoard->getNrOfCols() - 1)
                {
                    mBoard->addColsRight( extraRowsAndCols );
                }
                else if (inRow == 0)
                {
                    mBoard->addRowsOnTop( extraRowsAndCols );
                    row += extraRowsAndCols;
                    mStartRow += extraRowsAndCols;
                }
                else if (inRow == mBoard->getNrOfRows() - 1)
                {
                    mBoard->addRowsBelow( extraRowsAndCols );
                }
                mCurrentPlacedCol = col;
                mCurrentPlacedRow = row;
                emit tilePlaced(mCurrentPlacedCol, mCurrentPlacedRow, toBePlacedTile.getID(), toBePlacedTile.getRotation());
            }
        }
        else
        {
            // If no valid placement is found at the newly clicked position, restore earlier tile
            if (earlierTile)
            {
                mBoard->placeValidTile(*earlierTile, mCurrentPlacedCol, mCurrentPlacedRow);
                emit tilePlaced(mCurrentPlacedCol, mCurrentPlacedRow, earlierTile->getID(), earlierTile->getRotation());
            }
        }
    }
}

void
Game::placeStartTileOnBoard()
{
    if (mNextTile)
    {
        TileOnBoard::Rotation rotation = TileOnBoard::cw0; // TODO: get a random Rotation each time
        TileOnBoard toBePlacedTile = TileOnBoard(mNextTile.get(), rotation);
        unsigned int pos = mBoard->placeStartTile(toBePlacedTile);
        mStartCol = pos % mBoard->getNrOfCols();
        mStartRow = pos / mBoard->getNrOfCols();
        pickNextTile();
        emit tilePlaced(mStartCol, mStartRow, toBePlacedTile.getID(), toBePlacedTile.getRotation());
        if (mNextTile)
        {
            emit nextTile(mNextTile->getID());
        }
    }
}

void
Game::rotateTileOnBoard(unsigned int inCol, unsigned int inRow)
{
    if (inCol == mCurrentPlacedCol && inRow == mCurrentPlacedRow)
    {
        std::cout << "Rotating " << inCol << ", " << inRow << std::endl;
        mBoard->rotateTileOnBoard(inCol, inRow);
    }
    else
    {
        std::cout << "Not rotating" << std::endl;
    }
}

boost::optional< Tile >
Game::getNextTile() const
{
    return mNextTile;
}

void
Game::endTurn()
{
    mCurrentPlayer = (mCurrentPlayer + 1) % mPlayers.size();
    emit currentPlayerChanged( mPlayers[mCurrentPlayer] );
}

void
Game::onTileRotated(unsigned int inCol, unsigned int inRow, std::string inId, TileOnBoard::Rotation inRot)
{
    std::cout << "Game sees a rotation" << std::endl;
    emit tileRotated(inCol, inRow, inId, inRot);
}

void
Game::onTryToPlacePiece()
{
    if ( mCurrentPlacedRow != (unsigned int)-1
        && mCurrentPlacedCol != (unsigned int)-1
        && mPlayers[mCurrentPlayer].hasFreePieces() )
    {
        if ( mNextTile && mNextTile->getCenter() == Tile::Cloister )
        {
            // Place a meeple on this cloister
            if ( mPlayers[mCurrentPlayer].placePiece( mCurrentPlacedRow * mBoard->getNrOfCols() + mCurrentPlacedCol, Area::Central ) )
            {
                emit piecePlaced( mCurrentPlacedCol, mCurrentPlacedRow, mPlayers[mCurrentPlayer] );
            }
        }
    }
}

void
Game::onEndCurrentTurn()
{
    if (mCurrentPlacedRow != (unsigned int)-1 && mCurrentPlacedCol != (unsigned int)-1)
    {
        mBoard->checkForFinishedCloisters( mCurrentPlacedCol, mCurrentPlacedRow );
        mBoard->checkForFinishedCities( mCurrentPlacedCol, mCurrentPlacedRow );
        mBoard->checkForFinishedRoads( mCurrentPlacedCol, mCurrentPlacedRow );
        mCurrentPlacedRow = (unsigned int)-1;
        mCurrentPlacedCol = (unsigned int)-1;
        pickNextTile();
        if (mNextTile)
        {
            emit nextTile(mNextTile->getID());
        }
        endTurn();
    }
}

void
Game::pickNextTile()
{
    if (!mBag.empty())
    {
        std::vector< Tile >::iterator it = mBag.end();
        bool foundPossible = false;
        while (!foundPossible)
        {
            if (it != mBag.begin())
            {
                --it;
                Tile maybeNextTile = *it;
                if (mBoard->isPossibleTile(maybeNextTile))
                {
                    mNextTile = maybeNextTile;
                    mBag.erase(it);
                    foundPossible = true;
                }
            }
            else
            {
                // No more valid tiles to place, this should give End Of Game
                mNextTile = boost::optional< Tile >();
                emit endOfGame( mBag.size() );
            }
        }
        emit tilesLeft(mBag.size());
    }
    else
    {
        mNextTile = boost::optional< Tile >();
        emit endOfGame( 0 );
    }
}
