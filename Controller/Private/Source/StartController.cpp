#include "StartController.h"

#include "ModelViewGlue.h"
#include "Settings.h"

#include "Model/ModelPlayer.h"

#include "View/StartScreen.h"

#include <boost/foreach.hpp>

#include <cassert>
#include <sstream>

namespace
{
	std::string const kNrOfPlayers( "NrOfPlayers" );
	std::string const kPlayerName( "PlayerName" );
	std::string const kPlayerColor( "PlayerColor" );
	std::string const kPlayerAI( "PlayerAI" );
	std::string const kBaseGame( "Settings::BaseGame" );
	std::string const kTheExpansion( "Settings::TheExpansion" );

	std::string
	getPlayerNameKey( int inIndex )
	{
		std::stringstream keyStream;
		keyStream << kPlayerName << inIndex;
		return keyStream.str();
	}

	std::string
	getPlayerColorKey( int inIndex )
	{
		std::stringstream keyStream;
		keyStream << kPlayerColor << inIndex;
		return keyStream.str();
	}

	std::string
	getPlayerAIKey( int inIndex )
	{
		std::stringstream keyStream;
		keyStream << kPlayerAI << inIndex;
		return keyStream.str();
	}

	View::Color
	toColor( std::size_t inNumber )
	{
		switch ( inNumber )
		{
			case 0: return View::kRed;
			case 1: return View::kGreen;
			case 2: return View::kBlue;
			case 3: return View::kYellow;
			case 4: return View::kBlack;
			case 5: return View::kGray;
		}
		assert( !"No color matches this number" );
		return View::kRed;
	}

	std::size_t
	fromColor( View::Color inColor )
	{
		switch ( inColor )
		{
			case View::kRed: return 0;
			case View::kGreen: return 1;
			case View::kBlue: return 2;
			case View::kYellow: return 3;
			case View::kBlack: return 4;
			case View::kGray: return 5;
		}
		assert( !"Invalid color" );
		return 0;
	}
}

Controller::StartController::StartController()
:
	mStartScreen( new View::StartScreen() )
{
	// Get the players that were saved from last time.
	std::size_t const nrOfPlayers = Settings::getValue< std::size_t >( kNrOfPlayers, 0 );
	for ( std::size_t i = 0; i < nrOfPlayers; ++i )
	{
		std::string const playerName = Settings::getValue< std::string >( getPlayerNameKey( i ), "" );
		View::Color const playerColor = toColor( Settings::getValue< std::size_t >( getPlayerColorKey( i ), 0 ) );
		bool const playerAI = Settings::getValue< bool >( getPlayerAIKey( i ), true );
		mStartScreen->addPlayer( playerName, playerColor, playerAI );
	}
	// Get the expansions that were played with last time.
	std::set< Utils::Expansion::Type > expansions;
	if ( Settings::getValue< bool >( kBaseGame, true ) )
	{
		expansions.insert( Utils::Expansion::kBaseGame );
	}
	if ( Settings::getValue< bool >( kTheExpansion, false ) )
	{
		expansions.insert( Utils::Expansion::kTheExpansion );
	}
	mStartScreen->selectExpansions( expansions );
	// Connect to the view's signal.
	mStartScreen->startGame.connect
	(
		boost::bind( &Controller::StartController::onTryToStartGame, this, _1, _2 )
	);
	mStartScreen->show();
}

Controller::StartController::~StartController()
{
}

void
Controller::StartController::onTryToStartGame
(
	std::set< Utils::Expansion::Type > const & inExpansions,
	std::vector< View::PlayerInfo > const & inPlayers
)
{
	// Store the players' information.
	Settings::storeValue< std::size_t >( kNrOfPlayers, inPlayers.size() );
	int i = 0;
	for ( View::PlayerInfo const & player : inPlayers )
	{
		Settings::storeValue< std::string >( getPlayerNameKey( i ), player.name );
		Settings::storeValue< std::size_t >( getPlayerColorKey( i ), fromColor( player.color ) );
		Settings::storeValue< bool >( getPlayerAIKey( i ), player.isAI );
		++i;
	}
	// Store the chosen expansions.
	Settings::storeValue< bool >( kBaseGame, inExpansions.count( Utils::Expansion::kBaseGame ) > 0 );
	Settings::storeValue< bool >( kTheExpansion, inExpansions.count( Utils::Expansion::kTheExpansion ) > 0 );
	// Hide the start screen and start the game.
	mStartScreen->hide();
	startGame( inExpansions, inPlayers );
}