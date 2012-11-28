#include "piece.h"

Piece::Piece():
    mType(Piece::Follower),
    mColor(Red)
{
}

Piece::Piece(PieceType inType, Color inColor):
    mType(inType),
    mColor(inColor)
{
}

Piece::PieceType
Piece::getType()
{
    return mType;
}

Color
Piece::getColor()
{
    return mColor;
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
    std::string res = colorToString(mColor);
    res.append(" ");
    switch (mType)
    {
    case Follower:
        res.append("Follower");
    case LargeFollower:
        res.append("Large Follower");
    case Builder:
        res.append("Builder");
    case Pig:
        res.append("Pig");
    default:
        res.append("Not a valid Piece");
    }
    return res;
}
