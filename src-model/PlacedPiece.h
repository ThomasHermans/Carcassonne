#ifndef PLACEDPIECE_H
#define PLACEDPIECE_H

#include "src-model/Piece.h"
#include "src-model/Tile.h"

#include <string>

class PlacedPiece
{
public:
    PlacedPiece();
    PlacedPiece(const Piece & inPiece);

    Piece getPiece() const;
    unsigned int getLocation() const;
    Area::Area getArea() const;

    bool placePiece( unsigned int inLocation, Area::Area inArea );
    bool unplacePiece();

    std::string toString() const;

private:
    Piece mPiece;
    unsigned int mLocation;
    Area::Area mArea;
};

#endif // PLACEDPIECE_H
