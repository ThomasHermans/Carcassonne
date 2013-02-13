#include "src-view/BoardView.h"

#include <QGraphicsItem>
#include <QList>
#include <QMouseEvent>

#include <iostream>

BoardView::BoardView(QWidget *parent) :
    QGraphicsView( parent )
{
}

BoardView::BoardView(QGraphicsScene *scene, QWidget *parent) :
    QGraphicsView( scene, parent )
{
}

void
BoardView::mousePressEvent(QMouseEvent *event)
{
    QPointF scenePos = mapToScene( event->pos() );
    double scenex = scenePos.x();
    double sceney = scenePos.y();
    std::cout << "BoardView clicked at [" << scenex << ", " << sceney << "] in the scene." << std::endl;
    // TODO: get position of click.
    QGraphicsView::mousePressEvent( event );
    emit clicked(scenex, sceney);
}
