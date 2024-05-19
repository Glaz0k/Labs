#include <fstream>
#include <iostream>
#include <functional>
#include "dictionaryCommands.hpp"

int main(int argc, char* argv[])
{
    using namespace kravchenko;
    std::map< std::string, FrequencyDictionary > dictionaries;

    cmd::FreqArgs freqArgs;
    {
        using namespace std::placeholders;
        // freqArgs["WORD"] = cmd::freqWord;
        // freqArgs["LEAST"];
        // freqArgs["MOST"];
    }

    std::map< std::string, std::function< void(cmd::CmdStreams) > > cmds;
    {
        using namespace std::placeholders;
        cmds["SCANTEXT"] = std::bind(cmdScanText, _1, std::ref(dictionaries));
        cmds["NEW"] = std::bind(cmdNew, _1, std::ref(dictionaries));
        cmds["REMOVE"] = std::bind(cmdRemove, _1, std::ref(dictionaries));
        cmds["LIST"] = std::bind(cmdList, _1, std::cref(dictionaries));
        cmds["SAVE"] = std::bind(cmdSave, _1, std::cref(dictionaries));
        // cmds["FREQ"] = std::bind(cmdFreq, _1, std::cref(dictionaries), std::cref(freqArgs));
        // cmds["INTERSECT"] = std::bind(cmdIntersect, _1, std::ref(dictionaries));
        // cmds["UNION"] = std::bind(cmdUnion, _1, std::ref(dictionaries));
        // cmds["DIFFERENCE"] = std::bind(cmdDifference, _1, std::ref(dictionaries));
        // cmds["COMPLEMENT"] = std::bind(cmdComplement, _1, std::ref(dictionaries));
    }

    std::string command;
    while (std::cin >> command)
    {
        try
        {
            cmds.at(command)({ std::cin, std::cout });
            std::cout << '\n';
        }
        catch (const std::out_of_range&)
        {
            std::cout << "<INVALID COMMAND>" << '\n';
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
