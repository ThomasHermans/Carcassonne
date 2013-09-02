#include "src-model/Tile.h"

#include <algorithm>
#include <cassert>

std::string
FRCArea::fieldAreaToString(FieldArea inFieldArea)
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
FRCArea::roadAreaToString(RoadArea inRoadArea)
{
    switch (inRoadArea)
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
        return "No valid Area";
    }
}

std::string
FRCArea::cityAreaToString(CityArea inCityArea)
{
    return roadAreaToString(inCityArea);
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
    case Invalid:
    default:
        return "No valid Area";
    }
}

Tile::Tile():
    mID("D"),
    mTop(Road),
    mRight(City),
    mBottom(Road),
    mLeft(Field),
    mCenter(Nothing)
{
    mFields = std::vector< ContiguousField >();
    ContiguousField rightField = std::vector< FRCArea::FieldArea >();
    rightField.push_back(FRCArea::TopRight);
    rightField.push_back(FRCArea::BottomRight);
    ContiguousField leftField = std::vector< FRCArea::FieldArea >();
    leftField.push_back(FRCArea::BottomLeft);
    leftField.push_back(FRCArea::LeftBottom);
    leftField.push_back(FRCArea::LeftTop);
    leftField.push_back(FRCArea::TopLeft);
    mFields.push_back(rightField);
    mFields.push_back(leftField);
    mRoads = std::vector< ContiguousRoad >();
    mCities = std::vector< ContiguousCity >();
    ContiguousCity rightCity = std::vector< FRCArea::CityArea >();
    rightCity.push_back(FRCArea::Right);
    mCitiesPerField = std::map< ContiguousField, std::vector< ContiguousCity > >();
    std::vector< ContiguousCity > rightFieldCities;
    rightFieldCities.push_back( rightCity );
    std::vector< ContiguousCity > leftFieldCities;
    mCitiesPerField.insert( std::pair< Tile::ContiguousField, std::vector< Tile::ContiguousCity > >(rightField, rightFieldCities));
    mCitiesPerField.insert( std::pair< Tile::ContiguousField, std::vector< Tile::ContiguousCity > >(leftField, leftFieldCities));
    mShields = std::vector< FRCArea::CityArea >();
    mInns = std::vector< FRCArea::RoadArea >();
}

Tile::Tile(Center inCenter,
           const std::string & inID,
           const std::vector< ContiguousField > & inFields,
           const std::vector< ContiguousRoad > & inRoads,
           const std::vector< ContiguousCity > & inCities,
           const std::map< ContiguousField, std::vector< ContiguousCity > > & inCitiesPerField,
           const std::vector< FRCArea::CityArea > & inShields,
           const std::vector< FRCArea::RoadArea > & inInns):
    mID(inID),
    mTop(Tile::Field),
    mRight(Tile::Field),
    mBottom(Tile::Field),
    mLeft(Tile::Field),
    mCenter(inCenter),
    mFields(inFields),
    mRoads(inRoads),
    mCities(inCities),
    mCitiesPerField(inCitiesPerField),
    mShields(inShields),
    mInns(inInns)
{
    int top = 0;
    int right = 0;
    int bottom = 0;
    int left = 0;
    for ( unsigned int i = 0; i < inCities.size(); ++i )
    {
        for ( unsigned int j = 0; j < inCities[i].size(); ++j )
        {
            switch ( inCities[i][j] )
            {
            case FRCArea::Top:
                mTop = Tile::City;
                ++top;
                break;
            case FRCArea::Right:
                mRight = Tile::City;
                ++right;
                break;
            case FRCArea::Bottom:
                mBottom = Tile::City;
                ++bottom;
                break;
            case FRCArea::Left:
                mLeft = Tile::City;
                ++left;
                break;
            }
        }
    }
    for ( unsigned int i = 0; i < inRoads.size(); ++i )
    {
        for ( unsigned int j = 0; j < inRoads[i].size(); ++j )
        {
            switch ( inRoads[i][j] )
            {
            case FRCArea::Top:
                mTop = Tile::Road;
                ++top;
                break;
            case FRCArea::Right:
                mRight = Tile::Road;
                ++right;
                break;
            case FRCArea::Bottom:
                mBottom = Tile::Road;
                ++bottom;
                break;
            case FRCArea::Left:
                mLeft = Tile::Road;
                ++left;
                break;
            }
        }
    }
    assert( top <= 1 );
    assert( right <= 1 );
    assert( bottom <= 1 );
    assert( left <= 1 );
}

