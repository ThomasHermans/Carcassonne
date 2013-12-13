#ifndef STARTCONTROLLER_H
#define STARTCONTROLLER_H

#include "View/Typedefs.h"

#include <QObject>

#include <boost/scoped_ptr.hpp>

#include <string>
#include <vector>

class Player;

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

	signals:
		void startGame( std::vector< Player > const & inPlayers );

	private slots:
		void onTryToStartGame( std::vector< View::PlayerInfo > const & inPlayers );

	private:
		boost::scoped_ptr< View::StartScreen > mStartScreen;
	};
}

#endif // STARTCONTROLLER_H