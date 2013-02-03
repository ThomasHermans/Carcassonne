#include "Board_.h"

#include <sstream>

Board::Board(unsigned int inSize)
{
    mNrCols = (inSize % 2 == 1) ? inSize : inSize + 1;
    mNrRows = mNrCols;
    mBoard = std::vector< boost::optional< TileOnBoard > >(mNrCols * mNrRows, boost::optional< TileOnBoard >());
    mStartCol = 0;
    mStartRow = 0;
}

unsigned int Board::getNrOfRows() const
{
    return mNrRows;
}

unsigned int
Board::getNrOfCols() const
{
    return mNrCols;
}

unsigned int
Board::getStartRow() const
{
    return mStartRow;
}

unsigned int
Board::getStartCol() const
{
    return mStartCol;
}

boost::optional< TileOnBoard >
Board::getTile(unsigned int inCol, unsigned int inRow) const
{
    return mBoard[inRow * mNrCols + inCol];
}

void
Board::addRowsOnTop(unsigned int inNrOfRows)
{
    mBoard.insert(mBoard.begin(), mNrCols * inNrOfRows, boost::optional< TileOnBoard >());
    mNrRows += inNrOfRows;
    mStartRow += inNrOfRows;
}

void
Board::addRowsBelow(unsigned int inNrOfRows)
{
    mBoard.insert(mBoard.end(), mNrCols * inNrOfRows, boost::optional< TileOnBoard >());
    mNrRows += inNrOfRows;
}

void
Board::addColsLeft(unsigned int inNrOfCols)
{
    std::vector< boost::optional< TileOnBoard > >::iterator it = mBoard.begin();
    for(unsigned int i = 0; i < mNrRows; ++i)
    {
        for (unsigned int j = 0; j < inNrOfCols; ++j)
        {
            it = mBoard.insert(it, boost::optional< TileOnBoard >());
            ++it;
        }
        for (unsigned int j = 0; j < mNrCols; ++j)
        {
            ++it;
        }
    }
    mNrCols += inNrOfCols;
    mStartCol += inNrOfCols;
}

void
Board::addColsRight(unsigned int inNrOfCols)
{
    std::vector< boost::optional< TileOnBoard > >::iterator it = mBoard.begin();
    for(unsigned int i = 0; i < mNrRows; ++i)
    {
        for (unsigned int j = 0; j < mNrCols; ++j)
        {
            ++it;
        }
        for (unsigned int j = 0; j < inNrOfCols; ++j)
        {
            it = mBoard.insert(it, boost::optional< TileOnBoard >());
            ++it;
        }
    }
    mNrCols += inNrOfCols;
}

bool
Board::isPossibleTile(Tile inTile)
{
    bool possible = false;
    for (unsigned int row = 0; row < mNrRows; ++row)
    {
        for(unsigned int col = 0; col < mNrCols; ++col)
        {
            for (unsigned int rot = 0; rot < 4; ++rot)
            {
                TileOnBoard tileOnBoard(inTile, TileOnBoard::Rotation(rot * 3));
                if (isValidTilePlacement(tileOnBoard, col, row))
                {
                    possible = true;
                    break;
                }
            }
        }
    }
    return possible;
}

bool
Board::isEmptySpot(unsigned int inCol, unsigned int inRow) const
{
    bool valid = true;
    if (inCol < 0 || inCol >= mNrCols || inRow < 0 || inRow >= mNrRows)
    {
        valid = false;
    }
    // Check if no tile placed there yet
    if (mBoard[inRow * mNrCols + inCol])
    {
        valid = false;
    }
    return valid;
}

bool
Board::isValidTilePlacement(const TileOnBoard &inTile, unsigned int inCol, unsigned int inRow) const
{
    bool valid = isEmptySpot(inCol, inRow);
    if (valid)
    {
        valid = isValidAlternateTilePlacement(inTile, inCol, inRow);
    }
    return valid;
}

