#pragma once
#include <vector>
#include <set>
#include <string>
#include <cstdint>

static const size_t MEM_SIZE_ = 262144;
static const size_t ADDR_LEN_ = 18;
static const size_t CACHE_WAY_ = 4;
static const size_t CACHE_TAG_LEN_ = 9;
static const size_t CACHE_INDEX_LEN_ = 3;
static const size_t CACHE_OFFSET_LEN_ = 6;
static const size_t CACHE_SIZE_ = 2048;
static const size_t CACHE_LINE_SIZE_ = 64;
static const size_t CACHE_LINE_COUNT_ = 32;
static const size_t CACHE_SETS_ = 8;

class Memory {
public:
    Memory();

    void StoreByte(int address, int value);

    void StoreHalf(int address, int value);

    void StoreWord(int address, int value);

    int LoadByte(int address);

    int LoadHalf(int address);

    int LoadWord(int address);

private:
    std::vector<uint8_t> mem;
};

struct CacheSetInfo {
    bool hit = false;
    int to_replace = 0;
};

class CacheLRU {
public:
    CacheLRU(Memory& memory): requests_count(0), hits_count(0), memory(memory) {
        CACHE_WAY_s.resize(CACHE_WAY_);
    }

    void StoreByte(int address, uint8_t value);

    void StoreHalf(int address, uint16_t value);

    void StoreWord(int address, uint32_t value);

    int LoadByte(int address);

    int LoadHalf(int address);

    int LoadWord(int address);

    int GetRequestsCount() const;

    int GetHitsCount() const;

private:
    CacheSetInfo GetCacheSetInfo(int index, int tag);

    void CopyCacheLineInMemory(int CACHE_WAY_, int index, int tag);

    void CopyMemoryInCacheLine(int CACHE_WAY_, int index, int tag);

    void ParseAddress(int address, int& offset, int& index, int& tag);

    struct CacheLine {
        CacheLine(): tag(-1), last_used(-1) {
            bytes.resize(CACHE_LINE_SIZE_);
        }

        void StoreByte(uint8_t value, int offset);

        void StoreHalf(uint16_t value, int offset);

        void StoreWord(uint32_t value, int offset);

        int LoadByte(int offset);

        int LoadHalf(int offset);

        int LoadWord(int offset);

        std::vector<uint8_t> bytes;
        int tag;
        int last_used;
    };

    struct CacheWay {
        CacheWay() {
            cache_lines.resize(CACHE_SETS_);
        }

        std::vector<CacheLine> cache_lines;
    };

    std::vector<CacheWay> CACHE_WAY_s;
    int requests_count = 0;
    int hits_count = 0;
    Memory& memory;
};

class CachePLRU {
public:
    CachePLRU(Memory& memory): requests_count(0), hits_count(0), memory(memory) {
        CACHE_WAY_s.resize(CACHE_WAY_);
    }

    void StoreByte(int address, uint8_t value);

    void StoreHalf(int address, uint16_t value);

    void StoreWord(int address, uint32_t value);

    int LoadByte(int address);

    int LoadHalf(int address);

    int LoadWord(int address);

    int GetRequestsCount() const;

    int GetHitsCount() const;

private:
    CacheSetInfo GetCacheSetInfo(int index, int tag);

    bool AllAreTrue(int index) const;

    void SetZeroCacheLines(int index, int exception_index);

    void CopyCacheLineInMemory(int CACHE_WAY_, int index, int tag);

    void CopyMemoryInCacheLine(int CACHE_WAY_, int index, int tag);

    void ParseAddress(int address, int& offset, int& index, int& tag);

    struct CacheLine {
        CacheLine(): tag(-1), last_used(-1) {
            bytes.resize(CACHE_LINE_SIZE_);
        }

        void StoreByte(uint8_t value, int offset);

        void StoreHalf(uint16_t value, int offset);

        void StoreWord(uint32_t value, int offset);

        int LoadByte(int offset);

        int LoadHalf(int offset);

        int LoadWord(int offset);

        std::vector<uint8_t> bytes;
        int tag;
        int last_used;
        bool mru_bit = false;
    };

    struct CacheWay {
        CacheWay() {
            cache_lines.resize(CACHE_SETS_);
        }

        std::vector<CacheLine> cache_lines;
    };

    std::vector<CacheWay> CACHE_WAY_s;
    int requests_count = 0;
    int hits_count = 0;
    Memory& memory;
};

