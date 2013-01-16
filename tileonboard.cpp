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

FRCArea::FieldArea turn(FRCArea::FieldArea inFieldArea, TileOnBoard::Rotation inRotation)
{
    if (inFieldArea == FRCArea::Central)
        return FRCArea::Central;
    else
        return FRCArea::FieldArea((inFieldArea + inRotation) % 12);
}

FRCArea::RoadArea turn(FRCArea::RoadArea inRoadArea, TileOnBoard::Rotation inRotation)
{
    return FRCArea::RoadArea((inRoadArea + inRotation) % 12);
}
}

TileOnBoard::TileOnBoard():
    mTile(Tile::Tile()),
    mRotation(TileOnBoard::cw0)
{
    mOccupiedFields = std::vector< Tile::ContiguousField >();
    mOccupiedRoads = std::vector< Tile::ContiguousRoad >();
    mOccupiedCities = std::vector< Tile::ContiguousCity >();
    mPlacedPieces = std::vector< PlacedPiece >();
}

TileOnBoard::TileOnBoard(const Tile &inTile, Rotation inRotation):
    mTile(inTile),
    mRotation(inRotation)
{
    mOccupiedFields = std::vector< Tile::ContiguousField >();
    mOccupiedRoads = std::vector< Tile::ContiguousRoad >();
    mOccupiedCities = std::vector< Tile::ContiguousCity >();
    mPlacedPieces = std::vector< PlacedPiece >();
}

bool
TileOnBoard::placePiece(const PlacedPiece & inPlacedPiece)
{
    // TODO: check isValid(*this, inPlacedPiece) (from anonymous namespace above)
    // if valid, add inPlacedPiece to mPlacedPieces, return true
    // else return false
    return true;
}

Tile::Side
TileOnBoard::getTop() const
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
TileOnBoard::getRight() const
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
TileOnBoard::getBottom() const
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
TileOnBoard::getLeft() const
{
    switch (mRotation)
    {
    case cw90:
        return mTile.getBottom();
    case cw180:
        return mTile.getRight();
    case cw270:
        return mTile.getTop();
    case cw0:
    default:
        return mTile.getLeft();
    }
}

Tile::Center
TileOnBoard::getCenter() const
{
    return mTile.getCenter();
}

std::string
TileOnBoard::getID() const
{
    return mTile.getID();
}

Tile
TileOnBoard::getTile() const
{
    return mTile;
}

TileOnBoard::Rotation
TileOnBoard::getRotation() const
{
    return mRotation;
}

bool
TileOnBoard::matchesAbove(const TileOnBoard & inTileOnBoard) const
{
    return (this->getBottom() == inTileOnBoard.getTop());
}

bool
TileOnBoard::matchesRightOf(const TileOnBoard & inTileOnBoard) const
{
    return (this->getLeft() == inTileOnBoard.getRight());
}

bool
TileOnBoard::matchesBelow(const TileOnBoard & inTileOnBoard) const
{
    return (this->getTop() == inTileOnBoard.getBottom());
}

bool
TileOnBoard::matchesLeftOf(const TileOnBoard & inTileOnBoard) const
{
    return (this->getRight() == inTileOnBoard.getLeft());
}

std::vector< Tile::ContiguousField >
TileOnBoard::getContiguousFields() const
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

std::vector< Tile::ContiguousRoad >
TileOnBoard::getContiguousRoads() const
{
    std::vector< Tile::ContiguousRoad > res;
    std::vector< Tile::ContiguousRoad > orig = mTile.getContiguousRoads();
    for (unsigned int i = 0; i < orig.size(); ++i)
    {
        Tile::ContiguousRoad restemp;
        Tile::ContiguousRoad origtemp = orig[i];
        for (unsigned int j = 0; j < origtemp.size(); ++j)
        {
            restemp.push_back(turn(origtemp[j], mRotation));
        }
        res.push_back(restemp);
    }
    return res;
}