bool
Board::isValidAlternateTilePlacement(const TileOnBoard &inTile, unsigned int inCol, unsigned int inRow) const
{
    bool valid = true;
    // Check if one of four neighbors is placed
    int nrOfNeighbors = 0;
    if ((inRow > 0) && mBoard[(inRow - 1) * mNrCols + inCol])
    {
        ++nrOfNeighbors;
    }
    if ((inRow + 1 < mNrCols) && mBoard[(inRow + 1) * mNrCols + inCol])
    {
        ++nrOfNeighbors;
    }
    if ((inCol > 0) && mBoard[inRow * mNrCols + inCol - 1])
    {
        ++nrOfNeighbors;
    }
    if ((inCol + 1 < mNrCols) && mBoard[inRow * mNrCols + inCol + 1])
    {
        ++nrOfNeighbors;
    }
    if (nrOfNeighbors == 0)
    {
        valid = false;
    }
    // Check if sides match with four neighbors
    if ((inRow > 0)
            && mBoard[(inRow - 1) * mNrCols + inCol]
            && !inTile.matchesBelow(*mBoard[(inRow - 1) * mNrCols + inCol]))
    {
        valid = false;
    }
    if ((inRow + 1 < mNrCols)
            && mBoard[(inRow + 1) * mNrCols + inCol]
            && !inTile.matchesAbove(*mBoard[(inRow + 1) * mNrCols + inCol]))
    {
        valid = false;
    }
    if ((inCol > 0)
            && mBoard[inRow * mNrCols + inCol - 1]
            && !inTile.matchesRightOf(*mBoard[inRow * mNrCols + inCol - 1]))
    {
        valid = false;
    }
    if ((inCol + 1 < mNrCols)
            && mBoard[inRow * mNrCols + inCol + 1]
            && !inTile.matchesLeftOf(*mBoard[inRow * mNrCols + inCol + 1]))
    {
        valid = false;
    }
    return valid;
}

bool
Board::placeValidTile(const TileOnBoard &inTile, unsigned int inCol, unsigned int inRow)
{
    if (isValidTilePlacement(inTile, inCol, inRow))
    {
        placeTile(inTile, inCol, inRow);
        updateOccupiedRoads(inCol, inRow);
        updateOccupiedCities(inCol, inRow);
        updateOccupiedFields(inCol, inRow);
        return true;
    }
    else
    {
        return false;
    }
}

unsigned int
Board::placeStartTile(const TileOnBoard &inTile)
{
    // FIXME: should check if really placing start tile (in other words, check is board is empty)
    unsigned int col = mNrCols / 2;
    unsigned int row = mNrRows / 2;
    placeTile(inTile, col, row);
    mStartCol = col;
    mStartRow = row;
    return row * mNrCols + col;
}

void
Board::rotateTileOnBoard(unsigned int inCol, unsigned int inRow)
{
    boost::optional< TileOnBoard > toBeRotatedTile = mBoard[inRow * mNrCols + inCol];
    if (toBeRotatedTile)
    {
        TileOnBoard::Rotation currentRotation = toBeRotatedTile->getRotation();
        Tile tile = toBeRotatedTile->getTile();
        TileOnBoard::Rotation newRotation = currentRotation;
        TileOnBoard rotated = TileOnBoard(tile, newRotation);
        for (int i = 0; i < 4; ++i)
        {
            newRotation = TileOnBoard::Rotation((newRotation + TileOnBoard::cw90) % (TileOnBoard::cw90 * 4));
            rotated = TileOnBoard(tile, newRotation);
            if (isValidAlternateTilePlacement(rotated, inCol, inRow))
            {
                break;
            }
        }
        if (placeTile(rotated, inCol, inRow))
        {
            std::cout << "Board sees a rotation, " << rotated.getRotation() * 30 << std::endl;
            emit tileRotated(inCol, inRow, rotated.getID(), rotated.getRotation());
        }
    }
}

std::string
Board::toString() const
{
    std::stringstream res;
    for (unsigned int r = 0; r < mNrRows; ++r)
    {
        for (unsigned int c = 0; c < mNrCols; ++c)
        {
            res << shortPrint(c, r) << "\t";
        }
        res << std::endl;
    }
    return res.str();
}

std::string
Board::shortPrint(unsigned int inCol, unsigned int inRow) const
{
    std::stringstream res;
    if (mBoard[inRow * mNrCols + inCol])
    {
        res << mBoard[inRow * mNrCols + inCol]->getID() << (mBoard[inRow * mNrCols + inCol]->getRotation() * 30);
    }
    else
    {
        res << "/";
    }
    return res.str();
}

bool
Board::placeTile(const TileOnBoard &inTile, unsigned int inCol, unsigned int inRow)
{
    bool placed = false;
    if ((inCol >= 0) && (inCol < mNrCols) && (inRow >= 0) && (inRow < mNrRows))
    {
        mBoard[inRow * mNrCols + inCol] = boost::optional< TileOnBoard >( inTile );
        placed = true;
    }
    return placed;
}

