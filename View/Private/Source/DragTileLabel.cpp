#include "DragTileLabel.h"

#include "TileUtils.h"

#include "View/DragData.h"

#include <QApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QPixmap>
#include <QString>
#include <QTransform>
#include <QWidget>

#include <cassert>
#include <sstream>

namespace
{
	Dragging::Rotation
	getNextRotation( Dragging::Rotation inRotation )
	{
		switch ( inRotation )
		{
		case Dragging::kCw0:
			return Dragging::kCw270;
		case Dragging::kCw90:
			return Dragging::kCw0;
		case Dragging::kCw180:
			return Dragging::kCw90;
		case Dragging::kCw270:
			return Dragging::kCw180;
		}
		assert( !"Invalid Dragging::Rotation" );
		return Dragging::kCw0;
	}
}

DragTileLabel::DragTileLabel( QWidget * inParent )
:
	QLabel( inParent ),
	mTileId( "" ),
	mRotation( Dragging::kCw0 ),
	mPixmap(),
	mDragStartPosition(),
	mFaded( false )
{
	setObjectName( QString::fromUtf8( "DragTileLabel" ) );
	setFixedSize( QSize( Gui::kTileWidth, Gui::kTileHeight ) );
	setText( "." );
	setAlignment( Qt::AlignCenter );
}

DragTileLabel::~DragTileLabel()
{
}

void
DragTileLabel::setTile( std::string const & inTileId )
{
	mFaded = false;
	mTileId = inTileId;
	mRotation = Dragging::kCw0;
	updatePixmap();
}

void
DragTileLabel::fadeTile()
{
	mFaded = true;
	updatePixmap();
}

void
DragTileLabel::mouseMoveEvent( QMouseEvent * inEvent )
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
	Dragging::TileData * tileData = new Dragging::TileData( mTileId, mRotation );
	drag->setMimeData( tileData );
	drag->exec( Qt::MoveAction );
}

void
DragTileLabel::mousePressEvent( QMouseEvent * inEvent )
{
	if ( inEvent->button() == Qt::LeftButton )
	{
		mDragStartPosition = inEvent->pos();
	}
}

void
DragTileLabel::mouseReleaseEvent( QMouseEvent * inEvent )
{
	if ( inEvent->button() == Qt::LeftButton )
	{
		if ( (inEvent->pos() - mDragStartPosition).manhattanLength() < QApplication::startDragDistance() )
		{
			mRotation = getNextRotation( mRotation );
			updatePixmap();
		}
	}
}

void
DragTileLabel::updatePixmap()
{
	mPixmap = View::getPixmapForTile( mTileId, mRotation );
	if ( mFaded )
	{
		QPainter painter( &mPixmap );
		QColor color( 0, 0, 0, 128 );
		painter.fillRect( 0, 0, mPixmap.width(), mPixmap.height(), color );
	}
	setPixmap( mPixmap );
}