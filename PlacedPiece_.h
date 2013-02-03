#ifndef PLACEDPIECE_H
#define PLACEDPIECE_H

#include "Piece_.h"
#include "Tile_.h"

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
