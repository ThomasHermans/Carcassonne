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
			std::size_t inNr,
			Color inColor,
			QWidget * inParent
		);

		void setNr( std::size_t inNr );

	protected:
		virtual void mousePressEvent( QMouseEvent * inEvent );
		virtual void mouseMoveEvent( QMouseEvent * inEvent );

	private:
		std::size_t mNr;
		QLabel * mNrLabel;
		Piece mType;
		Color mColor;
		QPixmap mPixmap;
		QPoint mDragStartPosition;
	};
}

#endif // DRAGMEEPLELABEL_THHR_20130811
