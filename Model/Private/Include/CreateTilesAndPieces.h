#ifndef SETUPBASEGAME_H
#define SETUPBASEGAME_H

#include "Model/PlacedPiece.h"
#include "Tile.h"

#include "Model/Color.h"

#include "Utils/Typedefs.h"

#include <set>
#include <string>
#include <vector>

namespace Model
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

	Tile createTileEA();
	Tile createTileEB();
	Tile createTileEC();
	Tile createTileED();
	Tile createTileEE();
	Tile createTileEF();
	Tile createTileEG();
	Tile createTileEH();
	Tile createTileEI();
	Tile createTileEJ();
	Tile createTileEK();
	Tile createTileEL();
	Tile createTileEM();
	Tile createTileEN();
	Tile createTileEO();
	Tile createTileEP();
	Tile createTileEQ();

	/*
	 *	Create a shuffled list of Tiles containing all of the base game
	 *	tiles, having the start Tile on the back.
	 *	This means that Tiles should be removed using pop_back.
	 */
	std::vector< Tile >
	createBaseGameTiles();

	/*
	 *	Create a shuffled list of Tiles containing all of the tiles of
	 *	the expansion.
	 */
	std::vector< Tile >
	createTheExpansionTiles();

	/**
	 *	Create a shuffled list of tiles according the specified
	 *	set of expansions.
	 *	The start tile will be on the back of the list.
	 */
	std::vector< Tile >
	createTiles( std::set< Utils::Expansion::Type > const & inExpansions );

	/**
	 *	Create a list of tiles according to the specified string.
	 */
	std::vector< Tile >
	createTiles( std::string const & inTiles );

	/**
	 *	Create the correct pieces according to the specified
	 *	set of expansions.
	 */
	std::map< Piece::PieceType, std::size_t >
	createPieces( std::set< Utils::Expansion::Type > const & inExpansions );
}

#endif // SETUPBASEGAME_H
