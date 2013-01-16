#ifndef SETUPBASEGAME_H
#define SETUPBASEGAME_H

#include "tile.h"

#include <vector>

namespace cbgt
{
Tile createTileA();
Tile createTileB();
Tile createTileC();
Tile createTileD();
Tile createTileE();
Tile createTileF();
Tile createTileG();
Tile createTileH();
Tile createTileI();
Tile createTileJ();
Tile createTileK();
Tile createTileL();

Tile createTileN();
}

/*
  * returns a shuffled vector of Tiles containing all of the base game tiles, having the start Tile on the back.
  * This means that Tiles should be removed using pop_back.
  */
std::vector< Tile > createBaseGameTiles();

#endif // SETUPBASEGAME_H
