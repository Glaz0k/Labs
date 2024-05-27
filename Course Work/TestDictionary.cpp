#include "TestDictionary.hpp"
#include <sstream>
#include <vector>

void testDictionary(std::ostream& out, const ICmds& mapI, const IOCmds& mapIO)
{
    std::vector< std::string > testLines{
        "SEARCH word\n",
        "REMOVE word\n",
        "MOSTCOMMON\n",
        "READTEXT\n",
        "READTEXT not_file\n",
        "READTEXT 1984.txt\n",
        "SEARCH big\n",
        "SEARCH brother\n",
        "SEARCH not_existed_word\n",
        "REMOVE the\n",
        "REMOVE not_existed_word\n",
        "REMOVE and\n",
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
            if (mapI.find(cmd) != mapI.cend())
            {
                mapI.at(cmd)(in);
            }
            else if (mapIO.find(cmd) != mapIO.cend())
            {
                mapIO.at(cmd)(in, out);
            }
            else
            {
                out << "<INVALID COMMAND>" << '\n';
            }
        }
        catch (const std::invalid_argument& e)
        {
            out << e.what() << '\n';
        }
    }
}
