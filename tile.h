#ifndef TILE_H
#define TILE_H

#include <map>
#include <string>
#include <vector>

namespace FieldArea
{
    enum FieldArea
    {
        TopLeft = 0,
        TopRight = 2,
        RightTop = 3,
        RightBottom = 5,
        BottomRight = 6,
        BottomLeft = 8,
        LeftBottom = 9,
        LeftTop = 11,
        Central = 12
    };

    std::string fieldAreaToString(FieldArea inFieldArea);
}

namespace RoadOrCityArea
{
    enum RoadOrCityArea
    {
        Top = 1,
        Right = 4,
        Bottom = 7,
        Left = 10
    };

    std::string roadOrCityAreaToString(RoadOrCityArea inRoadOrCityArea);
}

namespace Area
{
    enum Area
    {
        TopLeft = 0,
        Top = 1,
        TopRight = 2,
        RightTop = 3,
        Right = 4,
        RightBottom = 5,
        BottomRight = 6,
        Bottom = 7,
        BottomLeft = 8,
        LeftBottom = 9,
        Left = 10,
        LeftTop = 11,
        Central = 12
    };

    std::string areaToString(Area inArea);
}

class Tile
{
public:
    enum Side
    {
        Field,
        Road,
        City
    };

    enum Center
    {
        Nothing,
        Cloister,
        Cathedral,
        CentralField
    };

    typedef std::vector< FieldArea::FieldArea > ContiguousField;
    typedef std::vector< RoadOrCityArea::RoadOrCityArea > ContiguousRoadOrCity;

    Tile();
    Tile(Side inTop, Side inRight, Side inBottom, Side inLeft, Center inCenter,
         std::string inID,
         std::vector< ContiguousField > inMeadows,
         std::vector< ContiguousRoadOrCity > inRoads,
         std::vector< ContiguousRoadOrCity > inCities,
         std::map< ContiguousField, std::vector< ContiguousRoadOrCity > > inCitiesPerField,
         std::vector< RoadOrCityArea::RoadOrCityArea > inShields);
    Tile(Side inTop, Side inRight, Side inBottom, Side inLeft, Center inCenter,
         std::string inID,
         std::vector< ContiguousField > inMeadows,
         std::vector< ContiguousRoadOrCity > inRoads,
         std::vector< ContiguousRoadOrCity > inCities,
         std::map< ContiguousField, std::vector< ContiguousRoadOrCity > > inCitiesPerField,
         std::vector< RoadOrCityArea::RoadOrCityArea > inShields,
         std::vector< RoadOrCityArea::RoadOrCityArea > inInns);


    static std::string sideToString(Side inSide);
    static std::string centerToString(Center inCenter);

    Side getTop();
    Side getRight();
    Side getBottom();
    Side getLeft();
    Center getCenter();
    std::string getID();
    std::vector< ContiguousField > getContiguousFields();
    std::vector< ContiguousRoadOrCity > getContiguousRoads();
    std::vector< ContiguousRoadOrCity > getContiguousCities();
    std::vector< ContiguousRoadOrCity > getCitiesPerField(ContiguousField inContiguousField);
    std::vector< ContiguousRoadOrCity > getCitiesPerField(FieldArea::FieldArea inFieldArea);
    std::vector< RoadOrCityArea::RoadOrCityArea > getShields();
    std::vector< RoadOrCityArea::RoadOrCityArea > getInns();

    ContiguousField getContiguousField(FieldArea::FieldArea inFieldArea);
    ContiguousRoadOrCity getContiguousRoad(RoadOrCityArea::RoadOrCityArea inRoadArea);
    ContiguousRoadOrCity getContiguousCity(RoadOrCityArea::RoadOrCityArea inCityArea);

    std::string toString();

private:
    std::string mID;
    Side mTop;
    Side mRight;
    Side mBottom;
    Side mLeft;
    Center mCenter;
    std::vector< ContiguousField > mFields;
    std::vector< ContiguousRoadOrCity > mRoads;
    std::vector< ContiguousRoadOrCity > mCities;
    std::map< ContiguousField, std::vector< ContiguousRoadOrCity > > mCitiesPerField;
    std::vector< RoadOrCityArea::RoadOrCityArea > mShields;
    std::vector< RoadOrCityArea::RoadOrCityArea > mInns;
};

#endif // TILE_H
