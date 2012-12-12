#include "tileonboard.h"

#include <algorithm>

namespace
{
bool isValid(TileOnBoard inTile, PlacedPiece inPlacedPiece)
{
    Area::Area location = inPlacedPiece.getLocation();
    // TODO: check whether inTile's fields, roads or cities contain location
    // if one of the three vectors contains location, return true
    // else return false
    return true;
}

FieldArea::FieldArea turn(FieldArea::FieldArea inFieldArea, TileOnBoard::Rotation inRotation)
{
    if (inFieldArea == FieldArea::Central)
        return FieldArea::Central;
    else
        return FieldArea::FieldArea((inFieldArea + inRotation) % 12);
}

RoadOrCityArea::RoadOrCityArea turn(RoadOrCityArea::RoadOrCityArea inRoadOrCityArea, TileOnBoard::Rotation inRotation)
{
    return RoadOrCityArea::RoadOrCityArea((inRoadOrCityArea + inRotation) % 12);
}
}

TileOnBoard::TileOnBoard():
    mTile(Tile::Tile()),
    mRotation(TileOnBoard::cw0)
{
    mOccupiedFields = std::vector< Tile::ContiguousField >();
    mOccupiedRoads = std::vector< Tile::ContiguousRoadOrCity >();
    mOccupiedCities = std::vector< Tile::ContiguousRoadOrCity >();
    mPlacedPieces = std::vector< PlacedPiece >();
}

TileOnBoard::TileOnBoard(Tile inTile, Rotation inRotation):
    mTile(inTile),
    mRotation(inRotation)
{
    mOccupiedFields = std::vector< Tile::ContiguousField >();
    mOccupiedRoads = std::vector< Tile::ContiguousRoadOrCity >();
    mOccupiedCities = std::vector< Tile::ContiguousRoadOrCity >();
    mPlacedPieces = std::vector< PlacedPiece >();
}

bool
TileOnBoard::placePiece(PlacedPiece inPlacedPiece)
{
    // TODO: check isValid(*this, inPlacedPiece) (from anonymous namespace above)
    // if valid, add inPlacedPiece to mPlacedPieces, return true
    // else return false
    return true;
}

Tile::Side
TileOnBoard::getTop()
{
    switch (mRotation)
    {
    case cw90:
        return mTile.getLeft();
    case cw180:
        return mTile.getBottom();
    case cw270:
        return mTile.getRight();
    case cw0:
    default:
        return mTile.getTop();
    }
}

Tile::Side
TileOnBoard::getRight()
{
    switch (mRotation)
    {
    case cw90:
        return mTile.getTop();
    case cw180:
        return mTile.getLeft();
    case cw270:
        return mTile.getBottom();
    case cw0:
    default:
        return mTile.getRight();
    }
}

Tile::Side
TileOnBoard::getBottom()
{
    switch (mRotation)
    {
    case cw90:
        return mTile.getRight();
    case cw180:
        return mTile.getTop();
    case cw270:
        return mTile.getLeft();
    case cw0:
    default:
        return mTile.getBottom();
    }
}

Tile::Side
TileOnBoard::getLeft()
{
    switch (mRotation)
    {
    case cw90:
        return mTile.getBottom();
    case cw180:
        return mTile.getRight();
    case cw270:
        return mTile.getRight();
    case cw0:
    default:
        return mTile.getLeft();
    }
}

Tile::Center
TileOnBoard::getCenter()
{
    return mTile.getCenter();
}

std::string
TileOnBoard::getID()
{
    return mTile.getID();
}

bool
TileOnBoard::matchesAbove(TileOnBoard inTileOnBoard)
{
    return (this->getBottom() == inTileOnBoard.getTop());
}

bool
TileOnBoard::matchesRightOf(TileOnBoard inTileOnBoard)
{
    return (this->getLeft() == inTileOnBoard.getRight());
}

bool
TileOnBoard::matchesBelow(TileOnBoard inTileOnBoard)
{
    return (this->getTop() == inTileOnBoard.getBottom());
}

bool
TileOnBoard::matchesLeftOf(TileOnBoard inTileOnBoard)
{
    return (this->getRight() == inTileOnBoard.getLeft());
}

