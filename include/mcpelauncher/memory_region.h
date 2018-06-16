#pragma once

#include <cstddef>

class MemoryRegion {

private:
    void* dataPtr;
    size_t dataSize;
    bool dataOwned = false;

public:
    MemoryRegion(void* ptr) : dataPtr(ptr) {}
    MemoryRegion(int fd);

    ~MemoryRegion();

    constexpr void* ptr() const { return dataPtr; }
    constexpr void* ptr(size_t o) const { return (void*) ((size_t) dataPtr + o); }

};