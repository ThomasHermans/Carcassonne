#ifndef STARTCONTROLLER_H
#define STARTCONTROLLER_H

#include "Utils/Typedefs.h"

#include <boost/scoped_ptr.hpp>
#include <boost/signals2/signal.hpp>

#include <set>
#include <string>
#include <vector>

namespace Model
{
	class Player;
}

namespace View
{
	struct PlayerInfo;
	class StartScreen;
}

namespace Controller
{
	class StartController
	{
	public:
		StartController();
		~StartController();

		boost::signals2::signal
		< void ( std::set< Utils::Expansion::Type > const & inExpansions,
			std::vector< View::PlayerInfo > const & inPlayers ) > startGame;

	private:
		void
		onTryToStartGame
		(
			std::set< Utils::Expansion::Type > const & inExpansions,
			std::vector< View::PlayerInfo > const & inPlayers
		);

	private:
		boost::scoped_ptr< View::StartScreen > mStartScreen;
	};
}

#endif // STARTCONTROLLER_H