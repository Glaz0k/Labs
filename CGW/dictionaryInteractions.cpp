#include "dictionaryInteractions.hpp"
#include <algorithm>
#include <functional>
#include <utility>
#include <vector>

void kravchenko::dictIntersect(const FrequencyDictionary& lhs, const FrequencyDictionary& rhs, FrequencyDictionary& out)
{
    std::vector< std::string > wordsOut;
    FrequencyDictionary newDict;

    using In = std::vector< std::string >::const_iterator;
    using Out = std::back_insert_iterator< std::vector< std::string > >;
    getSetWords(lhs, rhs, wordsOut, std::set_intersection<In, In, Out>);

    for (const std::string& word : wordsOut)
    {
        newDict[word] = lhs.at(word);
    }
    out = std::move(newDict);
}

void kravchenko::dictUnion(const FrequencyDictionary& lhs, const FrequencyDictionary& rhs, FrequencyDictionary& out)
{
    std::vector< std::string > wordsOut;
    FrequencyDictionary newDict;

    using In = std::vector< std::string >::const_iterator;
    using Out = std::back_insert_iterator< std::vector< std::string > >;
    getSetWords(lhs, rhs, wordsOut, std::set_union<In, In, Out>);

    for (const std::string& word : wordsOut)
    {
        try
        {
            newDict[word] = lhs.at(word);
        }
        catch (const std::out_of_range&)
        {
            newDict[word] = rhs.at(word);
        }
    }
    out = std::move(newDict);
}

void kravchenko::dictDifference(const FrequencyDictionary& lhs, const FrequencyDictionary& rhs, FrequencyDictionary& out)
{
    std::vector< std::string > wordsOut;
    FrequencyDictionary newDict;

    using In = std::vector< std::string >::const_iterator;
    using Out = std::back_insert_iterator< std::vector< std::string > >;
    getSetWords(lhs, rhs, wordsOut, std::set_difference<In, In, Out>);

    for (const std::string& word : wordsOut)
    {
        newDict[word] = lhs.at(word);
    }
    out = std::move(newDict);
}

void kravchenko::dictComplement(const FrequencyDictionary& lhs, const FrequencyDictionary& rhs, FrequencyDictionary& out)
{
    dictDifference(rhs, lhs, out);
}
