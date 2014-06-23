#include "Model/Tile.h"

#include <boost/foreach.hpp>

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

	std::map< Model::Area::Area, std::size_t >
	getFilledAreas( std::size_t inValue )
	{
		std::map< Model::Area::Area, std::size_t > areas;
		for ( int i = Model::Area::kTopLeft; i <= Model::Area::kCentral; ++i )
		{
			areas[ Model::Area::Area( i ) ] = inValue;
		}
		return areas;
	}

	std::size_t
	getExpectedCenterOutput( Model::Tile::Center inCenter )
	{
		if ( inCenter == Model::Tile::kCenterNoProject || inCenter == Model::Tile::kCenterCloister )
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}

	void
	assureEveryAreaIsTakenOnce( Model::Tile const & inTile )
	{
		typedef std::map< Model::Area::Area, std::size_t > AreaMap;
		AreaMap areas = getFilledAreas( 0 );
		std::vector< Model::ContiguousField > const fields = inTile.getContiguousFields();
		BOOST_FOREACH( Model::ContiguousField const & field, fields )
		{
			BOOST_FOREACH( Model::Area::Area area, field )
			{
				++areas[ area ];
			}
		}
		std::vector< Model::ContiguousField > const cities = inTile.getContiguousCities();
		BOOST_FOREACH( Model::ContiguousField const & city, cities )
		{
			BOOST_FOREACH( Model::Area::Area area, city )
			{
				++areas[ area ];
			}
		}
		std::vector< Model::ContiguousField > const roads = inTile.getContiguousRoads();
		BOOST_FOREACH( Model::ContiguousField const & road, roads )
		{
			BOOST_FOREACH( Model::Area::Area area, road )
			{
				++areas[ area ];
			}
		}
		for ( int i = Model::Area::kTopLeft; i < Model::Area::kCentral; ++i )
		{
			assert( areas[ Model::Area::Area( i ) ] == 1 );
		}
		assert( areas[ Model::Area::kCentral ] == getExpectedCenterOutput( inTile.getCenter() ) );
	}
}

Model::Tile::Tile()
:
	mID( "D" ),
	mTop( kSideRoad ),
	mRight( kSideCity ),
	mBottom( kSideRoad ),
	mLeft( kSideField ),
	mCenter( kCenterProject ),
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
	assureEveryAreaIsTakenOnce( *this );
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

bool
Model::Tile::hasInn( Area::Area inRoadArea ) const
{
	if ( !isRoad( inRoadArea ) )
	{
		return false;
	}
	ContiguousRoad const completeRoad = getContiguousRoad( inRoadArea );
	std::vector< Area::Area > const inns = getInns();
	BOOST_FOREACH( Area::Area area, completeRoad )
	{
		if ( std::find( inns.begin(), inns.end(), area ) != inns.end() )
		{
			return true;
		}
	}
	return false;
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