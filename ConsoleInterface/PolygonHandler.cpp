#include "PolygonHandler.hpp"
#include <string>
#include <sstream>
#include "PolygonCommands.hpp"

void kravchenko::PolygonHandler::inputData(std::istream& in)
{
    using inputIt = std::istream_iterator< Polygon >;
    while (!in.eof())
    {
        std::copy(inputIt{ in }, inputIt{}, std::back_inserter(data_));
        in.clear();
        in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
}

void kravchenko::PolygonHandler::handleCommands(std::istream& in, std::ostream& out)
{
    auto& cmds = getPolygonCommands();
    while (!in.eof())
    {
        std::string command;
        in >> command;
        std::string line;
        std::getline(in, line, '\n');
        std::istringstream argStream(line);
        try
        {
            auto cmdFunctor = cmds.at(command);
            cmdFunctor(data_, argStream, out);
        }
        catch (const std::out_of_range&)
        {
            out << "<INVALID COMMAND>";
        }
        catch (const InvalidCommand& e)
        {
            out << e.what();
        }
        catch (const std::exception&)
        {
            throw;
        }
    }
}
