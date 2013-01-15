#include <QtGui/QApplication>
#include "mainwindow.h"

#include <iostream>

#include "gamecontroller.h"

int main(int argc, char * argv[])
{

//    std::vector< Tile > tiles = createBaseGameTiles();

//    for (unsigned int i = 0; i < tiles.size(); ++i)
//    {
//        std::cout << i << ":\t" << tiles[i].toString() << std::endl;
//    }

    QApplication a(argc, argv);
    GameController gc;

    return a.exec();
}
