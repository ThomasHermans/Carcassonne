#ifndef TILEONBOARD_H
#define TILEONBOARD_H

#include "src-model/Color.h"
#include "src-model/PlacedPiece.h"
#include "src-model/Tile.h"

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
	std::string getID() const;
	Tile getTile() const;
	TileOnBoard::Rotation getRotation() const;
	std::vector< PlacedPiece > const & getPlacedPieces() const;

	bool matchesAbove(const TileOnBoard & inTileOnBoard) const;
	bool matchesRightOf(const TileOnBoard & inTileOnBoard) const;
	bool matchesBelow(const TileOnBoard & inTileOnBoard) const;
	bool matchesLeftOf(const TileOnBoard & inTileOnBoard) const;

	std::vector< Tile::ContiguousField > getContiguousFields() const;
	std::vector< Tile::ContiguousRoad > getContiguousRoads() const;
	std::vector< Tile::ContiguousCity > getContiguousCities() const;

	Tile::ContiguousField getContiguousField(FRCArea::FieldArea inFieldArea) const;
	Tile::ContiguousRoad getContiguousRoad(FRCArea::RoadArea inRoadArea) const;
	Tile::ContiguousCity getContiguousCity(FRCArea::CityArea inCityArea) const;

	std::vector< Tile::ContiguousCity > getCitiesPerField(FRCArea::FieldArea inFieldArea) const;

	bool placePiece(const PlacedPiece & inPlacedPiece);
	bool hasPieces() const;
	std::vector< PlacedPiece > removePieces( Area::Area inArea );
	std::vector< PlacedPiece > removeAllPieces();

	std::vector< Tile::ContiguousRoad > getFinishedRoads() const;
	std::vector< Tile::ContiguousCity > getFinishedCities() const;

	std::vector< FRCArea::CityArea > getShields() const;
	std::vector< FRCArea::RoadArea > getInns() const;

	bool isFieldOccupied( FRCArea::FieldArea inFieldArea ) const;
	bool isRoadOccupied( FRCArea::RoadArea inRoadArea ) const;
	bool isCityOccupied( FRCArea::CityArea inCityArea ) const;
	bool isCloisterOccupied() const;

	bool isRoadFinished( FRCArea::RoadArea inRoadArea ) const;
	bool isCityFinished( FRCArea::CityArea inCityArea ) const;
	bool isCloisterFinished() const;

	void occupyArea( Area::Area inArea );
	void occupyField( FRCArea::FieldArea inFieldArea );
	void occupyRoad( FRCArea::RoadArea inRoadArea );
	void occupyCity( FRCArea::CityArea inCityArea );
	void occupyCloister();

	void finishRoad( FRCArea::RoadArea inRoadArea );
	void finishCity( FRCArea::CityArea inCityArea );
	void finishCloister();

	std::string toString() const;

private:
	bool isOccupiedArea( Area::Area inArea ) const;
	bool isRoadOrCity( FRCArea::RoadArea inArea, Tile::Side inSide ) const;

private:
	Tile mTile;
	Rotation mRotation;
	std::vector< Tile::ContiguousField > mOccupiedFields;
	std::vector< Tile::ContiguousRoad > mOccupiedRoads;
	std::vector< Tile::ContiguousRoad > mFinishedRoads;
	std::vector< Tile::ContiguousCity > mOccupiedCities;
	std::vector< Tile::ContiguousCity > mFinishedCities;
	bool mCloisterFinished;
	std::vector< PlacedPiece > mPlacedPieces;
};

#endif // TILEONBOARD_H
