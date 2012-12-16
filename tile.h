#ifndef TILE_H
#define TILE_H

#include <map>
#include <string>
#include <vector>

namespace FRCArea
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

    enum RoadArea
    {
        Top = 1,
        Right = 4,
        Bottom = 7,
        Left = 10
    };

    typedef RoadArea CityArea;

    std::string roadAreaToString(RoadArea inRoadArea);
    std::string cityAreaToString(CityArea inCityArea);
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

    typedef std::vector< FRCArea::FieldArea > ContiguousField;
    typedef std::vector< FRCArea::RoadArea > ContiguousRoad;
    typedef std::vector< FRCArea::CityArea > ContiguousCity;

    Tile();
    Tile(Side inTop, Side inRight, Side inBottom, Side inLeft, Center inCenter,
         std::string inID,
         std::vector< ContiguousField > inMeadows,
         std::vector< ContiguousRoad > inRoads,
         std::vector< ContiguousCity > inCities,
         std::map< ContiguousField, std::vector< ContiguousCity > > inCitiesPerField,
         std::vector< FRCArea::CityArea > inShields);
    Tile(Side inTop, Side inRight, Side inBottom, Side inLeft, Center inCenter,
         std::string inID,
         std::vector< ContiguousField > inMeadows,
         std::vector< ContiguousRoad > inRoads,
         std::vector< ContiguousCity > inCities,
         std::map< ContiguousField, std::vector< ContiguousCity > > inCitiesPerField,
         std::vector< FRCArea::CityArea > inShields,
         std::vector< FRCArea::RoadArea > inInns);


    static std::string sideToString(Side inSide);
    static std::string centerToString(Center inCenter);

    Side getTop();
    Side getRight();
    Side getBottom();
    Side getLeft();
    Center getCenter();
    std::string getID();
    std::vector< ContiguousField > getContiguousFields();
    std::vector< ContiguousRoad > getContiguousRoads();
    std::vector< ContiguousCity > getContiguousCities();
    std::vector< ContiguousCity > getCitiesPerField(ContiguousField inContiguousField);
    std::vector< ContiguousCity > getCitiesPerField(FRCArea::FieldArea inFieldArea);
    std::vector< FRCArea::CityArea > getShields();
    std::vector< FRCArea::RoadArea > getInns();

    ContiguousField getContiguousField(FRCArea::FieldArea inFieldArea);
    ContiguousRoad getContiguousRoad(FRCArea::RoadArea inRoadArea);
    ContiguousCity getContiguousCity(FRCArea::CityArea inCityArea);

    std::string toString();

private:
    std::string mID;
    Side mTop;
    Side mRight;
    Side mBottom;
    Side mLeft;
    Center mCenter;
    std::vector< ContiguousField > mFields;
    std::vector< ContiguousRoad > mRoads;
    std::vector< ContiguousCity > mCities;
    std::map< ContiguousField, std::vector< ContiguousCity > > mCitiesPerField;
    std::vector< FRCArea::CityArea > mShields;
    std::vector< FRCArea::RoadArea > mInns;
};

#endif // TILE_H
