#include "Model/Rotation.h"

#include <cassert>

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