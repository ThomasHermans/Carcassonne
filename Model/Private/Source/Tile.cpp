#include "Tile.h"

#include <algorithm>
#include <cassert>

namespace
{
	bool
	tileHas( Tile const & inTile, Area::Area inArea, Tile::Side inSide )
	{
		switch ( inSide )
		{
		case Tile::Field:
			return inTile.isField( inArea );
		case Tile::Road:
			return inTile.isRoad( inArea );
		case Tile::City:
			return inTile.isCity( inArea );
		}
		assert( !"Invalid Tile::Side" );
		return false;
	}

	void
	assureSideHasNo( Tile const & inTile, Area::Area inArea, Tile::Side inSide )
	{
		// Make sure inTile has nothing of inSide at inArea
		assert( !tileHas( inTile, Area::Area( inArea - 1 ), inSide ) );
		assert( !tileHas( inTile, Area::Area( inArea ), inSide ) );
		assert( !tileHas( inTile, Area::Area( inArea + 1 ), inSide ) );
	}
	void
	assureSideHasAll( Tile const & inTile, Area::Area inArea, Tile::Side inSide )
	{
		// Make sure inTile has nothing of inSide at inArea
		assert( tileHas( inTile, Area::Area( inArea - 1 ), inSide ) );
		assert( tileHas( inTile, Area::Area( inArea ), inSide ) );
		assert( tileHas( inTile, Area::Area( inArea + 1 ), inSide ) );
	}
	void
	assureSideHasOnlyMiddle( Tile const & inTile, Area::Area inArea, Tile::Side inSide )
	{
		// Make sure inTile has nothing of inSide at inArea
		assert( !tileHas( inTile, Area::Area( inArea - 1 ), inSide ) );
		assert( tileHas( inTile, Area::Area( inArea ), inSide ) );
		assert( !tileHas( inTile, Area::Area( inArea + 1 ), inSide ) );
	}
	void
	assureSideHasOnlySurround( Tile const & inTile, Area::Area inArea, Tile::Side inSide )
	{
		// Make sure inTile has nothing of inSide at inArea
		assert( tileHas( inTile, Area::Area( inArea - 1 ), inSide ) );
		assert( !tileHas( inTile, Area::Area( inArea ), inSide ) );
		assert( tileHas( inTile, Area::Area( inArea + 1 ), inSide ) );
	}

	void
	assureSide( Tile const & inTile, Tile::Side inSide, Area::Area inArea )
	{
		switch ( inSide )
		{
		case Tile::Field:
		{
			assureSideHasNo( inTile, inArea, Tile::City );
			assureSideHasNo( inTile, inArea, Tile::Road );
			assureSideHasAll( inTile, inArea, Tile::Field );
			break;
		}
		case Tile::Road:
		{
			assureSideHasNo( inTile, inArea, Tile::City );
			assureSideHasOnlyMiddle( inTile, inArea, Tile::Road );
			assureSideHasOnlySurround( inTile, inArea, Tile::Field );
			break;
		}
		case Tile::City:
		{
			assureSideHasAll( inTile, inArea, Tile::City );
			assureSideHasNo( inTile, inArea, Tile::Road );
			assureSideHasNo( inTile, inArea, Tile::Field );
			break;
		}
		}
	}

	void
	assureSides( Tile const & inTile )
	{
		assureSide( inTile, inTile.getTop(), Area::Top );
		assureSide( inTile, inTile.getRight(), Area::Right );
		assureSide( inTile, inTile.getBottom(), Area::Bottom );
		assureSide( inTile, inTile.getLeft(), Area::Left );
	}
}

Tile::Tile()
:
	mID( "D" ),
	mTop( Road ),
	mRight( City ),
	mBottom( Road ),
	mLeft( Field ),
	mCenter( Nothing ),
	mFields(),
	mRoads(),
	mCities(),
	mCitiesPerField(),
	mShields(),
	mInns()
{
	ContiguousField rightField;
	rightField.push_back( Area::TopRight );
	rightField.push_back( Area::BottomRight );
	ContiguousField leftField;
	leftField.push_back( Area::BottomLeft );
	leftField.push_back( Area::LeftBottom );
	leftField.push_back( Area::LeftTop );
	leftField.push_back( Area::TopLeft );
	mFields.push_back( rightField );
	mFields.push_back( leftField );

	ContiguousCity rightCity;
	rightCity.push_back( Area::Right );

	std::vector< ContiguousCity > rightFieldCities;
	rightFieldCities.push_back( rightCity );
	std::vector< ContiguousCity > leftFieldCities;
	mCitiesPerField[ rightField ] = rightFieldCities;
	mCitiesPerField[ leftField ] = leftFieldCities;
}

