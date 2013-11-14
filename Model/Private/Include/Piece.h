#ifndef PIECE_H
#define PIECE_H

#include "Model/Color.h"

#include <string>

class PlacedPiece;

class Piece
{
public:
	enum PieceType
	{
		kFollower,
		kLargeFollower,
		kBuilder,
		kPig
	};

public:
	Piece();
	Piece( const Piece & inPiece );
	Piece( const PlacedPiece & inPlacedPiece );
	Piece( PieceType inType, Color::Color inColor );
	Piece & operator = ( const Piece & inPiece );

	PieceType getType() const;
	Color::Color getColor() const;
	int getPoints() const;

	std::string toString() const;

private:
	PieceType mType;
	Color::Color mColor;
};

#endif // PIECE_H
