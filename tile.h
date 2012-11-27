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
        Cloister
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
        LeftTop
    };

    enum RoadAndCityArea
    {
        Top,
        Right,
        Bottom,
        Left
    };

    typedef std::vector< FieldArea > ContiguousField;
    typedef std::vector< RoadAndCityArea > ContiguousRoadOrCity;

    Tile();
    Tile(Side inTop, Side inRight, Side inBottom, Side inLeft, Center inCenter,
         std::vector< ContiguousField > inMeadows,
         std::vector< ContiguousRoadOrCity > inRoads,
         std::vector< ContiguousRoadOrCity > inCastles);

    static std::string sideToString(Side inSide);
    static std::string centerToString(Center inCenter);
    static std::string fieldAreaToString(FieldArea inFieldArea);
    static std::string roadAndCityAreaToString(RoadAndCityArea inRoadAndCityArea);

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
};

#endif // TILE_H
