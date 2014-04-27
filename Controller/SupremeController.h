#ifndef SUPREMECONTROLLER_H
#define SUPREMECONTROLLER_H

#include "View/Typedefs.h"

#include <boost/scoped_ptr.hpp>

#include <set>
#include <string>
#include <vector>

namespace View
{
	class PlayerInfo;
}

namespace  Controller
{
	class GameController;
	class StartController;

	class SupremeController
	{
	public:
		SupremeController();
		~SupremeController();

		/**
		 *	Start a game with the specified expansions
		 *	and players.
		 */
		void
		startGame
		(
			std::set< View::Expansion::Type > const & inExpansions,
			std::vector< View::PlayerInfo > const & inPlayers
		);

	private:
		boost::scoped_ptr< StartController > mStartController;
		boost::scoped_ptr< GameController > mGameController;
	};
}

#endif // SUPREMECONTROLLER_H