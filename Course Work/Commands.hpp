#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <istream>
#include <ostream>
#include <string>
#include "RedBlackTree.hpp"

#include <map>

using FrequencyDictionary = RedBlackTree< std::string, size_t >;
bool parseDictionaryWord(std::string& word);

void cmdReadText(std::istream& in, FrequencyDictionary& dict);
void cmdInsert(std::istream& in, FrequencyDictionary& dict);
void cmdRemove(std::istream& in, FrequencyDictionary& dict);
void cmdSearch(std::istream& in, std::ostream& out, const FrequencyDictionary& dict);
void cmdMostCommon(std::istream& in, std::ostream& out, const FrequencyDictionary& dict);

#endif