#include "Cash.h"

Memory::Memory() {
    mem.resize(MEM_SIZE_, 0);
}

void Memory::StoreByte(int address, int value) {
    uint32_t* p = reinterpret_cast<uint32_t*>(&value);
    mem[address] = ((*p) & 0xff);
}

void Memory::StoreHalf(int address, int value) {
    uint32_t* pp = reinterpret_cast<uint32_t*>(&value);
    uint32_t p = *pp;
    ++address;
    mem[address--] = (p & 0xff);
    mem[address] = ((p >> 8) & 0xff);
}

void Memory::StoreWord(int address, int value) {
    uint32_t* pp = reinterpret_cast<uint32_t*>(&value);
    uint32_t p = *pp;
    address += 3;
    mem[address--] = (p & 0xff);
    mem[address--] = ((p >> 8) & 0xff);
    mem[address--] = ((p >> 16) & 0xff);
    mem[address--] = ((p >> 24) & 0xff);
}

int Memory::LoadByte(int address) {
    uint8_t bits = mem[address];
    return bits;
}

int Memory::LoadHalf(int address) {
    uint16_t bits = 0;
    bits += mem[address++];
    bits <<= 8;
    bits += mem[address];
    return bits;
}

int Memory::LoadWord(int address) {
    uint32_t bits = 0;
    for (int i = 0; i < 4; ++i) {
        bits += mem[address++];
        if (i != 3) { bits <<= 8; }
    }
    int* p = reinterpret_cast<int*>(&bits);
    return *p;
}


void CacheLRU::StoreByte(int address, uint8_t value) {
    ++requests_count;
    int offset, index, tag;
    ParseAddress(address, offset, index, tag);

    CacheSetInfo info = GetCacheSetInfo(index, tag);
    if (!info.hit) {
        CopyCacheLineInMemory(info.to_replace, index, tag);
        CopyMemoryInCacheLine(info.to_replace, index, tag);
    } else {
        ++hits_count;
    }

    CACHE_WAY_s[info.to_replace].cache_lines[index].last_used = requests_count;
    CACHE_WAY_s[info.to_replace].cache_lines[index].StoreByte(value, offset);
}

void CacheLRU::StoreHalf(int address, uint16_t value) {
    ++requests_count;
    int offset, index, tag;
    ParseAddress(address, offset, index, tag);

    CacheSetInfo info = GetCacheSetInfo(index, tag);
    if (!info.hit) {
        CopyCacheLineInMemory(info.to_replace, index, tag);
        CopyMemoryInCacheLine(info.to_replace, index, tag);
    } else {
        ++hits_count;
    }

    CACHE_WAY_s[info.to_replace].cache_lines[index].last_used = requests_count;
    CACHE_WAY_s[info.to_replace].cache_lines[index].StoreHalf(value, offset);
}

void CacheLRU::StoreWord(int address, uint32_t value) {
    ++requests_count;
    int offset, index, tag;
    ParseAddress(address, offset, index, tag);

    CacheSetInfo info = GetCacheSetInfo(index, tag);
    if (!info.hit) {
        CopyCacheLineInMemory(info.to_replace, index, tag);
        CopyMemoryInCacheLine(info.to_replace, index, tag);
    } else {
        ++hits_count;
    }

    CACHE_WAY_s[info.to_replace].cache_lines[index].last_used = requests_count;
    CACHE_WAY_s[info.to_replace].cache_lines[index].StoreWord(value, offset);
}

int CacheLRU::LoadByte(int address) {
    ++requests_count;
    int offset, index, tag;
    ParseAddress(address, offset, index, tag);

    CacheSetInfo info = GetCacheSetInfo(index, tag);
    if (!info.hit) {
        CopyCacheLineInMemory(info.to_replace, index, tag);
        CopyMemoryInCacheLine(info.to_replace, index, tag);
    } else {
        ++hits_count;
    }

    CACHE_WAY_s[info.to_replace].cache_lines[index].last_used = requests_count;
    return CACHE_WAY_s[info.to_replace].cache_lines[index].LoadByte(offset);
}

