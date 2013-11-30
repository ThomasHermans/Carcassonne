#ifndef USERINFOWIDGET_THHR_20130824
#define USERINFOWIDGET_THHR_20130824

#include "View/Typedefs.h"

#include <QWidget>

#include <string>

QT_BEGIN_NAMESPACE
	class QLabel;
QT_END_NAMESPACE

namespace View
{
	class DragMeepleLabel;

	class UserInfoWidget : public QWidget
	{
	public:
		UserInfoWidget
		(
			std::string const & inName,
			View::Color inColor,
			unsigned inNumberOfFollowers,
			QWidget * inParent
		);
		~UserInfoWidget();

		void setScore( unsigned inScore );
		void setNumberOfFollowers( unsigned inNumberOfFollowers );

	private:
		QLabel * mNameLabel;
		QLabel * mScoreLabel;
		DragMeepleLabel * mDragFollowersLabel;
	};
}

#endif // USERINFOWIDGET_THHR_20130824
