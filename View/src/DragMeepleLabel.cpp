#include "View/src/DragMeepleLabel.h"

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

namespace
{
	QColor
	toQColor( Dragging::Color inColor )
	{
		switch ( inColor )
		{
			case Dragging::kRed:
				return Qt::red;
			case Dragging::kGreen:
				return Qt::green;
			case Dragging::kBlue:
				return Qt::blue;
			case Dragging::kYellow:
				return Qt::yellow;
			case Dragging::kBlack:
				return Qt::black;
			case Dragging::kGray:
				return Qt::gray;
			default:
				return Qt::white;
		}
	}

	QPainterPath
	getMeeplePath( int inX, int inY, int inWidth, int inHeight )
	{
		QPainterPath path;
		path.moveTo( inX + .5 * inWidth, inY + 0 );
		path.lineTo( inX + .8 * inWidth, inY + inHeight );
		path.lineTo( inX + 0, inY + .4 * inHeight );
		path.lineTo( inX + inWidth, inY + .4 * inHeight );
		path.lineTo( inX + .2 * inWidth, inY + inHeight );
		path.lineTo( inX + .5 * inWidth, inY + 0 );
		path.setFillRule( Qt::WindingFill );
		return path;
	}
}

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

void
DragMeepleLabel::paintEvent( QPaintEvent * inEvent )
{
	QPainter painter( this );
	painter.fillPath( getMeeplePath( 0, 5, 30, 30 ), toQColor( mColor ) );
}
