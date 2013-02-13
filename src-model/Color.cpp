#include "src-model/Color.h"

std::string
Color::colorToString(Color inColor)
{
    std::string res = "";
    switch (inColor)
    {
    case Red:
        res.append("Red");
    case Green:
        res.append("Green");
    case Blue:
        res.append("Blue");
    case Yellow:
        res.append("Yellow");
    case Black:
        res.append("Black");
    case Gray:
        res.append("Gray");

    }
    return res;
}
