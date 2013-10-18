#ifndef STARTSCREEN_H
#define STARTSCREEN_H

#include "src-view/Typedefs.h"

#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>

#include <string>
#include <vector>
#include <utility>

class StartScreenRow;

class StartScreen : public QDialog
{
	Q_OBJECT
public:
	explicit StartScreen( QWidget * inParent = 0 );
	~StartScreen();

signals:
	void startGame( std::vector< std::pair< std::string, Gui::Color > > const & inPlayers );

private:
	Gui::Color findUnusedColor() const;

private slots:
	void addPlayer();
	void removePlayer();
	void updateColors( Gui::Color inColor );
	void playClicked();

private:
	QVBoxLayout * mLayout;
	std::vector< StartScreenRow * > mPlayerRows;
	QPushButton * mAddPlayerButton;
	QPushButton * mPlayButton;
};

#endif // STARTSCREEN_H