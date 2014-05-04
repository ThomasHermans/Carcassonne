#ifndef DRAGDATA_VIEW_THHR_20130818
#define DRAGDATA_VIEW_THHR_20130818

#include "View/Meeple.h"
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
		PieceData( View::Meeple inPiece );

		View::Meeple const & getMeeple() const;

	private:
		View::Meeple mMeeple;
	};
}

#endif // DRAGDATA_VIEW_THHR_20130818
