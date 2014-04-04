#include "Controller/SupremeController.h"

#include "Controller/GameController.h"

#include "StartController.h"

#include <iostream>

Controller::SupremeController::SupremeController( QObject * inParent )
:
	QObject( inParent ),
	mStartController( new StartController() ),
	mGameController()
{
	connect
	(
		mStartController.get(), SIGNAL( startGame( std::vector< Model::NewPlayer > ) ),
		this, SLOT( startGame( std::vector< Model::NewPlayer > ) )
	);
}

Controller::SupremeController::~SupremeController()
{
}

void
Controller::SupremeController::startGame( std::vector< Model::NewPlayer > const & inPlayers )
{
	std::cout << "Starting game by resetting mGameController..." << std::endl;
	mGameController.reset( new GameController( inPlayers ) );
}