std::vector< Tile::ContiguousField >
TileOnBoard::getContiguousFields()
{
    std::vector< Tile::ContiguousField > res;
    std::vector< Tile::ContiguousField > orig = mTile.getContiguousFields();
    for (unsigned int i = 0; i < orig.size(); ++i)
    {
        Tile::ContiguousField restemp;
        Tile::ContiguousField origtemp = orig[i];
        for (unsigned int j = 0; j < origtemp.size(); ++j)
        {
            restemp.push_back(turn(origtemp[j], mRotation));
        }
        res.push_back(restemp);
    }
    return res;
}

std::vector< Tile::ContiguousRoadOrCity >
TileOnBoard::getContiguousRoads()
{
    std::vector< Tile::ContiguousRoadOrCity > res;
    std::vector< Tile::ContiguousRoadOrCity > orig = mTile.getContiguousRoads();
    for (unsigned int i = 0; i < orig.size(); ++i)
    {
        Tile::ContiguousRoadOrCity restemp;
        Tile::ContiguousRoadOrCity origtemp = orig[i];
        for (unsigned int j = 0; j < origtemp.size(); ++j)
        {
            restemp.push_back(turn(origtemp[j], mRotation));
        }
        res.push_back(restemp);
    }
    return res;
}

std::vector< Tile::ContiguousRoadOrCity >
TileOnBoard::getContiguousCities()
{
    std::vector< Tile::ContiguousRoadOrCity > res;
    std::vector< Tile::ContiguousRoadOrCity > orig = mTile.getContiguousCities();
    for (unsigned int i = 0; i < orig.size(); ++i)
    {
        Tile::ContiguousRoadOrCity restemp;
        Tile::ContiguousRoadOrCity origtemp = orig[i];
        for (unsigned int j = 0; j < origtemp.size(); ++j)
        {
            restemp.push_back(turn(origtemp[j], mRotation));
        }
        res.push_back(restemp);
    }
    return res;
}

std::vector< Tile::ContiguousRoadOrCity >
TileOnBoard::getCitiesPerField(FieldArea::FieldArea inFieldArea)
{
    FieldArea::FieldArea tileFieldArea = turn(inFieldArea, TileOnBoard::Rotation((mRotation + 9) % 12));
    std::vector< Tile::ContiguousRoadOrCity > tileCities = mTile.getCitiesPerField(tileFieldArea);
    std::vector< Tile::ContiguousRoadOrCity > cities;
    for (unsigned int i = 0; i < tileCities.size(); ++i)
    {
        Tile::ContiguousRoadOrCity cityTemp;
        Tile::ContiguousRoadOrCity tileCityTemp = tileCities[i];
        for (unsigned int j = 0; j < tileCityTemp.size(); ++j)
        {
            cityTemp.push_back(turn(tileCityTemp[j], mRotation));
        }
        cities.push_back(cityTemp);
    }
    return cities;
}

std::vector< Tile::ContiguousRoadOrCity >
TileOnBoard::getFinishedRoads()
{
    return mFinishedRoads;
}

std::vector< Tile::ContiguousRoadOrCity >
TileOnBoard::getFinishedCities()
{
    return mFinishedCities;
}

std::vector< RoadOrCityArea::RoadOrCityArea >
TileOnBoard::getShields()
{
    std::vector< RoadOrCityArea::RoadOrCityArea > res;
    std::vector< RoadOrCityArea::RoadOrCityArea > orig = mTile.getShields();
    for (unsigned int i = 0; i < orig.size(); ++i)
    {
        res.push_back(turn(orig[i], mRotation));
    }
    return res;
}

std::vector< RoadOrCityArea::RoadOrCityArea >
TileOnBoard::getInns()
{
    std::vector< RoadOrCityArea::RoadOrCityArea > res;
    std::vector< RoadOrCityArea::RoadOrCityArea > orig = mTile.getInns();
    for (unsigned int i = 0; i < orig.size(); ++i)
    {
        res.push_back(turn(orig[i], mRotation));
    }
    return res;
}

bool
TileOnBoard::isFieldOccupied(FieldArea::FieldArea inFieldArea)
{
    bool occupied = false;
    for (unsigned int i = 0; i < mOccupiedFields.size(); ++i)
    {
        if ( std::find(mOccupiedFields[i].begin(), mOccupiedFields[i].end(), inFieldArea) != mOccupiedFields[i].end() )
        {
            occupied = true;
            break;
        }
    }
    return occupied;
}

