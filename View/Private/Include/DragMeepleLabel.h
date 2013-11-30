#ifndef DRAGMEEPLELABEL_THHR_20130811
#define DRAGMEEPLELABEL_THHR_20130811

#include "View/Typedefs.h"

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
			View::Piece inType,
			unsigned inNr,
			View::Color inColor,
			QWidget * inParent
		);

		~DragMeepleLabel();

		void setNr( unsigned inNr );
		void setColor( View::Color inColor );

	protected:
		void mousePressEvent( QMouseEvent * inEvent );
		void mouseMoveEvent( QMouseEvent * inEvent );
		void paintEvent( QPaintEvent * inEvent );

	private:
		QLabel * mNrLabel;
		unsigned mNr;
		View::Piece mType;
		View::Color mColor;
		QPoint mDragStartPosition;
	};
}

#endif // DRAGMEEPLELABEL_THHR_20130811
