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
		TileData( std::string const & inTile, Rotation inRotation );

		std::string const & getTile() const;
		Rotation getRotation() const;

	private:
		std::string mTile;
		Rotation mRotation;
	};

	class PieceData : public QMimeData
	{
		Q_OBJECT

	public:
		PieceData( Piece inPiece, Color inColor );

		Piece getPiece() const;
		Color getColor() const;

	private:
		Piece mPiece;
		Color mColor;
	};
}

#endif // DRAGDATA_VIEW_THHR_20130818
