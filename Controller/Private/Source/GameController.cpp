#include "Controller/GameController.h"

#include "ModelViewGlue.h"

#include "Model/Color.h"
#include "View/Typedefs.h"
#include "View/DragData.h"

#include <iostream>

Controller::GameController::GameController( std::string const & inTiles, QObject * inParent )
:
	QObject( inParent ),
	mGame(),
	mWindow( new View::GameWindow() ),
	mPlayers(),
	mNrRows( 0 ),
	mNrCols( 0 ),
	mStartRow( 0 ),
	mStartCol( 0 )
{
	// Set up default players
	mPlayers.push_back( Model::Player( "Yumi", Model::Color::kYellow ) );
	mPlayers.push_back( Model::Player( "Thomas", Model::Color::kGreen ) );
	// Create game with these players
	mGame.reset( new Model::Game( mPlayers, inTiles, this ) );
	addPlayers();
	makeConnections();
	startGame();
}

Controller::GameController::GameController( std::vector< Model::Player > const & inPlayers, QObject * inParent )
:
	QObject( inParent ),
	mGame( new Model::Game( inPlayers, this ) ),
	mWindow( new View::GameWindow() ),
	mPlayers( inPlayers ),
	mNrRows( 0 ),
	mNrCols( 0 ),
	mStartRow( 0 ),
	mStartCol( 0 )
{
	addPlayers();
	makeConnections();
	startGame();
}

void
Controller::GameController::onDimensionsChanged( unsigned inNrOfRows, unsigned inNrCols, unsigned inStartRow, unsigned inStartCol )
{
	mNrRows = inNrOfRows;
	mNrCols = inNrCols;
	mStartRow = inStartRow;
	mStartCol = inStartCol;
}

void
Controller::GameController::onTilePlaced
(
	unsigned inCol,
	unsigned inRow,
	std::string const & inId,
	Model::Rotation inRotation
)
{
	int x = Controller::xFromCol( inCol, mStartCol );
	int y = Controller::yFromRow( inRow, mStartRow );
	mWindow->setTile( x, y, inId, Controller::viewFromModel( inRotation ) );
	mWindow->fadeNextTile();
}

void
Controller::GameController::onTileUnplaced
(
	unsigned inCol,
	unsigned inRow
)
{
	int x = Controller::xFromCol( inCol, mStartCol );
	int y = Controller::yFromRow( inRow, mStartRow );
	mWindow->clearTile( x, y );
}

void
Controller::GameController::onTileRotated
(
	unsigned inCol,
	unsigned inRow,
	std::string const & inId,
	Model::Rotation inRotation
)
{
	int x = Controller::xFromCol( inCol, mStartCol );
	int y = Controller::yFromRow( inRow, mStartRow );
	mWindow->rotateTile( x, y, inId, Controller::viewFromModel( inRotation ) );
}

void
Controller::GameController::onNextTile( std::string const & inNextId )
{
	mWindow->setNextTile( inNextId );
}

void
Controller::GameController::onTilesLeft( unsigned inNr )
{
	mWindow->displayTilesLeft(inNr);
}

void
Controller::GameController::onPiecePlaced( unsigned inCol, unsigned inRow, Model::Area::Area inArea, Model::Player const & inCurrentPlayer )
{
	std::cout << inCurrentPlayer.getName() << " placed a piece." << std::endl;
	int x = Controller::xFromCol( inCol, mStartCol );
	int y = Controller::yFromRow( inRow, mStartRow );
	x += Controller::xFromArea( inArea ) - .5 * Gui::kTileWidth;
	y += Controller::yFromArea( inArea ) - .5 * Gui::kTileHeight;
	mWindow->placePiece( x, y, Controller::viewFromModel( inCurrentPlayer.getColor() ) );
}

void
Controller::GameController::onPieceReturned( unsigned inCol, unsigned inRow, Model::Area::Area inArea, Model::Player const & inPlayer )
{
	std::cout << inPlayer.getName() << " got a piece back." << std::endl;
	int x = Controller::xFromCol( inCol, mStartCol );
	int y = Controller::yFromRow( inRow, mStartRow );
	x += Controller::xFromArea( inArea ) - .5 * Gui::kTileWidth;
	y += Controller::yFromArea( inArea ) - .5 * Gui::kTileHeight;
	mWindow->returnPiece( x, y, Controller::viewFromModel( inPlayer.getColor() ) );
}

void
Controller::GameController::onPlayerInfoChanged( Model::Player const & inNewInfo )
{
	mWindow->setFollowersLeft( inNewInfo.getName(), inNewInfo.getNumberOfFreePieces() );
	mWindow->setScore( inNewInfo.getName(), inNewInfo.getScore() );
}

void
Controller::GameController::onCurrentPlayerChanged( Model::Player const & inCurrentPlayer )
{
	mWindow->setActivePlayer( inCurrentPlayer.getName() );
}

void
Controller::GameController::onEndOfGame( unsigned inTilesLeft )
{
	mGame->calculateEndPoints();
	std::cout << "Game has ended." << std::endl;
	if ( inTilesLeft >= 1 )
	{
		std::cout << "Number of unplayed tiles: " << inTilesLeft << std::endl;
	}
}

