#include "tile.h"

std::string
FieldArea::fieldAreaToString(FieldArea inFieldArea)
{
    switch (inFieldArea)
    {
    case TopLeft:
        return "TopLeft";
    case TopRight:
        return "TopRight";
    case RightTop:
        return "RightTop";
    case RightBottom:
        return "RightBottom";
    case BottomRight:
        return "BottomRight";
    case BottomLeft:
        return "BottomLeft";
    case LeftBottom:
        return "LeftBottom";
    case LeftTop:
        return "LeftTop";
    case Central:
        return "Central";
    default:
        return "No valid FieldArea";
    }
}

std::string
RoadOrCityArea::roadOrCityAreaToString(RoadOrCityArea inRoadOrCityArea)
{
    switch (inRoadOrCityArea)
    {
    case Top:
        return "Top";
    case Right:
        return "Right";
    case Bottom:
        return "Bottom";
    case Left:
        return "Left";
    default:
        return "No valid RoadOrCityArea";
    }
}

std::string
Area::areaToString(Area inArea)
{
    switch (inArea)
    {
    case TopLeft:
        return "AreaTopLeft";
    case TopRight:
        return "AreaTopRight";
    case RightTop:
        return "AreaRightTop";
    case RightBottom:
        return "AreaRightBottom";
    case BottomRight:
        return "AreaBottomRight";
    case BottomLeft:
        return "AreaBottomLeft";
    case LeftBottom:
        return "AreaLeftBottom";
    case LeftTop:
        return "AreaLeftTop";
    case Central:
        return "AreaCentral";
    case Top:
        return "AreaTop";
    case Right:
        return "AreaRight";
    case Bottom:
        return "AreaBottom";
    case Left:
        return "AreaLeft";
    default:
        return "No valid Area";
    }
}

Tile::Tile():
    mTop(Field),
    mRight(Field),
    mBottom(Field),
    mLeft(Field),
    mCenter(Cloister)
{
    mFields = std::vector< ContiguousField >();
    ContiguousField allFields = std::vector< FieldArea::FieldArea >();
    allFields.push_back(FieldArea::TopLeft);
    allFields.push_back(FieldArea::TopRight);
    allFields.push_back(FieldArea::RightTop);
    allFields.push_back(FieldArea::RightBottom);
    allFields.push_back(FieldArea::BottomRight);
    allFields.push_back(FieldArea::BottomLeft);
    allFields.push_back(FieldArea::LeftBottom);
    allFields.push_back(FieldArea::LeftTop);
    mFields.push_back(allFields);
    mRoads = std::vector< ContiguousRoadOrCity >();
    mCities = std::vector< ContiguousRoadOrCity >();
    mShields = std::vector< RoadOrCityArea::RoadOrCityArea >();
    mInns = std::vector< RoadOrCityArea::RoadOrCityArea >();
}

Tile::Tile(Side inTop, Side inRight, Side inBottom, Side inLeft, Center inCenter,
           std::vector< ContiguousField > inFields,
           std::vector< ContiguousRoadOrCity > inRoads,
           std::vector< ContiguousRoadOrCity > inCities,
           std::vector< RoadOrCityArea::RoadOrCityArea > inShields):
    mTop(inTop),
    mRight(inRight),
    mBottom(inBottom),
    mLeft(inLeft),
    mCenter(inCenter),
    mFields(inFields),
    mRoads(inRoads),
    mCities(inCities),
    mShields(inShields)
{
    mInns = std::vector< RoadOrCityArea::RoadOrCityArea >();
}

Tile::Tile(Side inTop, Side inRight, Side inBottom, Side inLeft, Center inCenter,
           std::vector< ContiguousField > inFields,
           std::vector< ContiguousRoadOrCity > inRoads,
           std::vector< ContiguousRoadOrCity > inCities,
           std::vector< RoadOrCityArea::RoadOrCityArea > inShields,
           std::vector< RoadOrCityArea::RoadOrCityArea > inInns):
    mTop(inTop),
    mRight(inRight),
    mBottom(inBottom),
    mLeft(inLeft),
    mCenter(inCenter),
    mFields(inFields),
    mRoads(inRoads),
    mCities(inCities),
    mShields(inShields),
    mInns(inInns)
{
}

