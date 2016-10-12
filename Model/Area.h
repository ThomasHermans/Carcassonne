#ifndef AREA_H
#define AREA_H

#include <array>
#include <vector>

namespace Model
{
	namespace Area
	{
		enum Area
		{
			kTopLeft = 0,
			kTop = 1,
			kTopRight = 2,
			kRightTop = 3,
			kRight = 4,
			kRightBottom = 5,
			kBottomRight = 6,
			kBottom = 7,
			kBottomLeft = 8,
			kLeftBottom = 9,
			kLeft = 10,
			kLeftTop = 11,
			kCentral = 12
		};
	}

	Area::Area oppositeSide( Area::Area inArea );

	std::array< Area::Area, 13 > getAllAreas();

	typedef std::vector< Area::Area > ContiguousProject;
	typedef std::vector< Area::Area > ContiguousField;
	typedef std::vector< Area::Area > ContiguousRoad;
	typedef std::vector< Area::Area > ContiguousCity;
}

#endif // AREA_H