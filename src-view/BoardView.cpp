#include "src-view/BoardView.h"

#include <QGraphicsItem>
#include <QKeyEvent>
#include <QList>
#include <QMouseEvent>

#include <iostream>

BoardView::BoardView( QWidget *parent ) :
	QGraphicsView( parent )
{
}

BoardView::BoardView( QGraphicsScene *scene, QWidget *parent ) :
	QGraphicsView( scene, parent )
{
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
}