#ifndef STARTSCREEN_H
#define STARTSCREEN_H

#include "View/Typedefs.h"

#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>

#include <map>
#include <string>
#include <vector>

namespace View
{
	class StartScreenRow;

	class StartScreen : public QDialog
	{
		Q_OBJECT
	public:
		explicit StartScreen( QWidget * inParent = 0 );

		bool addPlayer( QString const & inName, Color inColor );

	signals:
		void startGame( std::map< View::Color, std::string > const & inPlayers );

	private:
		Color findUnusedColor() const;

	private slots:
		bool addPlayer();
		void removePlayer();
		void updateColors( Color inColor );
		void playClicked();

	private:
		QVBoxLayout * mLayout;
		std::vector< StartScreenRow * > mPlayerRows;
		QPushButton * mAddPlayerButton;
		QPushButton * mPlayButton;
	};
}

#endif // STARTSCREEN_H
