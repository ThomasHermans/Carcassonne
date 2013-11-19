#ifndef TILE_H
#define TILE_H

#include "Area.h"

#include <map>
#include <string>
#include <vector>

class Tile
{
public:
	enum Side
	{
		kSideField,
		kSideRoad,
		kSideCity
	};

	enum Center
	{
		kCenterNothing,
		kCenterCloister
	};

	Tile();
	Tile
	(
		Center inCenter,
		std::string const & inID,
		std::vector< ContiguousField > const & inFields,
		std::vector< ContiguousRoad > const & inRoads,
		std::vector< ContiguousCity > const & inCities,
		std::map< ContiguousField, std::vector< ContiguousCity > > const & inCitiesPerField,
		std::vector< Area::Area > const & inShields = std::vector< Area::Area >(),
		std::vector< Area::Area > const & inInns = std::vector< Area::Area >()
	);


	static std::string sideToString(Side inSide);
	static std::string centerToString(Center inCenter);

	Side getTop() const;
	Side getRight() const;
	Side getBottom() const;
	Side getLeft() const;
	Center getCenter() const;
	std::string const & getID() const;
	std::vector< ContiguousField > const & getContiguousFields() const;
	std::vector< ContiguousRoad > const & getContiguousRoads() const;
	std::vector< ContiguousCity > const & getContiguousCities() const;
	std::vector< ContiguousCity > const & getCitiesPerField(const ContiguousField & inContiguousField) const;
	std::vector< ContiguousCity > const & getCitiesPerField( Area::Area inFieldArea ) const;
	std::vector< Area::Area > const & getShields() const;
	std::vector< Area::Area > const & getInns() const;

	ContiguousField getContiguousField( Area::Area inFieldArea ) const;
	ContiguousRoad getContiguousRoad(Area::Area inArea) const;
	ContiguousCity getContiguousCity(Area::Area inArea) const;

	bool isCloister( Area::Area inArea ) const;
	bool isRoad( Area::Area inArea ) const;
	bool isCity( Area::Area inArea ) const;
	bool isField( Area::Area inArea ) const;

	std::string toString() const;

private:
	std::string mID;
	Side mTop;
	Side mRight;
	Side mBottom;
	Side mLeft;
	Center mCenter;
	std::vector< ContiguousField > mFields;
	std::vector< ContiguousRoad > mRoads;
	std::vector< ContiguousCity > mCities;
	std::map< ContiguousField, std::vector< ContiguousCity > > mCitiesPerField;
	std::vector< Area::Area > mShields;
	std::vector< Area::Area > mInns;
};

#endif // TILE_H
