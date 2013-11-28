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

DragMeepleLabel::DragMeepleLabel
(
	View::Piece inType,
	unsigned inNr,
	View::Color inColor,
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

	layout->addSpacing( 40 );

	mNrLabel = new QLabel( QString::number( mNr ), this );
	layout->addWidget( mNrLabel );

	layout->addStretch();

	setLayout( layout );

	setFixedWidth( parentWidget()->width() );
	setFixedHeight( 40 );
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
DragMeepleLabel::setColor( View::Color inColor )
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
	Dragging::PieceData * dragData = new Dragging::PieceData( mType, mColor );
	drag->setMimeData( dragData );
	drag->setPixmap( getMeeplePixmap( mColor ) );

	drag->exec( Qt::MoveAction );
}

void
DragMeepleLabel::paintEvent( QPaintEvent * inEvent )
{
	QPainter painter( this );
	static QPainterPath path = View::getMeeplePath( 0, 5, Gui::kMeepleWidth, Gui::kMeepleHeight );
	painter.fillPath( path, View::toQColor( mColor ) );
}
