#include "placedpiece.h"

PlacedPiece::PlacedPiece():
    mPiece(Piece()),
    mLocation(Tile::AreaCentral)
{
}

PlacedPiece::PlacedPiece(Piece inPiece, Tile::Area inLocation):
    mPiece(inPiece),
    mLocation(inLocation)
{
}

Piece
PlacedPiece::getPiece()
{
    return mPiece;
}

Tile::Area
PlacedPiece::getLocation()
{
    return mLocation;
}

std::string
PlacedPiece::toString()
{
    std::string res = mPiece.toString();
    res.append(" on ");
    res.append(Tile::areaToString(mLocation));
    return res;
}
