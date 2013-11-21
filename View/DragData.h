#ifndef DRAGDATA_VIEW_THHR_20130818
#define DRAGDATA_VIEW_THHR_20130818

#include "View/Typedefs.h"

#include <QMimeData>

#include <string>

namespace Dragging
{
	class TileData : public QMimeData
	{
		Q_OBJECT
	public:
		TileData( std::string const & inTile, View::Rotation inRotation );

		std::string const & getTile() const;
		View::Rotation getRotation() const;

	private:
		std::string mTile;
		View::Rotation mRotation;
	};

	class PieceData : public QMimeData
	{
		Q_OBJECT

	public:
		PieceData( View::Piece inPiece, View::Color inColor );

		View::Piece getPiece() const;
		View::Color getColor() const;

	private:
		View::Piece mPiece;
		View::Color mColor;
	};
}

#endif // DRAGDATA_VIEW_THHR_20130818
