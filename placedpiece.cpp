#include "placedpiece.h"

namespace
{
bool isValid(Piece inPiece, Area::Area inLocation)
{
    bool res = true;
    switch (inPiece.getType())
    {
    case Piece::Builder:
        if ((inLocation == Area::TopLeft)
                || (inLocation == Area::TopRight)
                || (inLocation == Area::RightTop)
                || (inLocation == Area::RightBottom)
                || (inLocation == Area::BottomRight)
                || (inLocation == Area::BottomLeft)
                || (inLocation == Area::LeftBottom)
                || (inLocation == Area::LeftTop)
                || (inLocation == Area::Central))
        {
            res = false;
        }
        break;
    case Piece::Pig:
        if ((inLocation == Area::Top)
                || (inLocation == Area::Right)
                || (inLocation == Area::Bottom)
                || (inLocation == Area::Left))
        {
            res = false;
        }
        break;
    }
    return res;
}
}

PlacedPiece::PlacedPiece():
    mPiece(Piece()),
    mLocation(Area::Central)
{
}

PlacedPiece::PlacedPiece(Piece inPiece, Area::Area inLocation):
    mPiece(inPiece)
{
    if (isValid(inPiece, inLocation))
    {
        mLocation = inLocation;
    }
    else
    {
        switch (inPiece.getType())
        {
        case Piece::Follower:
        case Piece::LargeFollower:
        case Piece::Builder:
            mLocation = Area::Top;
            break;
        case Piece::Pig:
            mLocation = Area::TopLeft;
            break;
        default:
            mLocation = Area::Top;
        }
    }
}

Piece
PlacedPiece::getPiece()
{
    return mPiece;
}

Area::Area
PlacedPiece::getLocation()
{
    return mLocation;
}

std::string
PlacedPiece::toString()
{
    std::string res = mPiece.toString();
    res.append(" on ");
    res.append(Area::areaToString(mLocation));
    return res;
}
