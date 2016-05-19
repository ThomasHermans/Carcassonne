#ifndef USERINFOWIDGET_THHR_20130824
#define USERINFOWIDGET_THHR_20130824

#include "View/Meeple.h"
#include "View/Typedefs.h"

#include <QWidget>

#include <map>
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
		void setSupply( Meeple::MeepleType inType, std::size_t inAmount );

	private:
		Color mColor;
		QVBoxLayout * mMainLayout;
		QLabel * mNameLabel;
		QLabel * mScoreLabel;
		std::map< Meeple::MeepleType, DragMeepleLabel * > mDragLabels;
	};
}

#endif // USERINFOWIDGET_THHR_20130824
