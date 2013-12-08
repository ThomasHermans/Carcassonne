#ifndef BOARDVIEW_H
#define BOARDVIEW_H

#include "View/Typedefs.h"

#include <QGraphicsView>

#include <boost/optional.hpp>

#include <string>

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

		void placeTile( int inX, int inY, std::string const & inId, Rotation inRotation );
		void clearCurrentTile();

	signals:
		void clicked( int x, int y );
		void droppedPiece( Dragging::PieceData const & inData, int inX, int inY );
		void droppedTile( int inX, int inY, std::string const & inTileId, View::Rotation inRotation );
		void enterPressed();
		void spacePressed();

	protected:
		void mousePressEvent( QMouseEvent * inEvent );
		void mouseMoveEvent( QMouseEvent * inEvent );
		void mouseReleaseEvent( QMouseEvent * inEvent );
		void keyPressEvent( QKeyEvent * inEvent );

		void dragEnterEvent( QDragEnterEvent * inEvent );
		void dragMoveEvent( QDragMoveEvent * inEvent );
		void dropEvent( QDropEvent * inEvent );

	private:
		bool dragStartedOnNoTile() const;
		bool dragStartedOnCurrentlyPlacedTile() const;
		void zoomIn();
		void zoomOut();

	private:
		QPointF mPressPosition;
		bool mPanning;
		int mPanX;
		int mPanY;
		boost::optional< QPoint > mCurrentTilePosition;
		std::string mCurrentTile;
		Rotation mRotation;
	};
}

#endif // BOARDVIEW_H