int CacheLRU::LoadHalf(int address) {
    ++requests_count;
    int offset, index, tag;
    ParseAddress(address, offset, index, tag);

    CacheSetInfo info = GetCacheSetInfo(index, tag);
    if (!info.hit) {
        CopyCacheLineInMemory(info.to_replace, index, tag);
        CopyMemoryInCacheLine(info.to_replace, index, tag);
    } else {
        ++hits_count;
    }

    CACHE_WAY_s[info.to_replace].cache_lines[index].last_used = requests_count;
    return CACHE_WAY_s[info.to_replace].cache_lines[index].LoadHalf(offset);
}

int CacheLRU::LoadWord(int address) {
    ++requests_count;
    int offset, index, tag;
    ParseAddress(address, offset, index, tag);

    CacheSetInfo info = GetCacheSetInfo(index, tag);
    if (!info.hit) {
        CopyCacheLineInMemory(info.to_replace, index, tag);
        CopyMemoryInCacheLine(info.to_replace, index, tag);
    } else {
        ++hits_count;
    }

    CACHE_WAY_s[info.to_replace].cache_lines[index].last_used = requests_count;
    return CACHE_WAY_s[info.to_replace].cache_lines[index].LoadWord(offset);
}

int CacheLRU::GetRequestsCount() const {
    return requests_count;
}

int CacheLRU::GetHitsCount() const {
    return hits_count;
}

CacheSetInfo CacheLRU::GetCacheSetInfo(int index, int tag) {
    CacheSetInfo info;
    int used_time = 1000000000;
    for (int i = 0; i < CACHE_WAY_s.size(); ++i) {
        if (CACHE_WAY_s[i].cache_lines[index].tag == tag) {
            info.hit = true;
            info.to_replace = i;
            return info;
        } else {
            if (CACHE_WAY_s[i].cache_lines[index].last_used < used_time) {
                used_time = CACHE_WAY_s[i].cache_lines[index].last_used;
                info.to_replace = i;
            }
        }
    }

    return info;
}

void CacheLRU::CopyCacheLineInMemory(int CACHE_WAY_, int index, int tag) {
    int address = (tag << CACHE_TAG_LEN_) + (index * CACHE_LINE_SIZE_);
    int last_address = address + 64;
    for (int i = 0; address < last_address; ++address, ++i) {
        memory.StoreByte(address, CACHE_WAY_s[CACHE_WAY_].cache_lines[index].bytes[i]);
    }
}

void CacheLRU::CopyMemoryInCacheLine(int CACHE_WAY_, int index, int tag) {
    int address = (tag << CACHE_TAG_LEN_) + (index * CACHE_LINE_SIZE_);
    CacheLine& cache_line = CACHE_WAY_s[CACHE_WAY_].cache_lines[index];
    cache_line.tag = tag;
    cache_line.last_used = requests_count;
    int last_address = address + 64;
    for (int i = 0; address < last_address; ++address, ++i) {
        cache_line.bytes[i] = memory.LoadByte(address);
    }
}

void CacheLRU::ParseAddress(int address, int& offset, int& index, int& tag) {
    offset = address & 0x3f;
    address >>= CACHE_OFFSET_LEN_;
    index = address & 0b111;
    address >>= CACHE_INDEX_LEN_;
    tag = address & 0x1ff;
}


void CacheLRU::CacheLine::StoreByte(uint8_t value, int offset) {
    bytes[offset] = value;
}

void CacheLRU::CacheLine::StoreHalf(uint16_t value, int offset) {
    bytes[offset++] = value & 0xff;
    value >>= 8;
    bytes[offset] = value;
}

void CacheLRU::CacheLine::StoreWord(uint32_t value, int offset) {
    for (int i = 0; i < 4; ++i) {
        bytes[offset++] = value & 0xff;
        if (i != 3) {
            value >>= 8;
        }
    }
}

int CacheLRU::CacheLine::LoadByte(int offset) {
    return bytes[offset];
}

int CacheLRU::CacheLine::LoadHalf(int offset) {
    ++offset;
    uint16_t value = bytes[offset--];
    value <<= 8;
    value += bytes[offset];
    return value;
}

int CacheLRU::CacheLine::LoadWord(int offset) {
    offset += 3;
    uint32_t value = bytes[offset--];
    value <<= 8;
    value += bytes[offset--];
    value <<= 8;
    value += bytes[offset--];
    value <<= 8;
    value += bytes[offset--];
    return value;
}


