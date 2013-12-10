#ifndef SUPREMECONTROLLER_H
#define SUPREMECONTROLLER_H

#include "View/Typedefs.h"

#include <QObject>

#include <boost/scoped_ptr.hpp>

#include <map>
#include <string>

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
		void startGame( std::map< View::Color, std::string > const & inPlayers );

	private:
		boost::scoped_ptr< StartController > mStartController;
		boost::scoped_ptr< GameController > mGameController;
	};
}

#endif // SUPREMECONTROLLER_H