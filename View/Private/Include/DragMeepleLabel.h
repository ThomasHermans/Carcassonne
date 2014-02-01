#ifndef DRAGMEEPLELABEL_THHR_20130811
#define DRAGMEEPLELABEL_THHR_20130811

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
			Piece inType,
			unsigned inNr,
			Color inColor,
			QWidget * inParent
		);

		~DragMeepleLabel();

		void setNr( unsigned inNr );

	protected:
		virtual void mousePressEvent( QMouseEvent * inEvent );
		virtual void mouseMoveEvent( QMouseEvent * inEvent );
		virtual void paintEvent( QPaintEvent * inEvent );

	private:
		QLabel * mNrLabel;
		unsigned mNr;
		Piece mType;
		Color mColor;
		QPixmap mMeeplePixmap;
		QPoint mDragStartPosition;
	};
}

#endif // DRAGMEEPLELABEL_THHR_20130811
