#include "src-model/TileOnBoard.h"

#include <algorithm>
#include <cassert>

namespace
{
FRCArea::FieldArea turn(FRCArea::FieldArea inFieldArea, TileOnBoard::Rotation inRotation)
{
	if (inFieldArea == FRCArea::Central)
		return FRCArea::Central;
	else
		return FRCArea::FieldArea((inFieldArea + inRotation) % 12);
}

FRCArea::FieldArea unturn(FRCArea::FieldArea inFieldArea, TileOnBoard::Rotation inRotation)
{
	if (inFieldArea == FRCArea::Central)
		return FRCArea::Central;
	else
		return FRCArea::FieldArea((inFieldArea + 12 - inRotation) % 12);
}

FRCArea::RoadArea turn(FRCArea::RoadArea inRoadArea, TileOnBoard::Rotation inRotation)
{
	return FRCArea::RoadArea((inRoadArea + inRotation) % 12);
}

FRCArea::RoadArea unturn(FRCArea::RoadArea inRoadArea, TileOnBoard::Rotation inRotation)
{
	return FRCArea::RoadArea((inRoadArea + 12 - inRotation) % 12);
}
}

TileOnBoard::TileOnBoard()
:
	mTile( Tile::Tile() ),
	mRotation( TileOnBoard::cw0 ),
	mOccupiedFields(),
	mOccupiedRoads(),
	mFinishedRoads(),
	mOccupiedCities(),
	mFinishedCities(),
	mCloisterFinished( false ),
	mPlacedPieces()
{
}

TileOnBoard::TileOnBoard( const Tile &inTile, Rotation inRotation )
:
	mTile( inTile ),
	mRotation( inRotation ),
	mOccupiedFields(),
	mOccupiedRoads(),
	mFinishedRoads(),
	mOccupiedCities(),
	mFinishedCities(),
	mCloisterFinished( false ),
	mPlacedPieces()
{
}

Tile::Side
TileOnBoard::getTop() const
{
	switch (mRotation)
	{
	case cw90:
		return mTile.getLeft();
	case cw180:
		return mTile.getBottom();
	case cw270:
		return mTile.getRight();
	case cw0:
	default:
		return mTile.getTop();
	}
}

Tile::Side
TileOnBoard::getRight() const
{
	switch (mRotation)
	{
	case cw90:
		return mTile.getTop();
	case cw180:
		return mTile.getLeft();
	case cw270:
		return mTile.getBottom();
	case cw0:
	default:
		return mTile.getRight();
	}
}

Tile::Side
TileOnBoard::getBottom() const
{
	switch (mRotation)
	{
	case cw90:
		return mTile.getRight();
	case cw180:
		return mTile.getTop();
	case cw270:
		return mTile.getLeft();
	case cw0:
	default:
		return mTile.getBottom();
	}
}

Tile::Side
TileOnBoard::getLeft() const
{
	switch (mRotation)
	{
	case cw90:
		return mTile.getBottom();
	case cw180:
		return mTile.getRight();
	case cw270:
		return mTile.getTop();
	case cw0:
	default:
		return mTile.getLeft();
	}
}

Tile::Center
TileOnBoard::getCenter() const
{
	return mTile.getCenter();
}

std::string
TileOnBoard::getID() const
{
	return mTile.getID();
}

Tile
TileOnBoard::getTile() const
{
	return mTile;
}

TileOnBoard::Rotation
TileOnBoard::getRotation() const
{
	return mRotation;
}

std::vector< PlacedPiece > const &
TileOnBoard::getPlacedPieces() const
{
	return mPlacedPieces;
}

bool
TileOnBoard::matchesAbove(const TileOnBoard & inTileOnBoard) const
{
	return (this->getBottom() == inTileOnBoard.getTop());
}

bool
TileOnBoard::matchesRightOf(const TileOnBoard & inTileOnBoard) const
{
	return (this->getLeft() == inTileOnBoard.getRight());
}

bool
TileOnBoard::matchesBelow(const TileOnBoard & inTileOnBoard) const
{
	return (this->getTop() == inTileOnBoard.getBottom());
}

bool
TileOnBoard::matchesLeftOf(const TileOnBoard & inTileOnBoard) const
{
	return (this->getRight() == inTileOnBoard.getLeft());
}

