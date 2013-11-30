#ifndef ALLSCORESWIDGET_H
#define ALLSCORESWIDGET_H

#include <QWidget>

#include <map>
#include <string>

QT_BEGIN_NAMESPACE
	class QLabel;
	class QVBoxLayout;
QT_END_NAMESPACE

namespace View
{
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
}

#endif // ALLSCORESWIDGET_H