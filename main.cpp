#include <QtGui/QApplication>
#include "mainwindow.h"

#include <iostream>

#include "tile.h"
#include "tileonboard.h"

int main(int argc, char *argv[])
{
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
    
//    return a.exec();
    std::vector< Tile::ContiguousMeadow > t1Meadows;
    std::vector< Tile::MeadowArea > t1firstMeadow;
    t1firstMeadow.push_back(Tile::RightTop);
    t1firstMeadow.push_back(Tile::LeftTop);
    std::vector< Tile::MeadowArea > t1secondMeadow;
    t1secondMeadow.push_back(Tile::RightBottom);
    t1secondMeadow.push_back(Tile::BottomRight);
    t1secondMeadow.push_back(Tile::BottomLeft);
    t1secondMeadow.push_back(Tile::LeftBottom);
    t1Meadows.push_back(t1firstMeadow);
    t1Meadows.push_back(t1secondMeadow);

    std::vector< Tile::ContiguousRoadOrCastle > t1Roads;
    std::vector< Tile::RoadAndCastleArea > t1firstRoad;
    t1firstRoad.push_back(Tile::Right);
    t1firstRoad.push_back(Tile::Left);
    t1Roads.push_back(t1firstRoad);

    std::vector< Tile::ContiguousRoadOrCastle > t1Castles;
    std::vector< Tile::RoadAndCastleArea > t1firstCastle;
    t1firstCastle.push_back(Tile::Top);
    t1Castles.push_back(t1firstCastle);

    Tile startTile = Tile(Tile::Castle, Tile::Road, Tile::Meadow, Tile::Road, Tile::Nothing,
                          t1Meadows, t1Roads, t1Castles);

    std::vector< Tile::ContiguousMeadow > t2Meadows;
    std::vector< Tile::MeadowArea > t2firstMeadow;
    t2firstMeadow.push_back(Tile::BottomRight);
    t2firstMeadow.push_back(Tile::BottomLeft);
    t2firstMeadow.push_back(Tile::LeftBottom);
    t2firstMeadow.push_back(Tile::LeftTop);
    t2Meadows.push_back(t2firstMeadow);

    std::vector< Tile::ContiguousRoadOrCastle > t2Roads;

    std::vector< Tile::ContiguousRoadOrCastle > t2Castles;
    std::vector< Tile::RoadAndCastleArea > t2firstCastle;
    t2firstCastle.push_back(Tile::Top);
    t2firstCastle.push_back(Tile::Right);
    t2Castles.push_back(t2firstCastle);

    Tile t2 = Tile(Tile::Castle, Tile::Castle, Tile::Meadow, Tile::Meadow, Tile::Nothing,
                          t2Meadows, t2Roads, t2Castles);

    std::cout << startTile.ToString();
    std::cout << t2.ToString();

    TileOnBoard t1ob = TileOnBoard(startTile, TileOnBoard::cw0);
    TileOnBoard t2ob = TileOnBoard(t2, TileOnBoard::cw90);

    if (t2ob.MatchesAbove(t1ob))
        std::cout << "Matches on top" << std::endl;
    if (t2ob.MatchesRightOf(t1ob))
        std::cout << "Matches to the right of" << std::endl;
    if (t2ob.MatchesBelow(t1ob))
        std::cout << "Matches below" << std::endl;
    if (t2ob.MatchesLeftOf(t1ob))
        std::cout << "Matches to the left of" << std::endl;

}
