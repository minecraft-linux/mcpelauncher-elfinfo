#pragma once

#include <cstring>
#include <elf.h>
#include "memory_region.h"

struct Elf32Types {
    using Ehdr = Elf32_Ehdr;
    using Shdr = Elf32_Shdr;
    using Sym = Elf32_Sym;
};

template <typename T>
class ElfReader : public MemoryRegion {

public:
    ElfReader(void* ptr) : MemoryRegion(ptr) {}
    ElfReader(int fd) : MemoryRegion(fd) {}

    typename T::Ehdr* ehdr() const { return (typename T::Ehdr*) ptr(); }

    size_t shdrCount() const { return ehdr()->e_shnum; }
    typename T::Shdr* shdr(size_t i) const { return (typename T::Shdr*) ptr(ehdr()->e_shoff + ehdr()->e_shentsize * i); }

    const char* shstr() const { return (const char*) ptr(shdr(ehdr()->e_shstrndx)->sh_offset); }


    // utils

    bool checkMagic() const {
        return memcmp(ehdr()->e_ident, ELFMAG, SELFMAG) == 0;
    }

    typename T::Shdr* findShdr(const char* name) {
        size_t n = shdrCount();
        auto str = shstr();
        for (size_t i = 0; i < n; i++) {
            auto e = shdr(i);
            if (strcmp(&str[e->sh_name], name) == 0)
                return e;
        }
        return nullptr;
    }

};