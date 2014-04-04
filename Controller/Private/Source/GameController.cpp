#include "Controller/GameController.h"

#include "ModelViewGlue.h"

#include "Model/Color.h"
#include "Model/Private/Include/PlacedProject.h"
#include "View/Typedefs.h"
#include "View/DragData.h"

#include <boost/foreach.hpp>

#include <iostream>

namespace
{
	std::vector< Model::NewPlayer >
	createTestPlayers()
	{
		std::vector< Model::NewPlayer > players;
		players.push_back( Model::NewPlayer( "Yumi", Model::Color::kYellow ) );
		players.push_back( Model::NewPlayer( "Thomas", Model::Color::kGreen ) );
		return players;
	}
}

Controller::GameController::GameController( std::string const & inTiles, QObject * inParent )
:
	QObject( inParent ),
	mPlayers( createTestPlayers() ),
	mGame( mPlayers, inTiles ),
	mWindow( new View::GameWindow() )
{
	addPlayersToWindow();
	makeConnections();
	startGame();
}

Controller::GameController::GameController( std::vector< Model::NewPlayer > const & inPlayers, QObject * inParent )
:
	QObject( inParent ),
	mPlayers( inPlayers ),
	mGame( mPlayers ),
	mWindow( new View::GameWindow() )
{
	addPlayersToWindow();
	makeConnections();
	startGame();
}

void
Controller::GameController::onTilePlaced
(
	int inRow,
	int inCol,
	std::string const & inId,
	Model::Rotation inRotation
)
{
	int const x = xFromCol( inCol );
	int const y = yFromRow( inRow );
	mWindow->setTile( x, y, inId, viewFromModel( inRotation ) );
	mWindow->fadeNextTile();
}

void
Controller::GameController::onTileRemoved
(
	int inRow,
	int inCol
)
{
	int const x = xFromCol( inCol );
	int const y = yFromRow( inRow );
	mWindow->clearTile( x, y );
}

void
Controller::GameController::onNextTile( std::string const & inNextId )
{
	mWindow->setNextTile( inNextId );
}

void
Controller::GameController::onTilesLeft( std::size_t inNr )
{
	mWindow->displayTilesLeft( inNr );
}

void
Controller::GameController::onPiecePlaced( int inRow, int inCol, Model::PlacedPiece const & inPiece, Model::NewPlayer const & inCurrentPlayer )
{
	int x = xFromCol( inCol );
	int y = yFromRow( inRow );
	x += xFromArea( inPiece.getArea() ) - .5 * Gui::kTileWidth;
	y += yFromArea( inPiece.getArea() ) - .5 * Gui::kTileHeight;
	mWindow->placePiece( x, y, viewFromModel( inCurrentPlayer.getColor() ) );
}

void
Controller::GameController::onPieceRemoved( int inRow, int inCol, Model::PlacedPiece const & inPiece, Model::NewPlayer const & inPlayer )
{
	int x = xFromCol( inCol );
	int y = yFromRow( inRow );
	x += xFromArea( inPiece.getArea() ) - .5 * Gui::kTileWidth;
	y += yFromArea( inPiece.getArea() ) - .5 * Gui::kTileHeight;
	mWindow->returnPiece( x, y, viewFromModel( inPlayer.getColor() ) );
}

void
Controller::GameController::onPlayerInfoChanged( Model::NewPlayer const & inNewInfo )
{
	mWindow->setFollowersLeft( inNewInfo.getName(), inNewInfo.getNumberOfFreePieces() );
	mWindow->setScore( inNewInfo.getName(), inNewInfo.getScore() );
}

void
Controller::GameController::onCurrentPlayerChanged( Model::NewPlayer const & inCurrentPlayer )
{
	mWindow->setActivePlayer( inCurrentPlayer.getName() );
}

void
Controller::GameController::onEndOfGame( std::size_t inTilesLeft )
{
	mGame.awardEndPoints();
	std::cout << "Game has ended." << std::endl;
	if ( inTilesLeft >= 1 )
	{
		std::cout << "Number of unplayed tiles: " << inTilesLeft << std::endl;
	}
}