void
Board::updateOccupiedRoads(unsigned int inCol, unsigned int inRow)
{
    typedef std::pair< unsigned int, FRCArea::RoadArea > LocatedRoad;
    // the int gives the location of the TileOnBoard (=row * mNrCols + col)
    std::vector< Tile::ContiguousRoad > toBeUpdatedRoads = mBoard[inRow * mNrCols + inCol]->getContiguousRoads();
    for (unsigned int road = 0; road < toBeUpdatedRoads.size(); ++road)
    {
        std::vector< LocatedRoad > tempQueue;
        bool occupied = false;
        std::vector< FRCArea::RoadArea > roadsOfThisTile = toBeUpdatedRoads[road];
        unsigned int i = 0;
        for (; i < roadsOfThisTile.size(); ++i)
        {
            tempQueue.push_back(LocatedRoad(inRow * mNrCols + inCol, roadsOfThisTile[i]));
        }
        i = 0;
        while (i < tempQueue.size())
        {
            // Pick RoadArea i from tempQueue
            LocatedRoad current = tempQueue[i];
            unsigned int neighborLocation = 0;
            switch (current.second)
            {
            case FRCArea::Top:
                neighborLocation = current.first - mNrCols;
                if ((current.first >= mNrCols) && (mBoard[neighborLocation]))
                {
                    FRCArea::RoadArea neighborSide = FRCArea::Bottom;
                    updateOccupiedRoadsCheck(neighborLocation, neighborSide, occupied, tempQueue);
                }
                break;
            case FRCArea::Right:
                neighborLocation = current.first + 1;
                if ((current.first % mNrRows < mNrCols - 1) && (mBoard[neighborLocation]))
                {
                    FRCArea::RoadArea neighborSide = FRCArea::Left;
                    updateOccupiedRoadsCheck(neighborLocation, neighborSide, occupied, tempQueue);
                }
                break;
            case FRCArea::Bottom:
                neighborLocation = current.first + mNrCols;
                if ((neighborLocation < mNrRows * mNrCols) && (mBoard[neighborLocation]))
                {
                    FRCArea::RoadArea neighborSide = FRCArea::Top;
                    updateOccupiedRoadsCheck(neighborLocation, neighborSide, occupied, tempQueue);
                }
                break;
            case FRCArea::Left:
                neighborLocation = current.first - 1;
                if ((current.first % mNrRows > 0) && (mBoard[neighborLocation]))
                {
                    FRCArea::RoadArea neighborSide = FRCArea::Right;
                    updateOccupiedRoadsCheck(neighborLocation, neighborSide, occupied, tempQueue);
                }
                break;
            default:
                // Error, cannot happen
                break;
            }
            ++i;
        }
        // If occupied == true:
        // Mark all RoadAreas in tempQueue as occupied
        if (occupied)
        {
            for (i = 0; i < tempQueue.size(); ++i)
            {
                mBoard[tempQueue[i].first]->occupyRoad(tempQueue[i].second);
            }
        }
    }
}

void
Board::updateOccupiedRoadsCheck(unsigned int inNeighborLocation, FRCArea::RoadArea inNeighborSide, bool &ioOccupied,
                                std::vector< std::pair< unsigned int, FRCArea::RoadArea > > &inQueue)
{
    typedef std::pair< unsigned int, FRCArea::RoadArea > LocatedRoad;
    // If contination RoadArea is occupied: occupied = true, break
    if (mBoard[inNeighborLocation]->isRoadOccupied(inNeighborSide))
    {
        ioOccupied = true;
    }
    // Else, continuation RoadArea is unoccupied:
    // If tempQueue already contains continuation RoadArea: break
    // Else, tempQueue does not contain continuation RoadArea:
    else if (std::find(inQueue.begin(), inQueue.end(), LocatedRoad(inNeighborLocation, inNeighborSide)) == inQueue.end())
    {
        // Push back continuation RoadArea and all of its contiguous RoadAreas on tempQueue
        Tile::ContiguousRoad contRoad = mBoard[inNeighborLocation]->getContiguousRoad(inNeighborSide);
        for (unsigned int j = 0; j < contRoad.size(); ++j)
        {
            inQueue.push_back(LocatedRoad(inNeighborLocation, contRoad[j]));
        }
    }
}

