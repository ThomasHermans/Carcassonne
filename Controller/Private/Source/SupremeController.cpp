#include "Controller/SupremeController.h"

#include "Controller/GameController.h"

#include "StartController.h"

#include <iostream>

Controller::SupremeController::SupremeController()
:
	mStartController( new StartController() ),
	mGameController()
{
	mStartController->startGame.connect
	(
		boost::bind( &Controller::SupremeController::startGame, this, _1, _2 )
	);
}

Controller::SupremeController::~SupremeController()
{
}

void
Controller::SupremeController::startGame
(
	std::set< View::Expansion::Type > const & inExpansions,
	std::vector< View::PlayerInfo > const & inPlayers
)
{
	mGameController.reset( new GameController( inExpansions, inPlayers ) );
}
