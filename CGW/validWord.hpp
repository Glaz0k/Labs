#ifndef VALID_WORD_HPP
#define VALID_WORD_HPP

#include <string>
#include <iostream>

namespace kravchenko
{
    struct ValidWord
    {
        std::string& word;
    };
    std::istream& operator>>(std::istream& in, ValidWord&& word);
}

#endif