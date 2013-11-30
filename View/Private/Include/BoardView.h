#ifndef BOARDVIEW_H
#define BOARDVIEW_H

#include "View/Typedefs.h"

#include <QGraphicsView>

QT_BEGIN_NAMESPACE
	class QDragEnterEvent;
	class QDragMoveEvent;
	class QDropEvent;
	class QGraphicsScene;
	class QKeyEvent;
	class QMouseEvent;
	class QWidget;
QT_END_NAMESPACE

namespace Dragging
{
	class PieceData;
	class TileData;
}

namespace View
{
	class BoardView : public QGraphicsView
	{
		Q_OBJECT
	public:
		explicit BoardView( QWidget *parent = 0 );
		explicit BoardView( QGraphicsScene *inScene, QWidget *parent = 0 );

	signals:
		void clicked( int x, int y );
		void droppedPiece( Dragging::PieceData const & inData, int inX, int inY );
		void droppedTile( int inX, int inY, std::string const & inTileId, View::Rotation inRotation );
		void enterPressed();
		void spacePressed();

	protected:
		void mousePressEvent( QMouseEvent * inEvent );
		void keyPressEvent( QKeyEvent * inEvent );

		void dragEnterEvent( QDragEnterEvent * inEvent );
		void dragMoveEvent( QDragMoveEvent * inEvent );
		void dropEvent( QDropEvent * inEvent );
	};
}

#endif // BOARDVIEW_H
