#include "Controller/SupremeController.h"

#include "Controller/GameController.h"

#include "StartController.h"

#include <cassert>

namespace
{
	Color::Color
	convertToColor( Gui::Color inColor )
	{
		switch ( inColor )
		{
			case Gui::kRed:
				return Color::kRed;
			case Gui::kGreen:
				return Color::kGreen;
			case Gui::kBlue:
				return Color::kBlue;
			case Gui::kYellow:
				return Color::kYellow;
			case Gui::kBlack:
				return Color::kBlack;
			case Gui::kGray:
				return Color::kGray;
		}
		assert( !"Invalid Gui::Color" );
		return Color::kRed;
	}

	std::vector< Player >
	convertToPlayers( std::map< Gui::Color, std::string > const & inPlayers )
	{
		std::vector< Player > players;
		for ( std::map< Gui::Color, std::string >::const_iterator it = inPlayers.begin();
			it != inPlayers.end();
			++it )
		{
			players.push_back( Player( it->second, convertToColor( it->first ) ) );
		}
		return players;
	}
}

SupremeController::SupremeController( QObject * inParent )
:
	QObject( inParent ),
	mStartController( new StartController() ),
	mGameController()
{
	connect
	(
		mStartController.get(), SIGNAL( startGame( std::map< Gui::Color, std::string > ) ),
		this, SLOT( startGame( std::map< Gui::Color, std::string > ) )
	);
}

SupremeController::~SupremeController()
{
}

void
SupremeController::startGame( std::map< Gui::Color, std::string > const & inPlayers )
{
	std::vector< Player > players = convertToPlayers( inPlayers );
	mGameController.reset( new GameController( players ) );
}
