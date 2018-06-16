#include <mcpelauncher/memory_region.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdexcept>

MemoryRegion::MemoryRegion(int fd) {
    auto size = lseek64(fd, 0, SEEK_END);
    lseek64(fd, 0, SEEK_SET);
    if (size < 0)
        throw std::runtime_error("Failed to get file size");
    dataSize = (size_t) size;
    dataPtr = mmap(nullptr, dataSize, PROT_READ, MAP_PRIVATE, fd, 0);
    dataOwned = true;
}

MemoryRegion::~MemoryRegion() {
    if (dataOwned)
        munmap(dataPtr, dataSize);
}