Tile::Tile
(
	Center inCenter,
	std::string const & inID,
	std::vector< ContiguousField > const & inFields,
	std::vector< ContiguousRoad > const & inRoads,
	std::vector< ContiguousCity > const & inCities,
	std::map< ContiguousField, std::vector< ContiguousCity > > const & inCitiesPerField,
	std::vector< Area::Area > const & inShields,
	std::vector< Area::Area > const & inInns
)
:
	mID( inID ),
	mTop( Tile::Field ),
	mRight( Tile::Field ),
	mBottom( Tile::Field ),
	mLeft( Tile::Field ),
	mCenter( inCenter ),
	mFields( inFields ),
	mRoads( inRoads ),
	mCities( inCities ),
	mCitiesPerField( inCitiesPerField ),
	mShields( inShields ),
	mInns( inInns )
{
	int top = 0;
	int right = 0;
	int bottom = 0;
	int left = 0;
	for ( unsigned int i = 0; i < inCities.size(); ++i )
	{
		for ( unsigned int j = 0; j < inCities[i].size(); ++j )
		{
			switch ( inCities[i][j] )
			{
			case Area::Top:
				mTop = Tile::City;
				++top;
				break;
			case Area::Right:
				mRight = Tile::City;
				++right;
				break;
			case Area::Bottom:
				mBottom = Tile::City;
				++bottom;
				break;
			case Area::Left:
				mLeft = Tile::City;
				++left;
				break;
			case Area::TopLeft:
			case Area::TopRight:
			case Area::RightTop:
			case Area::RightBottom:
			case Area::BottomRight:
			case Area::BottomLeft:
			case Area::LeftBottom:
			case Area::LeftTop:
			case Area::Central:
				break;
			}
		}
	}
	for ( unsigned int i = 0; i < inRoads.size(); ++i )
	{
		for ( unsigned int j = 0; j < inRoads[i].size(); ++j )
		{
			switch ( inRoads[i][j] )
			{
			case Area::Top:
				mTop = Tile::Road;
				++top;
				break;
			case Area::Right:
				mRight = Tile::Road;
				++right;
				break;
			case Area::Bottom:
				mBottom = Tile::Road;
				++bottom;
				break;
			case Area::Left:
				mLeft = Tile::Road;
				++left;
				break;
			case Area::TopLeft:
			case Area::TopRight:
			case Area::RightTop:
			case Area::RightBottom:
			case Area::BottomRight:
			case Area::BottomLeft:
			case Area::LeftBottom:
			case Area::LeftTop:
			case Area::Central:
				break;
			}
		}
	}
	assert( top <= 1 );
	assert( right <= 1 );
	assert( bottom <= 1 );
	assert( left <= 1 );
	assureSides( *this );
}

Tile::Side
Tile::getTop() const
{
	return mTop;
}

Tile::Side
Tile::getRight() const
{
	return mRight;
}

Tile::Side
Tile::getBottom() const
{
	return mBottom;
}

Tile::Side
Tile::getLeft() const
{
	return mLeft;
}

Tile::Center
Tile::getCenter() const
{
	return mCenter;
}

std::string const &
Tile::getID() const
{
	return mID;
}

std::vector< Tile::ContiguousField > const &
Tile::getContiguousFields() const
{
	return mFields;
}

std::vector< Tile::ContiguousRoad > const &
Tile::getContiguousRoads() const
{
	return mRoads;
}

std::vector< Tile::ContiguousCity > const &
Tile::getContiguousCities() const
{
	return mCities;
}

std::vector< Tile::ContiguousCity > const &
Tile::getCitiesPerField(const ContiguousField &inContiguousField) const
{
	return mCitiesPerField.at(inContiguousField);
}

std::vector< Tile::ContiguousCity > const &
Tile::getCitiesPerField( Area::Area inFieldArea ) const
{
	ContiguousField cf = getContiguousField( inFieldArea );
	return mCitiesPerField.at(cf);
}

std::vector< Area::Area > const &
Tile::getShields() const
{
	return mShields;
}

std::vector< Area::Area > const &
Tile::getInns() const
{
	return mInns;
}

Tile::ContiguousField
Tile::getContiguousField( Area::Area inFieldArea ) const
{
	unsigned i = 0;
	for (; i < mFields.size(); ++i)
	{
		if ( std::find(mFields[i].begin(), mFields[i].end(), inFieldArea) != mFields[i].end() )
		{
			break;
		}
	}
	if (i < mFields.size())
	{
		return mFields[i];
	}
	else
	{
		return Tile::ContiguousField();
	}
}

