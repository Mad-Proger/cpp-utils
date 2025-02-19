#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#include <cerrno>
#include <system_error>

#include "Defer.hpp"
#include "MemoryMap.hpp"

namespace {

inline void errnoThrow(const char* what) {
    throw std::system_error(errno, std::system_category(), what);
}

}// namespace

namespace utils {

void MapDeleter::operator()(std::byte* mapped) noexcept {
    munmap(mapped, m_mapLength);
    close(m_fileDescriptor);
}

MapDeleter::MapDeleter(int fd, size_t fileSize): m_fileDescriptor(fd), m_mapLength(fileSize) {}

std::unique_ptr<std::byte[], MapDeleter> MapFile(const std::filesystem::path& path) {
    int fd = open(std::filesystem::absolute(path).c_str(), 0, O_RDWR);
    if (fd == -1) errnoThrow("could not open file to map");
    Defer closeFd([fd]() { close(fd); });

    off64_t end = lseek64(fd, 0, SEEK_END);
    if (end == -1) errnoThrow("could not find file end");
    off64_t start = lseek64(fd, 0, SEEK_SET);
    if (start == -1) errnoThrow("could not find file start");

    auto fileSize = static_cast<size_t>(end - start);
    auto mapped =
            static_cast<std::byte*>(mmap(nullptr, fileSize, PROT_READ | PROT_WRITE, MAP_FILE | MAP_SHARED, fd, 0));
    if (mapped == MAP_FAILED) errnoThrow("could not map file to memory");

    closeFd.cancel();
    return {mapped, MapDeleter(fd, fileSize)};
}

}// namespace utils
