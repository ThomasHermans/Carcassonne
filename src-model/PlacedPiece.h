#ifndef PLACEDPIECE_H
#define PLACEDPIECE_H

#include "src-model/Piece.h"
#include "src-model/Tile.h"

#include <string>

class PlacedPiece
{
public:
	PlacedPiece( const Piece & inPiece, Area::Area inArea );

	Piece getPiece() const;
	Piece::PieceType getType() const;
	Color::Color getColor() const;
	Area::Area getArea() const;

	std::string toString() const;

private:
	Piece mPiece;
	Area::Area mArea;
};

#endif // PLACEDPIECE_H
