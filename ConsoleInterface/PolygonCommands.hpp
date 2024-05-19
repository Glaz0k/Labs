#ifndef POLYGON_COMMANDS_HPP
#define POLYGON_COMMANDS_HPP

#include <iostream>
#include <vector>
#include "Polygon.hpp"
#include "PolygonHandler.hpp"

namespace kravchenko
{
    struct Area
    {
        void operator()(CommandArguments& args);
    };
    struct AccumulateAreaEvenOdd
    {
        double operator()(double acc, const Polygon& p, bool isEven);
    };
    struct AccumulateAreaMean
    {
        std::size_t numOfPolygons;
        double operator()(double acc, const Polygon& p);
    };
    struct AccumulateAreaNumOfVertex
    {
        double operator()(double acc, const Polygon& p, std::size_t numOfVertexes);
    };

    template< bool isMin >
    void MinMax(std::vector< Polygon >& data, std::istream& in, std::ostream& out);

    struct AccumulateMinMaxArea
    {
        double operator()(double acc, const Polygon& p, bool isMin);
    };
    struct AccumulateMinMaxVertexes
    {
        std::size_t operator()(std::size_t acc, const Polygon& p, bool isMin);
    };

    struct Count
    {
        void operator()(std::vector< Polygon >& data, std::istream& in, std::ostream& out);
    };
    struct CountEvenOdd
    {
        bool operator()(const Polygon& p, bool isEven);
    };
    struct CountNumOfVertex
    {
        bool operator()(const Polygon& p, std::size_t numOfVertexes);
    };

    struct RmEcho
    {
        void operator()(std::vector< Polygon >& data, std::istream& in, std::ostream& out);
    };
    struct ConsecutiveIdenticalPolygon
    {
        bool operator()(const Polygon& p1, const Polygon& p2, const Polygon& compared);
    };

    struct RightShapes
    {
        void operator()(std::vector< Polygon >& data, std::istream& in, std::ostream& out);
    };
    struct CountRightPolygons
    {
        bool operator()(const Polygon& p);
    };
}

#endif