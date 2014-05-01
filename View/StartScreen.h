#ifndef STARTSCREEN_H
#define STARTSCREEN_H

#include "View/Typedefs.h"

#include <QCheckBox>
#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>

#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/signals2/signal.hpp>

#include <set>
#include <string>
#include <vector>

namespace View
{
	class StartScreenRow;

	struct PlayerInfo
	{
		std::string name;
		Color color;

		PlayerInfo( std::string const & inName, Color inColor );
	};

	class StartScreen : public QDialog
	{
		Q_OBJECT
	public:
		boost::signals2::signal< void ( std::set< Expansion::Type >, std::vector< PlayerInfo > ) > startGame;
	public:
		explicit StartScreen( QWidget * inParent = 0 );
		~StartScreen();

		bool addPlayer( std::string const & inName, Color inColor );

	private:
		Color findUnusedColor() const;
		std::set< Expansion::Type > getSelectedExpansions() const;
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
