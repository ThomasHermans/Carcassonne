#include <QtGui/QApplication>
#include "mainwindow.h"

#include <iostream>

#include "tile.h"
#include "tileonboard.h"

int main(int argc, char * argv[])
{
    std::vector< Tile::ContiguousField > t1Fields;
    std::vector< FieldArea::FieldArea > t1firstField;
    t1firstField.push_back(FieldArea::RightTop);
    t1firstField.push_back(FieldArea::LeftTop);
    std::vector< FieldArea::FieldArea > t1secondField;
    t1secondField.push_back(FieldArea::RightBottom);
    t1secondField.push_back(FieldArea::BottomRight);
    t1secondField.push_back(FieldArea::BottomLeft);
    t1secondField.push_back(FieldArea::LeftBottom);
    t1Fields.push_back(t1firstField);
    t1Fields.push_back(t1secondField);

    std::vector< Tile::ContiguousRoadOrCity > t1Roads;
    std::vector< RoadOrCityArea::RoadOrCityArea > t1firstRoad;
    t1firstRoad.push_back(RoadOrCityArea::Right);
    t1firstRoad.push_back(RoadOrCityArea::Left);
    t1Roads.push_back(t1firstRoad);

    std::vector< Tile::ContiguousRoadOrCity > t1Cities;
    std::vector< RoadOrCityArea::RoadOrCityArea > t1firstCity;
    t1firstCity.push_back(RoadOrCityArea::Top);
    t1Cities.push_back(t1firstCity);

    std::vector< RoadOrCityArea::RoadOrCityArea > t1Shields;

    Tile startTile = Tile(Tile::City, Tile::Road, Tile::Field, Tile::Road, Tile::Nothing,
                          t1Fields, t1Roads, t1Cities, t1Shields);

    std::vector< Tile::ContiguousField > t2Fields;
    std::vector< FieldArea::FieldArea > t2firstField;
    t2firstField.push_back(FieldArea::BottomRight);
    t2firstField.push_back(FieldArea::BottomLeft);
    t2firstField.push_back(FieldArea::LeftBottom);
    t2firstField.push_back(FieldArea::LeftTop);
    t2Fields.push_back(t2firstField);

    std::vector< Tile::ContiguousRoadOrCity > t2Roads;

    std::vector< Tile::ContiguousRoadOrCity > t2Cities;
    std::vector< RoadOrCityArea::RoadOrCityArea > t2firstCity;
    t2firstCity.push_back(RoadOrCityArea::Top);
    t2firstCity.push_back(RoadOrCityArea::Right);
    t2Cities.push_back(t2firstCity);

    std::vector< RoadOrCityArea::RoadOrCityArea > t2Shields;

    Tile t2 = Tile(Tile::City, Tile::City, Tile::Field, Tile::Field, Tile::Nothing,
                          t2Fields, t2Roads, t2Cities, t2Shields);

    std::cout << startTile.toString();
    std::cout << t2.toString();

    TileOnBoard t1ob = TileOnBoard(startTile, TileOnBoard::cw0);
    TileOnBoard t2ob = TileOnBoard(t2, TileOnBoard::cw90);

    std::cout << t1ob.toString();
    std::cout << t2ob.toString();

    if (t2ob.matchesAbove(t1ob))
        std::cout << "Matches on top" << std::endl;
    if (t2ob.matchesRightOf(t1ob))
        std::cout << "Matches to the right of" << std::endl;
    if (t2ob.matchesBelow(t1ob))
        std::cout << "Matches below" << std::endl;
    if (t2ob.matchesLeftOf(t1ob))
        std::cout << "Matches to the left of" << std::endl;

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