Tile::ContiguousRoad
Tile::getContiguousRoad(Area::Area inArea) const
{
	unsigned int i = 0;
	for (; i < mRoads.size(); ++i)
	{
		if ( std::find(mRoads[i].begin(), mRoads[i].end(), inArea) != mRoads[i].end() )
		{
			break;
		}
	}
	if (i < mRoads.size())
	{
		return mRoads[i];
	}
	else
	{
		return Tile::ContiguousRoad();
	}
}

Tile::ContiguousCity
Tile::getContiguousCity(Area::Area inArea) const
{
	unsigned int i = 0;
	for (; i < mCities.size(); ++i)
	{
		if ( std::find(mCities[i].begin(), mCities[i].end(), inArea) != mCities[i].end() )
		{
			break;
		}
	}
	if (i < mCities.size())
	{
		return mCities[i];
	}
	else
	{
		return Tile::ContiguousCity();
	}
}

bool
Tile::isCloister( Area::Area inArea ) const
{
	return ( inArea == Area::Central && getCenter() == Tile::Cloister );	
}

bool
Tile::isRoad( Area::Area inArea ) const
{
	for ( std::vector< ContiguousRoad >::const_iterator it = mRoads.begin();
		it != mRoads.end();
		++it )
	{
		if ( std::find( it->begin(), it->end(), inArea ) != it->end() )
		{
			return true;
		}
	}
	return false;
}

bool
Tile::isCity( Area::Area inArea ) const
{
	for ( std::vector< ContiguousCity >::const_iterator it = mCities.begin();
		it != mCities.end();
		++it )
	{
		if ( std::find( it->begin(), it->end(), inArea ) != it->end() )
		{
			return true;
		}
	}
	return false;
}

bool
Tile::isField( Area::Area inArea ) const
{
	for ( std::vector< ContiguousField >::const_iterator it = mFields.begin();
		it != mFields.end();
		++it )
	{
		if ( std::find( it->begin(), it->end(), inArea ) != it->end() )
		{
			return true;
		}
	}
	return false;
}

std::string
Tile::toString() const
{
	std::string result;
	result.append("Top: ");
	result.append(sideToString(this->getTop()));
	result.append("\tRight: ");
	result.append(sideToString(this->getRight()));
	result.append("\tBottom: ");
	result.append(sideToString(this->getBottom()));
	result.append("\tLeft: ");
	result.append(sideToString(this->getLeft()));
	result.append("\tCenter: ");
	result.append(centerToString(this->getCenter()));
	if (!mFields.empty())
	{
		result.append("\nContiguous fields:");
		for (unsigned int i = 0; i < mFields.size(); i++)
		{
			result.append("\n\t- ");
			for (unsigned int j = 0; j < mFields[i].size(); j++)
			{
				result.append(Area::areaToString(mFields[i][j]));
				result.append(" ");
			}
		}
	}
	if (!mRoads.empty())
	{
		result.append("\nContiguous roads:");
		for (unsigned int i = 0; i < mRoads.size(); i++)
		{
			result.append("\n\t- ");
			for (unsigned int j = 0; j < mRoads[i].size(); j++)
			{
				result.append(Area::areaToString(mRoads[i][j]));
				result.append(" ");
			}
		}
	}
	if (!mCities.empty())
	{
		result.append("\nContiguous cities:");
		for (unsigned int i = 0; i < mCities.size(); i++)
		{
			result.append("\n\t- ");
			for (unsigned int j = 0; j < mCities[i].size(); j++)
			{
				result.append(Area::areaToString(mCities[i][j]));
				result.append(" ");
			}
		}
	}
	if (!mShields.empty())
	{
		result.append("\nShields at:\n\t- ");
		for (unsigned int i = 0; i < mShields.size(); i++)
		{
			result.append(Area::areaToString(mShields[i]));
			result.append(" ");
		}
	}
	if (!mInns.empty())
	{
		result.append("\nInns at:\n\t- ");
		for (unsigned int i = 0; i < mInns.size(); i++)
		{
			result.append(Area::areaToString(mInns[i]));
			result.append(" ");
		}
	}
	return result;
}

std::string
Tile::sideToString(Side inSide)
{
	switch (inSide)
	{
	case Field:
		return "Field";
	case Road:
		return "Road";
	case City:
		return "City";
	default:
		return "No valid side";
	}
}

std::string
Tile::centerToString(Center inCenter)
{
	switch (inCenter)
	{
	case Nothing:
		return "Nothing";
	case Cloister:
		return "Cloister";
	case Cathedral:
		return "Cathedral";
	case CentralField:
		return "CentralField";
	default:
		return "No valid center";
	}
}
