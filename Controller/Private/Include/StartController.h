#ifndef STARTCONTROLLER_H
#define STARTCONTROLLER_H

#include "View/Typedefs.h"

#include <QObject>

#include <boost/scoped_ptr.hpp>

#include <map>
#include <string>

namespace View
{
	class StartScreen;
}

class StartController : public QObject
{
	Q_OBJECT
public:
	StartController( QObject * inParent = 0 );
	~StartController();

signals:
	void startGame( std::map< Gui::Color, std::string > const & inPlayers );

private slots:
	void onTryToStartGame( std::map< Gui::Color, std::string > const & inPlayers );

private:
	boost::scoped_ptr< View::StartScreen > mStartScreen;
};

#endif // STARTCONTROLLER_H