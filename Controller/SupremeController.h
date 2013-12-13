#ifndef SUPREMECONTROLLER_H
#define SUPREMECONTROLLER_H

#include <QObject>

#include <boost/scoped_ptr.hpp>

#include <string>
#include <vector>

class Player;

namespace  Controller
{
	class GameController;
	class StartController;

	class SupremeController : QObject
	{
		Q_OBJECT
	public:
		SupremeController( QObject * inParent = 0 );
		~SupremeController();

	private slots:
		void startGame( std::vector< Player > const & inPlayers );

	private:
		boost::scoped_ptr< StartController > mStartController;
		boost::scoped_ptr< GameController > mGameController;
	};
}

#endif // SUPREMECONTROLLER_H