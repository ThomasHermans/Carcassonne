#ifndef SETUPBASEGAME_H
#define SETUPBASEGAME_H

#include "Model/Color.h"
#include "Model/src/PlacedPiece.h"
#include "Model/src/Tile.h"

#include <string>
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
Tile createTileM();
Tile createTileN();
Tile createTileO();
Tile createTileP();
Tile createTileQ();
Tile createTileR();
Tile createTileS();
Tile createTileT();
Tile createTileU();
Tile createTileV();
Tile createTileW();
Tile createTileX();

Tile createTile( char inChar );
}

/*
 * returns a shuffled vector of Tiles containing all of the base game tiles, having the start Tile on the back.
 * This means that Tiles should be removed using pop_back.
 */
std::vector< Tile > createBaseGameTiles();

std::vector< Tile > createTiles( std::string const & inTiles );

std::vector< Piece > createBaseGamePieces( Color::Color inColor );

#endif // SETUPBASEGAME_H
