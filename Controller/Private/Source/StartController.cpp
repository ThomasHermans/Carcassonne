#include "StartController.h"

#include "ModelViewGlue.h"
#include "Settings.h"

#include "Model/Player.h"

#include "View/StartScreen.h"
#include "View/Typedefs.h"

#include <QSettings>

namespace
{
	QString const kNrOfPlayers = "NrOfPlayers";
	QString const kPlayerName = "PlayerName";
	QString const kPlayerColor = "PlayerColor";

	QString
	getPlayerNameKey( int inIndex )
	{
		QString playerNameKey = kPlayerName;
		playerNameKey.append( QString::number( inIndex ) );
		return playerNameKey;
	}

	QString
	getPlayerColorKey( int inIndex )
	{
		QString playerColorKey = kPlayerColor;
		playerColorKey.append( QString::number( inIndex ) );
		return playerColorKey;
	}
}

Controller::StartController::StartController( QObject * inParent )
:
	QObject( inParent ),
	mStartScreen( new View::StartScreen() )
{
	QSettings const & settings = getSettings();
	int const nrOfPlayers = settings.value( kNrOfPlayers, 0 ).toInt();
	for ( int i = 0; i < nrOfPlayers; ++i )
	{
		QString const playerName = settings.value( getPlayerNameKey( i ), "" ).toString();
		View::Color const playerColor = View::Color( settings.value( getPlayerColorKey( i ), 0 ).toInt() );
		mStartScreen->addPlayer( playerName, playerColor );
	}
	connect
	(
		mStartScreen.get(), SIGNAL( startGame( std::vector< View::PlayerInfo > ) ),
		this, SLOT( onTryToStartGame( std::vector< View::PlayerInfo > ) )
	);
	mStartScreen->show();
}

Controller::StartController::~StartController()
{
}

void
Controller::StartController::onTryToStartGame( std::vector< View::PlayerInfo > const & inPlayers )
{
	QSettings & settings = getSettings();
	settings.setValue( kNrOfPlayers, inPlayers.size() );
	int index = 0;
	std::vector< Model::Player > players;
	for ( std::vector< View::PlayerInfo >::const_iterator it = inPlayers.begin();
		it != inPlayers.end();
		++it, ++index )
	{
		settings.setValue( getPlayerNameKey( index ), QString::fromStdString( it->name ) );
		settings.setValue( getPlayerColorKey( index ), int( it->color ) );
		players.push_back( Model::Player( it->name, modelFromView( it->color ) ) );
	}
	mStartScreen->hide();
	emit startGame( players );
}