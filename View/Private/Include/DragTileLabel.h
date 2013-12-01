#ifndef VIEW_DRAGTILELABEL_THHR_20131120
#define VIEW_DRAGTILELABEL_THHR_20131120

#include "View/Typedefs.h"

#include <QLabel>
#include <QPoint>
#include <QPixmap>

#include <string>

QT_BEGIN_NAMESPACE
	class QMouseEvent;
	class QWidget;
QT_END_NAMESPACE

namespace View
{
	class DragTileLabel : public QLabel
	{
	public:
		DragTileLabel( QWidget * inParent );
		~DragTileLabel();

		std::string const & getCurrentTile() const;
		View::Rotation getCurrentRotation() const;

		void setTile( std::string const & inTileId );
		void fadeTile();

	protected:
		void mouseMoveEvent( QMouseEvent * inEvent );
		void mousePressEvent( QMouseEvent * inEvent );
		void mouseReleaseEvent( QMouseEvent * inEvent );

	private:
		void updatePixmap();

	private:
		std::string mTileId;
		View::Rotation mRotation;
		QPixmap mPixmap;
		QPoint mDragStartPosition;
		bool mFaded;
	};
}

#endif