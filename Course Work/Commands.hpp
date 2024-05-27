#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <istream>
#include <ostream>
#include <string>
#include "RedBlackTree.hpp"

#include <map>

using FrequencyDictionary = RedBlackTree< std::string, size_t >;
bool parseDictionaryWord(std::string& word);

void cmdRead(std::istream& in, std::ostream&, FrequencyDictionary& dict);
void cmdInsert(std::istream& in, std::ostream&, FrequencyDictionary& dict);
void cmdSearch(std::istream& in, std::ostream& out, const FrequencyDictionary& dict);
void cmdDelete(std::istream& in, std::ostream&, FrequencyDictionary& dict);
void cmdClear(std::istream&, std::ostream& out, FrequencyDictionary& dict);
void cmdMostCommon(std::istream& in, std::ostream& out, const FrequencyDictionary& dict);

#endif