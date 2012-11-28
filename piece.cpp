#include "piece.h"

Piece::Piece():
    mType(Piece::Follower)
{
}

Piece::Piece(PieceType inType):
    mType(inType)
{
}

Piece::PieceType
Piece::getType()
{
    return mType;
}

int
Piece::getPoints()
{
    switch (mType)
    {
    case Follower:
        return 1;
    case LargeFollower:
        return 2;
    case Builder:
    case Pig:
    default:
        return 0;
    }
}

std::string
Piece::toString()
{
    switch (mType)
    {
    case Follower:
        return "Follower";
    case LargeFollower:
        return "Large Follower";
    case Builder:
        return "Builder";
    case Pig:
        return "Pig";
    default:
        return "Not a valid Piece";
    }
}
