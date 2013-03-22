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
    mCol( (unsigned int)-1 ),
    mRow( (unsigned int)-1 ),
    mArea( Area::Invalid )
{
}

PlacedPiece::PlacedPiece( const Piece & inPiece, unsigned int inCol, unsigned int inRow, Area::Area inArea ):
    mPiece( inPiece ),
    mCol( inCol ),
    mRow( inRow ),
    mArea( Area::Invalid )
{
    if ( isValid( inPiece, inArea ) )
    {
        mArea = inArea;
    }
}

Piece
PlacedPiece::getPiece() const
{
    return mPiece;
}

unsigned int
PlacedPiece::getCol() const
{
    return mCol;
}

unsigned int
PlacedPiece::getRow() const
{
    return mRow;
}

Area::Area
PlacedPiece::getArea() const
{
    return mArea;
}

std::string
PlacedPiece::toString() const
{
    std::string res = mPiece.toString();
    res.append(" on ");
    res.append(Area::areaToString(mArea));
    return res;
}
