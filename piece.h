#ifndef PIECE_H
#define PIECE_H

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
    Piece(PieceType inType);

    PieceType getType();

    int getPoints();

    std::string toString();

private:
    const PieceType mType;
};

#endif // PIECE_H