bool
TileOnBoard::isRoadOccupied(RoadOrCityArea::RoadOrCityArea inRoadArea)
{
    bool occupied = false;
    for (unsigned int i = 0; i < mOccupiedRoads.size(); ++i)
    {
        if ( std::find(mOccupiedRoads[i].begin(), mOccupiedRoads[i].end(), inRoadArea) != mOccupiedRoads[i].end() )
        {
            occupied = true;
            break;
        }
    }
    return occupied;
}

bool
TileOnBoard::isCityOccupied(RoadOrCityArea::RoadOrCityArea inCityArea)
{
    bool occupied = false;
    for (unsigned int i = 0; i < mOccupiedCities.size(); ++i)
    {
        if ( std::find(mOccupiedCities[i].begin(), mOccupiedCities[i].end(), inCityArea) != mOccupiedCities[i].end() )
        {
            occupied = true;
            break;
        }
    }
    return occupied;
}

bool
TileOnBoard::isRoadFinished(RoadOrCityArea::RoadOrCityArea inRoadArea)
{
    bool finished = false;
    for (unsigned int i = 0; i < mFinishedRoads.size(); ++i)
    {
        if ( std::find(mFinishedRoads[i].begin(), mFinishedRoads[i].end(), inRoadArea) != mFinishedRoads[i].end() )
        {
            finished = true;
            break;
        }
    }
    return finished;
}

bool
TileOnBoard::isCityFinished(RoadOrCityArea::RoadOrCityArea inCityArea)
{
    bool finished = false;
    for (unsigned int i = 0; i < mFinishedCities.size(); ++i)
    {
        if ( std::find(mFinishedCities[i].begin(), mFinishedCities[i].end(), inCityArea) != mFinishedCities[i].end() )
        {
            finished = true;
            break;
        }
    }
    return finished;
}

void
TileOnBoard::occupyField(FieldArea::FieldArea inFieldArea)
{
    if (!isFieldOccupied(inFieldArea))
    {
        std::vector< Tile::ContiguousField > contiguousFields = getContiguousFields();
        unsigned int i = 0;
        for (; i < contiguousFields.size(); ++i)
        {
            if ( std::find(contiguousFields[i].begin(), contiguousFields[i].end(), inFieldArea) != contiguousFields[i].end() )
            {
                break;
            }
        }
        if ( i < contiguousFields.size() )
        {
            Tile::ContiguousField occupiedField = contiguousFields[i];
            mOccupiedFields.push_back(occupiedField);
        }
    }
}

void
TileOnBoard::occupyRoad(RoadOrCityArea::RoadOrCityArea inRoadArea)
{
    if (!isRoadOccupied(inRoadArea))
    {
        std::vector< Tile::ContiguousRoadOrCity > contiguousRoads = getContiguousRoads();
        unsigned int i = 0;
        for (; i < contiguousRoads.size(); ++i)
        {
            if ( std::find(contiguousRoads[i].begin(), contiguousRoads[i].end(), inRoadArea) != contiguousRoads[i].end() )
            {
                break;
            }
        }
        if ( i < contiguousRoads.size() )
        {
            Tile::ContiguousRoadOrCity occupiedRoad = contiguousRoads[i];
            mOccupiedRoads.push_back(occupiedRoad);
        }
    }
}

void
TileOnBoard::occupyCity(RoadOrCityArea::RoadOrCityArea inCityArea)
{
    if (!isCityOccupied(inCityArea))
    {
        std::vector< Tile::ContiguousRoadOrCity > contiguousCities = getContiguousCities();
        unsigned int i = 0;
        for (; i < contiguousCities.size(); ++i)
        {
            if ( std::find(contiguousCities[i].begin(), contiguousCities[i].end(), inCityArea) != contiguousCities[i].end() )
            {
                break;
            }
        }
        if ( i < contiguousCities.size() )
        {
            Tile::ContiguousRoadOrCity occupiedCity = contiguousCities[i];
            mOccupiedCities.push_back(occupiedCity);
        }
    }
}

