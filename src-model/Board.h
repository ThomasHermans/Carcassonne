#ifndef BOARD_H
#define BOARD_H

#include "src-model/Tile.h"
#include "src-model/TileOnBoard.h"

#include <QObject>

#include <boost/optional/optional.hpp>

class Board : public QObject
{
    Q_OBJECT

    typedef std::pair< unsigned int, FRCArea::CityArea > LocatedCity;
    typedef std::pair< unsigned int, FRCArea::RoadArea > LocatedRoad;
    typedef std::pair< unsigned int, FRCArea::FieldArea > LocatedField;
    
public:
    Board(unsigned int inSize = 5);

    unsigned int getNrOfRows() const;
    unsigned int getNrOfCols() const;
    boost::optional< TileOnBoard > getTile(unsigned int inCol, unsigned int inRow) const;

    void addRowsOnTop(unsigned int inNrOfRows);
    void addRowsBelow(unsigned int inNrOfRows);
    void addColsLeft(unsigned int inNrOfCols);
    void addColsRight(unsigned int inNrOfCols);

    bool isPossibleTile(Tile inTile);
    bool isEmptySpot(unsigned int inCol, unsigned int inRow) const;
    bool isValidTilePlacement(const TileOnBoard & inTile, unsigned int inCol, unsigned int inRow) const;
    bool isValidAlternateTilePlacement(const TileOnBoard & inTile, unsigned int inCol, unsigned int inRow) const;
    bool placeValidTile(const TileOnBoard & inTile, unsigned int inCol, unsigned int inRow);
    unsigned int placeStartTile(const TileOnBoard & inTile);
    void rotateTileOnBoard(unsigned int inCol, unsigned int inRow);

    boost::optional< TileOnBoard > removeTile(unsigned int inCol, unsigned int inRow);

    void checkForFinishedCloisters(unsigned int inCol, unsigned int inRow);
    bool isFinishedCloister(unsigned int inCol, unsigned int inRow) const;
    bool isFullySurrounded(unsigned int inCol, unsigned int inRow) const;

    void checkForFinishedCities(unsigned int inCol, unsigned int inRow);

    std::string toString() const;
    std::string shortPrint(unsigned int inCol, unsigned int inRow) const;

signals:
    void tileRotated(unsigned int inCol, unsigned int inRow, std::string inId, TileOnBoard::Rotation inRot);
    void finishedCloister(unsigned int inCol, unsigned int inRow );
    void finishedCity(std::vector< std::pair< unsigned int, unsigned int > > inTiles);

private:
    bool placeTile(const TileOnBoard & inTile, unsigned int inCol, unsigned int inRow);

    bool isContinueued( LocatedCity inLocatedCity ) const;
    unsigned int getNeighborLocation( LocatedCity inLocatedCity ) const;

    void updateOccupiedRoads(unsigned int inCol, unsigned int inRow);
    void updateOccupiedRoadsCheck(unsigned int inNeighborLocation, FRCArea::RoadArea inNeighborSide, bool& ioOccupied,
                                  std::vector< std::pair< unsigned int, FRCArea::RoadArea > >& inQueue);
    void updateOccupiedCities(unsigned int inCol, unsigned int inRow);
    void updateOccupiedCitiesCheck(unsigned int inNeighborLocation, FRCArea::CityArea inNeighborSide, bool& ioOccupied,
                                  std::vector< std::pair< unsigned int, FRCArea::CityArea > >& inQueue);
    void updateOccupiedFields(unsigned int inCol, unsigned int inRow);
    void updateOccupiedFieldsCheck(unsigned int inNeighborLocation, FRCArea::FieldArea inNeighborSide, bool& ioOccupied,
                                   std::vector< std::pair< unsigned int, FRCArea::FieldArea > >& inQueue);

private:
    unsigned int mNrRows;
    unsigned int mNrCols;
    std::vector< boost::optional< TileOnBoard > > mBoard; // one vector of size cols * rows might be easier to use than nested vectors
};

#endif // BOARD_H
