#include <fstream>
#include <functional>
#include <iostream>
#include <cstring>
#include <string>
#include "Commands.hpp"
#include "RedBlackTree.hpp"
#include "TestTree.hpp"
#include "TestDictionary.hpp"

int main(int argc, char* argv[])
{
    FrequencyDictionary dict;
    RedBlackTree< std::string, std::function< void(std::istream&, std::ostream&) > > cmds;
    {
        using namespace std::placeholders;
        cmds["READ"] = std::bind(cmdRead, _1, _2, std::ref(dict));
        cmds["INSERT"] = std::bind(cmdInsert, _1, _2, std::ref(dict));
        cmds["DELETE"] = std::bind(cmdDelete, _1, _2, std::ref(dict));
        cmds["SEARCH"] = std::bind(cmdSearch, _1, _2, std::cref(dict));
        cmds["MOSTCOMMON"] = std::bind(cmdMostCommon, _1, _2, std::cref(dict));
        cmds["CLEAR"] = std::bind(cmdClear, _1, _2, std::ref(dict));
    }

    if (argc == 2 && std::strcmp(argv[1], "--test") == 0)
    {
        testTree(std::cout);
        testDictionary(std::cout, cmds);
        return 0;
    }

    std::string cmd;
    while (std::cin >> cmd)
    {
        try
        {
            cmds.at(cmd)(std::cin, std::cout);
        }
        catch (const std::out_of_range&)
        {
            std::cout << "<INVALID COMMAND>\n";
        }
        catch (const std::invalid_argument& e)
        {
            std::cout << e.what() << '\n';
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }

    return 0;
}