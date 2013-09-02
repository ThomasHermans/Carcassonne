#include "src-model/TileOnBoard.h"

#include <algorithm>
#include <cassert>

namespace
{
	FRCArea::FieldArea
	turn( FRCArea::FieldArea inFieldArea, TileOnBoard::Rotation inRotation )
	{
		if ( inFieldArea == FRCArea::Central )
			return FRCArea::Central;
		else
			return FRCArea::FieldArea( (inFieldArea + inRotation) % 12 );
	}

	FRCArea::FieldArea
	unturn( FRCArea::FieldArea inFieldArea, TileOnBoard::Rotation inRotation )
	{
		if ( inFieldArea == FRCArea::Central )
			return FRCArea::Central;
		else
			return FRCArea::FieldArea( (inFieldArea + 12 - inRotation) % 12 );
	}

	FRCArea::RoadArea
	turn( FRCArea::RoadArea inRoadArea, TileOnBoard::Rotation inRotation )
	{
		return FRCArea::RoadArea( (inRoadArea + inRotation) % 12 );
	}

	FRCArea::RoadArea
	unturn( FRCArea::RoadArea inRoadArea, TileOnBoard::Rotation inRotation )
	{
		return FRCArea::RoadArea( (inRoadArea + 12 - inRotation) % 12 );
	}
}

TileOnBoard::TileOnBoard()
:
	mTile( Tile::Tile() ),
	mRotation( TileOnBoard::cw0 ),
	mPlacedPieces()
{
}

TileOnBoard::TileOnBoard( const Tile &inTile, Rotation inRotation )
:
	mTile( inTile ),
	mRotation( inRotation ),
	mPlacedPieces()
{
}

