#ifndef MERGE_SORT_HPP
#define MERGE_SORT_HPP

#include <iterator>
#include <functional>
#include <vector>

namespace detail
{
    template< class It, class Compare = std::less< typename It::value_type > >
    void merge(It begin, It mid, It end, std::vector< typename It::value_type >& buffer, Compare comp = {})
    {
        It left = begin;
        It right = mid;
        while (left != mid && right != end)
        {
            if (comp(*left, *right))
            {
                buffer.push_back(std::move(*left));
                left++;
            }
            else
            {
                buffer.push_back(std::move(*right));
                right++;
            }
        }
        if (left != mid)
            buffer.insert(buffer.end(), left, mid);
        else
            buffer.insert(buffer.end(), right, end);
        std::move(buffer.begin(), buffer.end(), begin);
        buffer.clear();
    }
}

template< class It, class Compare = std::less< typename It::value_type > >
void mergeSortIterative(It begin, It end, Compare comp = {})
{
    using diffType = typename std::iterator_traits< It >::difference_type;
    diffType size = std::distance(begin, end);

    std::vector< typename It::value_type > buffer;
    buffer.reserve(size);

    for (diffType part = 1; part < size; part *= 2)
    {
        It right = begin;
        for (diffType j = 0; j < size - part; j += 2 * part)
        {
            It left = right;
            It mid = std::next(right, part);
            right = (j + 2 * part < size) ? std::next(right, 2 * part) : end;
            detail::merge(left, mid, right, buffer, comp);
        }
    }
}

#endif