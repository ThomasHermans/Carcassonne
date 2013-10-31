#ifndef USERINFOWIDGET_THHR_20130824
#define USERINFOWIDGET_THHR_20130824

#include "View/Typedefs.h"

#include <QWidget>

#include <string>

class DragMeepleLabel;

class QLabel;

class UserInfoWidget : public QWidget
{
public:
	UserInfoWidget
	(
		std::string const & inName,
		Dragging::Color inColor,
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

#endif // USERINFOWIDGET_THHR_20130824
