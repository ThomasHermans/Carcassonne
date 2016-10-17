#include "Controller/ModeratorController.h"

#include "Controller/Moderator.h"
#include "Controller/Player.h"

#include "ModelViewGlue.h"

#include "View/GameWindow.h"
#include "View/StartScreen.h"

Controller::ModeratorController::ModeratorController
(
	std::vector< View::PlayerInfo > const & inPlayers,
	std::set< Utils::Expansion::Type > const & inExpansions
)
:
	QObject(),
	mGameWindow( new View::GameWindow() ),
	mModerator( new Moderator( inPlayers, inExpansions, mGameWindow, this ) )
{
	connect( this, SIGNAL(gameStarted()), mModerator, SLOT(playGame()) );
	connect
	(
		mModerator,
		SIGNAL(tilePlaced(Utils::Location const &, Model::TileOnBoard const &, boost::optional< Utils::PlayerID > const &)),
		this,
		SLOT(onTilePlaced(Utils::Location const &, Model::TileOnBoard const &, boost::optional< Utils::PlayerID > const &))
	);
	connect
	(
		mModerator,
		SIGNAL(piecePlaced(Utils::Location const &, Model::PlacedPiece const &)),
		this,
		SLOT(onPiecePlaced(Utils::Location const &, Model::PlacedPiece const &))
	);
	connect
	(
		mModerator,
		SIGNAL(pieceRemoved(Utils::Location const &, Model::PlacedPiece const &)),
		this,
		SLOT(onPieceRemoved(Utils::Location const &, Model::PlacedPiece const &))
	);
	connect( mModerator, SIGNAL(playerScoreChanged(Utils::PlayerID,std::size_t)), this, SLOT(onPlayerScoreChanged(Utils::PlayerID,std::size_t)));
	connect( mModerator, SIGNAL(playerSupplyChanged(Utils::PlayerID,Model::Piece::PieceType,std::size_t)), this, SLOT(onPlayerSupplyChanged(Utils::PlayerID,Model::Piece::PieceType,std::size_t)));
	connect( mModerator, SIGNAL(tilesLeftChanged(unsigned)), this, SLOT(onTilesLeftChanged(unsigned)) );
	connect( mModerator, SIGNAL(nextTileChanged(std::string const &)), this, SLOT(onNextTileChanged(std::string const &)) );
	connect( mModerator, SIGNAL(currentPlayerChanged(std::string const &)), this, SLOT(onCurrentPlayerChanged(std::string const &)) );
}

Controller::ModeratorController::~ModeratorController()
{
}

void
Controller::ModeratorController::startGame()
{
	mGameWindow->show();
	emit gameStarted();
}

void
Controller::ModeratorController::onTilePlaced
(
	Utils::Location const & inLocation,
	Model::TileOnBoard const & inTile,
	boost::optional< Utils::PlayerID > const & inPlayer
)
{
	mGameWindow->fadeNextTile();
	boost::optional< View::Color > color;
	if ( inPlayer )
	{
		color = viewFromModel( mModerator->getPlayerColor( *inPlayer ) );
	}
	mGameWindow->setTile( inLocation, inTile.getID(), viewFromModel( inTile.getRotation() ), color );
}

void
Controller::ModeratorController::onTilesLeftChanged( unsigned inTilesLeft )
{
	mGameWindow->displayTilesLeft( inTilesLeft );
}

void
Controller::ModeratorController::onNextTileChanged( std::string const & inNextTile )
{
	mGameWindow->setNextTile( inNextTile );
}

void
Controller::ModeratorController::onPiecePlaced
(
	Utils::Location const & inLocation,
	Model::PlacedPiece const & inPiece
)
{
	mGameWindow->placePiece
	(
		xFromLocation( inLocation, inPiece.getArea() ),
		yFromLocation( inLocation, inPiece.getArea() ),
		viewFromModel( inPiece.getPiece() )
	);
}

void
Controller::ModeratorController::onPieceRemoved
(
	Utils::Location const & inLocation,
	Model::PlacedPiece const & inPiece
)
{
	mGameWindow->removePiece
	(
		xFromLocation( inLocation, inPiece.getArea() ),
		yFromLocation( inLocation, inPiece.getArea() ),
		viewFromModel( inPiece.getPiece() )
	);
}

void
Controller::ModeratorController::onCurrentPlayerChanged( std::string const & inName )
{
	mGameWindow->setActivePlayer( inName );
}

void
Controller::ModeratorController::onPlayerScoreChanged( Utils::PlayerID inPlayerID, std::size_t inScore )
{
	mGameWindow->setScore( mModerator->getPlayerName( inPlayerID ), inScore );
}

void
Controller::ModeratorController::onPlayerSupplyChanged( Utils::PlayerID inPlayerID, Model::Piece::PieceType inPieceType, std::size_t inAmount )
{
	mGameWindow->setPlayerSupply( mModerator->getPlayerName( inPlayerID ), viewFromModel( inPieceType ), inAmount );
}