#include "tile.h"

Tile::Tile():
    mTop(Field),
    mRight(Field),
    mBottom(Field),
    mLeft(Field),
    mCenter(Cloister)
{
    mFields = std::vector< ContiguousField >();
    ContiguousField allFields = std::vector< FieldArea >();
    allFields.push_back(TopLeft);
    allFields.push_back(TopRight);
    allFields.push_back(RightTop);
    allFields.push_back(RightBottom);
    allFields.push_back(BottomRight);
    allFields.push_back(BottomLeft);
    allFields.push_back(LeftBottom);
    allFields.push_back(LeftTop);
    mFields.push_back(allFields);
    mRoads = std::vector< ContiguousRoadOrCity >();
    mCities = std::vector< ContiguousRoadOrCity >();
    mShields = std::vector< RoadOrCityArea >();
}

Tile::Tile(Side inTop, Side inRight, Side inBottom, Side inLeft, Center inCenter,
           std::vector< ContiguousField > inFields,
           std::vector< ContiguousRoadOrCity > inRoads,
           std::vector< ContiguousRoadOrCity > inCities,
           std::vector< RoadOrCityArea > inShields):
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
    result.append("\nContiguous fields:");
    for (unsigned int i = 0; i < mFields.size(); i++)
    {
        result.append("\n- ");
        for (unsigned int j = 0; j < mFields[i].size(); j++)
        {
            result.append(fieldAreaToString(mFields[i][j]));
            result.append(" ");
        }
    }
    result.append("\nContiguous roads:");
    for (unsigned int i = 0; i < mRoads.size(); i++)
    {
        result.append("\n- ");
        for (unsigned int j = 0; j < mRoads[i].size(); j++)
        {
            result.append(roadOrCityAreaToString(mRoads[i][j]));
            result.append(" ");
        }
    }
    result.append("\nContiguous cities:");
    for (unsigned int i = 0; i < mCities.size(); i++)
    {
        result.append("\n- ");
        for (unsigned int j = 0; j < mCities[i].size(); j++)
        {
            result.append(roadOrCityAreaToString(mCities[i][j]));
            result.append(" ");
        }
    }
    result.append("\nShields at:\n- ");
    for (unsigned int i = 0; i < mShields.size(); i++)
    {
        result.append(roadOrCityAreaToString(mShields[i]));
        result.append(" ");
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

std::string
Tile::fieldAreaToString(FieldArea inFieldArea)
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
Tile::roadOrCityAreaToString(RoadOrCityArea inRoadOrCityArea)
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
Tile::areaToString(Area inArea)
{
    switch (inArea)
    {
    case AreaTopLeft:
        return "AreaTopLeft";
    case AreaTopRight:
        return "AreaTopRight";
    case AreaRightTop:
        return "AreaRightTop";
    case AreaRightBottom:
        return "AreaRightBottom";
    case AreaBottomRight:
        return "AreaBottomRight";
    case AreaBottomLeft:
        return "AreaBottomLeft";
    case AreaLeftBottom:
        return "AreaLeftBottom";
    case AreaLeftTop:
        return "AreaLeftTop";
    case AreaCentral:
        return "AreaCentral";
    case AreaTop:
        return "AreaTop";
    case AreaRight:
        return "AreaRight";
    case AreaBottom:
        return "AreaBottom";
    case AreaLeft:
        return "AreaLeft";
    default:
        return "No valid Area";
    }
}