std::vector< Tile::ContiguousCity >
TileOnBoard::getContiguousCities() const
{
    std::vector< Tile::ContiguousCity > res;
    std::vector< Tile::ContiguousCity > orig = mTile.getContiguousCities();
    for (unsigned int i = 0; i < orig.size(); ++i)
    {
        Tile::ContiguousCity restemp;
        Tile::ContiguousCity origtemp = orig[i];
        for (unsigned int j = 0; j < origtemp.size(); ++j)
        {
            restemp.push_back(turn(origtemp[j], mRotation));
        }
        res.push_back(restemp);
    }
    return res;
}

Tile::ContiguousField
TileOnBoard::getContiguousField(FRCArea::FieldArea inFieldArea) const
{
    FRCArea::FieldArea unRotatedArea = turn(inFieldArea, TileOnBoard::Rotation((mRotation + 9) % 12));
    Tile::ContiguousField unRotatedField = mTile.getContiguousField(unRotatedArea);
    Tile::ContiguousField rotatedField;
    for (unsigned int i = 0; i < unRotatedField.size(); ++i)
    {
        rotatedField.push_back(turn(unRotatedField[i], mRotation));
    }
    return rotatedField;
}

Tile::ContiguousRoad
TileOnBoard::getContiguousRoad(FRCArea::RoadArea inRoadArea) const
{
    FRCArea::RoadArea unRotatedArea = turn(inRoadArea, TileOnBoard::Rotation((mRotation + 9) % 12));
    Tile::ContiguousRoad unRotatedRoad = mTile.getContiguousRoad(unRotatedArea);
    Tile::ContiguousRoad rotatedRoad;
    for (unsigned int i = 0; i < unRotatedRoad.size(); ++i)
    {
        rotatedRoad.push_back(turn(unRotatedRoad[i], mRotation));
    }
    return rotatedRoad;
}

Tile::ContiguousCity
TileOnBoard::getContiguousCity(FRCArea::CityArea inCityArea) const
{
    FRCArea::CityArea unRotatedArea = turn(inCityArea, TileOnBoard::Rotation((mRotation + 9) % 12));
    Tile::ContiguousCity unRotatedCity = mTile.getContiguousCity(unRotatedArea);
    Tile::ContiguousCity rotatedCity;
    for (unsigned int i = 0; i < unRotatedCity.size(); ++i)
    {
        rotatedCity.push_back(turn(unRotatedCity[i], mRotation));
    }
    return rotatedCity;
}

std::vector< Tile::ContiguousCity >
TileOnBoard::getCitiesPerField(FRCArea::FieldArea inFieldArea) const
{
    FRCArea::FieldArea tileFieldArea = turn(inFieldArea, TileOnBoard::Rotation((mRotation + 9) % 12));
    std::vector< Tile::ContiguousCity > tileCities = mTile.getCitiesPerField(tileFieldArea);
    std::vector< Tile::ContiguousCity > cities;
    for (unsigned int i = 0; i < tileCities.size(); ++i)
    {
        Tile::ContiguousCity cityTemp;
        Tile::ContiguousCity tileCityTemp = tileCities[i];
        for (unsigned int j = 0; j < tileCityTemp.size(); ++j)
        {
            cityTemp.push_back(turn(tileCityTemp[j], mRotation));
        }
        cities.push_back(cityTemp);
    }
    return cities;
}

std::vector< Tile::ContiguousRoad >
TileOnBoard::getFinishedRoads() const
{
    return mFinishedRoads;
}

std::vector< Tile::ContiguousCity >
TileOnBoard::getFinishedCities() const
{
    return mFinishedCities;
}

std::vector< FRCArea::CityArea >
TileOnBoard::getShields() const
{
    std::vector< FRCArea::CityArea > res;
    std::vector< FRCArea::CityArea > orig = mTile.getShields();
    for (unsigned int i = 0; i < orig.size(); ++i)
    {
        res.push_back(turn(orig[i], mRotation));
    }
    return res;
}

std::vector< FRCArea::RoadArea >
TileOnBoard::getInns() const
{
    std::vector< FRCArea::RoadArea > res;
    std::vector< FRCArea::RoadArea > orig = mTile.getInns();
    for (unsigned int i = 0; i < orig.size(); ++i)
    {
        res.push_back(turn(orig[i], mRotation));
    }
    return res;
}

