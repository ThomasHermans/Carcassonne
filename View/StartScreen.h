#ifndef STARTSCREEN_H
#define STARTSCREEN_H

#include "View/Typedefs.h"

#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>

#include <boost/ptr_container/ptr_vector.hpp>

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
		explicit StartScreen( QWidget * inParent = 0 );
		~StartScreen();

		bool addPlayer( QString const & inName, Color inColor );

	signals:
		void startGame( std::vector< View::PlayerInfo > const & inPlayers );

	private:
		Color findUnusedColor() const;

	private slots:
		bool addPlayer();
		void removePlayer();
		void updateColors( Color inColor );
		void playClicked();

	private:
		QVBoxLayout * mLayout;
		boost::ptr_vector< StartScreenRow > mPlayerRows;
		QPushButton * mAddPlayerButton;
		QPushButton * mPlayButton;
	};
}

#endif // STARTSCREEN_H
