#ifndef BOARD_H
#define BOARD_H

#include "tileonboard.h"
#include "boost/optional/optional.hpp"

class Board
{
public:
    Board();

    unsigned int getNrOfRows() const;
    unsigned int getNrOfCols() const;
    boost::optional< TileOnBoard > getTile(unsigned int inCol, unsigned int inRow) const;

    void addRowsOnTop(unsigned int inNrOfRows);
    void addRowsBelow(unsigned int inNrOfRows);
    void addColsLeft(unsigned int inNrOfCols);
    void addColsRight(unsigned int inNrOfCols);

    bool isEmptySpot(unsigned int inCol, unsigned int inRow) const;
    bool isValidTilePlacement(const TileOnBoard & inTile, unsigned int inCol, unsigned int inRow) const;
    bool isValidAlternateTilePlacement(const TileOnBoard & inTile, unsigned int inCol, unsigned int inRow) const;
    bool placeValidTile(const TileOnBoard & inTile, unsigned int inCol, unsigned int inRow);
    void rotateTileOnBoard(unsigned int inCol, unsigned int inRow);

private:
    bool placeTile(const TileOnBoard & inTile, unsigned int inCol, unsigned int inRow);
    void updateOccupiedRoads(unsigned int inCol, unsigned int inRow);
    void updateOccupiedRoadsCheck(int inNeighborLocation, FRCArea::RoadArea inNeighborSide, bool& ioOccupied,
                                  std::vector< std::pair< int, FRCArea::RoadArea > >& inQueue);
    void updateOccupiedCities(unsigned int inCol, unsigned int inRow);
    void updateOccupiedCitiesCheck(int inNeighborLocation, FRCArea::CityArea inNeighborSide, bool& ioOccupied,
                                  std::vector< std::pair< int, FRCArea::CityArea > >& inQueue);
    void updateOccupiedFields(unsigned int inCol, unsigned int inRow);
    void updateOccupiedFieldsCheck(int inNeighborLocation, FRCArea::FieldArea inNeighborSide, bool& ioOccupied,
                                   std::vector< std::pair< int, FRCArea::FieldArea > >& inQueue);

private:
    std::vector< boost::optional< TileOnBoard > > mBoard; // one vector of size cols * rows might be easier to use than nested vectors
    unsigned int mNrRows;
    unsigned int mNrCols;
};

#endif // BOARD_H
