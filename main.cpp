#include <QtGui/QApplication>
#include "mainwindow.h"

#include <iostream>

#include "createbasegametiles.h"
#include "tile.h"
#include "tileonboard.h"

int main(int argc, char * argv[])
{

    std::vector< Tile > tiles = createBaseGameTiles();

    for (unsigned int i = 0; i < tiles.size(); ++i)
    {
        std::cout << i << ":\t" << tiles[i].toString() << std::endl;
    }

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