void
Board::updateOccupiedCities(unsigned int inCol, unsigned int inRow)
{
    typedef std::pair< unsigned int, FRCArea::CityArea > LocatedCity;
    // the int gives the location of the TileOnBoard (=row * mNrCols + col)
    std::vector< Tile::ContiguousCity > toBeUpdatedCities = mBoard[inRow * mNrCols + inCol]->getContiguousCities();
    for (unsigned int city = 0; city < toBeUpdatedCities.size(); ++city)
    {
        std::vector< LocatedCity > tempQueue;
        bool occupied = false;
        std::vector< FRCArea::CityArea > citiesOfThisTile = toBeUpdatedCities[city];
        unsigned int i = 0;
        for (; i < citiesOfThisTile.size(); ++i)
        {
            tempQueue.push_back(LocatedCity(inRow * mNrCols + inCol, citiesOfThisTile[i]));
        }
        i = 0;
        while (i < tempQueue.size())
        {
            // Pick CityArea i from tempQueue
            LocatedCity current = tempQueue[i];
            unsigned int neighborLocation = 0;
            switch (current.second)
            {
            case FRCArea::Top:
                neighborLocation = current.first - mNrCols;
                if ((current.first >= mNrCols) && (mBoard[neighborLocation]))
                {
                    FRCArea::CityArea neighborSide = FRCArea::Bottom;
                    updateOccupiedCitiesCheck(neighborLocation, neighborSide, occupied, tempQueue);
                }
                break;
            case FRCArea::Right:
                neighborLocation = current.first + 1;
                if ((current.first % mNrRows < mNrCols - 1) && (mBoard[neighborLocation]))
                {
                    FRCArea::CityArea neighborSide = FRCArea::Left;
                    updateOccupiedCitiesCheck(neighborLocation, neighborSide, occupied, tempQueue);
                }
                break;
            case FRCArea::Bottom:
                neighborLocation = current.first + mNrCols;
                if ((neighborLocation < mNrRows * mNrCols) && (mBoard[neighborLocation]))
                {
                    FRCArea::CityArea neighborSide = FRCArea::Top;
                    updateOccupiedCitiesCheck(neighborLocation, neighborSide, occupied, tempQueue);
                }
                break;
            case FRCArea::Left:
                neighborLocation = current.first - 1;
                if ((current.first % mNrRows > 0) && (mBoard[neighborLocation]))
                {
                    FRCArea::CityArea neighborSide = FRCArea::Right;
                    updateOccupiedCitiesCheck(neighborLocation, neighborSide, occupied, tempQueue);
                }
                break;
            default:
                // Error, cannot happen
                break;
            }
            ++i;
        }
        // If occupied == true:
        // Mark all CityAreas in tempQueue as occupied
        if (occupied)
        {
            for (i = 0; i < tempQueue.size(); ++i)
            {
                mBoard[tempQueue[i].first]->occupyCity(tempQueue[i].second);
            }
        }
    }
}

void
Board::updateOccupiedCitiesCheck(unsigned  int inNeighborLocation, FRCArea::CityArea inNeighborSide, bool &ioOccupied,
                                std::vector< std::pair< unsigned int, FRCArea::CityArea > > &inQueue)
{
    typedef std::pair< unsigned int, FRCArea::CityArea > LocatedCity;
    // If contination CityArea is occupied: occupied = true, break
    if (mBoard[inNeighborLocation]->isCityOccupied(inNeighborSide))
    {
        ioOccupied = true;
    }
    // Else, continuation CityArea is unoccupied:
    // If tempQueue already contains continuation CityArea: break
    // Else, tempQueue does not contain continuation CityArea:
    else if (std::find(inQueue.begin(), inQueue.end(), LocatedCity(inNeighborLocation, inNeighborSide)) == inQueue.end())
    {
        // Push back continuation CityArea and all of its contiguous CityAreas on tempQueue
        Tile::ContiguousCity contCity = mBoard[inNeighborLocation]->getContiguousCity(inNeighborSide);
        for (unsigned int j = 0; j < contCity.size(); ++j)
        {
            inQueue.push_back(LocatedCity(inNeighborLocation, contCity[j]));
        }
    }
}

