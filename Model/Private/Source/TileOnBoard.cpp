#include "TileOnBoard.h"

#include <algorithm>
#include <cassert>

namespace
{
	Model::Area::Area
	turn( Model::Area::Area inArea, Model::Rotation inRotation )
	{
		if ( inArea == Model::Area::kCentral )
			return Model::Area::kCentral;
		else
			return Model::Area::Area( (inArea + inRotation) % 12 );
	}

	Model::Area::Area
	unturn( Model::Area::Area inArea, Model::Rotation inRotation )
	{
		if ( inArea == Model::Area::kCentral )
			return Model::Area::kCentral;
		else
			return Model::Area::Area( (inArea + 12 - inRotation) % 12 );
	}
}

Model::TileOnBoard::TileOnBoard()
:
	mTile( Tile::Tile() ),
	mRotation( Model::kCw0 ),
	mPlacedPieces()
{
}

Model::TileOnBoard::TileOnBoard( const Tile &inTile, Rotation inRotation )
:
	mTile( inTile ),
	mRotation( inRotation ),
	mPlacedPieces()
{
}

Model::Tile::Side
Model::TileOnBoard::getTop() const
{
	switch ( mRotation )
	{
		case kCw90:
			return mTile.getLeft();
		case kCw180:
			return mTile.getBottom();
		case kCw270:
			return mTile.getRight();
		case kCw0:
		default:
			return mTile.getTop();
	}
}

Model::Tile::Side
Model::TileOnBoard::getRight() const
{
	switch ( mRotation )
	{
		case kCw90:
			return mTile.getTop();
		case kCw180:
			return mTile.getLeft();
		case kCw270:
			return mTile.getBottom();
		case kCw0:
		default:
			return mTile.getRight();
	}
}

Model::Tile::Side
Model::TileOnBoard::getBottom() const
{
	switch ( mRotation )
	{
		case kCw90:
			return mTile.getRight();
		case kCw180:
			return mTile.getTop();
		case kCw270:
			return mTile.getLeft();
		case kCw0:
		default:
			return mTile.getBottom();
	}
}

Model::Tile::Side
Model::TileOnBoard::getLeft() const
{
	switch ( mRotation )
	{
		case kCw90:
			return mTile.getBottom();
		case kCw180:
			return mTile.getRight();
		case kCw270:
			return mTile.getTop();
		case kCw0:
		default:
			return mTile.getLeft();
	}
}

Model::Tile::Center
Model::TileOnBoard::getCenter() const
{
	return mTile.getCenter();
}

std::string const &
Model::TileOnBoard::getID() const
{
	return mTile.getID();
}

Model::Tile const &
Model::TileOnBoard::getTile() const
{
	return mTile;
}

Model::Rotation
Model::TileOnBoard::getRotation() const
{
	return mRotation;
}

std::vector< Model::PlacedPiece > const &
Model::TileOnBoard::getPlacedPieces() const
{
	return mPlacedPieces;
}

bool
Model::TileOnBoard::hasInn( Area::Area inArea ) const
{
	return mTile.hasInn( unturn( inArea, mRotation ) );
}

bool
Model::TileOnBoard::matchesAbove( TileOnBoard const & inTileOnBoard) const
{
	return ( this->getBottom() == inTileOnBoard.getTop() );
}

bool
Model::TileOnBoard::matchesRightOf( TileOnBoard const & inTileOnBoard) const
{
	return ( this->getLeft() == inTileOnBoard.getRight() );
}

bool
Model::TileOnBoard::matchesBelow( TileOnBoard const & inTileOnBoard) const
{
	return ( this->getTop() == inTileOnBoard.getBottom() );
}

bool
Model::TileOnBoard::matchesLeftOf( TileOnBoard const & inTileOnBoard) const
{
	return ( this->getRight() == inTileOnBoard.getLeft() );
}