bool
TileOnBoard::isFieldOccupied(FRCArea::FieldArea inFieldArea) const
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
TileOnBoard::isRoadOccupied(FRCArea::RoadArea inRoadArea) const
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
TileOnBoard::isCityOccupied(FRCArea::CityArea inCityArea) const
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
TileOnBoard::isRoadFinished(FRCArea::RoadArea inRoadArea) const
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
TileOnBoard::isCityFinished(FRCArea::CityArea inCityArea) const
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
TileOnBoard::occupyField(FRCArea::FieldArea inFieldArea)
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
TileOnBoard::occupyRoad(FRCArea::RoadArea inRoadArea)
{
    if (!isRoadOccupied(inRoadArea))
    {
        std::vector< Tile::ContiguousRoad > contiguousRoads = getContiguousRoads();
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
            Tile::ContiguousRoad occupiedRoad = contiguousRoads[i];
            mOccupiedRoads.push_back(occupiedRoad);
        }
    }
}

void
TileOnBoard::occupyCity(FRCArea::CityArea inCityArea)
{
    if (!isCityOccupied(inCityArea))
    {
        std::vector< Tile::ContiguousCity > contiguousCities = getContiguousCities();
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
            Tile::ContiguousCity occupiedCity = contiguousCities[i];
            mOccupiedCities.push_back(occupiedCity);
        }
    }
}

void
TileOnBoard::finishRoad(FRCArea::RoadArea inRoadArea)
{
    if (!isRoadFinished(inRoadArea))
    {
        std::vector< Tile::ContiguousRoad > contiguousRoads = getContiguousRoads();
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
            Tile::ContiguousRoad finishedRoad = contiguousRoads[i];
            mFinishedRoads.push_back(finishedRoad);
        }
    }
}

void
TileOnBoard::finishCity(FRCArea::CityArea inCityArea)
{
    if (!isCityFinished(inCityArea))
    {
        std::vector< Tile::ContiguousCity > contiguousCities = getContiguousCities();
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
            Tile::ContiguousCity finishedCity = contiguousCities[i];
            mFinishedCities.push_back(finishedCity);
        }
    }
}

std::string
TileOnBoard::toString() const
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
                result.append(FRCArea::fieldAreaToString(fields[i][j]));
                result.append(" ");
            }
        }
    }
    std::vector< Tile::ContiguousRoad > roads = this->getContiguousRoads();
    if (!roads.empty())
    {
        result.append("\nContiguous roads:");
        for (unsigned int i = 0; i < roads.size(); i++)
        {
            result.append("\n\t- ");
            for (unsigned int j = 0; j < roads[i].size(); j++)
            {
                result.append(FRCArea::roadAreaToString(roads[i][j]));
                result.append(" ");
            }
        }
    }
    std::vector< Tile::ContiguousCity > cities = this->getContiguousCities();
    if (!cities.empty())
    {
        result.append("\nContiguous cities:");
        for (unsigned int i = 0; i < cities.size(); i++)
        {
            result.append("\n\t- ");
            for (unsigned int j = 0; j < cities[i].size(); j++)
            {
                result.append(FRCArea::cityAreaToString(cities[i][j]));
                result.append(" ");
            }
        }
    }
    std::vector< FRCArea::CityArea > shields = this->getShields();
    if (!shields.empty())
    {
        result.append("\nShields at:\n\t- ");
        for (unsigned int i = 0; i < shields.size(); i++)
        {
            result.append(FRCArea::cityAreaToString(shields[i]));
            result.append(" ");
        }
    }
    std::vector< FRCArea::RoadArea > inns = this->getInns();
    if (!inns.empty())
    {
        result.append("\nInns at:\n\t- ");
        for (unsigned int i = 0; i < inns.size(); i++)
        {
            result.append(FRCArea::roadAreaToString(inns[i]));
            result.append(" ");
        }
    }
    result.append("\n");
    return result;
}
