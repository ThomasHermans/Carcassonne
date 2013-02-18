#include "src-model/PlacedPiece.h"

namespace
{
bool isValid(Piece inPiece, Area::Area inArea)
{
    bool res = true;
    switch (inPiece.getType())
    {
    case Piece::Builder:
        if ((inArea == Area::TopLeft)
                || (inArea == Area::TopRight)
                || (inArea == Area::RightTop)
                || (inArea == Area::RightBottom)
                || (inArea == Area::BottomRight)
                || (inArea == Area::BottomLeft)
                || (inArea == Area::LeftBottom)
                || (inArea == Area::LeftTop)
                || (inArea == Area::Central))
        {
            res = false;
        }
        break;
    case Piece::Pig:
        if ((inArea == Area::Top)
                || (inArea == Area::Right)
                || (inArea == Area::Bottom)
                || (inArea == Area::Left))
        {
            res = false;
        }
        break;
    }
    return res;
}
}

PlacedPiece::PlacedPiece():
    mPiece( Piece() ),
    mLocation( (unsigned int)-1 ),
    mArea( Area::Invalid )
{
}

PlacedPiece::PlacedPiece( const Piece &inPiece ):
    mPiece( inPiece ),
    mLocation( (unsigned int)-1 ),
    mArea( Area::Invalid )
{
}

Piece
PlacedPiece::getPiece() const
{
    return mPiece;
}

unsigned int
PlacedPiece::getLocation() const
{
    return mLocation;
}

Area::Area
PlacedPiece::getArea() const
{
    return mArea;
}

bool
PlacedPiece::placePiece( unsigned int inLocation, Area::Area inArea )
{
    if ( inLocation != (unsigned int)-1 )
    {
        // Already placed, you can just move Pieces
        return false;
    }
    if ( isValid( getPiece(), inArea ) )
    {
        mLocation = inLocation;
        mArea = inArea;
        return true;
    }
    else
    {
        return false;
    }
}

bool
PlacedPiece::unplacePiece()
{
    mLocation = (unsigned int)-1;
    mArea = Area::Invalid;
    return true;
}

std::string
PlacedPiece::toString() const
{
    std::string res = mPiece.toString();
    res.append(" on ");
    res.append(Area::areaToString(mArea));
    return res;
}
