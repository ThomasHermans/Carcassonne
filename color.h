#ifndef COLOR_H
#define COLOR_H

#include <string>

namespace Color
{
    enum Color
    {
        Red,
        Green,
        Blue,
        Yellow,
        Black,
        Gray
    };

    std::string colorToString(Color inColor);
}

#endif // COLOR_H
