#include "RedBlackTree.hpp"
#include <string>

int main()
{
    RedBlackTree< std::string, size_t > test;
    test["test1"] = 1;
    test.insert({ "test2", 4 });
    test.insert({ "test3", 45 });
    test.insert({ "test4", 5367 });
    test.insert({ "test5", 212 });
    test.insert({ "test6", 234 });
    test.insert({ "test7", 221 });
    test.insert({ "test8", 20 });
    test.insert({ "test9", 24 });
    test.insert({ "test10", 282 });
    test.insert({ "teså11", 234 });
    auto it = test.find("test1");
    std::cout << (*it).second;
    test.erase("test8");
    return 0;
}