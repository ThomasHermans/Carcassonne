#ifndef COLOR_H
#define COLOR_H

#include <string>

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

    std::string colorToString( Color inColor );
}

#endif // COLOR_H
