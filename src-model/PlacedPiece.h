#ifndef PLACEDPIECE_H
#define PLACEDPIECE_H

#include "src-model/Piece.h"
#include "src-model/Tile.h"

#include <string>

class PlacedPiece
{
public:
    PlacedPiece( const Piece & inPiece, int inRelCol, int inRelRow, Area::Area inArea );

    Piece getPiece() const;
    int getCol() const;
    int getRow() const;
    Area::Area getArea() const;

    std::string toString() const;

private:
    Piece mPiece;
    int mRelCol;
    int mRelRow;
    Area::Area mArea;
};

#endif // PLACEDPIECE_H
