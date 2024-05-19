#ifndef DICTIONARY_COMMANDS_HPP
#define DICTIONARY_COMMANDS_HPP

#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <exception>
#include <algorithm>
#include <functional>

namespace kravchenko
{
    using FrequencyDictionary = std::map< std::string, size_t >;
    namespace cmd
    {
        struct CmdStreams
        {
            std::istream& in;
            std::ostream& out;
        };

        using FreqCommand = std::function< void(CmdStreams, const std::map< std::string, FrequencyDictionary >&) >;
        using FreqArgs = std::map< std::string, FreqCommand >;

        /*
        void freqWord(CmdStreams s, const std::map< std::string, FrequencyDictionary >& data);
        void freqLeast(CmdStreams s, const std::map< std::string, FrequencyDictionary >& data);
        void freqMost(CmdStreams s, const std::map< std::string, FrequencyDictionary >& data);

        template < class Compare >
        void freqPredicate(CmdStreams s, const std::map< std::string, FrequencyDictionary >& data, Compare comp);

        template<class Compare>
        void freqPredicate(CmdStreams s, const std::map<std::string, FrequencyDictionary>& data, Compare comp)
        {
            std::string dictName;
            s.in >> dictName;
        
            if (data.find(dictName) == data.cend())
            {
                throw std::invalid_argument("<INVALID COMMAND>");
            }
        
            size_t n = 10;
            if (s.in.peek() != '\n' && !(s.in >> n) && n == 0)
            {
                throw std::invalid_argument("<INVALID COMMAND>");
            }
        
            using WordPair = std::pair< std::string, size_t >;
            std::vector< WordPair > wordPairs;
            {
                using namespace std::placeholders;
                std::function< bool(const WordPair&, const WordPair&) > pairComp = std::bind(
                    comp,
                    std::bind(&WordPair::second, _1),
                    std::bind(&WordPair::second, _2)
                );
        
                std::copy(data.cbegin(), data.cend(), std::back_inserter(wordPairs));
                std::sort(wordPairs.begin(), wordPairs.end(), pairComp);
            }
        
            std::vector< std::string > sortedWords;
            std::function<std::string(const WordPair&)> getFirst = &WordPair::first;
            std::transform(wordPairs.begin(), wordPairs.end(), std::back_inserter(sortedWords), getFirst);
        
            using OutputItT = std::ostream_iterator< std::string >;
            n = std::min(n, sortedWords.size());
            std::copy_n(sortedWords, n, OutputItT{ s.out, "\n" });
        }
        */
    }

    void cmdScanText(cmd::CmdStreams s, std::map< std::string, FrequencyDictionary >& data);

    void cmdNew(cmd::CmdStreams s, std::map< std::string, FrequencyDictionary >& data);
    void cmdRemove(cmd::CmdStreams s, std::map< std::string, FrequencyDictionary >& data);
    void cmdList(cmd::CmdStreams s, const std::map< std::string, FrequencyDictionary >& data);
    void cmdSave(cmd::CmdStreams s, const std::map< std::string, FrequencyDictionary >& data);

    void cmdFreq(cmd::CmdStreams s, const std::map< std::string, FrequencyDictionary >& data, const cmd::FreqArgs& args);

    /*
    void cmdIntersect(cmd::CmdStreams s, std::map< std::string, FrequencyDictionary >& data);
    void cmdUnion(cmd::CmdStreams s, std::map< std::string, FrequencyDictionary >& data);
    void cmdDifference(cmd::CmdStreams s, std::map< std::string, FrequencyDictionary >& data);
    void cmdComplement(cmd::CmdStreams s, std::map< std::string, FrequencyDictionary >& data);
    */
}

#endif