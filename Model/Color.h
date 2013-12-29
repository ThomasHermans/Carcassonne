#ifndef COLOR_H
#define COLOR_H

#include <string>

namespace Model
{
	namespace Color
	{
		enum Color
		{
			kRed = 0,
			kGreen,
			kBlue,
			kYellow,
			kBlack,
			kGray
		};
	}

	std::string colorToString( Color::Color inColor );
}

#endif // COLOR_H
