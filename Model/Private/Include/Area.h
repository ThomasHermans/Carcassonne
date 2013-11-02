#ifndef AREA_H
#define AREA_H

#include <string>

namespace Area
{
	enum Area
	{
		TopLeft = 0,
		Top = 1,
		TopRight = 2,
		RightTop = 3,
		Right = 4,
		RightBottom = 5,
		BottomRight = 6,
		Bottom = 7,
		BottomLeft = 8,
		LeftBottom = 9,
		Left = 10,
		LeftTop = 11,
		Central = 12
	};

	std::string areaToString(Area inArea);

	Area oppositeSide( Area inArea );
}

#endif // AREA_H