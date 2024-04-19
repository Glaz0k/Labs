#include "Shapes.hpp"
#include "..\DataStructIO\delimiterI.hpp"

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

std::istream& kravchenko::operator>>(std::istream& in, Polygon& poly)
{
    // TODO: вставьте здесь оператор return
}
