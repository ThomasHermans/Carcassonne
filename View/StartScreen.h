#ifndef STARTSCREEN_H
#define STARTSCREEN_H

#include "View/Typedefs.h"

#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>

#include <map>
#include <string>
#include <vector>

class StartScreenRow;

class StartScreen : public QDialog
{
	Q_OBJECT
public:
	explicit StartScreen( QWidget * inParent = 0 );
	~StartScreen();

signals:
	void startGame( std::map< Gui::Color, std::string > const & inPlayers );

private:
	Gui::Color findUnusedColor() const;
	void loadDefaultPlayers();

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