void CachePLRU::StoreByte(int address, uint8_t value) {
    ++requests_count;
    int offset, index, tag;
    ParseAddress(address, offset, index, tag);

    CacheSetInfo info = GetCacheSetInfo(index, tag);
    if (!info.hit) {
        CopyCacheLineInMemory(info.to_replace, index, tag);
        CopyMemoryInCacheLine(info.to_replace, index, tag);
    } else {
        ++hits_count;
    }

    CACHE_WAY_s[info.to_replace].cache_lines[index].last_used = requests_count;
    CACHE_WAY_s[info.to_replace].cache_lines[index].mru_bit = true;
    SetZeroCacheLines(index, info.to_replace);
    CACHE_WAY_s[info.to_replace].cache_lines[index].StoreByte(value, offset);
}

void CachePLRU::StoreHalf(int address, uint16_t value) {
    ++requests_count;
    int offset, index, tag;
    ParseAddress(address, offset, index, tag);

    CacheSetInfo info = GetCacheSetInfo(index, tag);
    if (!info.hit) {
        CopyCacheLineInMemory(info.to_replace, index, tag);
        CopyMemoryInCacheLine(info.to_replace, index, tag);
    } else {
        ++hits_count;
    }

    CACHE_WAY_s[info.to_replace].cache_lines[index].last_used = requests_count;
    CACHE_WAY_s[info.to_replace].cache_lines[index].mru_bit = true;
    SetZeroCacheLines(index, info.to_replace);
    CACHE_WAY_s[info.to_replace].cache_lines[index].StoreHalf(value, offset);
}

void CachePLRU::StoreWord(int address, uint32_t value) {
    ++requests_count;
    int offset, index, tag;
    ParseAddress(address, offset, index, tag);

    CacheSetInfo info = GetCacheSetInfo(index, tag);
    if (!info.hit) {
        CopyCacheLineInMemory(info.to_replace, index, tag);
        CopyMemoryInCacheLine(info.to_replace, index, tag);
    } else {
        ++hits_count;
    }

    CACHE_WAY_s[info.to_replace].cache_lines[index].last_used = requests_count;
    CACHE_WAY_s[info.to_replace].cache_lines[index].mru_bit = true;
    SetZeroCacheLines(index, info.to_replace);
    CACHE_WAY_s[info.to_replace].cache_lines[index].StoreWord(value, offset);
}

int CachePLRU::LoadByte(int address) {
    ++requests_count;
    int offset, index, tag;
    ParseAddress(address, offset, index, tag);

    CacheSetInfo info = GetCacheSetInfo(index, tag);
    if (!info.hit) {
        CopyCacheLineInMemory(info.to_replace, index, tag);
        CopyMemoryInCacheLine(info.to_replace, index, tag);
    } else {
        ++hits_count;
    }

    CACHE_WAY_s[info.to_replace].cache_lines[index].last_used = requests_count;
    CACHE_WAY_s[info.to_replace].cache_lines[index].mru_bit = true;
    SetZeroCacheLines(index, info.to_replace);
    return CACHE_WAY_s[info.to_replace].cache_lines[index].LoadByte(offset);
}

int CachePLRU::LoadHalf(int address) {
    ++requests_count;
    int offset, index, tag;
    ParseAddress(address, offset, index, tag);

    CacheSetInfo info = GetCacheSetInfo(index, tag);
    if (!info.hit) {
        CopyCacheLineInMemory(info.to_replace, index, tag);
        CopyMemoryInCacheLine(info.to_replace, index, tag);
    } else {
        ++hits_count;
    }

    CACHE_WAY_s[info.to_replace].cache_lines[index].last_used = requests_count;
    CACHE_WAY_s[info.to_replace].cache_lines[index].mru_bit = true;
    SetZeroCacheLines(index, info.to_replace);
    return CACHE_WAY_s[info.to_replace].cache_lines[index].LoadHalf(offset);
}

