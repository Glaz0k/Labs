#include "Polygon.hpp"
#include "..\DataStructIO\delimiterI.hpp"
#include <algorithm>
#include <cmath>
#include <functional>
#include <numeric>

std::istream& kravchenko::operator>>(std::istream& in, Point& p)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }
    using cDel = DelimiterI<false>;
    Point temp{ 0, 0 };
    in >> cDel{ '(' } >> temp.x >> cDel{ ';' } >> temp.y >> cDel{ ')' };
    if (in)
    {
        p = temp;
    }
    return in;
}

std::ostream& kravchenko::operator<<(std::ostream& out, const Point& p)
{
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
        return out;
    }
    out << '(' << p.x << ';' << p.y << ')';
    return out;
}

std::istream& kravchenko::operator>>(std::istream& in, Polygon& p)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }
    std::size_t nVertex = 0;
    in >> nVertex;

    std::vector< Point > temp;
    temp.reserve(nVertex);

    // don't know how to replace it w/o cycle
    for (std::size_t i = 0; in && i < nVertex; ++i)
    {
        Point p{ 0, 0 };
        if (in >> p)
        {
            temp.push_back(p);
        }
    }
    
    if (in)
    {
        p.points = temp;
    }
    return in;
}

std::ostream& kravchenko::operator<<(std::ostream& out, const Polygon& p)
{
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
        return out;
    }
    using outputIt = std::ostream_iterator< Point >;
    out << p.points.size() << ' ';
    std::copy(p.points.cbegin(), p.points.cend(), outputIt{ out, " " });
    return out;
}

std::vector< kravchenko::Polygon::Triangle > kravchenko::Polygon::getComponentTriangles() const
{
    using namespace std::placeholders;
    std::vector< Triangle > triangles;
    triangles.reserve(points.size() - 2);
    auto getTriangle = std::bind(Triangle::getTriangle, points[0], _1, _2);
    auto secondPoint = std::next(points.cbegin());
    auto thirdPoint = std::next(secondPoint);
    std::transform(thirdPoint, points.cend(), secondPoint, std::back_inserter(triangles), getTriangle);
    return triangles;
}

kravchenko::Polygon::Triangle kravchenko::Polygon::Triangle::getTriangle(const Point& a, const Point& b, const Point& c)
{
    return Triangle{ a, b, c };
}

double kravchenko::Polygon::Triangle::getArea() const
{
    return 0.5 * std::abs((p2.x - p1.x) * (p3.y - p1.y) - (p3.x - p1.x) * (p2.y - p1.y));
}

double kravchenko::Polygon::getArea() const
{
    std::vector< Triangle > triangles = getComponentTriangles();
    return std::accumulate(triangles.cbegin(), triangles.cend(), 0.0, Polygon::AccumulateTriangleArea{});
}

double kravchenko::Polygon::AccumulateTriangleArea::operator()(double acc, const Triangle& t)
{
    return acc + t.getArea();
}
