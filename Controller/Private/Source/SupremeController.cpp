#include "Controller/SupremeController.h"

#include "Controller/GameController.h"

#include "StartController.h"

#include <cassert>

Controller::SupremeController::SupremeController( QObject * inParent )
:
	QObject( inParent ),
	mStartController( new StartController() ),
	mGameController()
{
	connect
	(
		mStartController.get(), SIGNAL( startGame( std::vector< Model::Player > ) ),
		this, SLOT( startGame( std::vector< Model::Player > ) )
	);
}

Controller::SupremeController::~SupremeController()
{
}

void
Controller::SupremeController::startGame( std::vector< Model::Player > const & inPlayers )
{
	mGameController.reset( new GameController( inPlayers ) );
}
