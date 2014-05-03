#include "DragMeepleLabel.h"

#include "MeepleUtils.h"
#include "QtGlue.h"

#include "View/DragData.h"
#include "View/Typedefs.h"

#include <QApplication>
#include <QColor>
#include <QDrag>
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QString>
#include <QWidget>

View::DragMeepleLabel::DragMeepleLabel
(
	Piece inType,
	std::size_t inNr,
	Color inColor,
	QWidget * inParent
)
:
	QWidget( inParent ),
	mNrLabel( new QLabel( QString::number( inNr ), this ) ),
	mType( inType ),
	mColor( inColor ),
	mPixmap( getMeeplePixmap( inType, mColor ) ),
	mDragStartPosition()
{
	setContentsMargins( 0, 0, 0, 0 );

	QHBoxLayout * layout = new QHBoxLayout( this );
	layout->setContentsMargins( 0, 0, 0, 0 );
	QLabel * pixmapLabel = new QLabel( this );
	pixmapLabel->setPixmap( mPixmap );
	layout->addWidget( pixmapLabel );
	layout->addSpacing( 5 );
	layout->addWidget( mNrLabel );
	layout->addStretch();

	setLayout( layout );

	setFixedHeight( 40 );
}

void
View::DragMeepleLabel::setNr( std::size_t inNr )
{
	mNrLabel->setText( QString::number( inNr ) );
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
	Dragging::PieceData * dragData = new Dragging::PieceData( mType, mColor );
	drag->setMimeData( dragData );
	drag->setPixmap( mPixmap );
	drag->setHotSpot( QPoint( Gui::kMeepleWidth / 2, Gui::kMeepleHeight / 2 ) );

	drag->exec( Qt::MoveAction );
}