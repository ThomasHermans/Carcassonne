#ifndef DRAGMEEPLELABEL_THHR_20130811
#define DRAGMEEPLELABEL_THHR_20130811

#include "View/Meeple.h"
#include "View/Typedefs.h"

#include <QPixmap>
#include <QPoint>
#include <QWidget>

QT_BEGIN_NAMESPACE
	class QLabel;
QT_END_NAMESPACE

namespace View
{
	class DragMeepleLabel : public QWidget
	{
	public:
		DragMeepleLabel
		(
			Meeple const & inMeeple,
			QWidget * inParent
		);

		void setNr( std::size_t inNr );

	protected:
		virtual void mousePressEvent( QMouseEvent * inEvent );
		virtual void mouseMoveEvent( QMouseEvent * inEvent );

	private:
		Meeple mMeeple;
		QPixmap mPixmap;
		QLabel * mNrLabel;
		QPoint mDragStartPosition;
	};
}

#endif // DRAGMEEPLELABEL_THHR_20130811
