#ifndef PLAYER_H
#define PLAYER_H

#include "src-model/Color.h"
#include "src-model/Piece.h"

#include <QObject>

#include <vector>

class Player : public QObject
{
	Q_OBJECT

public:
	Player( std::string inName, Color::Color inColor );
	Player( Player const & inPlayer );
	~Player();
	Player & operator = ( Player const & inPlayer );

	std::string getName() const;
	Color::Color getColor() const;
	unsigned getScore() const;
	unsigned getNumberOfFreePieces() const;

	bool hasFreePieces() const;

	Piece getPieceToPlace();
	void returnPiece( Piece inPiece );

	void awardPoints( unsigned inPoints );

signals:
	void nrOfFreePiecesChanged( Player const & inPlayer, unsigned inNewNrOfFreePieces );
	void scoreChanged( Player const & inPlayer, unsigned mScore );

private:
	std::string mName;
	Color::Color mColor;
	unsigned mScore;
	std::vector< Piece > mFreePieces;
};

#endif // PLAYER_H
