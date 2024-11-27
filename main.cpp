#include <iostream>
#include <vector>
#include <set>
#include <stdio.h>
#include <string>
#include <cstdint>
#include <fstream>
#include <map>
#include "AsmExecutor.h"
#include "Cash.h"
#include "Parser.h"

static const size_t MEM_SIZE = 262144;
static const size_t ADDR_LEN = 18;
static const size_t CACHE_WAY = 4;
static const size_t CACHE_TAG_LEN = 9;
static const size_t CACHE_INDEX_LEN = 3;
static const size_t CACHE_OFFSET_LEN = 6;
static const size_t CACHE_SIZE = 2048;
static const size_t CACHE_LINE_SIZE = 64;
static const size_t CACHE_LINE_COUNT = 32;
static const size_t CACHE_SETS = 8;

int main(int argc, char** argv) {
    Parsing::Options options;
    if (!ParseArguments(argc, argv, options)) {
        return 0;
    }

    std::vector<std::string> instructions = Parsing::ParseAsm(options.asm_file_name);
    AsmExecutor asm_executor(instructions);
    asm_executor.Execute();

    if (options.replacement_policy == Parsing::ReplacementPolicy::Both) {
        float lru = asm_executor.PrintPercentLRU();
        float plru = asm_executor.PrintPercentPLRU();
        printf("LRU\thit rate: %3.4f%%\npLRU\thit rate: %3.4f%%\n", lru, plru);

    } else if (options.replacement_policy == Parsing::ReplacementPolicy::LRU) {
        float lru = asm_executor.PrintPercentLRU();
        printf("LRU\thit rate: %3.4f%%\n", lru);
    } else {
        float plru = asm_executor.PrintPercentPLRU();
        printf("pLRU\thit rate: %3.4f%%\n", plru);
    }

    return 0;
}
