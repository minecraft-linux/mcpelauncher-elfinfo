#pragma once

#include <cstdio>
#include "elf_reader.h"

class MinecraftElfInfo {

public:
    int versionMajor, versionMinor, versionPatch;
    bool isBeta;

    template <typename T>
    static MinecraftElfInfo fromElf(ElfReader<T>& reader) {
        MinecraftElfInfo ret;
        auto sym = reader.findShdr(".dynsym");
        auto str = reader.findShdr(".dynstr");
        auto symp = reader.ptr(sym->sh_offset);
        auto strp = (const char*) reader.ptr(str->sh_offset);
        for (size_t o = 0; o < sym->sh_size; o += sym->sh_entsize) {
            auto sym = (typename T::Sym*) ((size_t) symp + o);
            printf("%s\n", &strp[sym->st_name]);
        }
        return ret;
    }

};