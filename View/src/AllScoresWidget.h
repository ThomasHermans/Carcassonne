#ifndef ALLSCORESWIDGET_H
#define ALLSCORESWIDGET_H

#include <QWidget>

#include <map>
#include <string>

class QLabel;
class QVBoxLayout;

class AllScoresWidget : public QWidget
{
public:
	AllScoresWidget( QWidget * inParent = 0 );
	~AllScoresWidget();

	void addPlayer( std::string const & inName );
	void setScore( std::string const & inName, unsigned inScore );

private:
	QVBoxLayout * mLayout;
	std::map< std::string, QLabel * > mScores;
};

#endif // ALLSCORESWIDGET_H