std::vector< Model::ContiguousField >
Model::TileOnBoard::getContiguousFields() const
{
	std::vector< ContiguousField > result;
	std::vector< ContiguousField > original = mTile.getContiguousFields();
	for ( unsigned i = 0; i < original.size(); ++i )
	{
		ContiguousField turned;
		ContiguousField origtemp = original[i];
		for ( unsigned j = 0; j < origtemp.size(); ++j )
		{
			turned.push_back( turn( origtemp[j], mRotation ) );
		}
		result.push_back( turned );
	}
	return result;
}

std::vector< Model::ContiguousRoad >
Model::TileOnBoard::getContiguousRoads() const
{
	std::vector< ContiguousRoad > result;
	std::vector< ContiguousRoad > original = mTile.getContiguousRoads();
	for ( unsigned i = 0; i < original.size(); ++i )
	{
		ContiguousRoad turned;
		ContiguousRoad origtemp = original[i];
		for ( unsigned j = 0; j < origtemp.size(); ++j )
		{
			turned.push_back( turn( origtemp[j], mRotation ) );
		}
		result.push_back( turned );
	}
	return result;
}

std::vector< Model::ContiguousCity >
Model::TileOnBoard::getContiguousCities() const
{
	std::vector< ContiguousCity > result;
	std::vector< ContiguousCity > original = mTile.getContiguousCities();
	for ( unsigned i = 0; i < original.size(); ++i )
	{
		ContiguousCity turned;
		ContiguousCity origtemp = original[i];
		for ( unsigned j = 0; j < origtemp.size(); ++j )
		{
			turned.push_back( turn( origtemp[j], mRotation ) );
		}
		result.push_back( turned );
	}
	return result;
}

bool
Model::TileOnBoard::isCloister( Area::Area inArea ) const
{
	return mTile.isCloister( unturn( inArea, mRotation ) );
}

bool
Model::TileOnBoard::isRoad( Area::Area inArea ) const
{
	return mTile.isRoad( unturn( inArea, mRotation ) );
}

bool
Model::TileOnBoard::isCity( Area::Area inArea ) const
{
	return mTile.isCity( unturn( inArea, mRotation ) );
}

bool
Model::TileOnBoard::isField( Area::Area inArea ) const
{
	return mTile.isField( unturn( inArea, mRotation ) );
}

Model::ContiguousField
Model::TileOnBoard::getContiguousField( Area::Area inArea ) const
{
	Area::Area unRotatedArea = unturn( inArea, mRotation );
	ContiguousField unRotatedField = mTile.getContiguousField( unRotatedArea );
	ContiguousField rotatedField;
	for ( unsigned i = 0; i < unRotatedField.size(); ++i )
	{
		rotatedField.push_back( turn( unRotatedField[i], mRotation ) );
	}
	return rotatedField;
}

Model::ContiguousRoad
Model::TileOnBoard::getContiguousRoad( Area::Area inArea ) const
{
	Area::Area unRotatedArea = unturn( inArea, mRotation );
	ContiguousRoad unRotatedRoad = mTile.getContiguousRoad( unRotatedArea );
	ContiguousRoad rotatedRoad;
	for ( unsigned i = 0; i < unRotatedRoad.size(); ++i )
	{
		rotatedRoad.push_back( turn( unRotatedRoad[i], mRotation ) );
	}
	return rotatedRoad;
}

Model::ContiguousCity
Model::TileOnBoard::getContiguousCity( Area::Area inArea ) const
{
	Area::Area unRotatedArea = unturn( inArea, mRotation );
	ContiguousCity unRotatedCity = mTile.getContiguousCity( unRotatedArea );
	ContiguousCity rotatedCity;
	for ( unsigned i = 0; i < unRotatedCity.size(); ++i )
	{
		rotatedCity.push_back( turn( unRotatedCity[i], mRotation ) );
	}
	return rotatedCity;
}

