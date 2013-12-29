#include "Tile.h"

#include <algorithm>
#include <cassert>

namespace
{
	bool
	tileHas( Model::Tile const & inTile, Model::Area::Area inArea, Model::Tile::Side inSide )
	{
		switch ( inSide )
		{
		case Model::Tile::kSideField:
			return inTile.isField( inArea );
		case Model::Tile::kSideRoad:
			return inTile.isRoad( inArea );
		case Model::Tile::kSideCity:
			return inTile.isCity( inArea );
		}
		assert( !"Invalid Model::Tile::Side" );
		return false;
	}

	void
	assureSideHasNo( Model::Tile const & inTile, Model::Area::Area inArea, Model::Tile::Side inSide )
	{
		// Make sure inTile has nothing of inSide at inArea
		assert( !tileHas( inTile, Model::Area::Area( inArea - 1 ), inSide ) );
		assert( !tileHas( inTile, Model::Area::Area( inArea ), inSide ) );
		assert( !tileHas( inTile, Model::Area::Area( inArea + 1 ), inSide ) );
	}
	void
	assureSideHasAll( Model::Tile const & inTile, Model::Area::Area inArea, Model::Tile::Side inSide )
	{
		// Make sure inTile has nothing of inSide at inArea
		assert( tileHas( inTile, Model::Area::Area( inArea - 1 ), inSide ) );
		assert( tileHas( inTile, Model::Area::Area( inArea ), inSide ) );
		assert( tileHas( inTile, Model::Area::Area( inArea + 1 ), inSide ) );
	}
	void
	assureSideHasOnlyMiddle( Model::Tile const & inTile, Model::Area::Area inArea, Model::Tile::Side inSide )
	{
		// Make sure inTile has nothing of inSide at inArea
		assert( !tileHas( inTile, Model::Area::Area( inArea - 1 ), inSide ) );
		assert( tileHas( inTile, Model::Area::Area( inArea ), inSide ) );
		assert( !tileHas( inTile, Model::Area::Area( inArea + 1 ), inSide ) );
	}
	void
	assureSideHasOnlySurround( Model::Tile const & inTile, Model::Area::Area inArea, Model::Tile::Side inSide )
	{
		// Make sure inTile has nothing of inSide at inArea
		assert( tileHas( inTile, Model::Area::Area( inArea - 1 ), inSide ) );
		assert( !tileHas( inTile, Model::Area::Area( inArea ), inSide ) );
		assert( tileHas( inTile, Model::Area::Area( inArea + 1 ), inSide ) );
	}

	void
	assureSide( Model::Tile const & inTile, Model::Tile::Side inSide, Model::Area::Area inArea )
	{
		switch ( inSide )
		{
		case Model::Tile::kSideField:
		{
			assureSideHasNo( inTile, inArea, Model::Tile::kSideCity );
			assureSideHasNo( inTile, inArea, Model::Tile::kSideRoad );
			assureSideHasAll( inTile, inArea, Model::Tile::kSideField );
			break;
		}
		case Model::Tile::kSideRoad:
		{
			assureSideHasNo( inTile, inArea, Model::Tile::kSideCity );
			assureSideHasOnlyMiddle( inTile, inArea, Model::Tile::kSideRoad );
			assureSideHasOnlySurround( inTile, inArea, Model::Tile::kSideField );
			break;
		}
		case Model::Tile::kSideCity:
		{
			assureSideHasAll( inTile, inArea, Model::Tile::kSideCity );
			assureSideHasNo( inTile, inArea, Model::Tile::kSideRoad );
			assureSideHasNo( inTile, inArea, Model::Tile::kSideField );
			break;
		}
		}
	}

	void
	assureSides( Model::Tile const & inTile )
	{
		assureSide( inTile, inTile.getTop(), Model::Area::kTop );
		assureSide( inTile, inTile.getRight(), Model::Area::kRight );
		assureSide( inTile, inTile.getBottom(), Model::Area::kBottom );
		assureSide( inTile, inTile.getLeft(), Model::Area::kLeft );
	}
}

Model::Tile::Tile()
:
	mID( "D" ),
	mTop( kSideRoad ),
	mRight( kSideCity ),
	mBottom( kSideRoad ),
	mLeft( kSideField ),
	mCenter( kCenterNothing ),
	mFields(),
	mRoads(),
	mCities(),
	mCitiesPerField(),
	mShields(),
	mInns()
{
	ContiguousField rightField;
	rightField.push_back( Area::kTopRight );
	rightField.push_back( Area::kBottomRight );
	ContiguousField leftField;
	leftField.push_back( Area::kBottomLeft );
	leftField.push_back( Area::kLeftBottom );
	leftField.push_back( Area::kLeftTop );
	leftField.push_back( Area::kTopLeft );
	mFields.push_back( rightField );
	mFields.push_back( leftField );

	ContiguousCity rightCity;
	rightCity.push_back( Area::kRight );

	std::vector< ContiguousCity > rightFieldCities;
	rightFieldCities.push_back( rightCity );
	std::vector< ContiguousCity > leftFieldCities;
	mCitiesPerField[ rightField ] = rightFieldCities;
	mCitiesPerField[ leftField ] = leftFieldCities;
}