void
Controller::GameController::onClicked( int inX, int inY )
{
	unsigned col = colFromX( inX, mStartCol );
	unsigned row = rowFromY( inY, mStartRow );
	mGame->rotateTile( col, row );
}

void
Controller::GameController::onTileDropped( int inX, int inY, std::string const & inTileId, View::Rotation inRotation )
{
	unsigned col = colFromX( inX, mStartCol );
	unsigned row = rowFromY( inY, mStartRow );
	mGame->dropTile( col, row, inTileId, modelFromView( inRotation ) );
}

void
Controller::GameController::onTryToPlacePiece( Dragging::PieceData const & inData, int inX, int inY )
{
	unsigned col = colFromX( inX, mStartCol );
	unsigned row = rowFromY( inY, mStartRow );
	std::cout << "GameController sees drop at x, y: " << inX << ", " << inY << ", which is col, row: " << col << ", " << row << std::endl;
	std::cout << "Which is at " << posXFromX( inX ) << ", " << posYFromY( inY ) << " at that tile." << std::endl;
	std::cout << "Which is at Area::" << areaFromPos( posXFromX( inX ), posYFromY( inY ) ) << std::endl;
	std::cout << "Drop contains " << inData.getColor() << " " << inData.getPiece() << std::endl;
	if ( col < mNrCols && row < mNrRows )
	{
		// Decipher data
		Model::Color::Color color = modelFromView( inData.getColor() );
		Model::Piece::PieceType type = modelFromView( inData.getPiece() );
		Model::Area::Area area = areaFromPos( posXFromX( inX ), posYFromY( inY ) );
		// Send to mGame
		mGame->tryToPlacePiece( color, type, col, row, area );
	}
}

void
Controller::GameController::addPlayers()
{
	for ( std::vector< Model::Player >::const_iterator it = mPlayers.begin();
		it != mPlayers.end();
		++it )
	{
		mWindow->addPlayer( it->getName(), viewFromModel( it->getColor() ), it->getNumberOfFreePieces() );
	}
}

void
Controller::GameController::makeConnections()
{
	connect( mGame.get(), SIGNAL( tilePlaced( unsigned int, unsigned int, std::string, Model::Rotation ) ),
			this, SLOT( onTilePlaced( unsigned int, unsigned int, std::string, Model::Rotation ) ) );
	connect( mGame.get(), SIGNAL( tileUnplaced( unsigned int, unsigned int ) ),
			this, SLOT( onTileUnplaced( unsigned int, unsigned int ) ) );
	connect( mGame.get(), SIGNAL( tileRotated( uint,uint,std::string,Model::Rotation ) ),
			this, SLOT( onTileRotated( uint,uint,std::string,Model::Rotation ) ) );
	connect( mGame.get(), SIGNAL( nextTile( std::string ) ), this, SLOT( onNextTile( std::string ) ) );
	connect( mGame.get(), SIGNAL( tilesLeft( uint ) ), this, SLOT( onTilesLeft( uint ) ) );

	connect( mGame.get(), SIGNAL( piecePlaced( uint, uint, Model::Area::Area, Model::Player ) ),
		this, SLOT( onPiecePlaced( uint, uint, Model::Area::Area, Model::Player ) ) );
	connect( mGame.get(), SIGNAL( pieceReturned( uint, uint, Model::Area::Area, Model::Player ) ),
		this, SLOT( onPieceReturned( uint, uint, Model::Area::Area, Model::Player ) ) );
	connect( mGame.get(), SIGNAL( playerInfoChanged( Model::Player ) ),
		this, SLOT( onPlayerInfoChanged( Model::Player ) ) );
	connect( mGame.get(), SIGNAL( currentPlayerChanged( Model::Player ) ),
		this, SLOT( onCurrentPlayerChanged( Model::Player ) ) );

	connect( mWindow.get(), SIGNAL( clicked( int, int ) ), this, SLOT( onClicked( int, int ) ) );
	connect( mWindow.get(), SIGNAL( tileDropped( int, int, std::string const &, View::Rotation ) ),
		this, SLOT( onTileDropped( int, int, std::string const &, View::Rotation ) ) );
	connect( mWindow.get(), SIGNAL( tryToPlacePiece( Dragging::PieceData, int, int ) ),
		this, SLOT( onTryToPlacePiece( Dragging::PieceData, int, int ) ) );
	connect( mWindow.get(), SIGNAL( endCurrentTurn() ), mGame.get(), SLOT( onEndCurrentTurn() ) );

	connect( mGame.get(), SIGNAL(dimensionsChanged(unsigned,unsigned,unsigned,unsigned)),
		this, SLOT(onDimensionsChanged(unsigned,unsigned,unsigned,unsigned)) );
	connect( mGame.get(), SIGNAL( endOfGame(uint) ), this, SLOT( onEndOfGame(uint) ) );
}

void
Controller::GameController::startGame()
{
	onCurrentPlayerChanged( mPlayers.front() );
	mWindow->show();
	mGame->placeStartTileOnBoard();
}