std::vector< Model::ContiguousCity >
Model::TileOnBoard::getCitiesPerField( Area::Area inFieldArea ) const
{
	Area::Area tileFieldArea = unturn( inFieldArea, mRotation );
	std::vector< ContiguousCity > tileCities = mTile.getCitiesPerField( tileFieldArea );
	std::vector< ContiguousCity > cities;
	for ( unsigned i = 0; i < tileCities.size(); ++i )
	{
		ContiguousCity cityTemp;
		ContiguousCity tileCityTemp = tileCities[i];
		for ( unsigned j = 0; j < tileCityTemp.size(); ++j )
		{
			cityTemp.push_back( turn( tileCityTemp[j], mRotation ) );
		}
		cities.push_back( cityTemp );
	}
	return cities;
}

bool
Model::TileOnBoard::placePiece( PlacedPiece const & inPlacedPiece )
{
	mPlacedPieces.push_back( inPlacedPiece );
	return true;
}

bool
Model::TileOnBoard::hasPieces() const
{
	return !mPlacedPieces.empty();
}

bool
Model::TileOnBoard::hasPiece( Area::Area inArea ) const
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

std::vector< Model::PlacedPiece >
Model::TileOnBoard::removePieces( Area::Area inArea )
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

std::vector< Model::PlacedPiece >
Model::TileOnBoard::removeAllPieces()
{
	std::vector< PlacedPiece > result = mPlacedPieces;
	mPlacedPieces.clear();
	return result;
}

std::vector< Model::Area::Area >
Model::TileOnBoard::getShields() const
{
	std::vector< Area::Area > result;
	std::vector< Area::Area > orig = mTile.getShields();
	for ( unsigned i = 0; i < orig.size(); ++i )
	{
		result.push_back( turn( orig[i], mRotation ) );
	}
	return result;
}

std::vector< Model::Area::Area >
Model::TileOnBoard::getInns() const
{
	std::vector< Area::Area > result;
	std::vector< Area::Area > orig = mTile.getInns();
	for ( unsigned i = 0; i < orig.size(); ++i )
	{
		result.push_back( turn( orig[i], mRotation ) );
	}
	return result;
}

std::string
Model::TileOnBoard::toString() const
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
	std::vector< ContiguousField > fields = this->getContiguousFields();
	if (!fields.empty())
	{
		result.append("\nContiguous fields:");
		for (unsigned i = 0; i < fields.size(); i++)
		{
			result.append("\n\t- ");
			for (unsigned j = 0; j < fields[i].size(); j++)
			{
				result.append( areaToString(fields[i][j]));
				result.append(" ");
			}
		}
	}
	std::vector< ContiguousRoad > roads = this->getContiguousRoads();
	if (!roads.empty())
	{
		result.append("\nContiguous roads:");
		for (unsigned i = 0; i < roads.size(); i++)
		{
			result.append("\n\t- ");
			for (unsigned j = 0; j < roads[i].size(); j++)
			{
				result.append( areaToString(roads[i][j]));
				result.append(" ");
			}
		}
	}
	std::vector< ContiguousCity > cities = this->getContiguousCities();
	if (!cities.empty())
	{
		result.append("\nContiguous cities:");
		for (unsigned i = 0; i < cities.size(); i++)
		{
			result.append("\n\t- ");
			for (unsigned j = 0; j < cities[i].size(); j++)
			{
				result.append( areaToString(cities[i][j]));
				result.append(" ");
			}
		}
	}
	std::vector< Area::Area > shields = this->getShields();
	if (!shields.empty())
	{
		result.append("\nShields at:\n\t- ");
		for (unsigned i = 0; i < shields.size(); i++)
		{
			result.append( areaToString(shields[i]));
			result.append(" ");
		}
	}
	std::vector< Area::Area > inns = this->getInns();
	if (!inns.empty())
	{
		result.append("\nInns at:\n\t- ");
		for (unsigned i = 0; i < inns.size(); i++)
		{
			result.append( areaToString( inns[i] ) );
			result.append(" ");
		}
	}
	result.append("\n");
	return result;
}