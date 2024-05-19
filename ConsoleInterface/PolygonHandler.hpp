#ifndef POLYGON_HANDLER_HPP
#define POLYGON_HANDLER_HPP

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <functional>
#include <exception>
#include "Polygon.hpp"

namespace kravchenko
{
    struct CommandArguments
    {
        std::vector< kravchenko::Polygon >& data;
        std::istream& in;
        std::ostream& out;
    };

    class PolygonHandler
    {
    public:
        void inputPolygons(std::istream& in);
        void handleCommands(std::istream& in, std::ostream& out);
    private:
        std::vector< kravchenko::Polygon > data_;
        static const std::map< std::string, std::function< void(CommandArguments&) > >& getCommandsMap();
    };

    class InvalidCommand : public std::exception
    {
    public:
        const char* what() const noexcept override;
    };
}

#endif