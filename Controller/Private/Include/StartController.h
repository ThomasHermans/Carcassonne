#ifndef STARTCONTROLLER_H
#define STARTCONTROLLER_H

#include "View/Typedefs.h"

#include <QObject>

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
	class StartController : public QObject
	{
		Q_OBJECT
	public:
		StartController( QObject * inParent = 0 );
		~StartController();

	boost::signals2::signal
	< void ( std::set< View::Expansion::Type > const & inExpansions,
		std::vector< View::PlayerInfo > const & inPlayers ) > startGame;

	private slots:
		void onTryToStartGame( std::vector< View::PlayerInfo > const & inPlayers );

	private:
		boost::scoped_ptr< View::StartScreen > mStartScreen;
	};
}

#endif // STARTCONTROLLER_H