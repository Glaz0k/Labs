#ifndef TEST_DICTIONARY_HPP
#define TEST_DICTIONARY_HPP

#include <functional>
#include <ostream>
#include <string>
#include "RedBlackTree.hpp"

using ICmds = RedBlackTree< std::string, std::function< void(std::istream&) > >;
using IOCmds = RedBlackTree< std::string, std::function< void(std::istream&, std::ostream&) > >;
void testDictionary(std::ostream& out, const ICmds& mapI, const IOCmds& mapIO);

#endif