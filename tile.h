#ifndef TILE_H
#define TILE_H

#include <vector>
#include <string>

class Tile
{
public:
    enum Side
    {
        Meadow,
        Road,
        Castle
    };

    enum Center
    {
        Nothing,
        Monastery
    };

    enum MeadowArea
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

    enum RoadAndCastleArea
    {
        Top,
        Right,
        Bottom,
        Left
    };

    typedef std::vector< MeadowArea > ContiguousMeadow;
    typedef std::vector< RoadAndCastleArea > ContiguousRoadOrCastle;

    Tile();
    Tile(Side inTop, Side inRight, Side inBottom, Side inLeft, Center inCenter, std::vector< ContiguousMeadow > inMeadows, std::vector< ContiguousRoadOrCastle > inRoads, std::vector< ContiguousRoadOrCastle > inCastles);

    static std::string SideToString(Side inSide);
    static std::string CenterToString(Center inCenter);
    static std::string MeadowAreaToString(MeadowArea inMeadowArea);
    static std::string RoadAndCastleAreaToString(RoadAndCastleArea inRoadAndCastleArea);

    Side GetTop();
    Side GetRight();
    Side GetBottom();
    Side GetLeft();
    Center GetCenter();
    std::vector< ContiguousMeadow > GetContiguousMeadows();
    std::vector< ContiguousRoadOrCastle > GetContiguousRoads();
    std::vector< ContiguousRoadOrCastle > GetContiguousCastles();

    std::string ToString();

private:
    Side mTop;
    Side mRight;
    Side mBottom;
    Side mLeft;
    Center mCenter;
    std::vector< ContiguousMeadow > mMeadows;
    std::vector< ContiguousRoadOrCastle > mRoads;
    std::vector< ContiguousRoadOrCastle > mCastles;
};

#endif // TILE_H
