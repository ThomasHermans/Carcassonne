#ifndef USERINFOWIDGET_THHR_20130824
#define USERINFOWIDGET_THHR_20130824

#include "View/Typedefs.h"

#include <QWidget>

#include <string>

QT_BEGIN_NAMESPACE
	class QLabel;
	class QVBoxLayout;
QT_END_NAMESPACE

namespace View
{
	class DragMeepleLabel;

	/**
	 *	A UserInfoWidget is a widget showing all the info
	 *	of a player. It shows the name, the current score
	 *	and the number of free pieces per piecetype.
	 */
	class UserInfoWidget : public QWidget
	{
	public:
		UserInfoWidget
		(
			std::string const & inName,
			Color inColor,
			QWidget * inParent
		);

		void setScore( std::size_t inScore );
		void setNumberOfFollowers( std::size_t inNumberOfFollowers );
		void enableLargeFollowers();
		void setNumberOfLargeFollowers( std::size_t inNumberOfLargeFollowers );

	private:
		void initializeLargeFollowers();

	private:
		Color mColor;
		QVBoxLayout * mLayout;
		QLabel * mNameLabel;
		QLabel * mScoreLabel;
		DragMeepleLabel * mDragFollowersLabel;
		DragMeepleLabel * mDragLargeFollowersLabel;
	};
}

#endif // USERINFOWIDGET_THHR_20130824
