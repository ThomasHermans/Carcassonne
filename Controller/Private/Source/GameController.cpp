#include "Controller/GameController.h"

#include "ModelViewGlue.h"

#include "Model/Color.h"
#include "View/Typedefs.h"
#include "View/DragData.h"

#include <cassert>

GameController::GameController( QObject *parent )
:
	QObject( parent ),
	mGame( new Game( this ) ),
	mWindow( new View::GameWindow() )
{
	addPlayers();
	makeConnections();
	startGame();
}

GameController::GameController( std::string const & inTiles, QObject * inParent )
:
	QObject( inParent ),
	mGame( new Game( inTiles, this ) ),
	mWindow( new View::GameWindow() )
{
	addPlayers();
	makeConnections();
	startGame();
}

GameController::GameController( std::vector< Player > const & inPlayers, QObject * inParent )
:
	QObject( inParent ),
	mGame( new Game( inPlayers, this ) ),
	mWindow( new View::GameWindow() )
{
	addPlayers();
	makeConnections();
	startGame();
}

void
GameController::onTilePlaced
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
GameController::onTileUnplaced
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
GameController::onTileRotated
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
GameController::onNextTile( std::string const & inNextId )
{
	mWindow->setNextTile( inNextId );
}

void
GameController::onTilesLeft( unsigned inNr )
{
	mWindow->displayTilesLeft(inNr);
}

void
GameController::onPiecePlaced( unsigned inCol, unsigned inRow, Area::Area inArea, Player const & inCurrentPlayer )
{
	std::cout << inCurrentPlayer.getName() << " placed a piece." << std::endl;
	int x = Controller::xFromCol( inCol, mGame->getStartCol() );
	int y = Controller::yFromRow( inRow, mGame->getStartRow() );
	x += Controller::xFromArea( inArea ) - .5 * Gui::kTileWidth;
	y += Controller::yFromArea( inArea ) - .5 * Gui::kTileHeight;
	mWindow->placePiece( x, y, Controller::viewFromModel( inCurrentPlayer.getColor() ) );
}

void
GameController::onPieceReturned( unsigned inCol, unsigned inRow, Area::Area inArea, Player const & inPlayer )
{
	std::cout << inPlayer.getName() << " got a piece back." << std::endl;
	int x = Controller::xFromCol( inCol, mGame->getStartCol() );
	int y = Controller::yFromRow( inRow, mGame->getStartRow() );
	x += Controller::xFromArea( inArea ) - .5 * Gui::kTileWidth;
	y += Controller::yFromArea( inArea ) - .5 * Gui::kTileHeight;
	mWindow->returnPiece( x, y, Controller::viewFromModel( inPlayer.getColor() ) );
}

void
GameController::onPlayerInfoChanged( Player const & inNewInfo )
{
	mWindow->setFollowersLeft( inNewInfo.getName(), inNewInfo.getNumberOfFreePieces() );
	mWindow->setScore( inNewInfo.getName(), inNewInfo.getScore() );
}

void
GameController::onCurrentPlayerChanged( Player const & inCurrentPlayer )
{
	mWindow->setActivePlayer( inCurrentPlayer.getName() );
}

void
GameController::onEndOfGame( unsigned inTilesLeft )
{
	mGame->calculateEndPoints();
	std::cout << "Game has ended." << std::endl;
	if ( inTilesLeft >= 1 )
	{
		std::cout << "Number of unplayed tiles: " << inTilesLeft << std::endl;
	}
}

void
GameController::onClicked( int inX, int inY, std::string const & inTileId, View::Rotation inRotation )
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
GameController::onTileDropped( int inX, int inY, std::string const & inTileId, View::Rotation inRotation )
{
	unsigned col = Controller::colFromX( inX, mGame->getStartCol() );
	unsigned row = Controller::rowFromY( inY, mGame->getStartRow() );
	mGame->dropTile( col, row, inTileId, Controller::modelFromView( inRotation ) );
}

void
GameController::onTryToPlacePiece( Dragging::PieceData const & inData, int inX, int inY )
{
	unsigned col = Controller::colFromX( inX, mGame->getStartCol() );
	unsigned row = Controller::rowFromY( inY, mGame->getStartRow() );
	std::cout << "GameController sees drop at x, y: " << inX << ", " << inY << ", which is col, row: " << col << ", " << row << std::endl;
	std::cout << "Which is at " << Controller::posXFromX( inX ) << ", " << Controller::posYFromY( inY ) << " at that tile." << std::endl;
	std::cout << "Which is at Area::" << Controller::areaFromPos( Controller::posXFromX( inX ), Controller::posYFromY( inY ) ) << std::endl;
	std::cout << "Drop contains " << inData.getColor() << " " << inData.getPiece() << std::endl;
	if ( col < mGame->getNrOfCols() && row < mGame->getNrOfRows() )
	{
		// Decipher data
		Color::Color color = Controller::modelFromView( inData.getColor() );
		Piece::PieceType type = Controller::modelFromView( inData.getPiece() );
		Area::Area area = Controller::areaFromPos( Controller::posXFromX( inX ), Controller::posYFromY( inY ) );
		// Send to mGame
		mGame->tryToPlacePiece( color, type, col, row, area );
	}
}

void
GameController::addPlayers()
{
	for ( std::vector< Player >::const_iterator it = mGame->getPlayers().begin();
		it != mGame->getPlayers().end();
		++it )
	{
		mWindow->addPlayer( it->getName(), Controller::viewFromModel( it->getColor() ), it->getNumberOfFreePieces() );
	}
}

void
GameController::makeConnections()
{
	connect( mGame, SIGNAL( tilePlaced(unsigned int, unsigned int, std::string, Model::Rotation) ),
			this, SLOT( onTilePlaced(unsigned int, unsigned int, std::string, Model::Rotation) ) );
	connect( mGame, SIGNAL( tileUnplaced(unsigned int, unsigned int) ),
			this, SLOT( onTileUnplaced(unsigned int, unsigned int) ) );
	connect( mGame, SIGNAL( tileRotated(uint,uint,std::string,Model::Rotation) ),
			this, SLOT( onTileRotated(uint,uint,std::string,Model::Rotation) ) );
	connect( mGame, SIGNAL( nextTile(std::string) ), this, SLOT( onNextTile(std::string) ) );
	connect( mGame, SIGNAL( tilesLeft(uint) ), this, SLOT( onTilesLeft(uint) ) );

	connect( mGame, SIGNAL( piecePlaced(uint, uint, Area::Area, Player) ),
		this, SLOT( onPiecePlaced(uint, uint, Area::Area, Player ) ) );
	connect( mGame, SIGNAL( pieceReturned(uint, uint, Area::Area, Player) ),
		this, SLOT( onPieceReturned(uint, uint, Area::Area, Player ) ) );
	connect( mGame, SIGNAL( playerInfoChanged(Player) ),
		this, SLOT( onPlayerInfoChanged(Player) ) );
	connect( mGame, SIGNAL( currentPlayerChanged(Player) ),
		this, SLOT( onCurrentPlayerChanged(Player) ) );

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
GameController::startGame()
{
	onCurrentPlayerChanged( mGame->getCurrentPlayer() );
	mWindow->show();
	if ( mGame->getNextTile() )
	{
		mGame->placeStartTileOnBoard();
	}
}
