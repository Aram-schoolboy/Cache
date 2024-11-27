#include "Parser.h"

std::string ToLower(const std::string& str) {
    std::string lower_string;
    for (char i: str) {
        lower_string += std::tolower(i);
    }
    return lower_string;
}

bool Parsing::ParseArguments(int argc, char** argv, Options& options) {
    for (int arg_index = 1; arg_index < argc; arg_index += 2) {
        std::string arg_name = argv[arg_index];
        if (arg_name == "--replacement") {
            std::string replacement_policy = argv[arg_index + 1];
            if (replacement_policy == "0") {
                options.replacement_policy = ReplacementPolicy::Both;
            } else if (replacement_policy == "1") {
                options.replacement_policy = ReplacementPolicy::LRU;
            } else {
                options.replacement_policy = ReplacementPolicy::pLRU;
            }
        } else if (arg_name == "--asm") {
            std::string asm_file_name = argv[arg_index + 1];
            options.asm_file_name = asm_file_name;
        } else {
            std::cerr << "Compile asm code to binary not supported";
            return false;
        }
    }

    return true;
}

std::vector<std::string> Parsing::ParseAsm(const std::string& file_name) {
    std::vector<std::string> instructions;
    std::ifstream file(file_name);
    std::string instruction;
    while (std::getline(file, instruction)) {
        if (instruction.size() > 2) {
            instructions.push_back(instruction);
        }
    }
    return instructions;
}

namespace IntegersParsing {
    int GetDigitFromChar(char digit_char) {
        static const int kNineCode = 57;
        static const int kZeroCode = 48;
        static const int kaCode = 97;
        static const int kfCode = 102;
        static const int kACode = 65;
        static const int kFCode = 70;

        int char_code = static_cast<unsigned char>(digit_char);

        if (char_code >= kZeroCode && char_code <= kNineCode) {
            return char_code - kZeroCode;
        } if (char_code >= kaCode && char_code <= kfCode) {
            return char_code - kaCode + 10;
        } if (char_code >= kACode && char_code <= kFCode) {
            return char_code - kACode + 10;
        }

        std::cerr << "not a digit " << digit_char << '\n';
        return -100;
    }

    int GetDecimalNumberFromString(const std::string& string_number) {
        int result = 0;
        int sign = 1;
        for (int i = 0; i < string_number.size(); ++i) {
            result *= 10;

            if (string_number[i] == '-') { sign = -1; continue;}

            int digit = GetDigitFromChar(string_number[i]);
            result += digit;
        }
        return result * sign;
    }

    int GetHexNumberFromString(const std::string& string_number) {
        int result = 0;
        int sign = 1;
        for (int i = 2; i < string_number.size(); ++i) {
            result *= 16;

            if (string_number[i] == '-') { sign = -1; continue;}

            int digit = GetDigitFromChar(string_number[i]);
            result += digit;
        }
        return result * sign;
    }

    int GetNumberFromString(const std::string& string_number) {
        if (string_number.size() > 2 && string_number[0] == '0' && string_number[1] == 'x') {
            return GetHexNumberFromString(string_number);
        }
        return GetDecimalNumberFromString(string_number);
    }
}
