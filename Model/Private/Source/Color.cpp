#include "Model/Color.h"

#include <cassert>

std::string
Color::colorToString( Color inColor )
{
	switch ( inColor )
	{
	case kRed:
		return "Red";
	case kGreen:
		return "Green";
	case kBlue:
		return "Blue";
	case kYellow:
		return "Yellow";
	case kBlack:
		return "Black";
	case kGray:
		return "Gray";
	}
	assert( !"Invalid Color to translate" );
	return "Red";
}
