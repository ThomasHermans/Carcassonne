#ifndef DRAGMEEPLELABEL_THHR_20130811
#define DRAGMEEPLELABEL_THHR_20130811

#include "View/Typedefs.h"

#include <QWidget>

class QLabel;

class DragMeepleLabel : public QWidget
{
public:
	DragMeepleLabel
	(
		Dragging::Piece inType,
		unsigned inNr,
		Dragging::Color inColor,
		QWidget * inParent
	);

	~DragMeepleLabel();

	void setNr( unsigned inNr );
	void setColor( Dragging::Color inColor );

protected:
	void mousePressEvent( QMouseEvent * inEvent );
	void mouseMoveEvent( QMouseEvent * inEvent );
	void paintEvent( QPaintEvent * inEvent );

private:
	QLabel * mNrLabel;
	unsigned mNr;
	Dragging::Piece mType;
	Dragging::Color mColor;
	QPoint mDragStartPosition;
};

#endif // DRAGMEEPLELABEL_THHR_20130811
