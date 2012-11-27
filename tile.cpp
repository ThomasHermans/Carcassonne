#include "tile.h"

Tile::Tile():
    mTop(Meadow),
    mRight(Meadow),
    mBottom(Meadow),
    mLeft(Meadow),
    mCenter(Monastery)
{
    mMeadows = std::vector< ContiguousMeadow >();
    ContiguousMeadow allMeadows = std::vector< MeadowArea >();
    allMeadows.push_back(TopLeft);
    allMeadows.push_back(TopRight);
    allMeadows.push_back(RightTop);
    allMeadows.push_back(RightBottom);
    allMeadows.push_back(BottomRight);
    allMeadows.push_back(BottomLeft);
    allMeadows.push_back(LeftBottom);
    allMeadows.push_back(LeftTop);
    mMeadows.push_back(allMeadows);
    mRoads = std::vector< ContiguousRoadOrCastle >();
    mCastles = std::vector< ContiguousRoadOrCastle >();
}

Tile::Tile(Side inTop, Side inRight, Side inBottom, Side inLeft, Center inCenter, std::vector< ContiguousMeadow > inMeadows, std::vector< ContiguousRoadOrCastle > inRoads, std::vector< ContiguousRoadOrCastle > inCastles):
    mTop(inTop),
    mRight(inRight),
    mBottom(inBottom),
    mLeft(inLeft),
    mCenter(inCenter),
    mMeadows(inMeadows),
    mRoads(inRoads),
    mCastles(inCastles)
{
}

Tile::Side
Tile::GetTop()
{
    return mTop;
}

Tile::Side
Tile::GetRight()
{
    return mRight;
}

Tile::Side
Tile::GetBottom()
{
    return mBottom;
}

Tile::Side
Tile::GetLeft()
{
    return mLeft;
}

Tile::Center
Tile::GetCenter()
{
    return mCenter;
}

std::vector< Tile::ContiguousMeadow >
Tile::GetContiguousMeadows()
{
    return mMeadows;
}

std::vector< Tile::ContiguousRoadOrCastle >
Tile::GetContiguousRoads()
{
    return mRoads;
}

std::vector< Tile::ContiguousRoadOrCastle >
Tile::GetContiguousCastles()
{
    return mCastles;
}

std::string
Tile::ToString()
{
    std::string result;
    result.append("Top: ");
    result.append(SideToString(this->GetTop()));
    result.append("\tRight: ");
    result.append(SideToString(this->GetRight()));
    result.append("\tBottom: ");
    result.append(SideToString(this->GetBottom()));
    result.append("\tLeft: ");
    result.append(SideToString(this->GetLeft()));
    result.append("\tCenter: ");
    result.append(CenterToString(this->GetCenter()));
    result.append("\nContiguous meadows:");
    for (unsigned int i = 0; i < mMeadows.size(); i++)
    {
        result.append("\n- ");
        for (unsigned int j = 0; j < mMeadows[i].size(); j++)
        {
            result.append(MeadowAreaToString(mMeadows[i][j]));
            result.append(" ");
        }
    }
    result.append("\nContiguous roads:");
    for (unsigned int i = 0; i < mRoads.size(); i++)
    {
        result.append("\n- ");
        for (unsigned int j = 0; j < mRoads[i].size(); j++)
        {
            result.append(RoadAndCastleAreaToString(mRoads[i][j]));
            result.append(" ");
        }
    }
    result.append("\nContiguous castles:");
    for (unsigned int i = 0; i < mCastles.size(); i++)
    {
        result.append("\n- ");
        for (unsigned int j = 0; j < mCastles[i].size(); j++)
        {
            result.append(RoadAndCastleAreaToString(mCastles[i][j]));
            result.append(" ");
        }
    }
    result.append("\n");
    return result;
}

std::string
Tile::SideToString(Side inSide)
{
    switch (inSide)
    {
    case Meadow:
        return "Meadow";
    case Road:
        return "Road";
    case Castle:
        return "Castle";
    default:
        return "No valid side";
    }
}

std::string
Tile::CenterToString(Center inCenter)
{
    switch (inCenter)
    {
    case Nothing:
        return "Nothing";
    case Monastery:
        return "Monastery";
    default:
        return "No valid center";
    }
}

std::string
Tile::MeadowAreaToString(MeadowArea inMeadowArea)
{
    switch (inMeadowArea)
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
    default:
        return "No valid MeadowArea";
    }
}

std::string
Tile::RoadAndCastleAreaToString(RoadAndCastleArea inRoadAndCastleArea)
{
    switch (inRoadAndCastleArea)
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
        return "No valid RoadAndCastleArea";
    }
}
