#ifndef DICTIONARY_INTERACTION_HPP
#define DICTIONARY_INTERACTION_HPP

#include "dictionaryCommands.hpp"

namespace kravchenko
{
    void dictIntersect(const FrequencyDictionary& lhs, const FrequencyDictionary& rhs, FrequencyDictionary& out);
    void dictUnion(const FrequencyDictionary& lhs, const FrequencyDictionary& rhs, FrequencyDictionary& out);
    void dictDifference(const FrequencyDictionary& lhs, const FrequencyDictionary& rhs, FrequencyDictionary& out);
    void dictComplement(const FrequencyDictionary& lhs, const FrequencyDictionary& rhs, FrequencyDictionary& out);

    template < class SetOp >
    void getSetWords(const FrequencyDictionary& lhs, const FrequencyDictionary& rhs, std::vector< std::string >& out, SetOp functor);
}

template<class SetOp >
void kravchenko::getSetWords(const FrequencyDictionary& lhs, const FrequencyDictionary& rhs, std::vector<std::string>& out, SetOp functor)
{
    std::vector< std::string > words1;
    std::vector< std::string > words2;
    std::function< std::string(const std::pair< std::string, size_t >) > getWord = &std::pair< std::string, size_t >::first;
    std::transform(lhs.cbegin(), lhs.cend(), std::back_inserter(words1), getWord);
    std::transform(rhs.cbegin(), rhs.cend(), std::back_inserter(words2), getWord);
    std::vector< std::string > wordsOut;
    functor(words1.cbegin(), words1.cend(), words2.cbegin(), words2.cend(), std::back_inserter(wordsOut));
    out = std::move(wordsOut);
}

#endif