std::vector< Tile::ContiguousField >
TileOnBoard::getContiguousFields() const
{
	std::vector< Tile::ContiguousField > res;
	std::vector< Tile::ContiguousField > orig = mTile.getContiguousFields();
	for (unsigned int i = 0; i < orig.size(); ++i)
	{
		Tile::ContiguousField restemp;
		Tile::ContiguousField origtemp = orig[i];
		for (unsigned int j = 0; j < origtemp.size(); ++j)
		{
			restemp.push_back(turn(origtemp[j], mRotation));
		}
		res.push_back(restemp);
	}
	return res;
}

std::vector< Tile::ContiguousRoad >
TileOnBoard::getContiguousRoads() const
{
	std::vector< Tile::ContiguousRoad > res;
	std::vector< Tile::ContiguousRoad > orig = mTile.getContiguousRoads();
	for (unsigned int i = 0; i < orig.size(); ++i)
	{
		Tile::ContiguousRoad restemp;
		Tile::ContiguousRoad origtemp = orig[i];
		for (unsigned int j = 0; j < origtemp.size(); ++j)
		{
			restemp.push_back(turn(origtemp[j], mRotation));
		}
		res.push_back(restemp);
	}
	return res;
}

std::vector< Tile::ContiguousCity >
TileOnBoard::getContiguousCities() const
{
	std::vector< Tile::ContiguousCity > res;
	std::vector< Tile::ContiguousCity > orig = mTile.getContiguousCities();
	for (unsigned int i = 0; i < orig.size(); ++i)
	{
		Tile::ContiguousCity restemp;
		Tile::ContiguousCity origtemp = orig[i];
		for (unsigned int j = 0; j < origtemp.size(); ++j)
		{
			restemp.push_back(turn(origtemp[j], mRotation));
		}
		res.push_back(restemp);
	}
	return res;
}

Tile::ContiguousField
TileOnBoard::getContiguousField(FRCArea::FieldArea inFieldArea) const
{
	FRCArea::FieldArea unRotatedArea = unturn(inFieldArea, mRotation);
	Tile::ContiguousField unRotatedField = mTile.getContiguousField(unRotatedArea);
	Tile::ContiguousField rotatedField;
	for (unsigned int i = 0; i < unRotatedField.size(); ++i)
	{
		rotatedField.push_back(turn(unRotatedField[i], mRotation));
	}
	return rotatedField;
}

Tile::ContiguousRoad
TileOnBoard::getContiguousRoad(FRCArea::RoadArea inRoadArea) const
{
	FRCArea::RoadArea unRotatedArea = unturn(inRoadArea, mRotation);
	Tile::ContiguousRoad unRotatedRoad = mTile.getContiguousRoad(unRotatedArea);
	Tile::ContiguousRoad rotatedRoad;
	for (unsigned int i = 0; i < unRotatedRoad.size(); ++i)
	{
		rotatedRoad.push_back(turn(unRotatedRoad[i], mRotation));
	}
	return rotatedRoad;
}

Tile::ContiguousCity
TileOnBoard::getContiguousCity(FRCArea::CityArea inCityArea) const
{
	FRCArea::CityArea unRotatedArea = unturn(inCityArea, mRotation);
	Tile::ContiguousCity unRotatedCity = mTile.getContiguousCity(unRotatedArea);
	Tile::ContiguousCity rotatedCity;
	for (unsigned int i = 0; i < unRotatedCity.size(); ++i)
	{
		rotatedCity.push_back(turn(unRotatedCity[i], mRotation));
	}
	return rotatedCity;
}

std::vector< Tile::ContiguousCity >
TileOnBoard::getCitiesPerField(FRCArea::FieldArea inFieldArea) const
{
	FRCArea::FieldArea tileFieldArea = turn(inFieldArea, TileOnBoard::Rotation((mRotation + 9) % 12));
	std::vector< Tile::ContiguousCity > tileCities = mTile.getCitiesPerField(tileFieldArea);
	std::vector< Tile::ContiguousCity > cities;
	for (unsigned int i = 0; i < tileCities.size(); ++i)
	{
		Tile::ContiguousCity cityTemp;
		Tile::ContiguousCity tileCityTemp = tileCities[i];
		for (unsigned int j = 0; j < tileCityTemp.size(); ++j)
		{
			cityTemp.push_back(turn(tileCityTemp[j], mRotation));
		}
		cities.push_back(cityTemp);
	}
	return cities;
}

bool
TileOnBoard::placePiece( PlacedPiece const & inPlacedPiece )
{
	Area::Area area = inPlacedPiece.getArea();
	if ( !isOccupiedArea( area ) )
	{
		occupyArea( area );
		mPlacedPieces.push_back( inPlacedPiece );
		return true;
	}
	return false;
}

