#pragma once

#include <cstdio>
#include <string>
#include <unordered_map>
#include "elf_reader.h"

class MinecraftElfInfo {

private:
    template <typename ElfT, typename OutT>
    static void handleSym(ElfReader<ElfT>& reader, OutT& ret, typename ElfT::Sym* sym) {
        if (sym == nullptr || sym->st_size < sizeof(OutT))
            return;
        ret = *((OutT*) (reader.ptr(sym->st_value)));
    }
    template <typename ElfT, typename OutT>
    static void handleSym(ElfReader<ElfT>& reader, OutT& ret,
                          std::unordered_map<std::string, typename ElfT::Sym*> const& m, std::string const& name) {
        auto e = m.find(name);
        if (e != m.end())
            handleSym(reader, ret, e->second);
    }

public:
    int versionMajor = 0, versionMinor = 0, versionPatch = 0, versionRevision = 0;
    bool isBeta = false;

    template <typename T>
    static MinecraftElfInfo fromElf(ElfReader<T>& reader) {
        MinecraftElfInfo ret;
        std::unordered_map<std::string, typename T::Sym*> sharedConstants;
        auto sym = reader.findShdr(".dynsym");
        auto str = reader.findShdr(".dynstr");
        auto symp = reader.ptr(sym->sh_offset);
        auto strp = (const char*) reader.ptr(str->sh_offset);
        for (size_t o = 0; o < sym->sh_size; o += sym->sh_entsize) {
            auto sym = (typename T::Sym*) ((size_t) symp + o);
            auto symName = &strp[sym->st_name];
            if (strncmp("_ZN15SharedConstants", symName, 20) == 0)
                sharedConstants[symName] = sym;
        }
        handleSym(reader, ret.versionMajor, sharedConstants, "_ZN15SharedConstants12MajorVersionE");
        handleSym(reader, ret.versionMinor, sharedConstants, "_ZN15SharedConstants12MinorVersionE");
        handleSym(reader, ret.versionPatch, sharedConstants, "_ZN15SharedConstants12PatchVersionE");
        handleSym(reader, ret.versionRevision, sharedConstants, "_ZN15SharedConstants15RevisionVersionE");
        handleSym(reader, ret.isBeta, sharedConstants, "_ZN15SharedConstants6IsBetaE");
        return ret;
    }

};