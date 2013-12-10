#include "StartController.h"

#include "Settings.h"

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
		View::Color const playerColor = settings.value( getPlayerColorKey( i ), View::kRed ).value< View::Color >();
		mStartScreen->addPlayer( playerName, playerColor );
	}
	connect
	(
		mStartScreen.get(), SIGNAL( startGame( std::map< View::Color, std::string > ) ),
		this, SLOT( onTryToStartGame( std::map< View::Color, std::string > ) )
	);
	mStartScreen->show();
}

Controller::StartController::~StartController()
{
}

void
Controller::StartController::onTryToStartGame( std::map< View::Color, std::string > const & inPlayers )
{
	QSettings & settings = getSettings();
	settings.setValue( kNrOfPlayers, inPlayers.size() );
	int index = 0;
	for ( std::map< View::Color, std::string >::const_iterator it = inPlayers.begin();
		it != inPlayers.end();
		++it, ++index )
	{
		settings.setValue( getPlayerNameKey( index ), QString::fromStdString( it->second ) );
		settings.setValue( getPlayerColorKey( index ), it->first );
	}
	mStartScreen->hide();
	emit startGame( inPlayers );
}