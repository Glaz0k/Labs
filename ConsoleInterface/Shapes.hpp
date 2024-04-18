#ifndef SHAPES_HPP
#define SHAPES_HPP

#include <vector>
#include <iostream>

namespace kravchenko
{
    struct Point
    {
        int x;
        int y;
    };
    std::istream& operator>>(std::istream& in, Point& p);

    struct Polygon
    {
        std::vector< Point > points;
    };
}

#endif