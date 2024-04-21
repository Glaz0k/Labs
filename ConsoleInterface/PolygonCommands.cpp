#include "PolygonCommands.hpp"
#include <string>
#include <numeric>
#include <iomanip>
#include <limits>
// #include "..\DataStructIO\streamGuard.hpp"

const std::map< std::string, std::function< void(std::vector< kravchenko::Polygon >&, std::istream&, std::ostream&) > >& kravchenko::getPolygonCommands()
{
    using namespace std::placeholders;
    static std::map< std::string, std::function< void(std::vector< kravchenko::Polygon >&, std::istream&, std::ostream&) > > commands
    {
        { "AREA", Area{} },
        { "MAX", std::bind(MinMax{}, _1, _2, _3, false) },
        { "MIN", std::bind(MinMax{}, _1, _2, _3, true) },
        { "COUNT", Count{} },
        { "RMECHO", RmEcho{} }
    };
    return commands;
}

const char* kravchenko::InvalidCommand::what() const noexcept
{
    return "<INVALID COMMAND>";
}

void kravchenko::Area::operator()(std::vector< Polygon >& data, std::istream& in, std::ostream& out)
{
    using namespace std::placeholders;
    std::string arg;
    in >> arg;
    std::function< double(double, const Polygon&) > getArea;
    if (arg == "EVEN")
    {
        getArea = std::bind(AccumulateAreaEvenOdd{}, _1, _2, true);
    }
    else if (arg == "ODD")
    {
        getArea = std::bind(AccumulateAreaEvenOdd{}, _1, _2, false);
    }
    else if (arg == "MEAN")
    {
        getArea = std::bind(AccumulateAreaMean{ data.size() }, _1, _2);
    }
    else try
    {
        getArea = std::bind(AccumulateAreaNumOfVertex{}, _1, _2, std::stoull(arg));
    }
    catch (...)
    {
        throw InvalidCommand();
    }
    // StreamGuard guard(out);
    out << std::setprecision(1) << std::fixed;
    out << std::accumulate(data.cbegin(), data.cend(), 0.0, getArea);
}

double kravchenko::AccumulateAreaEvenOdd::operator()(double acc, const Polygon& p, bool isEven)
{
    if (isEven == (p.points.size() % 2 == 0))
    {
        acc += p.getArea();
    }
    return acc;
}

double kravchenko::AccumulateAreaMean::operator()(double acc, const Polygon& p)
{
    return acc + (p.getArea() / numOfPolygons);
}

double kravchenko::AccumulateAreaNumOfVertex::operator()(double acc, const Polygon& p, std::size_t numOfVertexes)
{
    if (numOfVertexes == p.points.size())
    {
        acc += p.getArea();
    }
    return acc;
}

void kravchenko::MinMax::operator()(std::vector< Polygon >& data, std::istream& in, std::ostream& out, bool isMin)
{
    using namespace std::placeholders;
    std::string arg;
    in >> arg;
    if (arg == "AREA")
    {
        auto maxArea = std::bind(AccumulateMinMaxArea{}, _1, _2, isMin);
        double accInit = (isMin) ? std::numeric_limits< double >::max() : 0.0;
        // StreamGuard guard(out);
        out << std::setprecision(1) << std::fixed;
        out << std::accumulate(data.cbegin(), data.cend(), accInit, maxArea);
    }
    else if (arg == "VERTEXES")
    {
        auto maxVertexes = std::bind(AccumulateMinMaxVertexes{}, _1, _2, isMin);
        std::size_t accInit = (isMin) ? std::numeric_limits< std::size_t >::max() : 0;
        out << std::accumulate(data.cbegin(), data.cend(), accInit, maxVertexes);
    }
    else
    {
        throw InvalidCommand();
    }
}

double kravchenko::AccumulateMinMaxArea::operator()(double acc, const Polygon& p, bool isMin)
{
    return (isMin) ? std::min(acc, p.getArea()) : std::max(acc, p.getArea());
}

std::size_t kravchenko::AccumulateMinMaxVertexes::operator()(std::size_t acc, const Polygon& p, bool isMin)
{
    return (isMin) ? std::min(acc, p.points.size()) : std::max(acc, p.points.size());
}

void kravchenko::Count::operator()(std::vector< Polygon >& data, std::istream& in, std::ostream& out)
{
    using namespace std::placeholders;
    std::string arg;
    in >> arg;
    std::function< std::size_t(std::size_t, const Polygon&) > getCount;
    if (arg == "EVEN")
    {
        getCount = std::bind(AccumulateCountIf{}, _1, _2, true);
    }
    else if (arg == "ODD")
    {
        getCount = std::bind(AccumulateCountIf{}, _1, _2, false);
    }
    else try
    {
        getCount = std::bind(AccumulateCountNumOfVertex{}, _1, _2, std::stoull(arg));
    }
    catch (...)
    {
        throw InvalidCommand();
    }
    out << std::accumulate(data.cbegin(), data.cend(), 0, getCount);
}

std::size_t kravchenko::AccumulateCountIf::operator()(std::size_t acc, const Polygon& p, bool isEven)
{
    if (isEven == (p.points.size() % 2 == 0))
    {
        acc++;
    }
    return acc;
}

std::size_t kravchenko::AccumulateCountNumOfVertex::operator()(std::size_t acc, const Polygon& p, std::size_t numOfVertexes)
{
    if (p.points.size()  == numOfVertexes)
    {
        acc++;
    }
    return acc;
}

void kravchenko::RmEcho::operator()(std::vector< Polygon >& data, std::istream& in, std::ostream& out)
{
    Polygon arg;
    in >> arg;
    if (in)
    {
        // std::adjacent_find
        // *operator==(Polygon && Point)
        // std::remove_if
        // std::accumulate
    }
    else
    {
        throw InvalidCommand();
    }
}
