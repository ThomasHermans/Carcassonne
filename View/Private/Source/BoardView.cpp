#include "BoardView.h"

#include "View/DragData.h"

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QList>
#include <QMimeData>
#include <QMouseEvent>
#include <QString>
#include <QWidget>

#include <iostream>

BoardView::BoardView( QWidget *parent ) :
	QGraphicsView( parent )
{
	setAcceptDrops( true );
}

BoardView::BoardView( QGraphicsScene *scene, QWidget *parent ) :
	QGraphicsView( scene, parent )
{
	setAcceptDrops( true );
}

void
BoardView::mousePressEvent( QMouseEvent * inEvent )
{
	QPointF scenePos = mapToScene( inEvent->pos() );
	double scenex = scenePos.x();
	double sceney = scenePos.y();
	std::cout << "BoardView clicked at [" << scenex << ", " << sceney << "] in the scene." << std::endl;
	// TODO: get position of click.
	QGraphicsView::mousePressEvent( inEvent );
	emit clicked( scenex, sceney );
}

void
BoardView::keyPressEvent( QKeyEvent * inEvent )
{
	if ( inEvent->key() == Qt::Key_Enter || inEvent->key() == Qt::Key_Return )
	{
		emit enterPressed();
	}
	else if ( inEvent->key() == Qt::Key_Space )
	{
		emit spacePressed();
	}
}

void
BoardView::dragEnterEvent( QDragEnterEvent * inEvent )
{
	const DragData * droppedData = qobject_cast< const DragData * >( inEvent->mimeData() );
	if ( droppedData )
	{
		inEvent->acceptProposedAction();
	}
}

void
BoardView::dragMoveEvent( QDragMoveEvent * inEvent )
{
}

void
BoardView::dropEvent( QDropEvent * inEvent )
{
	const DragData * droppedData = qobject_cast< const DragData * >( inEvent->mimeData() );
	if ( droppedData )
	{
		QPointF dropPoint = mapToScene( inEvent->pos() );
		emit dropped( *droppedData, dropPoint.x(), dropPoint.y() );
	}
}
