#ifndef SUPREMECONTROLLER_H
#define SUPREMECONTROLLER_H

#include "src-view/Typedefs.h"

#include <QObject>

#include <boost/scoped_ptr.hpp>

#include <map>
#include <string>

class GameController;
class StartController;

class SupremeController : QObject
{
	Q_OBJECT
public:
	SupremeController( QObject * inParent = 0 );
	~SupremeController();

private slots:
	void startGame( std::map< Gui::Color, std::string > const & inPlayers );

private:
	boost::scoped_ptr< StartController > mStartController;
	boost::scoped_ptr< GameController > mGameController;
};

#endif // SUPREMECONTROLLER_H