Tile::Side
Tile::getTop()
{
    return mTop;
}

Tile::Side
Tile::getRight()
{
    return mRight;
}

Tile::Side
Tile::getBottom()
{
    return mBottom;
}

Tile::Side
Tile::getLeft()
{
    return mLeft;
}

Tile::Center
Tile::getCenter()
{
    return mCenter;
}

std::vector< Tile::ContiguousField >
Tile::getContiguousFields()
{
    return mFields;
}

std::vector< Tile::ContiguousRoadOrCity >
Tile::getContiguousRoads()
{
    return mRoads;
}

std::vector< Tile::ContiguousRoadOrCity >
Tile::getContiguousCities()
{
    return mCities;
}

std::vector< RoadOrCityArea::RoadOrCityArea >
Tile::getShields()
{
    return mShields;
}

std::vector< RoadOrCityArea::RoadOrCityArea >
Tile::getInns()
{
    return mInns;
}

std::string
Tile::toString()
{
    std::string result;
    result.append("Top: ");
    result.append(sideToString(this->getTop()));
    result.append("\tRight: ");
    result.append(sideToString(this->getRight()));
    result.append("\tBottom: ");
    result.append(sideToString(this->getBottom()));
    result.append("\tLeft: ");
    result.append(sideToString(this->getLeft()));
    result.append("\tCenter: ");
    result.append(centerToString(this->getCenter()));
    if (!mFields.empty())
    {
        result.append("\nContiguous fields:");
        for (unsigned int i = 0; i < mFields.size(); i++)
        {
            result.append("\n- ");
            for (unsigned int j = 0; j < mFields[i].size(); j++)
            {
                result.append(FieldArea::fieldAreaToString(mFields[i][j]));
                result.append(" ");
            }
        }
    }
    if (!mRoads.empty())
    {
        result.append("\nContiguous roads:");
        for (unsigned int i = 0; i < mRoads.size(); i++)
        {
            result.append("\n- ");
            for (unsigned int j = 0; j < mRoads[i].size(); j++)
            {
                result.append(RoadOrCityArea::roadOrCityAreaToString(mRoads[i][j]));
                result.append(" ");
            }
        }
    }
    if (!mCities.empty())
    {
        result.append("\nContiguous cities:");
        for (unsigned int i = 0; i < mCities.size(); i++)
        {
            result.append("\n- ");
            for (unsigned int j = 0; j < mCities[i].size(); j++)
            {
                result.append(RoadOrCityArea::roadOrCityAreaToString(mCities[i][j]));
                result.append(" ");
            }
        }
    }
    if (!mShields.empty())
    {
        result.append("\nShields at:\n- ");
        for (unsigned int i = 0; i < mShields.size(); i++)
        {
            result.append(RoadOrCityArea::roadOrCityAreaToString(mShields[i]));
            result.append(" ");
        }
    }
    if (!mInns.empty())
    {
        result.append("\nInns at:\n- ");
        for (unsigned int i = 0; i < mInns.size(); i++)
        {
            result.append(RoadOrCityArea::roadOrCityAreaToString(mInns[i]));
            result.append(" ");
        }
    }
    result.append("\n");
    return result;
}

std::string
Tile::sideToString(Side inSide)
{
    switch (inSide)
    {
    case Field:
        return "Field";
    case Road:
        return "Road";
    case City:
        return "City";
    default:
        return "No valid side";
    }
}

std::string
Tile::centerToString(Center inCenter)
{
    switch (inCenter)
    {
    case Nothing:
        return "Nothing";
    case Cloister:
        return "Cloister";
    case Cathedral:
        return "Cathedral";
    case CentralField:
        return "CentralField";
    default:
        return "No valid center";
    }
}
