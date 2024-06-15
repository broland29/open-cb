#include "../headers/Color.h"


std::ostream& operator<<(std::ostream& os, const Color& color)
{
    if (color == Color::WHITE)
    {
        os << "WHITE";
    }
    else if (color == Color::BLACK)
    {
        os << "BLACK";
    }
    return os;
}
