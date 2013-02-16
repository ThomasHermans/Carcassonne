#ifndef PIECE_H
#define PIECE_H

#include "src-model/Color.h"

#include <string>

class Piece
{
public:
    enum PieceType
    {
        Follower,
        LargeFollower,
        Builder,
        Pig
    };

public:
    Piece();
    Piece(const Piece & inPiece);
    Piece(PieceType inType, Color::Color inColor);
    Piece & operator=(const Piece & inPiece);

    PieceType getType() const;
    Color::Color getColor() const;
    int getPoints() const;

    std::string toString() const;

private:
    PieceType mType;
    Color::Color mColor;
};

#endif // PIECE_H