Tile::Side
Tile::getTop() const
{
    return mTop;
}

Tile::Side
Tile::getRight() const
{
    return mRight;
}

Tile::Side
Tile::getBottom() const
{
    return mBottom;
}

Tile::Side
Tile::getLeft() const
{
    return mLeft;
}

Tile::Center
Tile::getCenter() const
{
    return mCenter;
}

std::string const &
Tile::getID() const
{
    return mID;
}

std::vector< Tile::ContiguousField >
Tile::getContiguousFields() const
{
    return mFields;
}

std::vector< Tile::ContiguousRoad >
Tile::getContiguousRoads() const
{
    return mRoads;
}

std::vector< Tile::ContiguousCity >
Tile::getContiguousCities() const
{
    return mCities;
}

std::vector< Tile::ContiguousCity >
Tile::getCitiesPerField(const ContiguousField &inContiguousField) const
{
    return mCitiesPerField.at(inContiguousField);
}

std::vector< Tile::ContiguousCity >
Tile::getCitiesPerField(FRCArea::FieldArea inFieldArea) const
{
    ContiguousField cf = getContiguousField(inFieldArea);
    return mCitiesPerField.at(cf);
}

std::vector< FRCArea::CityArea >
Tile::getShields() const
{
    return mShields;
}

std::vector< FRCArea::RoadArea >
Tile::getInns() const
{
    return mInns;
}

Tile::ContiguousField
Tile::getContiguousField(FRCArea::FieldArea inFieldArea) const
{
    unsigned int i = 0;
    for (; i < mFields.size(); ++i)
    {
        if ( std::find(mFields[i].begin(), mFields[i].end(), inFieldArea) != mFields[i].end() )
        {
            break;
        }
    }
    if (i < mFields.size())
    {
        return mFields[i];
    }
    else
    {
        return Tile::ContiguousField();
    }
}

Tile::ContiguousRoad
Tile::getContiguousRoad(FRCArea::RoadArea inRoadArea) const
{
    unsigned int i = 0;
    for (; i < mRoads.size(); ++i)
    {
        if ( std::find(mRoads[i].begin(), mRoads[i].end(), inRoadArea) != mRoads[i].end() )
        {
            break;
        }
    }
    if (i < mRoads.size())
    {
        return mRoads[i];
    }
    else
    {
        return Tile::ContiguousRoad();
    }
}

Tile::ContiguousCity
Tile::getContiguousCity(FRCArea::CityArea inCityArea) const
{
    unsigned int i = 0;
    for (; i < mCities.size(); ++i)
    {
        if ( std::find(mCities[i].begin(), mCities[i].end(), inCityArea) != mCities[i].end() )
        {
            break;
        }
    }
    if (i < mCities.size())
    {
        return mCities[i];
    }
    else
    {
        return Tile::ContiguousCity();
    }
}

std::string
Tile::toString() const
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
            result.append("\n\t- ");
            for (unsigned int j = 0; j < mFields[i].size(); j++)
            {
                result.append(FRCArea::fieldAreaToString(mFields[i][j]));
                result.append(" ");
            }
        }
    }
    if (!mRoads.empty())
    {
        result.append("\nContiguous roads:");
        for (unsigned int i = 0; i < mRoads.size(); i++)
        {
            result.append("\n\t- ");
            for (unsigned int j = 0; j < mRoads[i].size(); j++)
            {
                result.append(FRCArea::roadAreaToString(mRoads[i][j]));
                result.append(" ");
            }
        }
    }
    if (!mCities.empty())
    {
        result.append("\nContiguous cities:");
        for (unsigned int i = 0; i < mCities.size(); i++)
        {
            result.append("\n\t- ");
            for (unsigned int j = 0; j < mCities[i].size(); j++)
            {
                result.append(FRCArea::cityAreaToString(mCities[i][j]));
                result.append(" ");
            }
        }
    }
    if (!mShields.empty())
    {
        result.append("\nShields at:\n\t- ");
        for (unsigned int i = 0; i < mShields.size(); i++)
        {
            result.append(FRCArea::cityAreaToString(mShields[i]));
            result.append(" ");
        }
    }
    if (!mInns.empty())
    {
        result.append("\nInns at:\n\t- ");
        for (unsigned int i = 0; i < mInns.size(); i++)
        {
            result.append(FRCArea::roadAreaToString(mInns[i]));
            result.append(" ");
        }
    }
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
