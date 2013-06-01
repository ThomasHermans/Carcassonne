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
	int getScore() const;
	int getNumberOfFreePieces() const;

	bool hasFreePieces() const;

	bool placePiece( int inRelCol, int inRelRow, Area::Area inArea );
	bool returnPiece( int inRelCol, int inRelRow, Area::Area inArea );

	bool hasPiece( int inRelCol, int inRelRow, Area::Area inArea );
	unsigned getNrOfPieces( int inRelCol, int inRelRow, Area::Area inArea );

	void awardPoints( unsigned inPoints );

private:
	std::string mName;
	Color::Color mColor;
	int mScore;
	std::vector< Piece > mFreePieces;
	std::vector< PlacedPiece > mPlacedPieces;
};

#endif // PLAYER_H
