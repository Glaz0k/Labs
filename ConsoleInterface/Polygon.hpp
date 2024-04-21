#ifndef POLYGON_HPP
#define POLYGON_HPP

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
    std::ostream& operator<<(std::ostream& out, const Point& p);

    struct Polygon
    {
        std::vector< Point > points;
        double getArea() const;
    private:
        struct Triangle
        {
            static Triangle getTriangle(const Point& a, const Point& b, const Point& c);
            const Point& p1;
            const Point& p2;
            const Point& p3;
            double getArea() const;
        };
        std::vector< Triangle > getComponentTriangles() const;
        struct AccumulateTriangleArea
        {
            double operator()(double acc, const Triangle& t);
        };
    };
    std::istream& operator>>(std::istream& in, Polygon& p);
    std::ostream& operator<<(std::ostream& out, const Polygon& p);
}

#endif