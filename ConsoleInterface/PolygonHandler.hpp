#ifndef POLYGON_HANDLER_HPP
#define POLYGON_HANDLER_HPP

#include <vector>
#include <iostream>
#include "Polygon.hpp"

namespace kravchenko
{
    class PolygonHandler
    {
    public:
        void inputData(std::istream& in);
        void handleCommands(std::istream& in, std::ostream& out);
    private:
        std::vector< kravchenko::Polygon > data_;
    };
}

#endif