void
TileOnBoard::finishRoad(RoadOrCityArea::RoadOrCityArea inRoadArea)
{
    if (!isRoadFinished(inRoadArea))
    {
        std::vector< Tile::ContiguousRoadOrCity > contiguousRoads = getContiguousRoads();
        unsigned int i = 0;
        for (; i < contiguousRoads.size(); ++i)
        {
            if ( std::find(contiguousRoads[i].begin(), contiguousRoads[i].end(), inRoadArea) != contiguousRoads[i].end() )
            {
                break;
            }
        }
        if ( i < contiguousRoads.size() )
        {
            Tile::ContiguousRoadOrCity finishedRoad = contiguousRoads[i];
            mFinishedRoads.push_back(finishedRoad);
        }
    }
}

void
TileOnBoard::finishCity(RoadOrCityArea::RoadOrCityArea inCityArea)
{
    if (!isCityFinished(inCityArea))
    {
        std::vector< Tile::ContiguousRoadOrCity > contiguousCities = getContiguousCities();
        unsigned int i = 0;
        for (; i < contiguousCities.size(); ++i)
        {
            if ( std::find(contiguousCities[i].begin(), contiguousCities[i].end(), inCityArea) != contiguousCities[i].end() )
            {
                break;
            }
        }
        if ( i < contiguousCities.size() )
        {
            Tile::ContiguousRoadOrCity finishedCity = contiguousCities[i];
            mFinishedCities.push_back(finishedCity);
        }
    }
}

std::string
TileOnBoard::toString()
{
    std::string result;
    result.append("Top: ");
    result.append(Tile::sideToString(this->getTop()));
    result.append("\tRight: ");
    result.append(Tile::sideToString(this->getRight()));
    result.append("\tBottom: ");
    result.append(Tile::sideToString(this->getBottom()));
    result.append("\tLeft: ");
    result.append(Tile::sideToString(this->getLeft()));
    result.append("\tCenter: ");
    result.append(Tile::centerToString(this->getCenter()));
    std::vector< Tile::ContiguousField > fields = this->getContiguousFields();
    if (!fields.empty())
    {
        result.append("\nContiguous fields:");
        for (unsigned int i = 0; i < fields.size(); i++)
        {
            result.append("\n\t- ");
            for (unsigned int j = 0; j < fields[i].size(); j++)
            {
                result.append(FieldArea::fieldAreaToString(fields[i][j]));
                result.append(" ");
            }
        }
    }
    std::vector< Tile::ContiguousRoadOrCity > roads = this->getContiguousRoads();
    if (!roads.empty())
    {
        result.append("\nContiguous roads:");
        for (unsigned int i = 0; i < roads.size(); i++)
        {
            result.append("\n\t- ");
            for (unsigned int j = 0; j < roads[i].size(); j++)
            {
                result.append(RoadOrCityArea::roadOrCityAreaToString(roads[i][j]));
                result.append(" ");
            }
        }
    }
    std::vector< Tile::ContiguousRoadOrCity > cities = this->getContiguousCities();
    if (!cities.empty())
    {
        result.append("\nContiguous cities:");
        for (unsigned int i = 0; i < cities.size(); i++)
        {
            result.append("\n\t- ");
            for (unsigned int j = 0; j < cities[i].size(); j++)
            {
                result.append(RoadOrCityArea::roadOrCityAreaToString(cities[i][j]));
                result.append(" ");
            }
        }
    }
    std::vector< RoadOrCityArea::RoadOrCityArea > shields = this->getShields();
    if (!shields.empty())
    {
        result.append("\nShields at:\n\t- ");
        for (unsigned int i = 0; i < shields.size(); i++)
        {
            result.append(RoadOrCityArea::roadOrCityAreaToString(shields[i]));
            result.append(" ");
        }
    }
    std::vector< RoadOrCityArea::RoadOrCityArea > inns = this->getInns();
    if (!inns.empty())
    {
        result.append("\nInns at:\n\t- ");
        for (unsigned int i = 0; i < inns.size(); i++)
        {
            result.append(RoadOrCityArea::roadOrCityAreaToString(inns[i]));
            result.append(" ");
        }
    }
    result.append("\n");
    return result;
}