bool
TileOnBoard::hasPieces() const
{
	return !mPlacedPieces.empty();
}

std::vector< PlacedPiece >
TileOnBoard::removePieces( Area::Area inArea )
{
	std::vector< PlacedPiece > result;
	std::vector< PlacedPiece >::iterator it = mPlacedPieces.begin();
	while ( it != mPlacedPieces.end() )
	{
		if ( it->getArea() == inArea )
		{
			result.push_back( *it );
			it = mPlacedPieces.erase( it );
		}
		else
		{
			++it;
		}
	}
	return result;
}

std::vector< PlacedPiece >
TileOnBoard::removeAllPieces()
{
	std::vector< PlacedPiece > result = mPlacedPieces;
	mPlacedPieces.clear();
	return result;
}

std::vector< Tile::ContiguousRoad >
TileOnBoard::getFinishedRoads() const
{
	return mFinishedRoads;
}

std::vector< Tile::ContiguousCity >
TileOnBoard::getFinishedCities() const
{
	return mFinishedCities;
}

std::vector< FRCArea::CityArea >
TileOnBoard::getShields() const
{
	std::vector< FRCArea::CityArea > res;
	std::vector< FRCArea::CityArea > orig = mTile.getShields();
	for (unsigned int i = 0; i < orig.size(); ++i)
	{
		res.push_back(turn(orig[i], mRotation));
	}
	return res;
}

std::vector< FRCArea::RoadArea >
TileOnBoard::getInns() const
{
	std::vector< FRCArea::RoadArea > res;
	std::vector< FRCArea::RoadArea > orig = mTile.getInns();
	for ( unsigned int i = 0; i < orig.size(); ++i )
	{
		res.push_back(turn(orig[i], mRotation));
	}
	return res;
}

bool
TileOnBoard::isFieldOccupied(FRCArea::FieldArea inFieldArea) const
{
	bool occupied = false;
	for ( unsigned int i = 0; i < mOccupiedFields.size(); ++i )
	{
		if ( std::find(mOccupiedFields[i].begin(), mOccupiedFields[i].end(), inFieldArea) != mOccupiedFields[i].end() )
		{
			occupied = true;
			break;
		}
	}
	return occupied;
}

bool
TileOnBoard::isRoadOccupied(FRCArea::RoadArea inRoadArea) const
{
	bool occupied = false;
	for ( unsigned int i = 0; i < mOccupiedRoads.size(); ++i )
	{
		if ( std::find(mOccupiedRoads[i].begin(), mOccupiedRoads[i].end(), inRoadArea) != mOccupiedRoads[i].end() )
		{
			occupied = true;
			break;
		}
	}
	return occupied;
}

bool
TileOnBoard::isCityOccupied(FRCArea::CityArea inCityArea) const
{
	bool occupied = false;
	for ( unsigned int i = 0; i < mOccupiedCities.size(); ++i )
	{
		if ( std::find(mOccupiedCities[i].begin(), mOccupiedCities[i].end(), inCityArea) != mOccupiedCities[i].end() )
		{
			occupied = true;
			break;
		}
	}
	return occupied;
}

bool
TileOnBoard::isCloisterOccupied() const
{
	if ( mTile.getCenter() == Tile::Cloister )
	{
		for ( std::vector< PlacedPiece >::const_iterator it = mPlacedPieces.begin();
			it != mPlacedPieces.end();
			++it )
		{
			if ( it->getArea() == Area::Central )
			{
				return true;
			}
		}
	}
	return false;
}

bool
TileOnBoard::isRoadFinished(FRCArea::RoadArea inRoadArea) const
{
	bool finished = false;
	for (unsigned int i = 0; i < mFinishedRoads.size(); ++i)
	{
		if ( std::find(mFinishedRoads[i].begin(), mFinishedRoads[i].end(), inRoadArea) != mFinishedRoads[i].end() )
		{
			finished = true;
			break;
		}
	}
	return finished;
}

bool
TileOnBoard::isCityFinished(FRCArea::CityArea inCityArea) const
{
	bool finished = false;
	for (unsigned int i = 0; i < mFinishedCities.size(); ++i)
	{
		if ( std::find(mFinishedCities[i].begin(), mFinishedCities[i].end(), inCityArea) != mFinishedCities[i].end() )
		{
			finished = true;
			break;
		}
	}
	return finished;
}