void
Controller::GameController::onClicked( int inX, int inY )
{
	mGame.rotateTile( rowFromY( inY ), colFromX( inX ) );
}

void
Controller::GameController::onTileDropped( int inX, int inY, std::string const & inTileId, View::Rotation inRotation )
{
	mGame.tryToPlaceTile( rowFromY( inY ), colFromX( inX ), inTileId, modelFromView( inRotation ) );
}

void
Controller::GameController::onTryToPlacePiece( Dragging::PieceData const & inData, int inX, int inY )
{
	int const col = colFromX( inX );
	int const row = rowFromY( inY );
	std::cout << "GameController sees drop at x, y: " << inX << ", " << inY << ", which is col, row: " << col << ", " << row << std::endl;
	std::cout << "Which is at " << posXFromX( inX ) << ", " << posYFromY( inY ) << " at that tile." << std::endl;
	std::cout << "Which is at Area::" << areaFromPos( posXFromX( inX ), posYFromY( inY ) ) << std::endl;
	std::cout << "Drop contains " << inData.getColor() << " " << inData.getPiece() << std::endl;
	// Decipher data
	Model::Color::Color const color = modelFromView( inData.getColor() );
	Model::Piece::PieceType const type = modelFromView( inData.getPiece() );
	Model::Area::Area const area = areaFromPos( posXFromX( inX ), posYFromY( inY ) );
	// Send to mGame
	mGame.tryToPlacePiece( Model::NewPlacedProject( row, col, area ), type, color );
}

void
Controller::GameController::onEndCurrentTurn()
{
	mGame.endTurn();
}

void
Controller::GameController::addPlayersToWindow()
{
	BOOST_FOREACH( Model::NewPlayer const & player, mPlayers )
	{
		mWindow->addPlayer( player.getName(), viewFromModel( player.getColor() ), player.getNumberOfFreePieces() );
	}
}

void
Controller::GameController::makeConnections()
{
	mGame.tilePlaced.connect( boost::bind( &Controller::GameController::onTilePlaced, this, _1, _2, _3, _4 ) );
	mGame.tileRemoved.connect( boost::bind( &Controller::GameController::onTileRemoved, this, _1, _2 ) );
	mGame.nextTile.connect( boost::bind( &Controller::GameController::onNextTile, this, _1 ) );
	mGame.tilesLeft.connect( boost::bind( &Controller::GameController::onTilesLeft, this, _1 ) );
	mGame.piecePlaced.connect( boost::bind( &Controller::GameController::onPiecePlaced, this, _1, _2, _3, _4 ) );
	mGame.pieceRemoved.connect( boost::bind( &Controller::GameController::onPieceRemoved, this, _1, _2, _3, _4 ) );
	mGame.playerInfoChanged.connect( boost::bind( &Controller::GameController::onPlayerInfoChanged, this, _1 ) );
	mGame.currentPlayerChanged.connect( boost::bind( &Controller::GameController::onCurrentPlayerChanged, this, _1 ) );
	mGame.endOfGame.connect( boost::bind( &Controller::GameController::onEndOfGame, this, _1 ) );

	connect( mWindow.get(), SIGNAL( clicked( int, int ) ), this, SLOT( onClicked( int, int ) ) );
	connect( mWindow.get(), SIGNAL( tileDropped( int, int, std::string const &, View::Rotation ) ),
		this, SLOT( onTileDropped( int, int, std::string const &, View::Rotation ) ) );
	connect( mWindow.get(), SIGNAL( tryToPlacePiece( Dragging::PieceData, int, int ) ),
		this, SLOT( onTryToPlacePiece( Dragging::PieceData, int, int ) ) );
	connect( mWindow.get(), SIGNAL( endCurrentTurn() ), this, SLOT( onEndCurrentTurn() ) );
}

void
Controller::GameController::startGame()
{
	onCurrentPlayerChanged( mPlayers.front() );
	mWindow->show();
	mGame.placeStartTileOnBoard();
}
