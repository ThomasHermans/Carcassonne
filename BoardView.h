#ifndef BOARDVIEW_H
#define BOARDVIEW_H

#include <QGraphicsView>

#include <QGraphicsScene>
#include <QWidget>

class BoardView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit BoardView( QWidget *parent = 0 );
    explicit BoardView( QGraphicsScene *inScene, QWidget *parent = 0 );

signals:
    void clicked(int x, int y);

protected:
    void mousePressEvent( QMouseEvent *event );
};

#endif // BOARDVIEW_H
