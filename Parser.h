#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <set>

std::string ToLower(const std::string& str);

namespace Parsing {
    enum class ReplacementPolicy {
        Both,
        LRU,
        pLRU,
    };

    struct Options {
        ReplacementPolicy replacement_policy;
        std::string asm_file_name;
    };

    bool ParseArguments(int argc, char** argv, Options& options);

    std::vector<std::string> ParseAsm(const std::string& file_name);
}

namespace IntegersParsing {
    int GetDigitFromChar(char digit_char);

    int GetDecimalNumberFromString(const std::string& string_number);

    int GetHexNumberFromString(const std::string& string_number);

    int GetNumberFromString(const std::string& string_number);
}