#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <istream>
#include <ostream>
#include <string>
#include <regex>
#include "RedBlackTree.hpp"

using FrequencyDictionary = RedBlackTree< std::string, size_t >;

template< bool isRead >
bool parseDictWord(const std::string& word);

void cmdRead(std::istream& in, std::ostream&, FrequencyDictionary& dict);
void cmdInsert(std::istream& in, std::ostream&, FrequencyDictionary& dict);
void cmdSearch(std::istream& in, std::ostream& out, const FrequencyDictionary& dict);
void cmdDelete(std::istream& in, std::ostream&, FrequencyDictionary& dict);
void cmdClear(std::istream&, std::ostream& out, FrequencyDictionary& dict);
void cmdMostCommon(std::istream& in, std::ostream& out, const FrequencyDictionary& dict);

template< bool isRead >
bool parseDictWord(std::string& word)
{
    static const std::regex wordRegex(R"(([A-Za-z][A-Za-z\-']*[A-Za-z])|[IiAa])");
    std::smatch wordMatch;
    if (isRead && std::regex_search(word, wordMatch, wordRegex))
    {
        word = wordMatch.str();
        return true;
    }
    else if (!isRead && std::regex_match(word, wordMatch, wordRegex))
    {
        word = wordMatch.str();
        return true;
    }
    return false;
}

#endif