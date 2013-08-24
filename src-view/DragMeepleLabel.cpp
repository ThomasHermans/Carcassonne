#include "src-view/DragMeepleLabel.h"

#include "src-view/DragData.h"
#include "src-view/Typedefs.h"

#include <QApplication>
#include <QColor>
#include <QDrag>
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QString>
#include <QWidget>

DragMeepleLabel::DragMeepleLabel
(
	Dragging::Piece inType,
	unsigned inNr,
	Dragging::Color inColor,
	QWidget * inParent
)
:
	QWidget( inParent ),
	mNrLabel(),
	mNr( inNr ),
	mType( inType ),
	mColor( inColor ),
	mDragStartPosition()
{
	setContentsMargins( 0, 0, 0, 0 );
	QHBoxLayout * layout = new QHBoxLayout( this );
	layout->setContentsMargins( 0, 0, 0, 0 );

	QLabel * iconLabel = new QLabel( "*", this );
	layout->addWidget( iconLabel );

	mNrLabel = new QLabel( QString::number( mNr ), this );
	layout->addWidget( mNrLabel );

	layout->addStretch();

	setLayout( layout );

	setFixedHeight( sizeHint().height() * 2 );
}

DragMeepleLabel::~DragMeepleLabel()
{}

void
DragMeepleLabel::setNr( unsigned inNr )
{
	mNr = inNr;
	mNrLabel->setText( QString::number( mNr ) );
}

void
DragMeepleLabel::setColor( Dragging::Color inColor )
{
	mColor = inColor;
}

void
DragMeepleLabel::mousePressEvent( QMouseEvent * inEvent )
{
	if ( inEvent->button() == Qt::LeftButton )
	{
		mDragStartPosition = inEvent->pos();
	}
}

void
DragMeepleLabel::mouseMoveEvent( QMouseEvent * inEvent )
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
	DragData * dragData = new DragData( mType, mColor );
	drag->setMimeData( dragData );

	drag->exec( Qt::MoveAction );
}