#ifndef PLACEDPIECE_H
#define PLACEDPIECE_H

#include "piece.h"
#include "tile.h"

#include <string>

class PlacedPiece
{
public:
    PlacedPiece();
    PlacedPiece(Piece inPiece, Area::Area mLocation);

    Piece getPiece();
    Area::Area getLocation();

    std::string toString();

private:
    Piece mPiece;
    Area::Area mLocation;
};

#endif // PLACEDPIECE_H
