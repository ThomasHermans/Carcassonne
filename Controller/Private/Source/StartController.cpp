#include "StartController.h"

#include "ModelViewGlue.h"
#include "Settings.h"

#include "Model/Player.h"

#include "View/StartScreen.h"
#include "View/Typedefs.h"

#include <boost/foreach.hpp>

#include <cassert>
#include <sstream>

namespace
{
	std::string const kNrOfPlayers = "NrOfPlayers";
	std::string const kPlayerName = "PlayerName";
	std::string const kPlayerColor = "PlayerColor";

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
	std::size_t const nrOfPlayers = Settings::getNumber( kNrOfPlayers, 0 );
	for ( std::size_t i = 0; i < nrOfPlayers; ++i )
	{
		std::string const playerName = Settings::getString( getPlayerNameKey( i ), "" );
		View::Color const playerColor = toColor( Settings::getNumber( getPlayerColorKey( i ), 0 ) );
		mStartScreen->addPlayer( playerName, playerColor );
	}
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
	std::set< View::Expansion::Type > const & inExpansions,
	std::vector< View::PlayerInfo > const & inPlayers
)
{
	Settings::storeNumber( kNrOfPlayers, inPlayers.size() );
	int i = 0;
	BOOST_FOREACH( View::PlayerInfo const & player, inPlayers )
	{
		Settings::storeString( getPlayerNameKey( i ), player.name );
		Settings::storeNumber( getPlayerColorKey( i ), fromColor( player.color ) );
		++i;
	}
	mStartScreen->hide();
	startGame( inExpansions, inPlayers );
}