#include "testMergeSort.hpp"

#include <chrono>
#include <ratio>
#include <limits>
#include <thread>
#include <random>

void detail::generateTestCase(std::vector< long long >& data, testCase t)
{
    switch (t)
    {
    case testCase::worst:
        generateWorst(data);
        break;
    case testCase::middle:
        generateMiddle(data);
        break;
    case testCase::best:
        generateBest(data);
        break;
    default:
        break;
    }
}

void detail::generateWorst(std::vector< long long >& data)
{
    generateBest(data);
    separate(data);
}

void detail::separate(std::vector< long long >& data)
{
    if (data.size() < 2)
        return;
    if (data.size() < 3)
    {
        std::swap(data[0], data[1]);
        return;
    }
    std::size_t m = (data.size() + 1) / 2;
    std::vector< long long > left(m, 0);
    std::vector< long long > right(data.size() - m, 0);

    std::size_t i = 0;
    for (std::size_t j = 0; j < data.size(); j += 2)
    {
        left[i] = data[j];
        if (j < data.size() - 1)
            right[i] = data[j + 1];
        i++;
    }
    separate(left);
    separate(right);
    i = 0;
    for (std::size_t j = 0; j < left.size(); j++)
        data[i++] = left[j];
    for (std::size_t j = 0; j < right.size(); j++)
        data[i++] = right[j];
}

void detail::generateMiddle(std::vector< long long >& data)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution< long long > distr(LLONG_MIN, LLONG_MAX);
    for (long long& num : data)
        num = distr(gen);
}

void detail::generateBest(std::vector< long long >& data)
{
    long long n = 0 - (data.size() / 2ll);
    for (long long& num : data)
    {
        num = n;
        n++;
    }
}

bool detail::isSorted(std::vector< long long >& data)
{
    for (std::size_t i = 1; i < data.size(); i++)
        if (data[i - 1] > data[i])
            return false;
    return true;
}

double testMergeSortMS(std::size_t n, testCase t)
{
    std::vector< long long > data(n, 0);
    detail::generateTestCase(data, t);

    using timePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;
    const timePoint t1 = std::chrono::high_resolution_clock::now();
    mergeSortIterative(data.begin(), data.end());
    const timePoint t2 = std::chrono::high_resolution_clock::now();

    using dur = std::chrono::duration<double, std::milli>;
    const dur time = t2 - t1;

    return (detail::isSorted(data)) ? time.count() : 0.0;
}

std::vector< double > testMergeSortRangeMS(std::size_t range1, std::size_t range2, std::size_t step, testCase t)
{
    if (range2 < range1)
        return std::vector< double >();
    std::vector< double > range;
    range.reserve((range2 - range1) / step);
    for (std::size_t n = range1; n <= range2; n += step)
        range.push_back(testMergeSortMS(n, t));
    return range;
}