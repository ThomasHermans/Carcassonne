#ifndef PLACEDPIECE_H
#define PLACEDPIECE_H

#include "piece.h"
#include "tile.h"

#include <string>

class PlacedPiece
{
public:
    PlacedPiece();
    PlacedPiece(const Piece & inPiece, Area::Area mLocation);

    Piece getPiece() const;
    Area::Area getLocation() const;

    std::string toString() const;

private:
    Piece mPiece;
    Area::Area mLocation;
};

#endif // PLACEDPIECE_H
