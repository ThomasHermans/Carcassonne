#ifndef BOARDVIEW_H
#define BOARDVIEW_H

#include <QGraphicsView>

class QDragEnterEvent;
class QDragMoveEvent;
class QDropEvent;
class QGraphicsScene;
class QKeyEvent;
class QMouseEvent;
class QWidget;

class DragData;

class BoardView : public QGraphicsView
{
	Q_OBJECT
public:
	explicit BoardView( QWidget *parent = 0 );
	explicit BoardView( QGraphicsScene *inScene, QWidget *parent = 0 );

signals:
	void clicked( int x, int y );
	void enterPressed();
	void spacePressed();
	void dropped( DragData const & inData, int inX, int inY );

protected:
	void mousePressEvent( QMouseEvent * inEvent );
	void keyPressEvent( QKeyEvent * inEvent );

	void dragEnterEvent( QDragEnterEvent * inEvent );
	void dragMoveEvent( QDragMoveEvent * inEvent );
	void dropEvent( QDropEvent * inEvent );
};

#endif // BOARDVIEW_H
