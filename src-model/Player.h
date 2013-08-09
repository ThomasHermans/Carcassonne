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
	int getScore() const;
	int getNumberOfFreePieces() const;

	bool hasFreePieces() const;

	Piece getPieceToPlace();
	void returnPiece( Piece inPiece );

	void awardPoints( unsigned inPoints );

signals:
	void nrOfFreePiecesChanged( Player const & inPlayer, unsigned int inNewNrOfFreePieces );
	void scoreChanged( int mScore );

private:
	std::string mName;
	Color::Color mColor;
	int mScore;
	std::vector< Piece > mFreePieces;
};

#endif // PLAYER_H
