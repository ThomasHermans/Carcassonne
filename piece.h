#ifndef PIECE_H
#define PIECE_H

#include <string>
#include "color.h"

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
    Piece(PieceType inType, Color inColor);

    PieceType getType();
    Color getColor();

    int getPoints();

    std::string toString();

private:
    const PieceType mType;
    const Color mColor;
};

#endif // PIECE_H
