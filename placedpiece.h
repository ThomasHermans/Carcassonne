#ifndef PLACEDPIECE_H
#define PLACEDPIECE_H

#include "piece.h"
#include "tile.h"

#include <string>

class PlacedPiece
{
public:
    PlacedPiece();
    PlacedPiece(Piece inPiece, Tile::Area mLocation);

    Piece getPiece();
    Tile::Area getLocation();

    std::string toString();

private:
    Piece mPiece;
    Tile::Area mLocation;
};

#endif // PLACEDPIECE_H
