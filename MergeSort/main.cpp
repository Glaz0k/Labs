#include <vector>
#include <limits>
#include <fstream>
#include "testMergeSort.hpp"

int main()
{
    std::ofstream file("tests.csv");
    if (!file.is_open())
        return -1;

    constexpr std::size_t rng = 4;
    constexpr std::size_t n[rng] = { 1000, 10000, 100000, 1000001 };

    file << "Case;";
    for (std::size_t i = 1; i < rng; i++)
    {
        for (std::size_t st = n[i - 1]; st < n[i]; st += n[i - 1])
            file << st << ';';
    }
    file << '\n';

    for (int j = 0; j <= 2; j++)
    {
        const char* testText = {};
        testCase t;
        switch (j)
        {
        case 0:
            t = testCase::worst;
            testText = "Worst";
            break;
        case 1:
            t = testCase::middle;
            testText = "Middle";
            break;
        case 2:
            t = testCase::best;
            testText = "Best";
            break;
        }
        file << testText << ';';
        for (std::size_t i = 1; i < rng; i++)
        {
            std::vector< double > rangeTest = testMergeSortRangeMS(n[i - 1], n[i] - 1, n[i - 1], t);
            std::copy(rangeTest.cbegin(), rangeTest.cend(), std::ostream_iterator< double >(file, ";"));
        }
        file << '\n';
    }
    file.close();
    return 0;
}