#include "BoardView.h"

#include "TileUtils.h"

#include "View/DragData.h"

#include <QApplication>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QList>
#include <QMimeData>
#include <QMouseEvent>
#include <QScrollBar>
#include <QString>
#include <QWidget>

#include <cmath>
#include <iostream>

namespace
{
	double const kScaleFactor = 1.41421356237; // sqrt 2
	double const kMaxScale = std::pow( kScaleFactor, 4 );
	double const kMinScale = 1. / kMaxScale;
}

View::BoardView::BoardView( QWidget *parent ) :
	QGraphicsView( parent ),
	mPressPosition(),
	mPanning( false ),
	mPanX( 0 ),
	mPanY( 0 ),
	mCurrentTilePosition( boost::none ),
	mCurrentTile(),
	mRotation( View::kCw0 )

{
	setAcceptDrops( true );
}

View::BoardView::BoardView( QGraphicsScene *scene, QWidget *parent ) :
	QGraphicsView( scene, parent ),
	mPressPosition(),
	mPanning( false ),
	mPanX( 0 ),
	mPanY( 0 ),
	mCurrentTilePosition( boost::none ),
	mCurrentTile(),
	mRotation( View::kCw0 )

{
	setAcceptDrops( true );
}

void
View::BoardView::placeTile( int inX, int inY, std::string const & inId, Rotation inRotation )
{
	mCurrentTilePosition = QPoint( inX, inY );
	mCurrentTile = inId;
	mRotation = inRotation;
}

void
View::BoardView::clearCurrentTile()
{
	mCurrentTilePosition = boost::none;
}

void
View::BoardView::mousePressEvent( QMouseEvent * inEvent )
{
	if ( inEvent->button() == Qt::LeftButton )
	{
		mPressPosition = mapToScene( inEvent->pos() );
		if ( dragStartedOnNoTile() )
		{
			mPanning = true;
			mPanX = inEvent->x();
			mPanY = inEvent->y();
		}
		else
		{
			mPanning = false;
		}
		inEvent->accept();
		return;
	}
}

void
View::BoardView::mouseMoveEvent( QMouseEvent * inEvent )
{
	if ( !( inEvent->buttons() & Qt::LeftButton ) )
	{
		return;
	}
	if ( dragStartedOnCurrentlyPlacedTile() )
	{
		if ( (inEvent->pos() - mPressPosition).manhattanLength() > QApplication::startDragDistance() )
		{
			// Drag the currently placed tile to another location
			QDrag * drag = new QDrag( this );
			Dragging::TileData * tileData = new Dragging::TileData( mCurrentTile, mRotation );
			drag->setMimeData( tileData );
			drag->setPixmap( getPixmapForTile( mCurrentTile, mRotation ) );
			drag->exec( Qt::MoveAction );
		}
	}
	if ( mPanning )
	{
		setCursor( Qt::ClosedHandCursor );
		horizontalScrollBar()->setValue( horizontalScrollBar()->value() - (inEvent->x() - mPanX ) );
		verticalScrollBar()->setValue( verticalScrollBar()->value() - (inEvent->y() - mPanY ) );
		mPanX = inEvent->x();
		mPanY = inEvent->y();
		inEvent->accept();
		return;
	}
	QGraphicsView::mouseMoveEvent( inEvent );
}

void
View::BoardView::mouseReleaseEvent( QMouseEvent * inEvent )
{
	if ( inEvent->button() == Qt::LeftButton )
	{
		mPanning = false;
		setCursor( Qt::ArrowCursor );
		if ( (mapToScene( inEvent->pos() ) - mPressPosition).manhattanLength() < QApplication::startDragDistance() )
		{
			QPointF const scenePos = mapToScene( inEvent->pos() );
			double const scenex = scenePos.x();
			double const sceney = scenePos.y();
			std::cout << "BoardView clicked at [" << scenex << ", " << sceney << "] in the scene." << std::endl;
			emit clicked( scenex, sceney );
		}
	}
}

void
View::BoardView::keyPressEvent( QKeyEvent * inEvent )
{
	if ( inEvent->key() == Qt::Key_Enter || inEvent->key() == Qt::Key_Return )
	{
		emit enterPressed();
		return;
	}
	else if ( inEvent->key() == Qt::Key_Space )
	{
		emit spacePressed();
		return;
	}
	else if ( inEvent->key() == Qt::Key_Plus && inEvent->modifiers().testFlag( Qt::ControlModifier ) )
	{
		zoomIn();
		return;
	}
	else if ( inEvent->key() == Qt::Key_Minus && inEvent->modifiers().testFlag( Qt::ControlModifier ) )
	{
		zoomOut();
		return;
	}
	else if ( inEvent->key() == Qt::Key_0 && inEvent->modifiers().testFlag( Qt::ControlModifier ) )
	{
		zoomTo100();
		return;
	}
	QGraphicsView::keyPressEvent( inEvent );
}

void
View::BoardView::dragEnterEvent( QDragEnterEvent * inEvent )
{
	Dragging::TileData const * tileData = qobject_cast< Dragging::TileData const * >( inEvent->mimeData() );
	if ( tileData )
	{
		inEvent->acceptProposedAction();
		return;
	}
	Dragging::PieceData const * pieceData = qobject_cast< Dragging::PieceData const * >( inEvent->mimeData() );
	if ( pieceData )
	{
		inEvent->acceptProposedAction();
		return;
	}
}

void
View::BoardView::dragMoveEvent( QDragMoveEvent * inEvent )
{
}

void
View::BoardView::dropEvent( QDropEvent * inEvent )
{
	Dragging::TileData const * tileData = qobject_cast< Dragging::TileData const * >( inEvent->mimeData() );
	if ( tileData )
	{
		QPointF dropPoint = mapToScene( inEvent->pos() );
		emit droppedTile( dropPoint.x(), dropPoint.y(), tileData->getTile(), tileData->getRotation() );
		return;
	}
	Dragging::PieceData const * pieceData = qobject_cast< Dragging::PieceData const * >( inEvent->mimeData() );
	if ( pieceData )
	{
		QPointF dropPoint = mapToScene( inEvent->pos() );
		emit droppedPiece( *pieceData, dropPoint.x(), dropPoint.y() );
		return;
	}
}

bool
View::BoardView::dragStartedOnNoTile() const
{
	if ( scene()->itemAt( mPressPosition ) )
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool
View::BoardView::dragStartedOnCurrentlyPlacedTile() const
{
	if ( !mCurrentTilePosition )
	{
		return false;
	}
	int const pressedX = mPressPosition.x();
	int const pressedY = mPressPosition.y();
	int const tileX = mCurrentTilePosition->x();
	int const tileY = mCurrentTilePosition->y();
	if ( pressedX < tileX || pressedX > tileX + Gui::kTileWidth )
	{
		return false;
	}
	if ( pressedY < tileY || pressedY > tileY + Gui::kTileHeight )
	{
		return false;
	}
	return true;
}

void
View::BoardView::zoomIn()
{
	if ( transform().m11() < kMaxScale )
	{
		scale( kScaleFactor, kScaleFactor );
	}
}

void
View::BoardView::zoomOut()
{
	if ( transform().m11() > kMinScale )
	{
		scale( 1. / kScaleFactor, 1. / kScaleFactor );
	}
}

void
View::BoardView::zoomTo100()
{
	resetTransform();
}