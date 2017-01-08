#include "Model/Rotation.h"

#include <cassert>

Model::Rotation
Model::rotateCW( Rotation inRotation )
{
	return Rotation( ( inRotation + kCw90 ) % ( kCw90 * 4 ) );
}

std::array< Model::Rotation, 4 >
Model::getAllRotations()
{
	return { kCw0, kCw90, kCw180, kCw270 };
}

std::string
Model::rotationToString( Rotation inRotation )
{
	switch ( inRotation )
	{
	case kCw0:
		return "0°";
	case kCw90:
		return "90°";
	case kCw180:
		return "180°";
	case kCw270:
		return "270°";
	}
	assert( !"Invalid Rotation" );
	return "Invalid Rotation";
}