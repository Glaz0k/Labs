#include "dictionaryCommands.hpp"
#include <fstream>
#include <limits>
#include "validWord.hpp"
#include "dictionaryInteractions.hpp"

void kravchenko::cmdScanText(cmd::CmdStreams s, std::map<std::string, FrequencyDictionary>& data)
{
    std::string dictName;
    s.in >> dictName;

    std::string fileName;
    s.in >> fileName;

    std::fstream file(fileName, std::ios_base::in);
    if (!file.is_open())
    {
        throw std::invalid_argument("<INVALID FILE>");
    }

    FrequencyDictionary& saveDict = data[dictName];
    while (!file.eof())
    {
        std::string word;
        if (file >> ValidWord{ word })
        {
            ++saveDict[word];
        }
        file.clear();
        file.ignore(std::numeric_limits< std::streamsize >::max(), ' ');
    }
    file.close();
}

void kravchenko::cmdNew(cmd::CmdStreams s, std::map<std::string, FrequencyDictionary>& data)
{
    std::string dictName;
    s.in >> dictName;

    if (data.find(dictName) == data.cend())
    {
        data[dictName];
    }
    else
    {
        throw std::invalid_argument("<EXIST>");
    }
}

void kravchenko::cmdRemove(cmd::CmdStreams s, std::map<std::string, FrequencyDictionary>& data)
{
    std::string dictName;
    s.in >> dictName;

    auto rmIterator = data.find(dictName);
    if (rmIterator != data.end())
    {
        data.erase(rmIterator);
    }
    else
    {
        throw std::invalid_argument("<INVALID COMMAND>");
    }
}

void kravchenko::cmdList(cmd::CmdStreams s, const std::map<std::string, FrequencyDictionary>& data)
{
    using OutputItT = std::ostream_iterator< std::string >;
    using DictPair = std::pair<std::string, FrequencyDictionary>;
    std::function<std::string(const DictPair&)> getFirst = &DictPair::first;
    std::transform(data.cbegin(), data.cend(), OutputItT{ s.out, " " }, getFirst);
}

void kravchenko::cmdSave(cmd::CmdStreams s, const std::map<std::string, FrequencyDictionary>& data)
{
    std::vector< std::string > dictNames;
    if (s.in.peek() == '\n')
    {
        using DictPair = std::pair<std::string, FrequencyDictionary>;
        std::function<std::string(const DictPair&)> getfirst = &DictPair::first;
        std::transform(data.cbegin(), data.cend(), std::back_inserter(dictNames), getfirst);
    }
    else
    {
        while (s.in.peek() != '\n')
        {
            std::string argument;
            s.in >> argument;
            if (data.find(argument) == data.cend())
            {
                throw std::invalid_argument("<INVALID COMMAND>");
            }
            dictNames.push_back(argument);
        }
    }

    for (std::string name : dictNames)
    {
        std::fstream file(name + ".txt", std::ios_base::out);
        if (!file.is_open())
        {
            s.out << "<INVALID FILE FOR " + name + ">\n";
            continue;
        }
        for (auto p : data.at(name))
        {
            file << p.first << " : " << p.second << '\n';
        }
        file.close();
    }
}

void kravchenko::cmdFreq(cmd::CmdStreams s, const std::map<std::string, FrequencyDictionary>& data, const cmd::FreqArgs& args)
{
    std::string argument;
    s.in >> argument;
    try
    {
        args.at(argument)(s, data);
    }
    catch (const std::out_of_range&)
    {
        throw std::invalid_argument("<INVALID COMMAND>");
    }
}

void kravchenko::cmdIntersect(cmd::CmdStreams s, std::map<std::string, FrequencyDictionary>& data)
{
    std::string newDictName;
    s.in >> newDictName;
    if (data.find(newDictName) != data.cend())
    {
        throw std::invalid_argument("<INVALID COMMAND>");
    }

    std::vector< std::string > dictNames;
    while (s.in.peek() != '\n')
    {
        std::string argument;
        s.in >> argument;
        if (data.find(argument) == data.cend())
        {
            throw std::invalid_argument("<INVALID COMMAND>");
        }
        dictNames.push_back(argument);
    }
    if (dictNames.size() < 2)
    {
        throw std::invalid_argument("<INVALID COMMAND>");
    }

    FrequencyDictionary newDict;
    dictIntersect(data[dictNames[0]], data[dictNames[1]], newDict);
    auto intersectPred = [&](const std::pair< std::string, FrequencyDictionary >& dictP) {
        dictIntersect(newDict, dictP.second, newDict);
    };
    std::for_each(std::next(data.cbegin(), 2), data.cend(), intersectPred);
    if (newDict.size() == 0)
    {
        throw std::invalid_argument("<NO INTERSECTION>");
    }
    data[newDictName] = newDict;
}

void kravchenko::cmd::freqWord(cmd::CmdStreams s, const std::map<std::string, FrequencyDictionary>& data)
{
    std::string dictName;
    s.in >> dictName;

    std::string word;
    s.in >> word;

    try
    {
        s.out << data.at(dictName).at(word);
    }
    catch (const std::out_of_range&)
    {
        throw std::invalid_argument("<INVALID COMMAND>");
    }
}

void kravchenko::cmd::freqLeast(CmdStreams s, const std::map<std::string, FrequencyDictionary>& data)
{
    freqPredicate(s, data, std::less< size_t >{});
}

void kravchenko::cmd::freqMost(cmd::CmdStreams s, const std::map<std::string, FrequencyDictionary>& data)
{
    freqPredicate(s, data, std::greater< size_t >{});
}
