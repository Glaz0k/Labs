#include "TestDictionary.hpp"
#include <sstream>
#include <vector>

void testDictionary(std::ostream& out, const Cmds& map)
{
    out << "\nStarted dictionary test\n";

    std::vector< std::string > testLines{
        "SEARCH word\n",
        "DELETE word\n",
        "MOSTCOMMON\n",
        "READ\n",
        "READ not_file\n",
        "READ 1984.txt\n",
        "SEARCH big\n",
        "SEARCH brother\n",
        "SEARCH abrakadabra\n",
        "DELETE the\n",
        "DELETE abrakadabra\n",
        "DELETE and\n",
        "INSERT bus 20000\n",
        "INSERT 11234 150\n",
        "INSERT souce\n",
        "INSERT 1942\n",
        "INSERT the 20000\n",
        "MOSTCOMMON\n",
        "MOSTCOMMON 1\n",
        "MOSTCOMMON JUNK\n",
        "MOSTCOMMON 0\n",
        "MOSTCOMMON 10\n",
        "CLEAR\n",
        "MOSTCOMMON 20\n"
    };

    for (auto& line : testLines)
    {
        out << line;
        std::istringstream in(line);
        std::string cmd;
        in >> cmd;
        try
        {
            map.at(cmd)(in, out);
        }
        catch (const std::out_of_range&)
        {
            out << "<INVALID COMMAND>\n";
        }
        catch (const std::invalid_argument& e)
        {
            out << e.what() << '\n';
        }
    }

    out << "\nEnded dictionary test\n";
}
