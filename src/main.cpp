#include <cstdio>
#include <mcpelauncher/minecraft_elf_info.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, const char* argv[]) {
    if (argc != 2) {
        printf("mcpelauncher-elfinfo <so-file>\n");
        return 1;
    }

    const char* path = argv[1];
    int fd = open(path, O_RDONLY);
    ElfReader<Elf32Types> reader (fd);
    if (!reader.checkMagic()) {
        printf("Invalid ELF file\n");
        return 1;
    }
    MinecraftElfInfo::fromElf(reader);
    close(fd);

    return 0;
}
