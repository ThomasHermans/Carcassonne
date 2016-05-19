#ifndef SUPREMECONTROLLER_H
#define SUPREMECONTROLLER_H

#include "Utils/Typedefs.h"

#ifndef Q_MOC_RUN
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#endif

#include <set>
#include <vector>

namespace View
{
	struct PlayerInfo;
}

namespace  Controller
{
	class ModeratorController;
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
			std::set< Utils::Expansion::Type > const & inExpansions,
			std::vector< View::PlayerInfo > const & inPlayers
		);

	private:
		boost::scoped_ptr< StartController > mStartController;
		boost::scoped_ptr< ModeratorController > mModeratorController;
	};
}

#endif // SUPREMECONTROLLER_H