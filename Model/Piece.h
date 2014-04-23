#ifndef PIECE_H
#define PIECE_H

#include "Model/Color.h"

#include <string>

namespace Model
{
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
		Piece( PlacedPiece const & inPlacedPiece );
		Piece( PieceType inType, Color::Color inColor );

		PieceType getType() const;
		Color::Color getColor() const;
		std::size_t getWeight() const;

	private:
		PieceType mType;
		Color::Color mColor;
	};
}

#endif // PIECE_H
