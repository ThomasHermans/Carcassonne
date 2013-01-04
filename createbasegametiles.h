#ifndef SETUPBASEGAME_H
#define SETUPBASEGAME_H

#include "tile.h"

#include <vector>

/*
  * returns a shuffled vector of Tiles containing all of the base game tiles, having the start Tile on the back.
  * This means that Tiles should be removed using pop_back.
  */
std::vector< Tile > createBaseGameTiles();

#endif // SETUPBASEGAME_H