bool
TileOnBoard::isCloisterFinished() const
{
	return mCloisterFinished;
}

void
TileOnBoard::occupyArea( Area::Area inArea )
{
	switch ( inArea )
	{
		case Area::Top:	// deliberate fallthroughs, RoadAreas
		case Area::Right:
		case Area::Bottom:
		case Area::Left:
		{
			if ( isRoadOrCity( FRCArea::RoadArea( inArea ), Tile::Road ) )
			{
				occupyRoad( FRCArea::RoadArea( inArea ) );
			}
			else
			{
				assert( isRoadOrCity( FRCArea::RoadArea( inArea ), Tile::City ) );
				occupyCity( FRCArea::CityArea( inArea ) );
			}
			return;
		}
		case Area::TopLeft:	// deliberate fallthroughs, FieldAreas
		case Area::TopRight:
		case Area::RightTop:
		case Area::RightBottom:
		case Area::BottomRight:
		case Area::BottomLeft:
		case Area::LeftBottom:
		case Area::LeftTop:
		{
			occupyField( FRCArea::FieldArea( inArea ) );
			return;
		}
		case Area::Central:
		{
			return; // do nothing as placing a piece there will be enough
		}
		case Area::Invalid:
		default:
		{
			assert( !"isOccupiedArea given Invalid area");
		}
	}
}

void
TileOnBoard::occupyField(FRCArea::FieldArea inFieldArea)
{
	if (!isFieldOccupied(inFieldArea))
	{
		std::vector< Tile::ContiguousField > contiguousFields = getContiguousFields();
		unsigned int i = 0;
		for (; i < contiguousFields.size(); ++i)
		{
			if ( std::find(contiguousFields[i].begin(), contiguousFields[i].end(), inFieldArea) != contiguousFields[i].end() )
			{
				break;
			}
		}
		if ( i < contiguousFields.size() )
		{
			Tile::ContiguousField occupiedField = contiguousFields[i];
			mOccupiedFields.push_back(occupiedField);
		}
	}
}

void
TileOnBoard::occupyRoad( FRCArea::RoadArea inRoadArea )
{
	if ( !isRoadOccupied( inRoadArea ) )
	{
		std::vector< Tile::ContiguousRoad > contiguousRoads = getContiguousRoads();
		unsigned int i = 0;
		for (; i < contiguousRoads.size(); ++i)
		{
			if ( std::find(contiguousRoads[i].begin(), contiguousRoads[i].end(), inRoadArea) != contiguousRoads[i].end() )
			{
				break;
			}
		}
		if ( i < contiguousRoads.size() )
		{
			Tile::ContiguousRoad occupiedRoad = contiguousRoads[i];
			mOccupiedRoads.push_back(occupiedRoad);
		}
	}
}

void
TileOnBoard::occupyCity( FRCArea::CityArea inCityArea )
{
	if ( !isCityOccupied( inCityArea ) )
	{
		std::vector< Tile::ContiguousCity > contiguousCities = getContiguousCities();
		unsigned int i = 0;
		for (; i < contiguousCities.size(); ++i )
		{
			if ( std::find( contiguousCities[i].begin(), contiguousCities[i].end(), inCityArea) != contiguousCities[i].end() )
			{
				break;
			}
		}
		if ( i < contiguousCities.size() )
		{
			Tile::ContiguousCity occupiedCity = contiguousCities[i];
			mOccupiedCities.push_back(occupiedCity);
		}
	}
}

void
TileOnBoard::finishRoad( FRCArea::RoadArea inRoadArea )
{
	if ( !isRoadFinished( inRoadArea ) )
	{
		std::vector< Tile::ContiguousRoad > contiguousRoads = getContiguousRoads();
		unsigned int i = 0;
		for (; i < contiguousRoads.size(); ++i)
		{
			if ( std::find(contiguousRoads[i].begin(), contiguousRoads[i].end(), inRoadArea) != contiguousRoads[i].end() )
			{
				break;
			}
		}
		if ( i < contiguousRoads.size() )
		{
			Tile::ContiguousRoad finishedRoad = contiguousRoads[i];
			mFinishedRoads.push_back(finishedRoad);
		}
	}
}

void
TileOnBoard::finishCity( FRCArea::CityArea inCityArea )
{
	if ( !isCityFinished( inCityArea ) )
	{
		std::vector< Tile::ContiguousCity > contiguousCities = getContiguousCities();
		unsigned int i = 0;
		for (; i < contiguousCities.size(); ++i)
		{
			if ( std::find(contiguousCities[i].begin(), contiguousCities[i].end(), inCityArea) != contiguousCities[i].end() )
			{
				break;
			}
		}
		if ( i < contiguousCities.size() )
		{
			Tile::ContiguousCity finishedCity = contiguousCities[i];
			mFinishedCities.push_back(finishedCity);
		}
	}
}

