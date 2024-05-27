#ifndef TEST_DICTIONARY_HPP
#define TEST_DICTIONARY_HPP

#include <functional>
#include <ostream>
#include <string>
#include "RedBlackTree.hpp"

using Cmds = RedBlackTree< std::string, std::function< void(std::istream&, std::ostream&) > >;
void testDictionary(std::ostream& out, const Cmds& map);

#endif