int CachePLRU::LoadWord(int address) {
    ++requests_count;
    int offset, index, tag;
    ParseAddress(address, offset, index, tag);

    CacheSetInfo info = GetCacheSetInfo(index, tag);
    if (!info.hit) {
        CopyCacheLineInMemory(info.to_replace, index, tag);
        CopyMemoryInCacheLine(info.to_replace, index, tag);
    } else {
        ++hits_count;
    }

    CACHE_WAY_s[info.to_replace].cache_lines[index].last_used = requests_count;
    CACHE_WAY_s[info.to_replace].cache_lines[index].mru_bit = true;
    SetZeroCacheLines(index, info.to_replace);
    return CACHE_WAY_s[info.to_replace].cache_lines[index].LoadWord(offset);
}

int CachePLRU::GetRequestsCount() const {
    return requests_count;
}

int CachePLRU::GetHitsCount() const {
    return hits_count;
}

CacheSetInfo CachePLRU::GetCacheSetInfo(int index, int tag) {
    CacheSetInfo info;
    int used_time = 1000000000;
    for (int i = 0; i < CACHE_WAY_s.size(); ++i) {
        if (CACHE_WAY_s[i].cache_lines[index].tag == tag) {
            info.hit = true;
            info.to_replace = i;
            return info;
        } else {
            if (!CACHE_WAY_s[i].cache_lines[index].mru_bit && CACHE_WAY_s[i].cache_lines[index].last_used < used_time) {
                info.to_replace = i;
                used_time = -100;
                info.hit = false;
            }
        }
    }

    return info;
}

bool CachePLRU::AllAreTrue(int index) const {
    for (int i = 0; i < CACHE_WAY_s.size(); ++i) {
        if (!CACHE_WAY_s[i].cache_lines[index].mru_bit) { return false; }
    }
    return true;
}

void CachePLRU::SetZeroCacheLines(int index, int exception_index) {
    if (!AllAreTrue(index)) { return; }

    for (int i = 0; i < CACHE_WAY_s.size(); ++i) {
        if (i == exception_index) { continue; }
        CACHE_WAY_s[i].cache_lines[index].mru_bit = false;
    }
}

void CachePLRU::CopyCacheLineInMemory(int CACHE_WAY_, int index, int tag) {
    int address = (tag << CACHE_TAG_LEN_) + (index * CACHE_LINE_SIZE_);
    int last_address = address + 64;
    for (int i = 0; address < last_address; ++address, ++i) {
        memory.StoreByte(address, CACHE_WAY_s[CACHE_WAY_].cache_lines[index].bytes[i]);
    }
}

void CachePLRU::CopyMemoryInCacheLine(int CACHE_WAY_, int index, int tag) {
    int address = (tag << CACHE_TAG_LEN_) + (index * CACHE_LINE_SIZE_);
    CacheLine& cache_line = CACHE_WAY_s[CACHE_WAY_].cache_lines[index];
    cache_line.tag = tag;
    cache_line.last_used = requests_count;
    int last_address = address + 64;
    for (int i = 0; address < last_address; ++address, ++i) {
        cache_line.bytes[i] = memory.LoadByte(address);
    }
}

void CachePLRU::ParseAddress(int address, int& offset, int& index, int& tag) {
    offset = address & 0x3f;
    address >>= CACHE_OFFSET_LEN_;
    index = address & 0b111;
    address >>= CACHE_INDEX_LEN_;
    tag = address & 0x1ff;
}


void CachePLRU::CacheLine::StoreByte(uint8_t value, int offset) {
    bytes[offset] = value;
}

void CachePLRU::CacheLine::StoreHalf(uint16_t value, int offset) {
    bytes[offset++] = value & 0xff;
    value >>= 8;
    bytes[offset] = value;
}

void CachePLRU::CacheLine::StoreWord(uint32_t value, int offset) {
    for (int i = 0; i < 4; ++i) {
        bytes[offset++] = value & 0xff;
        if (i != 3) {
            value >>= 8;
        }
    }
}

int CachePLRU::CacheLine::LoadByte(int offset) {
    return bytes[offset];
}

int CachePLRU::CacheLine::LoadHalf(int offset) {
    ++offset;
    uint16_t value = bytes[offset--];
    value <<= 8;
    value += bytes[offset];
    return value;
}

int CachePLRU::CacheLine::LoadWord(int offset) {
    offset += 3;
    uint32_t value = bytes[offset--];
    value <<= 8;
    value += bytes[offset--];
    value <<= 8;
    value += bytes[offset--];
    value <<= 8;
    value += bytes[offset--];
    return value;
}