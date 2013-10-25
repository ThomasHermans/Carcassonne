#ifndef DRAGDATA_VIEW_THHR_20130818
#define DRAGDATA_VIEW_THHR_20130818

#include "View/Typedefs.h"

#include <QMimeData>

class DragData : public QMimeData
{
	Q_OBJECT

public:
	DragData( Dragging::Piece inPiece, Dragging::Color inColor );
	~DragData();

	Dragging::Piece getPiece() const;
	Dragging::Color getColor() const;

private:
	Dragging::Piece mPiece;
	Dragging::Color mColor;
};

#endif // DRAGDATA_VIEW_THHR_20130818
