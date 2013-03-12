#ifndef PLAYER_H
#define PLAYER_H

#include "src-model/Color.h"
#include "src-model/Piece.h"
#include "src-model/PlacedPiece.h"

class Player
{
public:
    Player( std::string inName, Color::Color inColor );
	~Player();

	std::string getName() const;
	Color::Color getColor() const;

	bool hasFreePieces() const;

	bool placePiece( unsigned int inLocation, Area::Area inArea );

private:
	std::string mName;
    Color::Color mColor;
	int mScore;
	std::vector< Piece > mFreePieces;
	std::vector< PlacedPiece > mPlacedPieces;
};

#endif // PLAYER_H