void
TileOnBoard::finishCloister()
{
	mCloisterFinished = true;
}

std::string
TileOnBoard::toString() const
{
	std::string result;
	result.append("Top: ");
	result.append(Tile::sideToString(this->getTop()));
	result.append("\tRight: ");
	result.append(Tile::sideToString(this->getRight()));
	result.append("\tBottom: ");
	result.append(Tile::sideToString(this->getBottom()));
	result.append("\tLeft: ");
	result.append(Tile::sideToString(this->getLeft()));
	result.append("\tCenter: ");
	result.append(Tile::centerToString(this->getCenter()));
	std::vector< Tile::ContiguousField > fields = this->getContiguousFields();
	if (!fields.empty())
	{
		result.append("\nContiguous fields:");
		for (unsigned int i = 0; i < fields.size(); i++)
		{
			result.append("\n\t- ");
			for (unsigned int j = 0; j < fields[i].size(); j++)
			{
				result.append(FRCArea::fieldAreaToString(fields[i][j]));
				result.append(" ");
			}
		}
	}
	std::vector< Tile::ContiguousRoad > roads = this->getContiguousRoads();
	if (!roads.empty())
	{
		result.append("\nContiguous roads:");
		for (unsigned int i = 0; i < roads.size(); i++)
		{
			result.append("\n\t- ");
			for (unsigned int j = 0; j < roads[i].size(); j++)
			{
				result.append(FRCArea::roadAreaToString(roads[i][j]));
				result.append(" ");
			}
		}
	}
	std::vector< Tile::ContiguousCity > cities = this->getContiguousCities();
	if (!cities.empty())
	{
		result.append("\nContiguous cities:");
		for (unsigned int i = 0; i < cities.size(); i++)
		{
			result.append("\n\t- ");
			for (unsigned int j = 0; j < cities[i].size(); j++)
			{
				result.append(FRCArea::cityAreaToString(cities[i][j]));
				result.append(" ");
			}
		}
	}
	std::vector< FRCArea::CityArea > shields = this->getShields();
	if (!shields.empty())
	{
		result.append("\nShields at:\n\t- ");
		for (unsigned int i = 0; i < shields.size(); i++)
		{
			result.append(FRCArea::cityAreaToString(shields[i]));
			result.append(" ");
		}
	}
	std::vector< FRCArea::RoadArea > inns = this->getInns();
	if (!inns.empty())
	{
		result.append("\nInns at:\n\t- ");
		for (unsigned int i = 0; i < inns.size(); i++)
		{
			result.append(FRCArea::roadAreaToString(inns[i]));
			result.append(" ");
		}
	}
	result.append("\n");
	return result;
}

bool
TileOnBoard::isOccupiedArea( Area::Area inArea ) const
{
	switch ( inArea )
	{
		case Area::Top:	// deliberate fallthroughs, RoadAreas
		case Area::Right:
		case Area::Bottom:
		case Area::Left:
		{
			return
			(
				isRoadOccupied( FRCArea::RoadArea( inArea ) )
				||
				isCityOccupied( FRCArea::CityArea( inArea ) )
			);
		}
		case Area::TopLeft:	// deliberate fallthroughs, FieldAreas
		case Area::TopRight:
		case Area::RightTop:
		case Area::RightBottom:
		case Area::BottomRight:
		case Area::BottomLeft:
		case Area::LeftBottom:
		case Area::LeftTop:
		{
			return isFieldOccupied( FRCArea::FieldArea( inArea ) );
		}
		case Area::Central:
		{
			return isCloisterOccupied();
		}
		case Area::Invalid:
		default:
		{
			assert( !"isOccupiedArea given Invalid area");
		}
	}
}

bool
TileOnBoard::isRoadOrCity( FRCArea::RoadArea inArea, Tile::Side inSide ) const
{
	switch ( inArea )
	{
		case FRCArea::Top:
			return getTop() == inSide;
		case FRCArea::Right:
			return getRight() == inSide;
		case FRCArea::Bottom:
			return getBottom() == inSide;
		case FRCArea::Left:
			return getLeft() == inSide;
		default:
			assert( !"Invalid inArea" );
	}
}
