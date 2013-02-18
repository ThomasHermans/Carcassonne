#include "src-model/Color.h"

std::string
Color::colorToString(Color inColor)
{
    switch (inColor)
    {
    case Red:
        return "Red";
    case Green:
        return "Green";
    case Blue:
        return "Blue";
    case Yellow:
        return "Yellow";
    case Black:
        return "Black";
    case Gray:
        return "Gray";
    default:
        return "Invalid color";
    }
}
