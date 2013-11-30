#include "StartController.h"

#include "View/StartScreen.h"

StartController::StartController( QObject * inParent )
:
	QObject( inParent ),
	mStartScreen( new View::StartScreen() )
{
	connect
	(
		mStartScreen.get(), SIGNAL( startGame( std::map< Gui::Color, std::string > ) ),
		this, SLOT( onTryToStartGame( std::map< Gui::Color, std::string > ) )
	);
	mStartScreen->show();
}

StartController::~StartController()
{
}

void
StartController::onTryToStartGame( std::map< Gui::Color, std::string > const & inPlayers )
{
	mStartScreen->hide();
	emit startGame( inPlayers );
}