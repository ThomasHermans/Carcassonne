#include "Controller/SupremeController.h"

#include "Controller/GameController.h"

#include "StartController.h"

#include <cassert>

namespace
{
	Color::Color
	convertToColor( View::Color inColor )
	{
		switch ( inColor )
		{
			case View::kRed:
				return Color::kRed;
			case View::kGreen:
				return Color::kGreen;
			case View::kBlue:
				return Color::kBlue;
			case View::kYellow:
				return Color::kYellow;
			case View::kBlack:
				return Color::kBlack;
			case View::kGray:
				return Color::kGray;
		}
		assert( !"Invalid View::Color" );
		return Color::kRed;
	}

	std::vector< Player >
	convertToPlayers( std::map< View::Color, std::string > const & inPlayers )
	{
		std::vector< Player > players;
		for ( std::map< View::Color, std::string >::const_iterator it = inPlayers.begin();
			it != inPlayers.end();
			++it )
		{
			players.push_back( Player( it->second, convertToColor( it->first ) ) );
		}
		return players;
	}
}

Controller::SupremeController::SupremeController( QObject * inParent )
:
	QObject( inParent ),
	mStartController( new StartController() ),
	mGameController()
{
	connect
	(
		mStartController.get(), SIGNAL( startGame( std::map< View::Color, std::string > ) ),
		this, SLOT( startGame( std::map< View::Color, std::string > ) )
	);
}

Controller::SupremeController::~SupremeController()
{
}

void
Controller::SupremeController::startGame( std::map< View::Color, std::string > const & inPlayers )
{
	std::vector< Player > players = convertToPlayers( inPlayers );
	mGameController.reset( new GameController( players ) );
}
