#include "Controller/GameController.h"

#include "ModelViewGlue.h"

#include "Model/Color.h"
#include "View/Typedefs.h"
#include "View/DragData.h"

#include <cassert>

Controller::GameController::GameController( QObject *parent )
:
	QObject( parent ),
	mGame( new Model::Game( this ) ),
	mWindow( new View::GameWindow() )
{
	addPlayers();
	makeConnections();
	startGame();
}

Controller::GameController::GameController( std::string const & inTiles, QObject * inParent )
:
	QObject( inParent ),
	mGame( new Model::Game( inTiles, this ) ),
	mWindow( new View::GameWindow() )
{
	addPlayers();
	makeConnections();
	startGame();
}

Controller::GameController::GameController( std::vector< Model::Player > const & inPlayers, QObject * inParent )
:
	QObject( inParent ),
	mGame( new Model::Game( inPlayers, this ) ),
	mWindow( new View::GameWindow() )
{
	addPlayers();
	makeConnections();
	startGame();
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
	int x = Controller::xFromCol( inCol, mGame->getStartCol() );
	int y = Controller::yFromRow( inRow, mGame->getStartRow() );
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
	int x = Controller::xFromCol( inCol, mGame->getStartCol() );
	int y = Controller::yFromRow( inRow, mGame->getStartRow() );
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
	int x = Controller::xFromCol( inCol, mGame->getStartCol() );
	int y = Controller::yFromRow( inRow, mGame->getStartRow() );
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
	int x = Controller::xFromCol( inCol, mGame->getStartCol() );
	int y = Controller::yFromRow( inRow, mGame->getStartRow() );
	x += Controller::xFromArea( inArea ) - .5 * Gui::kTileWidth;
	y += Controller::yFromArea( inArea ) - .5 * Gui::kTileHeight;
	mWindow->placePiece( x, y, Controller::viewFromModel( inCurrentPlayer.getColor() ) );
}

void
Controller::GameController::onPieceReturned( unsigned inCol, unsigned inRow, Model::Area::Area inArea, Model::Player const & inPlayer )
{
	std::cout << inPlayer.getName() << " got a piece back." << std::endl;
	int x = Controller::xFromCol( inCol, mGame->getStartCol() );
	int y = Controller::yFromRow( inRow, mGame->getStartRow() );
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
Controller::GameController::onClicked( int inX, int inY, std::string const & inTileId, View::Rotation inRotation )
{
	unsigned col = Controller::colFromX( inX, mGame->getStartCol() );
	unsigned row = Controller::rowFromY( inY, mGame->getStartRow() );
	std::cout << "GameController sees a click at x, y: " << inX << ", " << inY << ", which is col, row: " << col << ", " << row << std::endl;
	if ( col < mGame->getNrOfCols() && row < mGame->getNrOfRows() )
	{
		mGame->clickTile( col, row, inTileId, Controller::modelFromView( inRotation ) );
	}
}

void
Controller::GameController::onTileDropped( int inX, int inY, std::string const & inTileId, View::Rotation inRotation )
{
	unsigned col = colFromX( inX, mGame->getStartCol() );
	unsigned row = rowFromY( inY, mGame->getStartRow() );
	mGame->dropTile( col, row, inTileId, modelFromView( inRotation ) );
}

void
Controller::GameController::onTryToPlacePiece( Dragging::PieceData const & inData, int inX, int inY )
{
	unsigned col = colFromX( inX, mGame->getStartCol() );
	unsigned row = rowFromY( inY, mGame->getStartRow() );
	std::cout << "GameController sees drop at x, y: " << inX << ", " << inY << ", which is col, row: " << col << ", " << row << std::endl;
	std::cout << "Which is at " << posXFromX( inX ) << ", " << posYFromY( inY ) << " at that tile." << std::endl;
	std::cout << "Which is at Area::" << areaFromPos( posXFromX( inX ), posYFromY( inY ) ) << std::endl;
	std::cout << "Drop contains " << inData.getColor() << " " << inData.getPiece() << std::endl;
	if ( col < mGame->getNrOfCols() && row < mGame->getNrOfRows() )
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
	for ( std::vector< Model::Player >::const_iterator it = mGame->getPlayers().begin();
		it != mGame->getPlayers().end();
		++it )
	{
		mWindow->addPlayer( it->getName(), viewFromModel( it->getColor() ), it->getNumberOfFreePieces() );
	}
}

void
Controller::GameController::makeConnections()
{
	connect( mGame, SIGNAL( tilePlaced( unsigned int, unsigned int, std::string, Model::Rotation ) ),
			this, SLOT( onTilePlaced( unsigned int, unsigned int, std::string, Model::Rotation ) ) );
	connect( mGame, SIGNAL( tileUnplaced( unsigned int, unsigned int ) ),
			this, SLOT( onTileUnplaced( unsigned int, unsigned int ) ) );
	connect( mGame, SIGNAL( tileRotated( uint,uint,std::string,Model::Rotation ) ),
			this, SLOT( onTileRotated( uint,uint,std::string,Model::Rotation ) ) );
	connect( mGame, SIGNAL( nextTile( std::string ) ), this, SLOT( onNextTile( std::string ) ) );
	connect( mGame, SIGNAL( tilesLeft( uint ) ), this, SLOT( onTilesLeft( uint ) ) );

	connect( mGame, SIGNAL( piecePlaced( uint, uint, Model::Area::Area, Model::Player ) ),
		this, SLOT( onPiecePlaced( uint, uint, Model::Area::Area, Model::Player ) ) );
	connect( mGame, SIGNAL( pieceReturned( uint, uint, Model::Area::Area, Model::Player ) ),
		this, SLOT( onPieceReturned( uint, uint, Model::Area::Area, Model::Player ) ) );
	connect( mGame, SIGNAL( playerInfoChanged( Model::Player ) ),
		this, SLOT( onPlayerInfoChanged( Model::Player ) ) );
	connect( mGame, SIGNAL( currentPlayerChanged( Model::Player ) ),
		this, SLOT( onCurrentPlayerChanged( Model::Player ) ) );

	connect( mWindow, SIGNAL( clicked( int, int, std::string const &, View::Rotation ) ),
		this, SLOT( onClicked( int, int, std::string const &, View::Rotation ) ) );
	connect( mWindow, SIGNAL( tileDropped( int, int, std::string const &, View::Rotation ) ),
		this, SLOT( onTileDropped( int, int, std::string const &, View::Rotation ) ) );
	connect( mWindow, SIGNAL( tryToPlacePiece( Dragging::PieceData, int, int ) ),
		this, SLOT( onTryToPlacePiece( Dragging::PieceData, int, int ) ) );
	connect( mWindow, SIGNAL( endCurrentTurn() ), mGame, SLOT( onEndCurrentTurn() ) );

	connect( mGame, SIGNAL( endOfGame(uint) ), this, SLOT( onEndOfGame(uint) ) );
}

void
Controller::GameController::startGame()
{
	onCurrentPlayerChanged( mGame->getCurrentPlayer() );
	mWindow->show();
	if ( mGame->getNextTile() )
	{
		mGame->placeStartTileOnBoard();
	}
}
