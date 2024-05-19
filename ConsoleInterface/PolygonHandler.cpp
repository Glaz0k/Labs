#include "PolygonHandler.hpp"
#include "PolygonCommands.hpp"

void kravchenko::PolygonHandler::inputPolygons(std::istream& in)
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
    auto& cmds = PolygonHandler::getCommandsMap();
    while (!in.eof())
    {
        std::string command;
        in >> command;
        try
        {
            auto cmdFunctor = cmds.at(command); 
            CommandArguments penis{ data_, in, out };
            cmdFunctor(penis);
            in.clear();
            in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
        }
        catch (const std::out_of_range&)
        {
            out << "<INVALID COMMAND>";
        }
        catch (const InvalidCommand& e)
        {
            out << e.what();
        }
        catch (...)
        {
            throw;
        }
    }
}

const std::map< std::string, std::function< void(kravchenko::CommandArguments&) > >& kravchenko::PolygonHandler::getCommandsMap()
{
    using namespace std::placeholders;
    static std::map< std::string, std::function< void(CommandArguments&) > > commands
    {
        { "AREA", Area{} }
    };
    return commands;
}

const char* kravchenko::InvalidCommand::what() const noexcept
{
    return "<INVALID COMMAND>";
}
