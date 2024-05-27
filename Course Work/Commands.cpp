#include "Commands.hpp"
#include <algorithm>
#include <fstream>
#include <limits>
#include <regex>
#include <utility>
#include <vector>

bool parseDictionaryWord(std::string& word)
{
    static const std::regex wordRegex(R"(([A-Za-z][A-Za-z\-']*[A-Za-z])|[IiAa])");
    std::smatch wordMatch;
    if (std::regex_search(word, wordMatch, wordRegex))
    {
        word = wordMatch.str();
        return true;
    }
    return false;
}

void cmdRead(std::istream& in, std::ostream&, FrequencyDictionary& dict)
{
    std::string fileName;
    in >> fileName;

    std::ifstream file(fileName);
    if (!file.is_open())
    {
        throw std::invalid_argument("<INVALID FILE>");
    }

    std::string word;
    while (file >> word)
    {
        if (parseDictionaryWord(word))
        {
            dict[word]++;
        }
    }
    file.close();
}

void cmdInsert(std::istream& in, std::ostream&, FrequencyDictionary& dict)
{
    std::string word;
    in >> word;
    if (!parseDictionaryWord(word))
    {
        throw std::invalid_argument("<INVALID WORD>");
    }

    size_t count = 0;
    if (in.peek() == '\n' || !(in >> count) || count == 0)
    {
        throw std::invalid_argument("<INVALID COUNT>");
    }
    if (dict.find(word) != dict.end())
    {
        throw std::invalid_argument("<WORD EXIST>");
    }
    dict[word] = count;
}

void cmdSearch(std::istream& in, std::ostream& out, const FrequencyDictionary& dict)
{
    std::string word;
    in >> word;
    if (!parseDictionaryWord(word))
    {
        throw std::invalid_argument("<INVALID WORD>");
    }

    auto toSearch = dict.find(word);
    if (toSearch == dict.cend())
    {
        throw std::invalid_argument("<WORD NOT FOUND>");
    }
    out << (*toSearch).second << '\n';
}

void cmdDelete(std::istream& in, std::ostream&, FrequencyDictionary& dict)
{
    std::string word;
    in >> word;
    if (!parseDictionaryWord(word))
    {
        throw std::invalid_argument("<INVALID WORD>");
    }

    auto toRemove = dict.find(word);
    if (toRemove == dict.end())
    {
        throw std::invalid_argument("<WORD NOT FOUND>");
    }
    dict.erase(toRemove);
}

void cmdClear(std::istream&, std::ostream& out, FrequencyDictionary& dict)
{
    dict.clear();
    out << "<DICTIONARY CLEARED>\n";
}

void cmdMostCommon(std::istream& in, std::ostream& out, const FrequencyDictionary& dict)
{
    if (dict.empty())
    {
        throw std::invalid_argument("<EMPTY DICTIONARY>");
    }
    size_t n = 3;
    if ((in.peek() != '\n') && (!(in >> n) || (n == 0)))
    {
        throw std::invalid_argument("<INVALID NUMBER>");
    }

    std::vector< std::pair< size_t, std::string > > sorted;
    sorted.reserve(dict.size());
    for (auto it = dict.cbegin(); it != dict.cend(); it++)
    {
        const auto& p = *it;
        sorted.push_back({ p.second, p.first });
    }
    std::sort(sorted.rbegin(), sorted.rend());
    for (auto it = sorted.cbegin(); (it != sorted.cend()) && n; ++it)
    {
        const auto& p = *it;
        out << p.second << " : " << p.first << '\n';
        --n;
    }
}