void
Board::updateOccupiedFields(unsigned int inCol, unsigned int inRow)
{
    typedef std::pair< unsigned int, FRCArea::FieldArea > LocatedField;
    // the int gives the location of the TileOnBoard (=row * mNrCols + col)
    std::vector< Tile::ContiguousField > toBeUpdatedFields = mBoard[inRow * mNrCols + inCol]->getContiguousFields();
    for (unsigned int field = 0; field < toBeUpdatedFields.size(); ++field)
    {
        std::vector< LocatedField > tempQueue;
        bool occupied = false;
        std::vector< FRCArea::FieldArea > fieldsOfThisTile = toBeUpdatedFields[field];
        unsigned int i = 0;
        for (; i < fieldsOfThisTile.size(); ++i)
        {
            tempQueue.push_back(LocatedField(inRow * mNrCols + inCol, fieldsOfThisTile[i]));
        }
        i = 0;
        while (i < tempQueue.size())
        {
            // Pick FieldArea i from tempQueue
            LocatedField current = tempQueue[i];
            unsigned int neighborLocation = 0;
            switch (current.second)
            {
            case FRCArea::TopLeft:
                neighborLocation = current.first - mNrCols;
                if ((current.first >= mNrCols) && (mBoard[neighborLocation]))
                {
                    FRCArea::FieldArea neighborSide = FRCArea::BottomLeft;
                    updateOccupiedFieldsCheck(neighborLocation, neighborSide, occupied, tempQueue);
                }
                break;
            case FRCArea::TopRight:
                neighborLocation = current.first - mNrCols;
                if ((current.first >= mNrCols) && (mBoard[neighborLocation]))
                {
                    FRCArea::FieldArea neighborSide = FRCArea::BottomRight;
                    updateOccupiedFieldsCheck(neighborLocation, neighborSide, occupied, tempQueue);
                }
                break;
            case FRCArea::RightTop:
                neighborLocation = current.first + 1;
                if ((current.first % mNrRows < mNrCols - 1) && (mBoard[neighborLocation]))
                {
                    FRCArea::FieldArea neighborSide = FRCArea::LeftTop;
                    updateOccupiedFieldsCheck(neighborLocation, neighborSide, occupied, tempQueue);
                }
                break;
            case FRCArea::RightBottom:
                neighborLocation = current.first + 1;
                if ((current.first % mNrRows < mNrCols - 1) && (mBoard[neighborLocation]))
                {
                    FRCArea::FieldArea neighborSide = FRCArea::LeftBottom;
                    updateOccupiedFieldsCheck(neighborLocation, neighborSide, occupied, tempQueue);
                }
                break;
            case FRCArea::BottomRight:
                neighborLocation = current.first + mNrCols;
                if ((neighborLocation < mNrRows * mNrCols) && (mBoard[neighborLocation]))
                {
                    FRCArea::FieldArea neighborSide = FRCArea::TopRight;
                    updateOccupiedFieldsCheck(neighborLocation, neighborSide, occupied, tempQueue);
                }
                break;
            case FRCArea::BottomLeft:
                neighborLocation = current.first + mNrCols;
                if ((neighborLocation < mNrRows * mNrCols) && (mBoard[neighborLocation]))
                {
                    FRCArea::FieldArea neighborSide = FRCArea::TopLeft;
                    updateOccupiedFieldsCheck(neighborLocation, neighborSide, occupied, tempQueue);
                }
                break;
            case FRCArea::LeftBottom:
                neighborLocation = current.first - 1;
                if ((current.first % mNrRows > 0) && (mBoard[neighborLocation]))
                {
                    FRCArea::FieldArea neighborSide = FRCArea::RightBottom;
                    updateOccupiedFieldsCheck(neighborLocation, neighborSide, occupied, tempQueue);
                }
                break;
            case FRCArea::LeftTop:
                neighborLocation = current.first - 1;
                if ((current.first % mNrRows > 0) && (mBoard[neighborLocation]))
                {
                    FRCArea::FieldArea neighborSide = FRCArea::RightTop;
                    updateOccupiedFieldsCheck(neighborLocation, neighborSide, occupied, tempQueue);
                }
                break;
            default:
                // Error, cannot happen
                break;
            }
            ++i;
        }
        // If occupied == true:
        // Mark all FieldAreas in tempQueue as occupied
        if (occupied)
        {
            for (i = 0; i < tempQueue.size(); ++i)
            {
                mBoard[tempQueue[i].first]->occupyField(tempQueue[i].second);
            }
        }
    }
}

void
Board::updateOccupiedFieldsCheck(unsigned int inNeighborLocation, FRCArea::FieldArea inNeighborSide, bool &ioOccupied,
                                std::vector< std::pair< unsigned int, FRCArea::FieldArea > > &inQueue)
{
    typedef std::pair< unsigned int, FRCArea::FieldArea > LocatedField;
    // If contination FieldArea is occupied: occupied = true, break
    if (mBoard[inNeighborLocation]->isFieldOccupied(inNeighborSide))
    {
        ioOccupied = true;
    }
    // Else, continuation FieldArea is unoccupied:
    // If tempQueue already contains continuation FieldArea: break
    // Else, tempQueue does not contain continuation FieldArea:
    else if (std::find(inQueue.begin(), inQueue.end(), LocatedField(inNeighborLocation, inNeighborSide)) == inQueue.end())
    {
        // Push back continuation FieldArea and all of its contiguous FieldAreas on tempQueue
        Tile::ContiguousField contField = mBoard[inNeighborLocation]->getContiguousField(inNeighborSide);
        for (unsigned int j = 0; j < contField.size(); ++j)
        {
            inQueue.push_back(LocatedField(inNeighborLocation, contField[j]));
        }
    }
}
