#ifndef PLAYER_H
#define PLAYER_H

#include "src-model/Color.h"
#include "src-model/Piece.h"
#include "src-model/PlacedPiece.h"

#include <QObject>

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

	bool placePiece( int inRelCol, int inRelRow, Area::Area inArea );
	bool returnPiece( int inRelCol, int inRelRow, Area::Area inArea );

	bool hasPiece( int inRelCol, int inRelRow, Area::Area inArea );
	unsigned getNrOfPieces( int inRelCol, int inRelRow, Area::Area inArea );

	void awardPoints( unsigned inPoints );

signals:
	void nrOfFreePiecesChanged( Player const & inPlayer, unsigned int inNewNrOfFreePieces );
	void scoreChanged( int mScore );

private:
	std::string mName;
	Color::Color mColor;
	int mScore;
	std::vector< Piece > mFreePieces;
	std::vector< PlacedPiece > mPlacedPieces;
};

#endif // PLAYER_H
