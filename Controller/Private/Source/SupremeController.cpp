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
				return Color::Red;
			case Gui::kGreen:
				return Color::Green;
			case Gui::kBlue:
				return Color::Blue;
			case Gui::kYellow:
				return Color::Yellow;
			case Gui::kBlack:
				return Color::Black;
			case Gui::kGray:
				return Color::Gray;
		}
		assert( !"Invalid Gui::Color" );
		return Color::Red;
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
