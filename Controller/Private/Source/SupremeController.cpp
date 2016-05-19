#include "Controller/SupremeController.h"

#include "Controller/GUIPlayer.h"
#include "Controller/ModeratorController.h"
#include "Controller/Player.h"
#include "Controller/RobotPlayer.h"

#include "ModelViewGlue.h"
#include "StartController.h"

#include "View/GameWindow.h"
#include "View/StartScreen.h"

#include <iostream>

Controller::SupremeController::SupremeController()
:
	mStartController( new StartController() ),
	mModeratorController()
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
	std::set< Utils::Expansion::Type > const & inExpansions,
	std::vector< View::PlayerInfo > const & inPlayers
)
{
	assert( !inPlayers.empty() );
	
	mModeratorController.reset( new ModeratorController( inPlayers, inExpansions ) );
	mModeratorController->startGame();
}
