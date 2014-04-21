#ifndef TILEONBOARD_H
#define TILEONBOARD_H

#include "Model/Color.h"
#include "Model/Rotation.h"
#include "PlacedPiece.h"
#include "Tile.h"

#include <utility>
#include <vector>

namespace Model
{
	class TileOnBoard
	{
	public:
		TileOnBoard();
		TileOnBoard( const Tile & inTile, Rotation inRotation );

		Tile::Side getTop() const;
		Tile::Side getRight() const;
		Tile::Side getBottom() const;
		Tile::Side getLeft() const;
		Tile::Center getCenter() const;
		std::string const & getID() const;
		Tile const & getTile() const;
		Model::Rotation getRotation() const;
		std::vector< PlacedPiece > const & getPlacedPieces() const;

		/**
		 *	Does this TileOnBoard have an inn next to the specified Area?
		 */
		bool hasInn( Area::Area inArea ) const;

		bool matchesAbove( TileOnBoard const & inTileOnBoard ) const;
		bool matchesRightOf( TileOnBoard const & inTileOnBoard ) const;
		bool matchesBelow( TileOnBoard const & inTileOnBoard ) const;
		bool matchesLeftOf( TileOnBoard const & inTileOnBoard ) const;

		std::vector< ContiguousField > getContiguousFields() const;
		std::vector< ContiguousRoad > getContiguousRoads() const;
		std::vector< ContiguousCity > getContiguousCities() const;

		bool isCloister( Area::Area inArea ) const;
		bool isRoad( Area::Area inArea ) const;
		bool isCity( Area::Area inArea ) const;
		bool isField( Area::Area inArea ) const;

		ContiguousField getContiguousField( Area::Area inFieldArea ) const;
		ContiguousRoad getContiguousRoad( Area::Area inArea ) const;
		ContiguousCity getContiguousCity( Area::Area inArea ) const;

		std::vector< ContiguousCity > getCitiesPerField( Area::Area inFieldArea ) const;

		bool placePiece( PlacedPiece const & inPlacedPiece);
		bool hasPieces() const;
		bool hasPiece( Area::Area inArea ) const;
		std::vector< PlacedPiece > removePieces( Area::Area inArea );
		std::vector< PlacedPiece > removeAllPieces();

		std::vector< Area::Area > getShields() const;
		std::vector< Area::Area > getInns() const;

		std::string toString() const;

	private:
		Tile mTile;
		Rotation mRotation;
		std::vector< PlacedPiece > mPlacedPieces;
	};
}

#endif // TILEONBOARD_H
