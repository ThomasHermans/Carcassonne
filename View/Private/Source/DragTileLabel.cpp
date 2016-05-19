#include "DragTileLabel.h"

#include "QtGlue.h"
#include "TileUtils.h"

#include "View/DragData.h"

#include <QApplication>
#include <QDrag>
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
	View::Rotation
	getNextRotation( View::Rotation inRotation )
	{
		switch ( inRotation )
		{
		case View::kCw0:
			return View::kCw270;
		case View::kCw90:
			return View::kCw0;
		case View::kCw180:
			return View::kCw90;
		case View::kCw270:
			return View::kCw180;
		}
		assert( !"Invalid View::Rotation" );
		return View::kCw0;
	}
}

View::DragTileLabel::DragTileLabel( QWidget * inParent )
:
	QLabel( inParent ),
	mTileId( "" ),
	mRotation( View::kCw0 ),
	mPixmap(),
	mDragStartPosition(),
	mFaded( false )
{
	setObjectName( QString::fromUtf8( "DragTileLabel" ) );
	setFixedSize( QSize( Gui::kTileWidth, Gui::kTileHeight ) );
	setText( "." );
	setAlignment( Qt::AlignCenter );
}

View::DragTileLabel::~DragTileLabel()
{
}

std::string const &
View::DragTileLabel::getCurrentTile() const
{
	return mTileId;
}

View::Rotation
View::DragTileLabel::getCurrentRotation() const
{
	return mRotation;
}

void
View::DragTileLabel::setTile( std::string const & inTileId )
{
	mFaded = false;
	mTileId = inTileId;
	mRotation = View::kCw0;
	updatePixmap();
}

void
View::DragTileLabel::fadeTile()
{
	mFaded = true;
	updatePixmap();
}

void
View::DragTileLabel::mouseMoveEvent( QMouseEvent * inEvent )
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
	drag->setPixmap( mPixmap );
	drag->setDragCursor( getEmptyPixmap(), Qt::MoveAction );
	drag->setHotSpot( QPoint( Gui::kTileWidth / 2, Gui::kTileHeight / 2 ) );
	drag->exec( Qt::MoveAction );
}

void
View::DragTileLabel::mousePressEvent( QMouseEvent * inEvent )
{
	if ( inEvent->button() == Qt::LeftButton )
	{
		mDragStartPosition = inEvent->pos();
	}
}

void
View::DragTileLabel::mouseReleaseEvent( QMouseEvent * inEvent )
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
View::DragTileLabel::updatePixmap()
{
	if ( !mTileId.empty() )
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
}