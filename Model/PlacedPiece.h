#ifndef PLACEDPIECE_H
#define PLACEDPIECE_H

#include "Model/Piece.h"
#include "Model/Tile.h"

#include <string>

namespace Model
{
	class PlacedPiece
	{
	public:
		PlacedPiece( const Piece & inPiece, Area::Area inArea );

		Piece getPiece() const;
		Piece::PieceType getType() const;
		Color::Color getColor() const;
		Area::Area getArea() const;

	private:
		Piece mPiece;
		Area::Area mArea;
	};

	bool operator == ( PlacedPiece const & inLeft, PlacedPiece const & inRight );
}

#endif // PLACEDPIECE_H
