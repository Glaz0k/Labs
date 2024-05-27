#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include "Commands.hpp"
#include "RedBlackTree.hpp"
#include "TestDictionary.hpp"

int main(int argc, char* argv[])
{
    std::cout << argc << '\n';
    std::cout << argv[1] << '\n';
    FrequencyDictionary dict;
    RedBlackTree< std::string, std::function< void(std::istream&) > > cmdsI;
    {
        using namespace std::placeholders;
        cmdsI["READTEXT"] = std::bind(cmdReadText, _1, std::ref(dict));
        cmdsI["INSERT"] = std::bind(cmdInsert, _1, std::ref(dict));
        cmdsI["REMOVE"] = std::bind(cmdInsert, _1, std::ref(dict));
    }
    RedBlackTree< std::string, std::function< void(std::istream&, std::ostream&) > > cmdsIO;
    {
        using namespace std::placeholders;
        cmdsIO["SEARCH"] = std::bind(cmdSearch, _1, _2, std::cref(dict));
        cmdsIO["MOSTCOMMON"] = std::bind(cmdMostCommon, _1, _2, std::cref(dict));
    }

    if (argc == 2 && std::string(argv[1]) == "--test")
    {
        testDictionary(std::cout, cmdsI, cmdsIO);
        return 0;
    }

    std::string cmd;
    while (std::cin >> cmd)
    {
        try
        {
            if (cmdsI.find(cmd) != cmdsI.end())
            {
                cmdsI[cmd](std::cin);
            }
            else if (cmdsIO.find(cmd) != cmdsIO.end())
            {
                cmdsIO[cmd](std::cin, std::cout);
            }
            else
            {
                std::cout << "<INVALID COMMAND>" << '\n';
            }
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