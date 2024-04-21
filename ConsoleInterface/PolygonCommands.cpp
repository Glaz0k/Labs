#include "PolygonCommands.hpp"
#include <string>
#include <numeric>
#include <iomanip>
#include <limits>
#include <algorithm>
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
        { "RMECHO", RmEcho{} },
        { "RIGHTSHAPES", RightShapes{} }
    };
    return commands;
}

const char* kravchenko::InvalidCommand::what() const noexcept
{
    return "<INVALID COMMAND>";
}

void kravchenko::Area::operator()(std::vector< Polygon >& data, std::istream& in, std::ostream& out)
{
    std::string arg;
    in >> arg;
    std::function< double(double, const Polygon&) > getArea;
    using namespace std::placeholders;
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
    catch (const std::invalid_argument&)
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
    std::string arg;
    in >> arg;
    using namespace std::placeholders;
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
    std::string arg;
    in >> arg;
    std::function< bool(const Polygon&) > countPred;
    using namespace std::placeholders;
    if (arg == "EVEN")
    {
        countPred = std::bind(CountEvenOdd{}, _1, true);
    }
    else if (arg == "ODD")
    {
        countPred = std::bind(CountEvenOdd{}, _1, false);
    }
    else try
    {
        countPred = std::bind(CountNumOfVertex{}, _1, std::stoull(arg));
    }
    catch (const std::invalid_argument&)
    {
        throw InvalidCommand();
    }
    out << std::count_if(data.cbegin(), data.cend(), countPred);
}

bool kravchenko::CountEvenOdd::operator()(const Polygon& p, bool isEven)
{
    return (isEven == (p.points.size() % 2 == 0));
}

bool kravchenko::CountNumOfVertex::operator()(const Polygon& p, std::size_t numOfVertexes)
{
    return (p.points.size() == numOfVertexes);
}

void kravchenko::RmEcho::operator()(std::vector< Polygon >& data, std::istream& in, std::ostream& out)
{
    Polygon arg;
    in >> arg;
    if (in)
    {
        using namespace std::placeholders;
        auto identicalPred = std::bind(ConsecutiveIdenticalPolygon{}, _1, _2, arg);
        auto last = std::unique(data.begin(), data.end(), identicalPred);
        auto erasedNum = std::distance(last, data.end());
        data.erase(last, data.end());
        out << erasedNum;
    }
    else
    {
        throw InvalidCommand();
    }
}

bool kravchenko::ConsecutiveIdenticalPolygon::operator()(const Polygon& p1, const Polygon& p2, const Polygon& compared)
{
    return (compared.isIdentical(p1) && compared.isIdentical(p2));
}

void kravchenko::RightShapes::operator()(std::vector< Polygon >& data, std::istream& in, std::ostream& out)
{
    using namespace std::placeholders;
    out << std::count_if(data.cbegin(), data.cend(), CountRightPolygons{});
}

bool kravchenko::CountRightPolygons::operator()(const Polygon& p)
{
    return p.hasRightAngle();
}