Model::Tile::Tile
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
	mTop( Model::Tile::kSideField ),
	mRight( Model::Tile::kSideField ),
	mBottom( Model::Tile::kSideField ),
	mLeft( Model::Tile::kSideField ),
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
			case Area::kTop:
				mTop = Model::Tile::kSideCity;
				++top;
				break;
			case Area::kRight:
				mRight = Model::Tile::kSideCity;
				++right;
				break;
			case Area::kBottom:
				mBottom = Model::Tile::kSideCity;
				++bottom;
				break;
			case Area::kLeft:
				mLeft = Model::Tile::kSideCity;
				++left;
				break;
			case Area::kTopLeft:
			case Area::kTopRight:
			case Area::kRightTop:
			case Area::kRightBottom:
			case Area::kBottomRight:
			case Area::kBottomLeft:
			case Area::kLeftBottom:
			case Area::kLeftTop:
			case Area::kCentral:
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
			case Area::kTop:
				mTop = Model::Tile::kSideRoad;
				++top;
				break;
			case Area::kRight:
				mRight = Model::Tile::kSideRoad;
				++right;
				break;
			case Area::kBottom:
				mBottom = Model::Tile::kSideRoad;
				++bottom;
				break;
			case Area::kLeft:
				mLeft = Model::Tile::kSideRoad;
				++left;
				break;
			case Area::kTopLeft:
			case Area::kTopRight:
			case Area::kRightTop:
			case Area::kRightBottom:
			case Area::kBottomRight:
			case Area::kBottomLeft:
			case Area::kLeftBottom:
			case Area::kLeftTop:
			case Area::kCentral:
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

Model::Tile::Side
Model::Tile::getTop() const
{
	return mTop;
}

Model::Tile::Side
Model::Tile::getRight() const
{
	return mRight;
}

Model::Tile::Side
Model::Tile::getBottom() const
{
	return mBottom;
}

Model::Tile::Side
Model::Tile::getLeft() const
{
	return mLeft;
}

Model::Tile::Center
Model::Tile::getCenter() const
{
	return mCenter;
}

std::string const &
Model::Tile::getID() const
{
	return mID;
}

std::vector< Model::ContiguousField > const &
Model::Tile::getContiguousFields() const
{
	return mFields;
}

std::vector< Model::ContiguousRoad > const &
Model::Tile::getContiguousRoads() const
{
	return mRoads;
}

std::vector< Model::ContiguousCity > const &
Model::Tile::getContiguousCities() const
{
	return mCities;
}

std::vector< Model::ContiguousCity > const &
Model::Tile::getCitiesPerField(const ContiguousField &inContiguousField) const
{
	return mCitiesPerField.at(inContiguousField);
}

std::vector< Model::ContiguousCity > const &
Model::Tile::getCitiesPerField( Area::Area inFieldArea ) const
{
	ContiguousField cf = getContiguousField( inFieldArea );
	return mCitiesPerField.at(cf);
}

std::vector< Model::Area::Area > const &
Model::Tile::getShields() const
{
	return mShields;
}

std::vector< Model::Area::Area > const &
Model::Tile::getInns() const
{
	return mInns;
}

Model::ContiguousField
Model::Tile::getContiguousField( Area::Area inFieldArea ) const
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
		return ContiguousField();
	}
}

Model::ContiguousRoad
Model::Tile::getContiguousRoad( Area::Area inArea ) const
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
		return ContiguousRoad();
	}
}

Model::ContiguousCity
Model::Tile::getContiguousCity(Area::Area inArea) const
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
		return ContiguousCity();
	}
}

bool
Model::Tile::isCloister( Area::Area inArea ) const
{
	return ( inArea == Area::kCentral && getCenter() == Model::Tile::kCenterCloister );	
}

bool
Model::Tile::isRoad( Area::Area inArea ) const
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
Model::Tile::isCity( Area::Area inArea ) const
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
Model::Tile::isField( Area::Area inArea ) const
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
Model::Tile::toString() const
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
				result.append( areaToString( mFields[i][j] ) );
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
				result.append( areaToString( mRoads[i][j] ) );
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
				result.append( areaToString( mCities[i][j] ) );
				result.append(" ");
			}
		}
	}
	if (!mShields.empty())
	{
		result.append("\nShields at:\n\t- ");
		for (unsigned int i = 0; i < mShields.size(); i++)
		{
			result.append( areaToString( mShields[i] ) );
			result.append(" ");
		}
	}
	if (!mInns.empty())
	{
		result.append("\nInns at:\n\t- ");
		for (unsigned int i = 0; i < mInns.size(); i++)
		{
			result.append( areaToString( mInns[i] ) );
			result.append(" ");
		}
	}
	return result;
}

std::string
Model::Tile::sideToString( Side inSide )
{
	switch ( inSide )
	{
	case kSideField:
		return "Field";
	case kSideRoad:
		return "Road";
	case kSideCity:
		return "City";
	}
	assert( !"No valid Model::Tile::Side" );
	return "No valid Model::Tile::Side";
}

std::string
Model::Tile::centerToString( Center inCenter )
{
	switch ( inCenter )
	{
	case kCenterNothing:
		return "Nothing";
	case kCenterCloister:
		return "Cloister";
	}
	assert( !"No valid Model::Tile::Center" );
	return "No valid Model::Tile::Center";
}
