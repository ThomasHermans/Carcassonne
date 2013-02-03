#include "Piece.h"

Piece::Piece():
    mType(Piece::Follower),
    mColor(Color::Red)
{
}

Piece::Piece(const Piece & inPiece):
    mType(inPiece.getType()),
    mColor(inPiece.getColor())
{
}

Piece::Piece(PieceType inType, Color::Color inColor):
    mType(inType),
    mColor(inColor)
{
}

Piece&
Piece::operator=(const Piece & rhs)
{
    if(this != &rhs)
    {
        this->mType = rhs.getType();
        this->mColor = rhs.getColor();
    }
    return *this;
}

Piece::PieceType
Piece::getType() const
{
    return mType;
}

Color::Color
Piece::getColor() const
{
    return mColor;
}

int
Piece::getPoints() const
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
Piece::toString() const
{
    std::string res = Color::colorToString(mColor);
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