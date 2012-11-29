#include "tileonboard.h"
#include "assert.h"

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
            result.append("\n- ");
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
            result.append("\n- ");
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
            result.append("\n- ");
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
        result.append("\nShields at:\n- ");
        for (unsigned int i = 0; i < shields.size(); i++)
        {
            result.append(RoadOrCityArea::roadOrCityAreaToString(shields[i]));
            result.append(" ");
        }
    }
    std::vector< RoadOrCityArea::RoadOrCityArea > inns = this->getInns();
    if (!inns.empty())
    {
        result.append("\nInns at:\n- ");
        for (unsigned int i = 0; i < inns.size(); i++)
        {
            result.append(RoadOrCityArea::roadOrCityAreaToString(inns[i]));
            result.append(" ");
        }
    }
    result.append("\n");
    return result;
}
