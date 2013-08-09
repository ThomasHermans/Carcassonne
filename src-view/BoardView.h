#ifndef BOARDVIEW_H
#define BOARDVIEW_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QWidget>

class BoardView : public QGraphicsView
{
	Q_OBJECT
public:
	explicit BoardView( QWidget *parent = 0 );
	explicit BoardView( QGraphicsScene *inScene, QWidget *parent = 0 );

signals:
	void clicked(int x, int y);
	void enterPressed();
	void spacePressed();
	void mPressed();

protected:
	void mousePressEvent( QMouseEvent * inEvent );
	void keyPressEvent( QKeyEvent * inEvent );
};

#endif // BOARDVIEW_H
