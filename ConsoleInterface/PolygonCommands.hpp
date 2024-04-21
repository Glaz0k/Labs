#ifndef POLYGON_COMMANDS_HPP
#define POLYGON_COMMANDS_HPP

#include <map>
#include <istream>
#include <vector>
#include <functional>
#include <exception>
#include "Polygon.hpp"

namespace kravchenko
{
    const std::map< std::string, std::function< void(std::vector< kravchenko::Polygon >&, std::istream&, std::ostream&) > >& getPolygonCommands();

    class InvalidCommand : std::exception
    {
    public:
        const char* what() const noexcept;
    };

    struct Area
    {
        void operator()(std::vector< Polygon >& data, std::istream& in, std::ostream& out);
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

    struct MinMax
    {
        void operator()(std::vector< Polygon >& data, std::istream& in, std::ostream& out, bool isMin);
    };
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
    struct AccumulateCountIf
    {
        std::size_t operator()(std::size_t acc, const Polygon& p, bool isEven);
    };
    struct AccumulateCountNumOfVertex
    {
        std::size_t operator()(std::size_t acc, const Polygon& p, std::size_t numOfVertexes);
    };

    struct RmEcho
    {
        void operator()(std::vector< Polygon >& data, std::istream& in, std::ostream& out);
    };
}

#endif