Tile::Side
TileOnBoard::getTop() const
{
	switch ( mRotation )
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
	switch ( mRotation )
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
	switch ( mRotation )
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
	switch ( mRotation )
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

std::string const &
TileOnBoard::getID() const
{
	return mTile.getID();
}

Tile const &
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
TileOnBoard::matchesAbove( TileOnBoard const & inTileOnBoard) const
{
	return ( this->getBottom() == inTileOnBoard.getTop() );
}

bool
TileOnBoard::matchesRightOf( TileOnBoard const & inTileOnBoard) const
{
	return ( this->getLeft() == inTileOnBoard.getRight() );
}

bool
TileOnBoard::matchesBelow( TileOnBoard const & inTileOnBoard) const
{
	return ( this->getTop() == inTileOnBoard.getBottom() );
}

bool
TileOnBoard::matchesLeftOf( TileOnBoard const & inTileOnBoard) const
{
	return ( this->getRight() == inTileOnBoard.getLeft() );
}

std::vector< Tile::ContiguousField >
TileOnBoard::getContiguousFields() const
{
	std::vector< Tile::ContiguousField > result;
	std::vector< Tile::ContiguousField > original = mTile.getContiguousFields();
	for ( unsigned i = 0; i < original.size(); ++i )
	{
		Tile::ContiguousField turned;
		Tile::ContiguousField origtemp = original[i];
		for ( unsigned j = 0; j < origtemp.size(); ++j )
		{
			turned.push_back( turn( origtemp[j], mRotation ) );
		}
		result.push_back( turned );
	}
	return result;
}

std::vector< Tile::ContiguousRoad >
TileOnBoard::getContiguousRoads() const
{
	std::vector< Tile::ContiguousRoad > result;
	std::vector< Tile::ContiguousRoad > original = mTile.getContiguousRoads();
	for ( unsigned i = 0; i < original.size(); ++i )
	{
		Tile::ContiguousRoad turned;
		Tile::ContiguousRoad origtemp = original[i];
		for ( unsigned j = 0; j < origtemp.size(); ++j )
		{
			turned.push_back( turn( origtemp[j], mRotation ) );
		}
		result.push_back( turned );
	}
	return result;
}

std::vector< Tile::ContiguousCity >
TileOnBoard::getContiguousCities() const
{
	std::vector< Tile::ContiguousCity > result;
	std::vector< Tile::ContiguousCity > original = mTile.getContiguousCities();
	for ( unsigned i = 0; i < original.size(); ++i )
	{
		Tile::ContiguousCity turned;
		Tile::ContiguousCity origtemp = original[i];
		for ( unsigned j = 0; j < origtemp.size(); ++j )
		{
			turned.push_back( turn( origtemp[j], mRotation ) );
		}
		result.push_back( turned );
	}
	return result;
}

bool
TileOnBoard::isCloister( Area::Area inArea ) const
{
	return ( inArea == Area::Central && getCenter() == Tile::Cloister );
}

bool
TileOnBoard::isRoad( Area::Area inArea ) const
{
	switch ( inArea )
	{
		case Area::Top:
			return getTop() == Tile::Road;
		case Area::Right:
			return getRight() == Tile::Road;
		case Area::Bottom:
			return getBottom() == Tile::Road;
		case Area::Left:
			return getLeft() == Tile::Road;
		default:
			return false;
	}
}

Tile::ContiguousField
TileOnBoard::getContiguousField( FRCArea::FieldArea inFieldArea ) const
{
	FRCArea::FieldArea unRotatedArea = unturn( inFieldArea, mRotation );
	Tile::ContiguousField unRotatedField = mTile.getContiguousField( unRotatedArea );
	Tile::ContiguousField rotatedField;
	for ( unsigned i = 0; i < unRotatedField.size(); ++i )
	{
		rotatedField.push_back( turn( unRotatedField[i], mRotation ) );
	}
	return rotatedField;
}

Tile::ContiguousRoad
TileOnBoard::getContiguousRoad( FRCArea::RoadArea inRoadArea ) const
{
	FRCArea::RoadArea unRotatedArea = unturn( inRoadArea, mRotation );
	Tile::ContiguousRoad unRotatedRoad = mTile.getContiguousRoad( unRotatedArea );
	Tile::ContiguousRoad rotatedRoad;
	for ( unsigned i = 0; i < unRotatedRoad.size(); ++i )
	{
		rotatedRoad.push_back( turn( unRotatedRoad[i], mRotation ) );
	}
	return rotatedRoad;
}

Tile::ContiguousCity
TileOnBoard::getContiguousCity( FRCArea::CityArea inCityArea ) const
{
	FRCArea::CityArea unRotatedArea = unturn( inCityArea, mRotation );
	Tile::ContiguousCity unRotatedCity = mTile.getContiguousCity( unRotatedArea );
	Tile::ContiguousCity rotatedCity;
	for ( unsigned i = 0; i < unRotatedCity.size(); ++i )
	{
		rotatedCity.push_back( turn( unRotatedCity[i], mRotation ) );
	}
	return rotatedCity;
}

std::vector< Tile::ContiguousCity >
TileOnBoard::getCitiesPerField( FRCArea::FieldArea inFieldArea ) const
{
	FRCArea::FieldArea tileFieldArea = unturn( inFieldArea, mRotation );
	std::vector< Tile::ContiguousCity > tileCities = mTile.getCitiesPerField( tileFieldArea );
	std::vector< Tile::ContiguousCity > cities;
	for ( unsigned i = 0; i < tileCities.size(); ++i )
	{
		Tile::ContiguousCity cityTemp;
		Tile::ContiguousCity tileCityTemp = tileCities[i];
		for ( unsigned j = 0; j < tileCityTemp.size(); ++j )
		{
			cityTemp.push_back( turn( tileCityTemp[j], mRotation ) );
		}
		cities.push_back( cityTemp );
	}
	return cities;
}

bool
TileOnBoard::placePiece( PlacedPiece const & inPlacedPiece )
{
	mPlacedPieces.push_back( inPlacedPiece );
	return true;
}

bool
TileOnBoard::hasPieces() const
{
	return !mPlacedPieces.empty();
}

bool
TileOnBoard::hasPiece( Area::Area inArea ) const
{
	for ( std::vector< PlacedPiece >::const_iterator it = mPlacedPieces.begin();
		it != mPlacedPieces.end();
		++it )
	{
		if ( it->getArea() == inArea )
		{
			return true;
		}
	}
	return false;
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

std::vector< FRCArea::CityArea >
TileOnBoard::getShields() const
{
	std::vector< FRCArea::CityArea > result;
	std::vector< FRCArea::CityArea > orig = mTile.getShields();
	for ( unsigned i = 0; i < orig.size(); ++i )
	{
		result.push_back( turn( orig[i], mRotation ) );
	}
	return result;
}

std::vector< FRCArea::RoadArea >
TileOnBoard::getInns() const
{
	std::vector< FRCArea::RoadArea > result;
	std::vector< FRCArea::RoadArea > orig = mTile.getInns();
	for ( unsigned i = 0; i < orig.size(); ++i )
	{
		result.push_back( turn( orig[i], mRotation ) );
	}
	return result;
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
		for (unsigned i = 0; i < fields.size(); i++)
		{
			result.append("\n\t- ");
			for (unsigned j = 0; j < fields[i].size(); j++)
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
		for (unsigned i = 0; i < roads.size(); i++)
		{
			result.append("\n\t- ");
			for (unsigned j = 0; j < roads[i].size(); j++)
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
		for (unsigned i = 0; i < cities.size(); i++)
		{
			result.append("\n\t- ");
			for (unsigned j = 0; j < cities[i].size(); j++)
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
		for (unsigned i = 0; i < shields.size(); i++)
		{
			result.append(FRCArea::cityAreaToString(shields[i]));
			result.append(" ");
		}
	}
	std::vector< FRCArea::RoadArea > inns = this->getInns();
	if (!inns.empty())
	{
		result.append("\nInns at:\n\t- ");
		for (unsigned i = 0; i < inns.size(); i++)
		{
			result.append(FRCArea::roadAreaToString(inns[i]));
			result.append(" ");
		}
	}
	result.append("\n");
	return result;
}