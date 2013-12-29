#include "Model/Color.h"

#include <cassert>

std::string
Model::colorToString( Color::Color inColor )
{
	switch ( inColor )
	{
	case Color::kRed:
		return "Red";
	case Color::kGreen:
		return "Green";
	case Color::kBlue:
		return "Blue";
	case Color::kYellow:
		return "Yellow";
	case Color::kBlack:
		return "Black";
	case Color::kGray:
		return "Gray";
	}
	assert( !"Invalid Color to translate" );
	return "Red";
}
