#ifndef MODEL_ROTATION_THHR_20131121
#define MODEL_ROTATION_THHR_20131121

#include <string>

namespace Model
{
	enum Rotation
	{
		kCw0 = 0,
		kCw90 = 3,
		kCw180 = 6,
		kCw270 = 9
	};

	std::string rotationToString( Rotation inRotation );
}

#endif