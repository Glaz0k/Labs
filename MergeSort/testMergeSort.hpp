#ifndef TEST_MERGE_SORT_HPP
#define TEST_MERGE_SORT_HPP

#include <vector>
#include "mergeSort.hpp"

enum testCase
{
    worst,
    middle,
    best
};

namespace detail
{
    void generateTestCase(std::vector< long long >& data, testCase t);
    void generateWorst(std::vector< long long >& data);
    void separate(std::vector< long long >& data);
    void generateMiddle(std::vector< long long >& data);
    void generateBest(std::vector< long long >& data);
    bool isSorted(std::vector< long long >& data);
}

double testMergeSortMS(std::size_t n, testCase t);
std::vector< double > testMergeSortRangeMS(std::size_t range1, std::size_t range2, std::size_t step, testCase t);

#endif