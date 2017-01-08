#ifndef MODEL_ROTATION_THHR_20131121
#define MODEL_ROTATION_THHR_20131121

#include <string>
#include <array>

namespace Model
{
	enum Rotation
	{
		kCw0 = 0,
		kCw90 = 3,
		kCw180 = 6,
		kCw270 = 9
	};

	/**
	 *	Returns a Rotation that is 90 degrees clockwise
	 *	rotated from the inRotation.
	 */
	Rotation rotateCW( Rotation inRotation );

	std::array< Rotation, 4 > getAllRotations();

	std::string rotationToString( Rotation inRotation );
}

#endif