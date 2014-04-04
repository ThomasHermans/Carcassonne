#ifndef SUPREMECONTROLLER_H
#define SUPREMECONTROLLER_H

#include <QObject>

#include <boost/scoped_ptr.hpp>

#include <string>
#include <vector>

namespace Model
{
	class NewPlayer;
}

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
		void startGame( std::vector< Model::NewPlayer > const & inPlayers );

	private:
		boost::scoped_ptr< StartController > mStartController;
		boost::scoped_ptr< GameController > mGameController;
	};
}

#endif // SUPREMECONTROLLER_H