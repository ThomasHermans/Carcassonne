#ifndef TILEONBOARD_H
#define TILEONBOARD_H

#include "Model/Color.h"
#include "PlacedPiece.h"
#include "Tile.h"

#include <utility>
#include <vector>

class TileOnBoard
{
public:
	enum Rotation
	{
		cw0 = 0,
		cw90 = 3,
		cw180 = 6,
		cw270 = 9
	};

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
	TileOnBoard::Rotation getRotation() const;
	std::vector< PlacedPiece > const & getPlacedPieces() const;

	bool matchesAbove( TileOnBoard const & inTileOnBoard ) const;
	bool matchesRightOf( TileOnBoard const & inTileOnBoard ) const;
	bool matchesBelow( TileOnBoard const & inTileOnBoard ) const;
	bool matchesLeftOf( TileOnBoard const & inTileOnBoard ) const;

	std::vector< Tile::ContiguousField > getContiguousFields() const;
	std::vector< Tile::ContiguousRoad > getContiguousRoads() const;
	std::vector< Tile::ContiguousCity > getContiguousCities() const;

	bool isCloister( Area::Area inArea ) const;
	bool isRoad( Area::Area inArea ) const;
	bool isCity( Area::Area inArea ) const;

	Tile::ContiguousField getContiguousField( FRCArea::FieldArea inFieldArea ) const;
	Tile::ContiguousRoad getContiguousRoad( FRCArea::RoadArea inRoadArea ) const;
	Tile::ContiguousCity getContiguousCity( FRCArea::CityArea inCityArea ) const;

	std::vector< Tile::ContiguousCity > getCitiesPerField( FRCArea::FieldArea inFieldArea ) const;

	bool placePiece( PlacedPiece const & inPlacedPiece);
	bool hasPieces() const;
	bool hasPiece( Area::Area inArea ) const;
	std::vector< PlacedPiece > removePieces( Area::Area inArea );
	std::vector< PlacedPiece > removeAllPieces();

	std::vector< FRCArea::CityArea > getShields() const;
	std::vector< FRCArea::RoadArea > getInns() const;

	std::string toString() const;

private:
	Tile mTile;
	Rotation mRotation;
	std::vector< PlacedPiece > mPlacedPieces;
};

#endif // TILEONBOARD_H
