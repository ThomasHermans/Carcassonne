#include "Controller/GameController.h"

#include "ModelViewGlue.h"

#include "Model/Color.h"
#include "Model/Private/Include/PlacedProject.h"

#include "View/DragData.h"
#include "View/StartScreen.h"
#include "View/Typedefs.h"

#include <boost/foreach.hpp>

#include <cassert>
#include <iostream>
#include <set>

namespace
{
	std::vector< Model::Player >
	createTestPlayers()
	{
		std::set< Utils::Expansion::Type > testExpansions;
		testExpansions.insert( Utils::Expansion::kBaseGame );
		std::vector< Model::Player > players;
		players.push_back( Model::Player( "Yumi", Model::Color::kYellow, testExpansions ) );
		players.push_back( Model::Player( "Thomas", Model::Color::kGreen, testExpansions ) );
		return players;
	}

	std::vector< Model::Player >
	modelFromView
	(
		std::set< Utils::Expansion::Type > const & inExpansions,
		std::vector< View::PlayerInfo > const & inPlayers
	)
	{
		std::vector< Model::Player > players;
		BOOST_FOREACH( View::PlayerInfo const & playerInfo, inPlayers )
		{
			players.push_back
			(
				Model::Player
				(
					playerInfo.name,
					Controller::modelFromView( playerInfo.color ),
					inExpansions
				)
			);
		}
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

Controller::GameController::GameController
(
	std::set< Utils::Expansion::Type > const & inExpansions,
	std::vector< View::PlayerInfo > const & inPlayers,
	QObject * inParent
)
:
	QObject( inParent ),
	mPlayers( ::modelFromView( inExpansions, inPlayers ) ),
	mGame( mPlayers, inExpansions ),
	mWindow( new View::GameWindow() )
{
	addPlayersToWindow();
	makeConnections();
	startGame();
}

void
Controller::GameController::onTilePlaced
(
	Utils::Location const & inLocation,
	std::string const & inId,
	Model::Rotation inRotation
)
{
	mWindow->setTile( inLocation, inId, viewFromModel( inRotation ) );
	mWindow->fadeNextTile();
}

void
Controller::GameController::onTileRemoved( Utils::Location const & inLocation )
{
	mWindow->clearTile( inLocation );
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
Controller::GameController::onPiecePlaced( Utils::Location const & inLocation, Model::PlacedPiece const & inPiece )
{
	int x = xFromCol( inLocation.second );
	int y = yFromRow( inLocation.first );
	x += xFromArea( inPiece.getArea() ) - .5 * Gui::kTileWidth;
	y += yFromArea( inPiece.getArea() ) - .5 * Gui::kTileHeight;
	mWindow->placePiece( x, y, viewFromModel( inPiece.getPiece() ) );
}

void
Controller::GameController::onPieceRemoved( Utils::Location const & inLocation, Model::PlacedPiece const & inPiece )
{
	int x = xFromCol( inLocation.second );
	int y = yFromRow( inLocation.first );
	x += xFromArea( inPiece.getArea() ) - .5 * Gui::kTileWidth;
	y += yFromArea( inPiece.getArea() ) - .5 * Gui::kTileHeight;
	mWindow->returnPiece( x, y, viewFromModel( inPiece.getPiece() ) );
}

void
Controller::GameController::onPlayerInfoChanged( Model::Player const & inNewInfo )
{
	std::string const name = inNewInfo.getName();
	mWindow->setFollowersLeft( name, inNewInfo.getNumberOfFreePieces( Model::Piece::kFollower ) );
	mWindow->setLargeFollowersLeft( name, inNewInfo.getNumberOfFreePieces( Model::Piece::kLargeFollower ) );
	mWindow->setScore( name, inNewInfo.getScore() );
}

void
Controller::GameController::onCurrentPlayerChanged( Model::Player const & inCurrentPlayer )
{
	mWindow->setActivePlayer( inCurrentPlayer.getName() );
}

void
Controller::GameController::onPossibleLocationsChanged( Utils::Locations const & inLocations )
{
	mWindow->setPossibleLocations( inLocations );
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
	std::cout << "Drop contains " << inData.getMeeple().getColor() << " " << inData.getMeeple().getType() << std::endl;
	// Decipher data
	Model::Piece const piece = modelFromView( inData.getMeeple() );
	Model::Area::Area const area = areaFromPos( posXFromX( inX ), posYFromY( inY ) );
	// Send to mGame
	mGame.tryToPlacePiece( Model::PlacedProject( row, col, area ), piece.getType(), piece.getColor() );
}

void
Controller::GameController::onEndCurrentTurn()
{
	mGame.endTurn();
}

void
Controller::GameController::addPlayersToWindow()
{
	BOOST_FOREACH( Model::Player const & player, mPlayers )
	{
		std::string const name = player.getName();
		mWindow->addPlayer( name, viewFromModel( player.getColor() ) );
		mWindow->setFollowersLeft( name, player.getNumberOfFreePieces( Model::Piece::kFollower ) );
		std::size_t const nrOfLargeFollowers = player.getNumberOfFreePieces( Model::Piece::kLargeFollower );
		if ( nrOfLargeFollowers > 0 )
		{
			mWindow->enableLargeFollowers( name );
			mWindow->setLargeFollowersLeft( name, nrOfLargeFollowers );
		}
	}
}

void
Controller::GameController::makeConnections()
{
	mGame.tilePlaced.connect( boost::bind( &Controller::GameController::onTilePlaced, this, _1, _2, _3 ) );
	mGame.tileRemoved.connect( boost::bind( &Controller::GameController::onTileRemoved, this, _1 ) );
	mGame.nextTile.connect( boost::bind( &Controller::GameController::onNextTile, this, _1 ) );
	mGame.tilesLeft.connect( boost::bind( &Controller::GameController::onTilesLeft, this, _1 ) );
	mGame.piecePlaced.connect( boost::bind( &Controller::GameController::onPiecePlaced, this, _1, _2 ) );
	mGame.pieceRemoved.connect( boost::bind( &Controller::GameController::onPieceRemoved, this, _1, _2 ) );
	mGame.playerInfoChanged.connect( boost::bind( &Controller::GameController::onPlayerInfoChanged, this, _1 ) );
	mGame.currentPlayerChanged.connect( boost::bind( &Controller::GameController::onCurrentPlayerChanged, this, _1 ) );
	mGame.possibleLocationsChanged.connect( boost::bind( &Controller::GameController::onPossibleLocationsChanged, this, _1 ) );
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
