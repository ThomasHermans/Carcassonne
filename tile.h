#ifndef TILE_H
#define TILE_H

#include <vector>
#include <string>

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

    enum FieldArea
    {
        TopLeft,
        TopRight,
        RightTop,
        RightBottom,
        BottomLeft,
        BottomRight,
        LeftBottom,
        LeftTop,
        Central
    };

    enum RoadOrCityArea
    {
        Top,
        Right,
        Bottom,
        Left
    };

    enum Area
    {
        AreaTopLeft,
        AreaTopRight,
        AreaRightTop,
        AreaRightBottom,
        AreaBottomLeft,
        AreaBottomRight,
        AreaLeftBottom,
        AreaLeftTop,
        AreaCentral,
        AreaTop,
        AreaRight,
        AreaBottom,
        AreaLeft
    };

    typedef std::vector< FieldArea > ContiguousField;
    typedef std::vector< RoadOrCityArea > ContiguousRoadOrCity;

    Tile();
    Tile(Side inTop, Side inRight, Side inBottom, Side inLeft, Center inCenter,
         std::vector< ContiguousField > inMeadows,
         std::vector< ContiguousRoadOrCity > inRoads,
         std::vector< ContiguousRoadOrCity > inCastles,
         std::vector< RoadOrCityArea > inShields);

    static std::string sideToString(Side inSide);
    static std::string centerToString(Center inCenter);
    static std::string fieldAreaToString(FieldArea inFieldArea);
    static std::string roadOrCityAreaToString(RoadOrCityArea inRoadOrCityArea);
    static std::string areaToString(Area inArea);

    Side getTop();
    Side getRight();
    Side getBottom();
    Side getLeft();
    Center getCenter();
    std::vector< ContiguousField > getContiguousFields();
    std::vector< ContiguousRoadOrCity > getContiguousRoads();
    std::vector< ContiguousRoadOrCity > getContiguousCities();

    std::string toString();

private:
    Side mTop;
    Side mRight;
    Side mBottom;
    Side mLeft;
    Center mCenter;
    std::vector< ContiguousField > mFields;
    std::vector< ContiguousRoadOrCity > mRoads;
    std::vector< ContiguousRoadOrCity > mCities;
    std::vector< RoadOrCityArea > mShields;
};

#endif // TILE_H
