#include "DragMeepleLabel.h"

#include "MeepleUtils.h"
#include "QtGlue.h"

#include "View/DragData.h"

#include <QApplication>
#include <QColor>
#include <QDrag>
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QString>

View::DragMeepleLabel::DragMeepleLabel
(
	Meeple const & inMeeple,
	QWidget * inParent
)
:
	QWidget( inParent ),
	mMeeple( inMeeple ),
	mPixmap( getMeeplePixmap( inMeeple ) ),
	mAmountLabel( new QLabel( QString::number( 0 ), this ) ),
	mDragStartPosition()
{
	setContentsMargins( 0, 0, 0, 0 );

	QHBoxLayout * layout = new QHBoxLayout( this );
	layout->setContentsMargins( 0, 0, 0, 0 );
	QLabel * pixmapLabel = new QLabel( this );
	pixmapLabel->setPixmap( mPixmap );
	layout->addWidget( pixmapLabel );
	layout->addSpacing( 5 );
	layout->addWidget( mAmountLabel );
	layout->addStretch();

	setLayout( layout );

	setFixedHeight( 40 );
}

void
View::DragMeepleLabel::setAmount( std::size_t inAmount )
{
	mAmountLabel->setText( QString::number( inAmount ) );
}

void
View::DragMeepleLabel::mousePressEvent( QMouseEvent * inEvent )
{
	if ( inEvent->button() == Qt::LeftButton )
	{
		mDragStartPosition = inEvent->pos();
	}
}

void
View::DragMeepleLabel::mouseMoveEvent( QMouseEvent * inEvent )
{
	if ( !( inEvent->buttons() & Qt::LeftButton ) )
	{
		return;
	}
	if ( (inEvent->pos() - mDragStartPosition).manhattanLength() < QApplication::startDragDistance() )
	{
		return;
	}

	QDrag * drag = new QDrag( this );
	Dragging::PieceData * dragData = new Dragging::PieceData( mMeeple );
	drag->setMimeData( dragData );
	drag->setPixmap( mPixmap );
	drag->setDragCursor( getEmptyPixmap(), Qt::MoveAction );
	drag->setHotSpot( QPoint( Gui::kMeepleWidth / 2, Gui::kMeepleHeight / 2 ) );

	drag->exec( Qt::MoveAction );
}