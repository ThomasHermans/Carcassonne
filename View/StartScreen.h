#ifndef STARTSCREEN_H
#define STARTSCREEN_H

#include "View/Typedefs.h"

#include "Utils/Typedefs.h"

#include <QDialog>

#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/signals2/signal.hpp>

#include <set>
#include <string>
#include <vector>

QT_BEGIN_NAMESPACE
	class QCheckBox;
	class QPushButton;
	class QVBoxLayout;
QT_END_NAMESPACE

namespace View
{
	class StartScreenRow;

	struct PlayerInfo
	{
		std::string name;
		Color color;

		PlayerInfo( std::string const & inName, Color inColor );
	};

	/**
	 *	The StartScreen is the screen where you decide with how many
	 *	players you'll play, who will be what color and with which
	 *	expansions you'll play.
	 */
	class StartScreen : public QDialog
	{
		Q_OBJECT
	public:
		boost::signals2::signal< void ( std::set< Utils::Expansion::Type >, std::vector< PlayerInfo > ) > startGame;
	public:
		/**
		 *	Construct an empty StartScreen.
		 */
		explicit StartScreen();
		~StartScreen();

		/**
		 *	Add a player with the given information to the start screen.
		 */
		bool addPlayer( std::string const & inName, Color inColor );

		/**
		 *	Select the given expansions to be played with.
		 */
		void selectExpansions( std::set< Utils::Expansion::Type > const & inExpansions );

	private:
		Color findUnusedColor() const;
		std::set< Utils::Expansion::Type > getSelectedExpansions() const;
		std::vector< PlayerInfo > getPlayers() const;

	private slots:
		bool addPlayer();
		void removePlayer();
		void updateColors( Color inColor );
		void playClicked();

	private:
		QVBoxLayout * mLayout;
		boost::ptr_vector< StartScreenRow > mPlayerRows;
		QPushButton * mAddPlayerButton;
		QCheckBox * mBaseGameBox;
		QCheckBox * mTheExpansionBox;
		QPushButton * mPlayButton;
	};
}

#endif // STARTSCREEN_H
